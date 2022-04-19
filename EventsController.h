//
//  EventsController.h
//
//  Created by skorokhodov on 11/9/17.
//

#ifndef EventsController_h
#define EventsController_h

#include <string>
#include <unordered_map>

enum Event : unsigned char
{
    EVENT_A,
    EVENT_B
};

class EventsController
{
private:
    EventsController();
public:
    typedef std::function<void(void*)> EventCb;
    void subscribeToEvent(Event event, const EventCb& cb, void* subscriber);
    void unsubscribeFromEvent(Event event, void* subscripber);
    void fireEvent(Event event_name, void* data = nullptr) const;
    static EventsController& instance();
private:
    static void Log(const char* fmt, ...);
    typedef std::pair<Event, EventCb> Subscription;
    typedef std::unordered_multimap<Event, EventCb> SubscriberSubscriptions;
    typedef std::unordered_map<void*, SubscriberSubscriptions> Subscriptions;
    Subscriptions mSubscriptions;
    static bool mVerbose;
};

#endif /* EventsController */
