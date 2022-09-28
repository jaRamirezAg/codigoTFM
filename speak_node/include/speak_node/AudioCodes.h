#ifndef AUDIO_CODES_H
#define AUDIO_CODES_H

#include <stdint.h>

enum class AudioCode: uint8_t
{
	ok_google = 0,
	alexa = 1,
	echo = 2,
    noise =	3,
	beep = 4
};


#endif // AUDIO_CODES_H