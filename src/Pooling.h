#pragma once
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <typeindex>
#include <memory>
#include <mutex>
#include <type_traits>

class IPooleable;

class Pooling {
public:
    static Pooling& Instance() {
        static Pooling instance;
        return instance;
    }

    template <typename T>
    void CreatePool(size_t size) {
        static_assert(std::is_base_of_v<IPooleable, T>,
            "T must inherit from IPooleable");

        std::lock_guard lock(mutex_);
        auto& pool = GetPool<T>();

        // Preallocate objects
        for (size_t i = 0; i < size; ++i) {
            auto obj = std::make_unique<T>();
            obj->ResetPoolObject();
            pool.available.push(obj.get());
            pool.all_objects.push_back(std::move(obj));
        }
    }

    template <typename T>
    std::shared_ptr<T> AcquireObject() {
        std::lock_guard lock(mutex_);
        auto& pool = GetPool<T>();

        T* raw_ptr = nullptr;

        if (!pool.available.empty()) {
            raw_ptr = pool.available.front();
            pool.available.pop();
        }
        else {
            auto new_obj = std::make_unique<T>();
            new_obj->ResetPoolObject();
            raw_ptr = new_obj.get();
            pool.all_objects.push_back(std::move(new_obj));
            if (debug)
                std::cout << "Pool expanded for " << typeid(T).name() << "\n";

        }
        pool.checked_out.insert(raw_ptr);
        raw_ptr->InitPoolObject();

        return std::shared_ptr<T>(raw_ptr, [](T*) {});
    }

    template <typename T>
    const std::vector<std::shared_ptr<T>> AcquireActiveObjects() {
        std::lock_guard<std::mutex> lock(mutex_);
        auto& pool = GetPool<T>();
        std::vector<std::shared_ptr<T>> active;
        active.reserve(pool.checked_out.size());
        for (T* obj : pool.checked_out) {
            active.emplace_back(obj, [](T*) {});
        }
        return active;
    }

    template <typename T>
    const std::vector<std::shared_ptr<T>> AcquireInactiveObjects() {
        std::lock_guard<std::mutex> lock(mutex_);
        auto& pool = GetPool<T>();
        std::vector<std::shared_ptr<T>> inactive;
        inactive.reserve(pool.available.size());
        std::queue<T*> temp_queue = pool.available;
        while (!temp_queue.empty()) {
            T* obj = temp_queue.front();
            temp_queue.pop();
            inactive.emplace_back(obj, [](T*) {});
        }
        return inactive;
    }

    template <typename T>
    const std::vector<std::shared_ptr<T>> AcquireAllObjects() {
        std::lock_guard<std::mutex> lock(mutex_);
        auto& pool = GetPool<T>();
        std::vector<std::shared_ptr<T>> all;
        all.reserve(pool.checked_out.size() + pool.available.size());
        for (T* obj : pool.checked_out) {
            all.emplace_back(obj, [](T*) {});
        }
        std::queue<T*> temp_queue = pool.available;
        while (!temp_queue.empty()) {
            T* obj = temp_queue.front();
            temp_queue.pop();
            all.emplace_back(obj, [](T*) {});
        }
        return all;
    }

    template <typename T>
    void ReturnObject(T* obj) {
        std::lock_guard lock(mutex_);
        auto& pool = GetPool<T>();

        if (pool.checked_out.erase(obj)) {
            obj->ResetPoolObject();
            pool.available.push(obj);
        }
        else {
            if (debug)
                std::cerr << "Object not from this pool!\n";
        }
    }

    template <typename T>
    bool DeletePool(bool force = false) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto type = std::type_index(typeid(T));
        auto it = pools_.find(type);

        if (it == pools_.end()) {
            if (debug)
                std::cout << "Pool for type " << type.name() << " not found.\n";
            return false;
        }

        TypePool<T>& pool = static_cast<TypePool<T>&>(*it->second);

        if (!force && !pool.checked_out.empty()) {
            if (debug)
                std::cout << "Cannot delete pool - " << pool.checked_out.size() << " objects still checked out. Use force=true to override.\n";
            return false;
        }

        if (force && !pool.checked_out.empty()) {
            if (debug)
                std::cout << "WARNING: Force deleting pool with " << pool.checked_out.size() << " active objects. Any existing pointers will become invalid!\n";

            for (T* obj : pool.checked_out) {
                obj->ResetPoolObject();
            }
        }

        pool.Clear();

        pools_.erase(it);
        if (debug)
            std::cout << "Pool for type " << type.name() << " deleted " << (force ? "(forced)" : "(normal)") << "\n";
        return true;
    }

    template <typename T>
    bool ReturnAllToPool() {
        std::lock_guard<std::mutex> lock(mutex_);
        auto type = std::type_index(typeid(T));
        auto it = pools_.find(type);

        if (it == pools_.end()) {
            if (debug)
                std::cout << "Pool for type " << type.name() << " not found.\n";
            return false;
        }

        TypePool<T>& pool = static_cast<TypePool<T>&>(*it->second);

        if (!pool.checked_out.empty()) {
            if (debug)
                std::cout << "WARNING: Force returnig to pool with " << pool.checked_out.size() << " active objects. Any existing pointers will become invalid!\n";

            for (T* obj : pool.checked_out) {
                obj->ResetPoolObject();
                pool.available.push(obj);
            }
        }

        pool.checked_out.clear();

        if(debug)
            std::cout << "Pool for type " << type.name() << " returned " << "\n";
        return true;
    }

    ~Pooling() {
        std::lock_guard lock(mutex_);
        for (auto& entry : pools_) {
            auto& pool = *static_cast<TypePoolBase*>(entry.second.get());
            pool.Clear();
        }
    }
public:
        bool debug = false;
private:
    struct TypePoolBase {
        virtual ~TypePoolBase() = default;
        virtual void Clear() = 0;
    };

    template <typename T>
    struct TypePool : TypePoolBase {
        std::vector<std::unique_ptr<T>> all_objects;
        std::queue<T*> available;
        std::unordered_set<T*> checked_out;

        void Clear() override {
            all_objects.clear();
            available = {};
            checked_out.clear();
        }
    };

    template <typename T>
    TypePool<T>& GetPool() {
        auto type = std::type_index(typeid(T));
        auto it = pools_.find(type);

        if (it == pools_.end()) {
            auto new_pool = std::make_unique<TypePool<T>>();
            auto& pool_ref = *new_pool;
            pools_.emplace(type, std::move(new_pool));
            return pool_ref;
        }

        return *static_cast<TypePool<T>*>(it->second.get());
    }
    private:
        std::unordered_map<std::type_index, std::unique_ptr<TypePoolBase>> pools_;
        std::mutex mutex_;
};