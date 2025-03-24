#pragma once
#include <functional>
#include <map>
#include <iostream>


struct EventId
{
    int id;
};

template <typename... Args>
class SystemEvent {
    public:
        EventId Subscribe(std::function<void(Args...)> callback) {
            int id = next_id_.id++;
            subscribers_[id] = callback;
            return EventId{ id };
        }
        template <typename = std::enable_if_t<(sizeof...(Args) > 0)>>
            EventId Subscribe(std::function<void()> callback) {
            return Subscribe([callback](Args...) {
                callback();
                });
        }

        template <typename T>
        EventId Subscribe(T* instance, void (T::* method)(Args...)) {
            return Subscribe([instance, method](Args... args) {
                (instance->*method)(args...);
                });
        }
        template <typename T, typename = std::enable_if_t<(sizeof...(Args) > 0)>>
            EventId Subscribe(T* instance, void (T::* method)()) {
            return Subscribe([instance, method](Args...) {
                (instance->*method)();
                });
        }

        void Unsubscribe(const EventId& eventId) {
            subscribers_.erase(eventId.id);
        }

        void Trigger(Args... args) {
            for (const auto& pair : subscribers_) {
                pair.second(args...);
            }
        }
    private:
        EventId next_id_ = { 0 };
        std::map<int, std::function<void(Args...)>> subscribers_;
};
