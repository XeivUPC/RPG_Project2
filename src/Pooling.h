#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>
#include <mutex>
#include <type_traits>
#include <string_view>

using namespace std;

class IPooleable;
class Pooling
{
public:
    static Pooling& Instance() {
        static Pooling instance;
        return instance;
    }

    template <typename T>
    void CreatePool(size_t size) {
        static_assert(is_base_of<IPooleable, T>::value, "T must inherit from IPooleable");

        lock_guard<mutex> lock(poolMutex);

        if (poolDictionary.find(typeid(T)) != poolDictionary.end()) {
            cout << "Pool already exists for type " << typeid(T).name() << ". No need to create a new one." << endl;
            return; 
        }

        auto pool = make_shared<vector<T>>();
        pool->reserve(size);

        for (size_t i = 0; i < size; ++i) {
            pool->emplace_back(T());
        }

        poolDictionary[typeid(T)] = pool;
    }

    template <typename T>
    shared_ptr<T> AcquireObject() {
        static_assert(is_base_of<IPooleable, T>::value, "T must inherit from IPooleable");

        lock_guard<mutex> lock(poolMutex);
        auto it = poolDictionary.find(typeid(T));
        if (it != poolDictionary.end()) {
            auto pool = static_pointer_cast<vector<T>>(it->second);

            if (!pool->empty()) {
                T obj = move(pool->back());
                pool->pop_back();

                return make_shared<T>(move(obj));
            }
            else {
                cout << "Pool of " << typeid(T).name() << " is empty, creating new object." << endl;
                return make_shared<T>();
            }
        }
        cout << "Pool not found for type " << typeid(T).name() << ", cannot acquire object." << endl;
        return make_shared<T>();
    }

    template <typename T>
    void ReturnObject(const shared_ptr<T>& obj) {
        static_assert(is_base_of<IPooleable, T>::value, "T must inherit from IPooleable");

        lock_guard<mutex> lock(poolMutex);
        auto it = poolDictionary.find(typeid(T));
        if (it != poolDictionary.end()) {
            auto pool = static_pointer_cast<vector<T>>(it->second);
            pool->push_back(*obj);
        }
        else {
            cout << "Error: No pool exists for the returned object type " << typeid(T).name() << "." << endl;
        }
    }

private:
    Pooling() = default;
    ~Pooling() = default;

    Pooling(const Pooling&) = delete;
    Pooling& operator=(const Pooling&) = delete;

private:
    unordered_map<type_index, shared_ptr<void>> poolDictionary;
    mutable std::mutex poolMutex;
};
