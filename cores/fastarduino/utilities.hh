#ifndef UTILITIES_HH
#define	UTILITIES_HH

#include <avr/io.h>

class ClearInterrupt
{
public:
	ClearInterrupt() __attribute__((always_inline)) : _sreg(SREG)
	{
		cli();
	}
	~ClearInterrupt() __attribute__((always_inline))
	{
		SREG = _sreg;
	}

private:
	const uint8_t _sreg;
};

inline void set_ioreg_bit(uint8_t IOREG, uint8_t BIT)
{
	asm volatile("SBI %[IOREG], %[BIT] \n\t"::[IOREG] "I" (IOREG), [BIT] "I" (BIT));
}

inline void clear_ioreg_bit(uint8_t IOREG, uint8_t BIT)
{
	asm volatile("CBI %[IOREG], %[BIT] \n\t"::[IOREG] "I" (IOREG), [BIT] "I" (BIT));
}

inline bool ioreg_bit_value(uint8_t IOREG, uint8_t BIT)
{
	bool result = false;
	asm volatile(
		"SBIC %[IOREG], %[BIT] \n\t"
		"LDI %[RESULT], 1\n\t"			// Bit is set, set result value accordingly
		:[RESULT] "+r" (result)
		:[IOREG] "I" (IOREG), [BIT] "I" (BIT)
	);
	return result;
}

inline void set_ioreg_byte(uint8_t IOREG, uint8_t value)
{
	asm volatile("OUT %[IOREG], %[VALUE]\n\t"::[IOREG] "I" (IOREG), [VALUE] "r" (value));
}

inline uint8_t get_ioreg_byte(uint8_t IOREG)
{
	uint8_t value = 0;
	asm volatile("IN %[VALUE], %[IOREG]\n\t":[VALUE] "+r" (value):[IOREG] "I" (IOREG));
	return value;
}

#endif	/* UTILITIES_HH */

