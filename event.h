#ifndef HAVE_EVENT_H
#define HAVE_EVENT_H

#include <stdint.h>
#include <stddef.h>

struct event {
	uint32_t time;
	int type;
	float freq;
};

extern const struct event events[];

extern const size_t events_num;

#endif
