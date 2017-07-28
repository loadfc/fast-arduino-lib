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
 * Empty version of streams API, useful when you want to introduce traces that
 * you can easily disable without adding preprocessor conditional compilation
 * everywhere traces are output.
 * Note that you shall not include "empty_streams.h" and "streams.h" in the same
 * source file.
 */
#ifndef EMPTYSTREAMS_HH
#define	EMPTYSTREAMS_HH

#include <stddef.h>
#include "flash.h"
#include "utilities.h"

namespace streams
{
	/**
	 * Implements an empty formatted output that does nothing.
	 * It can be used everywhere a `streams::FormattedOutput` is expected but 
	 * produces no code at all.
	 * The following example shows how to add traces that can be disabled at 
	 * compile time by defining a specific macro:
	 * @code
	 * // Include the right header
	 * #ifdef NO_TRACE
	 * #include <fastarduino/empty_streams.h>
	 * #else
	 * #include <fastarduino/streams.h>
	 * #endif
	 * //...
	 * // Instantiate the right output
	 * #ifdef NO_TRACE
	 * streams::EmptyOutput trace;
	 * #else
	 * // We use UART for tracing
	 * static char output_buffer[64];
	 * serial::hard::UATX<board::USART::USART0> uatx{output_buffer};
	 * auto trace = uatx.fout();
	 * #endif
	 * //...
	 * // Directly use trace anywhere, this won't generate any code if `NO_TRACE`
	 * // was defined at compile time
	 * void f(int value)
	 * {
	 *     //... do something
	 *     trace << F("DEBUG") << value << streams::endl;
	 *     //... do something else
	 * }
	 * @endcode
	 */
	class EmptyOutput
	{
	public:
		/// @cond notdocumented
		EmptyOutput() {}

		void flush() {}
		void put(char c UNUSED, bool call_on_put UNUSED = true) {}
		void put(const char* content UNUSED, size_t size UNUSED) {}
		void puts(const char* str UNUSED) {}
		void puts(const flash::FlashStorage* str UNUSED) {}

		EmptyOutput& operator << (char c UNUSED)
		{
			return *this;
		}
		EmptyOutput& operator << (const char* s UNUSED)
		{
			return *this;
		}
		EmptyOutput& operator << (int v UNUSED)
		{
			return *this;
		}
		EmptyOutput& operator << (unsigned int v UNUSED)
		{
			return *this;
		}
		EmptyOutput& operator << (long v UNUSED)
		{
			return *this;
		}
		EmptyOutput& operator << (unsigned long v UNUSED)
		{
			return *this;
		}
		EmptyOutput& operator << (double v UNUSED)
		{
			return *this;
		}

		typedef void (*Manipulator)(EmptyOutput&);
		EmptyOutput& operator << (Manipulator f UNUSED)
		{
			return *this;
		}
		/// @endcond
	};

	/// @cond notdocumented
	inline void bin(EmptyOutput& stream UNUSED) {}
	inline void oct(EmptyOutput& stream UNUSED) {}
	inline void dec(EmptyOutput& stream UNUSED) {}
	inline void hex(EmptyOutput& stream UNUSED) {}
	inline void flush(EmptyOutput& stream UNUSED) {}
	inline void endl(EmptyOutput& stream UNUSED) {}
	/// @endcond
}

#endif	/* EMPTYSTREAMS_HH */
/// @endcond
