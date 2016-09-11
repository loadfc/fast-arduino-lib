/*
 * Hardware UART test sample.
 */

#include <avr/interrupt.h>
#include <util/delay.h>

#include <fastarduino/uart.hh>

// Define vectors we need in the example
USE_UART0()

// Buffers for UART
static const uint8_t INPUT_BUFFER_SIZE = 64;
static const uint8_t OUTPUT_BUFFER_SIZE = 64;
static char input_buffer[INPUT_BUFFER_SIZE];
static char output_buffer[OUTPUT_BUFFER_SIZE];

int main()
{
	// Enable interrupts at startup time
	sei();
	
	// Start UART
	UART<Board::USART::USART0> uart{input_buffer, output_buffer};
	uart.begin(115200);
//	uart.begin(230400);
	InputBuffer& in = uart.in();
//	FormattedInput<InputBuffer> in = uart.fin();
	FormattedOutput<OutputBuffer> out = uart.fout();

	// Event Loop
	while (true)
	{
		out.puts("Enter a letter: ");
		out.flush();
//		int input = in.get();
		int input = ::get(in);
		out.put(input);
		out.put('\n');
		out << (char) input << ' ' << dec << input << ' ' << oct << input << ' ' << hex << input << ' ' << bin << input << endl;
		out.flush();
		_delay_ms(1000.0);
	}
}
