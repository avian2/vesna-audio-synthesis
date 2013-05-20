#ifndef HAVE_EVENT_H
#define HAVE_EVENT_H

#include <stdint.h>
#include <stddef.h>

#include "dds.h"

struct event {
	uint32_t time;
	int type;
	float freq;
	int attn;
};

extern const struct event events[];

extern const size_t events_num;

extern const unsigned max_channels;

struct sequencer {
	unsigned cur_event;
	unsigned cur_time;

	float fs;

	unsigned* tw_list;
	int* attn_list;
};

void sequencer_init(struct sequencer* seq, float fs);

void sequencer_next(struct sequencer* seq, dds_t* dds_buffer, size_t dds_buffer_len, unsigned dsmul);
#endif
