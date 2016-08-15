#ifndef FASTIO_HH
#define	FASTIO_HH

#include "utilities.hh"
#include "Board.hh"

// This class maps to a PORT and handles it all 8 bits at a time
// SRAM size supposed to be 0
template<uint8_t PIN>
class FastPort
{
public:
	FastPort() {}
	FastPort(uint8_t ddr, uint8_t port = 0) __attribute__((always_inline))
	{
		set_DDR(ddr);
		set_PORT(port);
	}
	void set_PORT(uint8_t port) __attribute__((always_inline))
	{
		set_ioreg_byte(_PORT, port);
	}
	uint8_t get_PORT() __attribute__((always_inline))
	{
		return get_ioreg_byte(_PORT);
	}
	void set_DDR(uint8_t ddr) __attribute__((always_inline))
	{
		set_ioreg_byte(_DDR, ddr);
	}
	uint8_t get_DDR() __attribute__((always_inline))
	{
		return get_ioreg_byte(_DDR);
	}
	void set_PIN(uint8_t pin) __attribute__((always_inline))
	{
		set_ioreg_byte(_PIN, pin);
	}
	uint8_t get_PIN() __attribute__((always_inline))
	{
		return get_ioreg_byte(_PIN);
	}

private:
	static const constexpr uint8_t _PIN = PIN;
	static const constexpr uint8_t _DDR = _PIN + 1;
	static const constexpr uint8_t _PORT = _PIN + 2;
};

enum class PinMode
{
	INPUT,
	INPUT_PULLUP,
	OUTPUT,
};

// This class maps to a specific pin
// SRAM size supposed to be 0
template<Board::DigitalPin DPIN>
class FastPin
{
public:
	FastPin(PinMode mode, bool value = false)
	{
		if (mode == PinMode::OUTPUT)
			set_ioreg_bit(_DDR, _BIT);
		else
			clear_ioreg_bit(_DDR, _BIT);
		if (value || mode == PinMode::INPUT_PULLUP)
			set_ioreg_bit(_PORT, _BIT);
		else
			clear_ioreg_bit(_PORT, _BIT);
	}
	void set() __attribute__((always_inline))
	{
		set_ioreg_bit(_PORT, _BIT);
	}
	void clear() __attribute__((always_inline))
	{
		clear_ioreg_bit(_PORT, _BIT);
	}
	void toggle() __attribute__((always_inline))
	{
		set_ioreg_bit(_PIN, _BIT);
	}
	bool value() __attribute__((always_inline))
	{
		return ioreg_bit_value(_PIN, _BIT);
	}
	
private:
	static const constexpr REGISTER _PIN = Board::PIN_REG(DPIN);
	static const constexpr REGISTER _DDR = Board::DDR_REG(DPIN);
	static const constexpr REGISTER _PORT = Board::PORT_REG(DPIN);
	static const constexpr uint8_t _BIT = Board::BIT(DPIN);
};


#endif	/* FASTIO_HH */
