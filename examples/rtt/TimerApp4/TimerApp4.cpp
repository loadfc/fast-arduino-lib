/*
 * Timer compilation example.
 * Shows how to use 2 CTC Timers (not RTT) to blink 1 LED for some period of time, then stop completely.
 * 
 * Wiring: TODO
 * - on ATmega328P based boards (including Arduino UNO):
 *   - D13 (PB5) LED connected to ground through a resistor
 * - on Arduino MEGA:
 *   - D13 (PB7) LED connected to ground through a resistor
 * - on ATtinyX4 based boards:
 *   - D7 (PA7) LED connected to ground through a resistor
 */

#include <fastarduino/FastIO.hh>
#include <fastarduino/Timer.hh>
#include <fastarduino/time.hh>

// Define vectors we need in the example
USE_TIMERS(0, 1)

constexpr const Board::Timer BLINK_TIMER = Board::Timer::TIMER0;
using BLINK_TIMER_TYPE = Timer<BLINK_TIMER>;
constexpr const uint32_t BLINK_PERIOD_US = 10000;
constexpr const BLINK_TIMER_TYPE::TIMER_PRESCALER BLINK_PRESCALER = BLINK_TIMER_TYPE::prescaler(BLINK_PERIOD_US);
static_assert(BLINK_TIMER_TYPE::is_adequate(BLINK_PRESCALER, BLINK_PERIOD_US), 
		"BLINK_TIMER_TYPE::is_adequate(BLINK_PRESCALER, BLINK_PERIOD_US)");
constexpr const BLINK_TIMER_TYPE::TIMER_TYPE BLINK_COUNTER = BLINK_TIMER_TYPE::counter(BLINK_PRESCALER, BLINK_PERIOD_US);

constexpr const Board::Timer SUSPEND_TIMER = Board::Timer::TIMER1;
using SUSPEND_TIMER_TYPE = Timer<SUSPEND_TIMER>;
constexpr const uint32_t SUSPEND_PERIOD_US = 4000000;
constexpr const SUSPEND_TIMER_TYPE::TIMER_PRESCALER SUSPEND_PRESCALER = SUSPEND_TIMER_TYPE::prescaler(SUSPEND_PERIOD_US);
static_assert(SUSPEND_TIMER_TYPE::is_adequate(SUSPEND_PRESCALER, SUSPEND_PERIOD_US), 
		"SUSPEND_TIMER_TYPE::is_adequate(SUSPEND_PRESCALER, SUSPEND_PERIOD_US)");
constexpr const SUSPEND_TIMER_TYPE::TIMER_TYPE SUSPEND_COUNTER = SUSPEND_TIMER_TYPE::counter(SUSPEND_PRESCALER, SUSPEND_PERIOD_US);

class BlinkHandler: public TimerCallback
{
public:
	BlinkHandler(): _led{PinMode::OUTPUT, false} {}
	
	virtual void on_timer() override
	{
		_led.set();
		Time::delay_us(1000);
		_led.clear();
	}
	
private:
	FastPinType<Board::DigitalPin::LED>::TYPE _led;
};

class SuspendHandler: public TimerCallback
{
public:
	SuspendHandler(BLINK_TIMER_TYPE& blink_timer):_blink_timer{blink_timer} {}
	
	virtual void on_timer() override
	{
		if (_blink_timer.is_suspended())
			_blink_timer._resume();
		else
			_blink_timer._suspend();
	}
	
private:
	BLINK_TIMER_TYPE& _blink_timer;
};

int main()
{
	BlinkHandler blink_handler;
	BLINK_TIMER_TYPE blink_timer{blink_handler};
	SuspendHandler suspend_handler{blink_timer};
	SUSPEND_TIMER_TYPE suspend_timer{suspend_handler};
	blink_timer._begin(BLINK_PRESCALER, BLINK_COUNTER);
	suspend_timer._begin(SUSPEND_PRESCALER, SUSPEND_COUNTER);
	sei();
	
	while (true) ;
}
