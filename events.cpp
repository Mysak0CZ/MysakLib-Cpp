#include "events.h"
#include "logger.h"

namespace MLib
{
bool DEBUG_EVENTS = false;

Event::Event(EventEmitter* source, event_t type, eventdata_t data) :
    target(source), type(type), data(data)
{
}

void Event::stopPropagation()
{
	bubbles = false;
	Log(LOG_warning, true) << "Event::stopPropagation called! This has no effect, did you mean Event::stopImmediatePropagation?";
}
void Event::stopImmediatePropagation()
{
	bubbles = false;
	canceled = true;
	if (DEBUG_EVENTS) {
		Log(LOG_debug, true) << "Event propagation stopped";
	}
}

void EventEmitter::trigger(event_t event, eventdata_t data)
{
	auto targets = receivers.find(event);
	if (targets == receivers.end()) {
		if (DEBUG_EVENTS) {
			Log(LOG_warning, true) << string_format<void*, event_t>("Emitter %p send event %lu: no receiver", this, event);
		}
		return;
	}
	Event e(this, event, data);
	if (DEBUG_EVENTS) {
		Log(LOG_debug, true) << string_format<void*, event_t>("Emitter %p send event %lu", this, event);
	}
	for (auto receiver : targets->second) {
		receiver->onEvent(e);
		if (e.canceled)
			break;
	}
	if (e.bubbles) {
		// TODO: Add parent event emitters
	}
}

EventEmitter::EventEmitter()
{
}
EventEmitter::EventEmitter(EventEmitter& other)
{
	for (auto event : other.receivers) {
		for (auto receiver : event.second) {
			on(event.first, receiver);
		}
	}
	if (DEBUG_EVENTS) {
		Log(LOG_debug, true) << string_format<void*, void*>("Emitter %p copy to %p", &other, this);
	}
}
EventEmitter::EventEmitter(EventEmitter&& other) :
    receivers(std::move(other.receivers))
{
	for (auto event : receivers) {
		for (auto target : event.second) {
			auto me = target->eventSubscribed.find(&other);
			if (me != target->eventSubscribed.end()) {
				target->eventSubscribed.insert(*me);
				target->eventSubscribed.erase(me);
			}
		}
	}
	if (DEBUG_EVENTS) {
		Log(LOG_debug, true) << string_format<void*, void*>("Emitter %p move to %p", &other, this);
	}
}
EventEmitter::~EventEmitter()
{
	for (auto event : receivers) {
		for (auto target : event.second) {
			target->eventSubscribed.erase(this);
		}
	}
	receivers.clear();
	if (DEBUG_EVENTS) {
		Log(LOG_debug, true) << string_format<void*>("Emitter %p destroy", this);
	}
}

void EventEmitter::on(event_t event, EventReceiver* target)
{
	if (target == nullptr) {
		Log(LOG_error, true) << "NULL target for EventEmitter::on";
		return;
	}
	receivers[event].insert(target);
	target->eventSubscribed[this].insert(event);
	if (DEBUG_EVENTS) {
		Log(LOG_debug, true) << string_format<void*, event_t, void*>("Emitter %p register event %lu for %p", this, event, target);
	}
}
void EventEmitter::off(event_t event, EventReceiver* target)
{
	if (target == nullptr) {
		Log(LOG_error, true) << "NULL target for EventEmitter::off";
		return;
	}

	auto targets = receivers.find(event);
	if (targets == receivers.end()) {
		Log(LOG_warning, true) << "EventEmitter::off: event " << event << " not registered";
		return;
	}
	if (targets->second.erase(target) == 0) {
		Log(LOG_warning, true) << "EventEmitter::off: event " << event << ": target was not registered";
		return;
	}
	if (targets->second.size() == 0) {
		receivers.erase(targets);
	}

	auto me = target->eventSubscribed.find(this);
	if (me == target->eventSubscribed.end()) {
		Log(LOG_error, true) << "EventEmitter::off: target didn't have backward link for event " << event;
		return;
	}
	if (me->second.erase(event) == 0) {
		Log(LOG_error, true) << "EventEmitter::off: target didn't have backward event for " << event;
		return;
	}
	if (me->second.size() == 0) {
		target->eventSubscribed.erase(me);
	}
	if (DEBUG_EVENTS) {
		Log(LOG_debug, true) << string_format<void*, event_t, void*>("Emitter %p unregister event %lu for %p", this, event, target);
	}
}
void EventEmitter::off(EventReceiver* target)
{
	if (target == nullptr) {
		Log(LOG_error, true) << "NULL target for EventEmitter::off(all)";
		return;
	}
	bool found = false;
	for (auto event = receivers.begin(); event != receivers.end();) {
		if (event->second.erase(target) > 0) {
			found = true;
			if (event->second.size() == 0) {
				event = receivers.erase(event);
				continue;
			}
		}
		event++;
	}
	if (!found) {
		Log(LOG_warning, true) << "EventEmitter::off(all): target was not registered";
		return;
	}
	auto me = target->eventSubscribed.find(this);
	if (me == target->eventSubscribed.end()) {
		Log(LOG_error, true) << "EventEmitter::off(all): target didn't have backward link";
		return;
	}
	target->eventSubscribed.erase(me);
	if (DEBUG_EVENTS) {
		Log(LOG_debug, true) << string_format<void*, void*>("Emitter %p unregister all events for %p", this, target);
	}
}


EventReceiver::EventReceiver()
{
}
EventReceiver::EventReceiver(EventReceiver& other)
{
	for (auto target : other.eventSubscribed) {
		for (auto event : target.second) {
			target.first->on(event, this);
		}
	}
	if (DEBUG_EVENTS) {
		Log(LOG_debug, true) << string_format<void*, void*>("Receiver %p copy to %p", &other, this);
	}
}
EventReceiver::EventReceiver(EventReceiver&& other)
{
	for (;;) {
		auto target = other.eventSubscribed.begin();
		if (target == other.eventSubscribed.end())
			break;
		for (auto event : target->second) {
			target->first->on(event, this);
		}
		target->first->off(&other);
	}
	assert(other.eventSubscribed.size() == 0);
	if (DEBUG_EVENTS) {
		Log(LOG_debug, true) << string_format<void*, void*>("Receiver %p move to %p", &other, this);
	}
}
EventReceiver::~EventReceiver()
{
	size_t size = eventSubscribed.size();
	for (;;) {
		auto it = eventSubscribed.begin();
		if (it == eventSubscribed.end())
			break;
		it->first->off(this);
		assert(eventSubscribed.size() < size);
		size = eventSubscribed.size();
	}
	if (DEBUG_EVENTS) {
		Log(LOG_debug, true) << string_format<void*>("Receiver %p destroy", this);
	}
}

}  // namespace MLib