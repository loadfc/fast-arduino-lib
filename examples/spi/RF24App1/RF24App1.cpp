/*
 * NRF24L01+ test example.
 */

#include <avr/interrupt.h>
#include <util/delay.h>

#include <fastarduino/FastIO.hh>
#include <fastarduino/RTT.hh>
#include <fastarduino/NRF24L01.hh>

static const constexpr Board::DigitalPin PIN_CONFIG = Board::DigitalPin::D7;

static const constexpr Board::ExternalInterruptPin PIN_IRQ = Board::ExternalInterruptPin::EXT0;
static const constexpr Board::DigitalPin PIN_CSN = Board::DigitalPin::D8;
static const constexpr Board::DigitalPin PIN_CE = Board::DigitalPin::D9;

static const constexpr Board::Timer RTT_TIMER = Board::Timer::TIMER2;

static const uint16_t NETWORK = 0xFFFF;
static const uint8_t MASTER = 0x01;
static const uint8_t SLAVE = 0x02;

// Define vectors we need in the exampleUSE_UART0();
USE_EMPTY_INT0();
USE_RTT_TIMER2();

static bool is_master()
{
	FastPin<PIN_CONFIG> config{PinMode::INPUT_PULLUP};
	return config.value();
}

//TODO add UART for master (only)
int main()
{
	// Enable interrupts at startup time
	sei();
	
	bool master = is_master();
	uint8_t self_device = master ? MASTER : SLAVE;
	uint8_t other_device = master ? SLAVE : MASTER;

	FastPin<Board::DigitalPin::LED> led{PinMode::OUTPUT, false};
	RTT<Board::Timer::TIMER2> rtt;
	rtt.begin();

	NRF24L01<Board::ExternalInterruptPin::EXT0> rf{NETWORK, self_device, PIN_CSN, PIN_CE};
	rf.begin();
	
	// Event Loop
	uint8_t sent_port = 0;
	bool send = master;
	while (true)
	{
		if (send)
		{
			// Send
			if (rf.send(other_device, sent_port, 0, 0) < 0)
			{
				
			}
			else
			{

			}
			++sent_port;
		}
		else
		{
			// Receive
			uint8_t src, port;
			if (rf.recv(src, port, 0, 0) < 0)
			{
				
			}
			else
			{
				
			}
		}
		send = !send;
		Time::delay_ms(1000);
	}
}
