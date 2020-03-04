#pragma once

#include <bits/stdc++.h>
#include "types.h"

namespace MLib
{
typedef unsigned long event_t;
typedef void* eventdata_t;
class EventEmitter;
class EventReceiver;

extern bool DEBUG_EVENTS;

class Event
{
   private:
	bool bubbles = true;
	bool canceled = false;
	Event(EventEmitter* source, event_t type, eventdata_t data);

   public:
	const EventEmitter* target;
	const event_t type;
	void* data;


	void stopPropagation();
	void stopImmediatePropagation();

	friend EventEmitter;
};

class EventEmitter
{
   private:
	std::unordered_map<event_t, std::unordered_set<EventReceiver*>> receivers;

   protected:
	void trigger(event_t event, eventdata_t data = nullptr);

   public:
	EventEmitter();
	EventEmitter(EventEmitter& other);
	EventEmitter(EventEmitter&& other);
	~EventEmitter();

	void on(event_t event, EventReceiver* target);
	void off(event_t event, EventReceiver* target);
	void off(EventReceiver* target);
};

class EventReceiver
{
   private:
	std::unordered_map<EventEmitter*, std::unordered_set<event_t>> eventSubscribed;

   protected:
	virtual void onEvent(Event& event) = 0;

   public:
	EventReceiver();
	EventReceiver(EventReceiver& other);
	EventReceiver(EventReceiver&& other);
	~EventReceiver();

	friend EventEmitter;
};

}  // namespace MLib