/*
 * This program is just for personal experiments here on AVR features and C++ stuff
 * to check compilation and link of FastArduino port and pin API.
 * It does not do anything interesting as far as hardware is concerned.
 */

// Imperial march tones thanks:
// http://processors.wiki.ti.com/index.php/Playing_The_Imperial_March

// Example of square wave generation, using CTC mode and COM toggle
#include <fastarduino/time.h>
#include <fastarduino/devices/tone_player.h>

// Board-dependent settings
static constexpr const board::Timer NTIMER = board::Timer::TIMER1;
static constexpr const board::DigitalPin OUTPUT = board::PWMPin::D9_PB1_OC1A;

using devices::audio::Tone;
using namespace devices::audio::SpecialTone;
using GENERATOR = devices::audio::ToneGenerator<NTIMER, OUTPUT>;
using PLAYER = devices::audio::TonePlayer<NTIMER, OUTPUT>;
using QTONEPLAY = PLAYER::QTonePlay;

static const QTONEPLAY music[] PROGMEM =
{
	// First part
	QTONEPLAY{Tone::A1, 500},
	QTONEPLAY{Tone::A1, 500},
	QTONEPLAY{Tone::A1, 500},
	QTONEPLAY{Tone::F1, 350},
	QTONEPLAY{Tone::C2, 150},
	QTONEPLAY{Tone::A1, 500},
	QTONEPLAY{Tone::F1, 350},
	QTONEPLAY{Tone::C2, 150},
	QTONEPLAY{Tone::A1, 650},
	QTONEPLAY{Tone::SILENCE, 150},

	// Second part
	QTONEPLAY{Tone::E2, 500},
	QTONEPLAY{Tone::E2, 500},
	QTONEPLAY{Tone::E2, 500},
	QTONEPLAY{Tone::F2, 350},
	QTONEPLAY{Tone::C2, 150},
	QTONEPLAY{Tone::Gs1, 500},
	QTONEPLAY{Tone::F1, 350},
	QTONEPLAY{Tone::C2, 150},
	QTONEPLAY{Tone::A1, 650},
	QTONEPLAY{Tone::SILENCE, 150},

	// Third part (repeated once)
	QTONEPLAY{REPEAT_START},
	QTONEPLAY{Tone::A2, 500},
	QTONEPLAY{Tone::A1, 300},
	QTONEPLAY{Tone::A1, 150},
	QTONEPLAY{Tone::A2, 400},
	QTONEPLAY{Tone::Gs2, 200},
	QTONEPLAY{Tone::G2, 200},
	QTONEPLAY{Tone::Fs2, 125},
	QTONEPLAY{Tone::F2, 125},
	QTONEPLAY{Tone::Fs2, 250},
	QTONEPLAY{Tone::SILENCE, 250},

	QTONEPLAY{Tone::As1, 250},
	QTONEPLAY{Tone::Ds2, 400},
	QTONEPLAY{Tone::D2, 200},
	QTONEPLAY{Tone::Cs2, 200},
	QTONEPLAY{Tone::C2, 125},
	QTONEPLAY{Tone::B1, 125},
	QTONEPLAY{Tone::C2, 250},
	QTONEPLAY{Tone::SILENCE, 250},

	QTONEPLAY{Tone::F1, 125},
	QTONEPLAY{Tone::Gs1, 500},
	QTONEPLAY{Tone::F1, 375},
	QTONEPLAY{Tone::A1, 125},
	QTONEPLAY{Tone::C2, 500},
	QTONEPLAY{Tone::A1, 375},
	QTONEPLAY{Tone::C2, 125},
	QTONEPLAY{Tone::E2, 650},
	QTONEPLAY{Tone::SILENCE, 250},
	QTONEPLAY{REPEAT_END, 1},

	QTONEPLAY{END, 0}
};

int main() __attribute__((OS_main));
int main()
{
	sei();
	time::delay_ms(5000);

	GENERATOR generator;
	PLAYER player{generator};
	player.play_flash(music);
}
