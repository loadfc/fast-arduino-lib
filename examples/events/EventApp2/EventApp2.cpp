/*
 * LED chaser, with input settings
 * This program shows usage of FastArduino port API to handle several output at a time, plus individual input pins.
 * On Arduino, you should branch LED (in series with 330 Ohm resistors to limit current) on the following pins:
 * - D0-D7
 * i.e. all pins mapped to AVR ATmega328 PORT D
 * Then, you should branch 4 switches (I use DIP switches which are convenient on breadboard):
 * - one side to A0-A2 (number of simultaneously lit LED) and A3 (chase direction)
 * - the other side to GND (we use internal pullup resistors for inputs)
 */

#include <avr/interrupt.h>
#include <util/delay.h>

#include <fastarduino/IO.hh>
#include <fastarduino/Events.hh>
#include <fastarduino/watchdog.hh>

using namespace Events;

#if defined(ARDUINO_UNO) || defined(BREADBOARD_ATMEGA328P)
static constexpr const Board::Port LED_PORT = Board::Port::PORT_D;
#elif defined (ARDUINO_MEGA)
static constexpr const Board::Port LED_PORT = Board::Port::PORT_A;
#elif defined (BREADBOARD_ATTINYX4)
static constexpr const Board::Port LED_PORT = Board::Port::PORT_A;
#else
#error "Current target is not yet supported!"
#endif

class LedHandler: public EventHandler, private IOPort
{
public:
	LedHandler() : EventHandler{Type::WDT_TIMER}, IOPort{LED_PORT, 0xFF}, _value{0} {}
	virtual void on_event(UNUSED const Event& event) override
	{
		uint8_t value = _value;
		if (value == 0)
			value = 0x01;
		else
			value <<= 1;
		set_PORT(value);
		_value = value;
	}
	
private:
	uint8_t _value;
};

static const uint8_t EVENT_QUEUE_SIZE = 32;

// Prepare event queue
static Event buffer[EVENT_QUEUE_SIZE];
static Queue<Event> event_queue{buffer};
	
int main()
{
	// Enable interrupts at startup time
	sei();

	// Prepare Dispatcher and Handlers
	Dispatcher dispatcher;
	LedHandler handler;
	dispatcher.insert(handler);
	
	// Start watchdog
	Watchdog watchdog{event_queue};
	watchdog.begin(Watchdog::TimeOut::TO_64ms);
	
	// Event Loop
	while (true)
	{
		Event event = pull(event_queue);
		dispatcher.dispatch(event);
	}
}
