//   Copyright 2016-2017 Jean-Francois Poilpret
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

/// @cond api

/**
 * @file 
 * Watchdog API.
 */
#ifndef WATCHDOG_HH
#define WATCHDOG_HH

#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "interrupts.h"
#include "events.h"
#include "boards/board.h"

/**
 * Register the necessary ISR (interrupt Service Routine) for a watchdog::Watchdog
 * to work properly.
 * @sa watchdog::Watchdog
 */
#define REGISTER_WATCHDOG_CLOCK_ISR() REGISTER_ISR_METHOD_(WDT_vect, watchdog::Watchdog, &watchdog::Watchdog::on_tick)

/**
 * Register the necessary ISR (interrupt Service Routine) with a callback method
 * that will be called every time a watchdog timeout occurs, according to how
 * watchdog::WatchdogSignal was started.
 * @param HANDLER the class holding the callback method
 * @param CALLBACK the method of @p HANDLER that will be called when the interrupt
 * is triggered; this must be a proper PTMF (pointer to member function).
 */
#define REGISTER_WATCHDOG_ISR_METHOD(HANDLER, CALLBACK) REGISTER_ISR_METHOD_(WDT_vect, HANDLER, CALLBACK)

/**
 * Register the necessary ISR (interrupt Service Routine) with a callback function
 * that will be called every time a watchdog timeout occurs, according to how
 * watchdog::WatchdogSignal was started.
 * @param CALLBACK the function that will be called when the interrupt is
 * triggered
 */
#define REGISTER_WATCHDOG_ISR_FUNCTION(CALLBACK) REGISTER_ISR_FUNCTION_(WDT_vect, CALLBACK)

/**
 * Register an empty ISR (interrupt Service Routine) for a watchdog::WatchdogSignal.
 * This may be needed when using watchdog just to awaken a sleeping MCU, but without
 * any necessary immediate callback.
 */
#define REGISTER_WATCHDOG_ISR_EMPTY() EMPTY_INTERRUPT(WDT_vect)

/**
 * Defines the simple API for Watchdog timer management.
 */
namespace watchdog
{
	/**
	 * Simple API to handle watchdog signals. In this mode, the AVR watchdog
	 * timer is used to wake up the MCU at a specific timer period, in order to
	 * leave a low-power sleep mode, to e.g. perform periodic tasks.
	 * With this API, you do not need to register an ISR callback if you do not
	 * need one; in this situation you need to register an empty ISR with
	 * `REGISTER_WATCHDOG_ISR_EMPTY()` macro.
	 * If you want to register a callback on every watchdog timer tick, then you
	 * can use either `REGISTER_WATCHDOG_ISR_FUNCTION` or 
	 * `REGISTER_WATCHDOG_ISR_METHOD` macros.
	 */
	class WatchdogSignal
	{
	public:
		/**
		 * Defines the watchdog timeout period; watchdog interrupts will be 
		 * triggered at the selected period. The MCU will be awakened at this 
		 * period too.
		 * Note that watchdog timeout period is not very accurate, you would
		 * normally not use for real-time counting.
		 */
		enum class TimeOut : uint8_t
		{
			/** Watchdog timeout 16 ms. */
			TO_16ms = 0,
			/** Watchdog timeout 32 ms. */
			TO_32ms,
			/** Watchdog timeout 64 ms. */
			TO_64ms,
			/** Watchdog timeout 125 ms. */
			TO_125ms,
			/** Watchdog timeout 250 ms. */
			TO_250ms,
			/** Watchdog timeout 500 ms. */
			TO_500ms,
			/** Watchdog timeout 1 second. */
			TO_1s,
			/** Watchdog timeout 2 seconds. */
			TO_2s,
			/** Watchdog timeout 4 seconds. */
			TO_4s,
			/** Watchdog timeout 8 seconds. */
			TO_8s
		};

		/**
		 * Start the watchdog timer with the given @p timeout period.
		 * From now on, watchdog interrupts get generated at @p timeout period,
		 * and the MCU gets awakened at this period too, if sleeping.
		 * @param timeout the watchdog timeout period
		 */
		void begin(TimeOut timeout = TimeOut::TO_16ms);

		/**
		 * Stop this watchdog timer. No more watchdog interrupts get triggered.
		 */
		void end()
		{
			synchronized
			{
				WDTCSR = _BV(WDCE) | _BV(WDE);
				WDTCSR = 0;
			}
		}

	protected:
		/// @cond notdocumented
		inline void begin_with_config(uint8_t config) INLINE
		{
			wdt_reset();
			MCUSR |= 1 << WDRF;
			WDTCSR = _BV(WDCE) | _BV(WDE);
			WDTCSR = config;
		}
		/// @endcond
	};

	/**
	 * Simple API to use watchdog timer as a clock for events generation.
	 * For this to work correctly, you need to register the proper ISR through
	 * `REGISTER_WATCHDOG_CLOCK_ISR()` macro first, then ensure you call
	 * `register_watchdog_handler()`.
	 */
	class Watchdog : public WatchdogSignal
	{
	public:
		/**
		 * Construct a new watchdog-based clock that will, for each watchdog 
		 * timeout, add an event to the given @p event_queue for further 
		 * processing.
		 * This clock also counts elapsed milliseconds since it was started with
		 * `begin()`.
		 * @param event_queue the queue to which `Event`s will be pushed on each 
		 * watchdog tick
		 */
		Watchdog(containers::Queue<events::Event>& event_queue)
			: millis_{0}, millis_per_tick_{0}, event_queue_{event_queue}
		{
		}

		/// @cond notdocumented
		Watchdog(const Watchdog&) = delete;
		/// @endcond

		/**
		 * Register this watchdog instance with the matching ISR that should 
		 * have been registered with REGISTER_WATCHDOG_CLOCK_ISR().
		 * @sa REGISTER_WATCHDOG_CLOCK_ISR()
		 */
		void register_watchdog_handler()
		{
			interrupt::register_handler(*this);
		}

		/**
		 * Start the watchdog clock with the given @p timeout period.
		 * From now on, watchdog interrupts get generated at @p timeout period,
		 * and the MCU gets awakened at this period too, if sleeping.
		 * @param timeout the watchdog timeout period
		 */
		void begin(TimeOut timeout = TimeOut::TO_16ms);

		/**
		 * Get the number of milliseconds that elapsed since `begin()` was called.
		 * Note: the precision is limited by the @p timeout value used with 
		 * `begin()`.
		 * @return number of milliseconds elpased since watchdog was started
		 */
		uint32_t millis() const
		{
			synchronized return millis_;
		}

		/**
		 * Delay program execution for the given amount of milliseconds.
		 * Contrarily to `time::delay_ms()`, this method does not perform a busy
		 * loop, but it calls `time::yield()` which will put the MCU in sleep
		 * mode but will be awakened by the watchdog interrupt and check if
		 * required delay has elapsed already.
		 * Note that the delay precision depends on the actual timeout value
		 * passed when `begin()` has been called.
		 * 
		 * @param ms the number of milliseconds to hold program execution
		 * @sa time::yield()
		 */
		void delay(uint32_t ms);

		/// @cond notdocumented
		void on_tick()
		{
			millis_ += millis_per_tick_;
			event_queue_._push(events::Event{events::Type::WDT_TIMER});
		}
		/// @endcond

	private:
		volatile uint32_t millis_;
		uint16_t millis_per_tick_;
		containers::Queue<events::Event>& event_queue_;
	};
}

#endif /* WATCHDOG_HH */
/// @endcond
