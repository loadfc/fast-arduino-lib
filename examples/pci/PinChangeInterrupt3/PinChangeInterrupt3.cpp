/*
 * Pin Change Interrupt example. Multiple PCI.
 * This program shows usage of Pin Change Interrupt (PCI) FastArduino support to light LEDs when buttons are pushed.
 * This sample uses a handler called by PCINT vector.
 * Concretely, the example takes PCI input from 3 switches and lights one of 3 LEDs (1 LED per button).
 * It also toggles a 4th LED on each PCI interrupt.
 * 
 * This is similar to PinChangeInterrupt2 except it used IOPort instead of IOPin,
 * which is more size-efficient.
 * 
 * Wiring:
 * - on ATmega328P based boards (including Arduino UNO):
 *   - D1, D3, D5, D7 (port D) branch 4 LED (in series with 330 Ohm resistors to limit current) connected to ground
 *   - D14, D16, D17 (port C, ADC0, ADC2, ADC3) branch 3 buttons connected to ground
 * - on Arduino MEGA:
 *   - D22-D25 (port A) branch 4 LED (in series with 330 Ohm resistors to limit current) connected to ground
 *   - D53-D51 (port B) branch 3 buttons connected to ground
 * - on ATtinyX4 based boards:
 *   - D0-D3 (port A) branch 4 LED (in series with 330 Ohm resistors to limit current) connected to ground
 *   - D8-D10 (port B) branch 3 buttons connected to ground
 */

#include <avr/interrupt.h>

#include <fastarduino/FastIO.hh>
#include <fastarduino/PCI.hh>
#include <fastarduino/power.hh>

#if defined(ARDUINO_UNO) || defined(BREADBOARD_ATMEGA328P)
static constexpr const uint8_t LED1 = _BV(Board::BIT(Board::DigitalPin::D1));
static constexpr const uint8_t LED2 = _BV(Board::BIT(Board::DigitalPin::D3));
static constexpr const uint8_t LED3 = _BV(Board::BIT(Board::DigitalPin::D5));
static constexpr const uint8_t LED4 = _BV(Board::BIT(Board::DigitalPin::D7));
static constexpr const Board::Port LED_PORT = Board::Port::PORT_D;
static constexpr const Board::DigitalPin SWITCH1 = Board::DigitalPin::D14;
static constexpr const Board::DigitalPin SWITCH2 = Board::DigitalPin::D16;
static constexpr const Board::DigitalPin SWITCH3 = Board::DigitalPin::D17;
static constexpr const uint8_t SW1 = _BV(Board::BIT(SWITCH1));
static constexpr const uint8_t SW2 = _BV(Board::BIT(SWITCH2));
static constexpr const uint8_t SW3 = _BV(Board::BIT(SWITCH3));
static constexpr const Board::Port SWITCH_PORT = Board::Port::PORT_C;
// Define vectors we need in the example
USE_PCI1()

#elif defined (ARDUINO_MEGA)
static constexpr const uint8_t LED1 = _BV(Board::BIT(Board::DigitalPin::D22));
static constexpr const uint8_t LED2 = _BV(Board::BIT(Board::DigitalPin::D23));
static constexpr const uint8_t LED3 = _BV(Board::BIT(Board::DigitalPin::D24));
static constexpr const uint8_t LED4 = _BV(Board::BIT(Board::DigitalPin::D25));
static constexpr const Board::Port LED_PORT = Board::Port::PORT_A;
static constexpr const Board::DigitalPin SWITCH1 = Board::DigitalPin::D53;
static constexpr const Board::DigitalPin SWITCH2 = Board::DigitalPin::D52;
static constexpr const Board::DigitalPin SWITCH3 = Board::DigitalPin::D51;
static constexpr const uint8_t SW1 = _BV(Board::BIT(SWITCH1));
static constexpr const uint8_t SW2 = _BV(Board::BIT(SWITCH2));
static constexpr const uint8_t SW3 = _BV(Board::BIT(SWITCH3));
static constexpr const Board::Port SWITCH_PORT = Board::Port::PORT_B;
// Define vectors we need in the example
USE_PCI0()

#elif defined (BREADBOARD_ATTINYX4)
static constexpr const uint8_t LED1 = _BV(Board::BIT(Board::DigitalPin::D0));
static constexpr const uint8_t LED2 = _BV(Board::BIT(Board::DigitalPin::D1));
static constexpr const uint8_t LED3 = _BV(Board::BIT(Board::DigitalPin::D2));
static constexpr const uint8_t LED4 = _BV(Board::BIT(Board::DigitalPin::D3));
static constexpr const Board::Port LED_PORT = Board::Port::PORT_A;
static constexpr const Board::DigitalPin SWITCH1 = Board::DigitalPin::D8;
static constexpr const Board::DigitalPin SWITCH2 = Board::DigitalPin::D9;
static constexpr const Board::DigitalPin SWITCH3 = Board::DigitalPin::D10;
static constexpr const uint8_t SW1 = _BV(Board::BIT(SWITCH1));
static constexpr const uint8_t SW2 = _BV(Board::BIT(SWITCH2));
static constexpr const uint8_t SW3 = _BV(Board::BIT(SWITCH3));
static constexpr const Board::Port SWITCH_PORT = Board::Port::PORT_B;
// Define vectors we need in the example
USE_PCI1()

#else
#error "Current target is not yet supported!"
#endif

class PinChangeHandler: public ExternalInterruptHandler
{
public:
	PinChangeHandler():_switches{0x00, 0xFF}, _leds{0xFF} {}
	
	virtual bool on_pin_change() override
	{
		uint8_t switches = _switches.get_PIN();
		uint8_t leds = (_leds.get_PIN() & LED4) ^ LED4;
		if (!(switches & SW1)) leds |= LED1;
		if (!(switches & SW2)) leds |= LED2;
		if (!(switches & SW3)) leds |= LED3;
		_leds.set_PORT(leds);
		return true;
	}
	
private:
	FastPort<SWITCH_PORT> _switches;
	FastPort<LED_PORT> _leds;	
};

int main()
{
	// Enable interrupts at startup time
	sei();

	PinChangeHandler handler;
	PCIType<SWITCH1>::TYPE pci{&handler};
	
	pci.enable_pins(SW1 | SW2 | SW3);
	pci.enable();

	// Event Loop
	while (true)
	{
		Power::sleep(Board::SleepMode::POWER_DOWN);
	}
}
