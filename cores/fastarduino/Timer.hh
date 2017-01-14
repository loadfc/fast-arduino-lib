#ifndef TIMER_HH
#define TIMER_HH

#include <avr/interrupt.h>
#include <stddef.h>

#include "utilities.hh"
#include "Board_traits.hh"

//TODO do we need to put everything here in a namespace?

// These macros are internally used in further macros and should not be used in your programs
#define ALIAS_TIMER_(TN, T0)	\
	ISR(TIMER ## TN ## _COMPA_vect, ISR_ALIASOF(TIMER ## T0 ## _COMPA_vect));

#define PREPEND_TIMER_(TN, ...) Board::Timer::TIMER ## TN

#define USE_TIMERS(T0, ...)																								\
ISR(TIMER ## T0 ## _COMPA_vect)																							\
{																														\
	timer_impl::CallbackHandler<FOR_EACH_SEP(PREPEND_TIMER_, , EMPTY, COMMA, EMPTY, T0, ##__VA_ARGS__)>::callback();	\
}																														\
																														\
FOR_EACH(ALIAS_TIMER_, T0, ##__VA_ARGS__)

// Forward declaration necessary to be declared as friend
// Complete declaration can be found at the end of this file
namespace timer_impl
{
	template<Board::Timer T0, Board::Timer... TS> struct CallbackHandler;
}

class TimerCallback
{
public:
	virtual void on_timer() = 0;
};

template<Board::Timer TIMER>
class Timer
{
protected:
	using TRAIT = Board::Timer_trait<TIMER>;
	using PRESCALERS_TRAIT = typename TRAIT::PRESCALERS_TRAIT;
	
public:
	using TIMER_TYPE = typename TRAIT::TYPE;
	using TIMER_PRESCALER = typename PRESCALERS_TRAIT::TYPE;

	Timer(TimerCallback& callback) INLINE
	{
		_callback = &callback;
	}

	static constexpr bool is_adequate(TIMER_PRESCALER p, uint32_t us)
	{
		return prescaler_is_adequate(prescaler_quotient(p, us));
	}
	static constexpr TIMER_PRESCALER prescaler(uint32_t us)
	{
		return best_prescaler(PRESCALERS_TRAIT::ALL_PRESCALERS, us);
	}
	static constexpr TIMER_TYPE counter(TIMER_PRESCALER prescaler, uint32_t us)
	{
		return (TIMER_TYPE) prescaler_quotient(prescaler, us) - 1;
	}
	static constexpr TIMER_TYPE counter(uint32_t us)
	{
		return (TIMER_TYPE) prescaler_quotient(prescaler(us), us) - 1;
	}

	inline void begin(TIMER_PRESCALER prescaler, TIMER_TYPE max)
	{
		synchronized _begin(prescaler, max);
	}

	// We'll need additional methods in Timer_trait<>
	inline void _begin(TIMER_PRESCALER prescaler, TIMER_TYPE max)
	{
		// OCnA & OCnB disconnected, CTC (Clear Timer on Compare match)
		(volatile uint8_t&) TRAIT::TCCRA = TRAIT::CTC_TCCRA;
		// Don't force output compare (FOCA & FOCB), Clock Select according to prescaler
		(volatile uint8_t&) TRAIT::TCCRB = TRAIT::CTC_TCCRB | TRAIT::TCCRB_prescaler(prescaler);
		// Set timer counter compare match (when value reached, 1ms has elapsed)
		(volatile TIMER_TYPE&) TRAIT::OCRA = max;
		// Reset timer counter
		(volatile TIMER_TYPE&) TRAIT::TCNT = 0;
		// Set timer interrupt mode (set interrupt on OCRnA compare match)
		(volatile uint8_t&) TRAIT::TIMSK = _BV(OCIE0A);
	}
	inline void end()
	{
		synchronized _end();
	}
	inline void _end()
	{
		// Stop timer
		(volatile uint8_t&) TRAIT::TCCRB = 0;
		// Clear timer interrupt mode (set interrupt on OCRnA compare match)
		(volatile uint8_t&) TRAIT::TIMSK = 0;
	}
	
private:
	static constexpr uint32_t prescaler_quotient(TIMER_PRESCALER p, uint32_t us)
	{
		return (F_CPU / 1000000UL * us) / _BV(uint8_t(p));
	}

	static constexpr uint32_t prescaler_remainder(TIMER_PRESCALER p, uint32_t us)
	{
		return (F_CPU / 1000000UL * us) % _BV(uint8_t(p));
	}

	static constexpr bool prescaler_is_adequate(uint32_t quotient)
	{
		return quotient > 1 and quotient < TRAIT::MAX_COUNTER;
	}

	static constexpr TIMER_PRESCALER best_prescaler_in_2(TIMER_PRESCALER p1, TIMER_PRESCALER p2, uint32_t us)
	{
		return (!prescaler_is_adequate(prescaler_quotient(p1, us)) ? p2 :
				!prescaler_is_adequate(prescaler_quotient(p2, us)) ? p1 :
				prescaler_remainder(p1, us) < prescaler_remainder(p2, us) ? p1 :
				prescaler_remainder(p1, us) > prescaler_remainder(p2, us) ? p2 :
				prescaler_quotient(p1, us) > prescaler_quotient(p2, us) ? p1 : p2);
	}

	static constexpr TIMER_PRESCALER best_prescaler(const TIMER_PRESCALER* begin, const TIMER_PRESCALER* end, uint32_t us)
	{
		return (begin + 1 == end ? *begin : best_prescaler_in_2(*begin, best_prescaler(begin + 1 , end, us), us));
	}

	template<size_t N>
	static constexpr TIMER_PRESCALER best_prescaler(const TIMER_PRESCALER(&prescalers)[N], uint32_t us)
	{
		return best_prescaler(prescalers, prescalers + N, us);
	}
	
	static void call_back_if_needed()
	{
		if (((volatile uint8_t&) TRAIT::TIFR) & _BV(OCF0A))
			_callback->on_timer();
	}

	static TimerCallback* _callback;
	
	template<Board::Timer, Board::Timer...> friend struct timer_impl::CallbackHandler;
};

template<Board::Timer TIMER>
TimerCallback* Timer<TIMER>::_callback = 0;

namespace timer_impl
{
	template<Board::Timer T0, Board::Timer... TS>
	struct CallbackHandler
	{
		static void callback()
		{
			CallbackHandler<T0>::callback();
			CallbackHandler<TS...>::callback();
		}
	};

	template<Board::Timer T>
	struct CallbackHandler<T>
	{
		static void callback()
		{
			Timer<T>::call_back_if_needed();
		}
	};
}

#endif /* TIMER_HH */
