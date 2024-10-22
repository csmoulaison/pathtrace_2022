#pragma once
#include <stdint.h>

struct Color {
	union {
		struct {
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		} channels;
		uint32_t value;
	};
};

