#ifndef OBSERVER_MODULE
#define OBSERVER_MODULE

#include <algorithm>
#include <functional>
#include <vector>

using namespace std;

template <typename T>
class Observer {
   public:
    using Callback = function<void(const T&)>;

   private:
    vector<Callback> subscribers;
    T state;

   public:
    Observer() {}

   public:
    void subscribe(Callback cb) { subscribers.push_back(cb); }

    void unsubscribe(Callback cb) {
        subscribers.erase(remove(subscribers.begin(), subscribers.end(), cb));
    }

    void setState(const T& newState) {
        state = newState;
        notify();
    }

    void clear() { subscribers.clear(); }

   private:
    void notify() {
        for (const auto& subscriber : subscribers) {
            subscriber(state);
        }
    }
};

#endif
