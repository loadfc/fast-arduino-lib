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

#ifndef PWM_H
#define PWM_H

#include "boards/board.h"
#include "boards/board_traits.h"
#include "gpio.h"
#include "timer.h"

namespace analog
{
	using timer::TimerOutputMode;
	
	template<board::DigitalPin PIN, bool PULSED = false>
	class PWMOutput
	{
		using TRAIT = board_traits::PWMPin_trait<PIN>;
		using TIMER_TRAIT = board_traits::Timer_trait<TRAIT::TIMER>;
		
	public:
		static constexpr const uint8_t COM = TRAIT::COM;
		using TIMER = timer::Timer<TRAIT::TIMER>;
		
		PWMOutput(TIMER& timer, TimerOutputMode output_mode = TimerOutputMode::NON_INVERTING):_timer{timer}
		{
			static_assert(TRAIT::HAS_PWM, "PIN must be a PWM pin");
			// Initialize pin as output
			gpio::FastPinType<PIN>::set_mode(gpio::PinMode::OUTPUT);
			if (TIMER_TRAIT::IS_16BITS || !PULSED)
				// Set com mode for pin
				_timer.template set_output_mode<COM>(output_mode);
		}
		
		inline void set_output_mode(TimerOutputMode output_mode)
		{
			if (TIMER_TRAIT::IS_16BITS || !PULSED)
				_timer.template set_output_mode<COM>(output_mode);
		}

		using TYPE = typename TIMER_TRAIT::TYPE;
		static constexpr const TYPE MAX = TIMER_TRAIT::MAX_PWM;
		
		inline void set_duty(TYPE duty)
		{
			_timer.template set_max<COM>(duty);
		}

	private:
		TIMER& _timer;
	};
}

#endif /* PWM_H */

