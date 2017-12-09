//
//  EventsController.cpp
//  Cloud Raiders Debug
//
//  Created by skorokhodov on 11/9/17.
//
#include <stdio.h>
#include "EventsController.h"

bool EventsController::mVerbose(true);

EventsController& EventsController::instance()
{
    static EventsController _instance;
    return _instance;
}

EventsController::EventsController()
{
    
}

void EventsController::Log(const char* fmt, ...)
{
    char buf[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, 512, fmt, args);
    buf[511] = '\0';
    va_end(args);
    printf("EventsController: %s\n", buf);
}

void EventsController::unsubscribeFromEvent(Event event, void* subscriber)
{
    Log("%p unsubscribe from event %u", subscriber, (unsigned)event);
    Subscriptions::iterator it = mSubscriptions.find(subscriber);
    if(it == mSubscriptions.end())
    {
        Log("Warning! Not existing subscriber %p try to unsubscribe from event %u", subscriber, (unsigned)event);
        return;
    }
    mSubscriptions.erase(it);
}

void EventsController::subscribeToEvent(Event event, const EventCb& callback, void* subscriber)
{
    Log("%p subscribe to event %u", subscriber, (unsigned)event);
    Subscriptions::iterator it = mSubscriptions.find(subscriber);
    if(it == mSubscriptions.end())
    {
        mSubscriptions[subscriber];
    }
    SubscriberSubscriptions& subscriberSubscriptions = mSubscriptions[subscriber];
    subscriberSubscriptions.insert(Subscription(event, callback));
}
void EventsController::fireEvent(Event event, void* data) const
{
    Log("Fire event %u", (unsigned)event);
    bool haveSubscriptions = false;
    for(auto&& subscriberSubscriptionsEntry : mSubscriptions)
    {
        const SubscriberSubscriptions& subscriberSubscriptions = subscriberSubscriptionsEntry.second;
        auto&& range = subscriberSubscriptions.equal_range(event);
        if(range.first == subscriberSubscriptions.end())
        {
            continue;
        }
        for(auto it = range.first; it != range.second; ++it)
        {
            haveSubscriptions = true;
            it->second(data);
        }
    }
    if(!haveSubscriptions)
    {
        Log("No subscriptions to event %u", (unsigned)event);
    }
}
