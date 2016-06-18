#ifndef EVENTS_HH
#define	EVENTS_HH

#include <stddef.h>
#include "Queue.hh"
#include "LinkedList.hh"

namespace Event
{
	//TODO remove name "Type" to allow usage as uint8_t anywhere...
	enum Type
	{
		NO_EVENT = 0,
		WDT_TIMER = 1,
		
		USER_EVENT = 128
	} __attribute__((packed));

	class Event
	{
	public:
		Event(Type type = NO_EVENT, uint16_t value = 0) __attribute__((always_inline)) : _type{type}, _value{value} {}
		Type type() const __attribute__((always_inline))
		{
			return _type;
		}
		uint16_t value() const __attribute__((always_inline))
		{
			return _value;
		}

	private:
		Type _type;
		uint16_t _value;
	};

	class AbstractHandler;

	// Dispatcher should be used only from non-interrupt code
	class Dispatcher: public LinkedList<AbstractHandler>
	{
	public:
		void dispatch(const Event& event);
	};

	// AbstractHandler used on more specific handlers types below
	// This class should normally never be used directly by developers
	class AbstractHandler: public Link<AbstractHandler>
	{
	public:
		//TODO make it private? (only Dispatcher should call it)
		void handle(const Event& event)
		{
			_f(_env, event);
		}

	//TODO make it private and define all other classes as friends as we don't want app developers to subclass Handler
	protected:
		typedef void (*F)(void* env, const Event& event);
		AbstractHandler(Type type = NO_EVENT, void* env = 0, F f = 0): _type{type}, _f{f}, _env{env} {}
		
	private:
		Type _type;
		F _f;
		void* _env;
		
		friend class Dispatcher;
	};
	
	// Derive this class to define event handlers based on a virtual method.
	class VirtualHandler: public AbstractHandler
	{
	protected:
		VirtualHandler(Type type = NO_EVENT): AbstractHandler{type, this, apply} {}
		virtual void on_event(const Event& event) = 0;
		
	private:
		static void apply(void* env, const Event& event)
		{
			((VirtualHandler*) env)->on_event(event);
		}
	};

	// Instantiate this template with a Functor when a functor is applicable.
	// FUNCTOR must be a class defining:
	// void operator()(const Event&);
	// This approach generally gives smaller code and data than VirtualHandler approach
	template<typename FUNCTOR>
	class FunctorHandler: public AbstractHandler
	{
	public:
		FunctorHandler(): AbstractHandler{} {}
		FunctorHandler(Type type, FUNCTOR f): AbstractHandler{type, this, apply}, _f{f} {}
	private:
		static void apply(void* env, const Event& event)
		{
			((FunctorHandler<FUNCTOR>*) env)->_f(event);
		}
		FUNCTOR _f;
	};
};
#endif	/* EVENTS_HH */
