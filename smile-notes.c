#include "event.h"

const struct event events[] = {
	{ .time = 0, .type = 1, .freq = 622.253967, .attn = 1 },
	{ .time = 0, .type = 1, .freq = 77.781746, .attn = 1 },
	{ .time = 178132, .type = 0, .freq = 123.470825 },
	{ .time = 178132, .type = 0, .freq = 164.813778 }
};

const size_t events_num = 3418;

const unsigned max_channels = 6;
