#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

#include "dds.h"
#include "event.h"
#include "wavetable.h"

static const unsigned output_words[] = { 0, 85, 170, 255 };

static const struct vss_dds_output output = {
	.bits = 8,
	.size = sizeof(output_words)/sizeof(*output_words),
	.words = output_words
};

#define DDS_BUFF_SIZE	64*4

dds_t dds_buffer[DDS_BUFF_SIZE];

void output_byte(void)
{
	static uint32_t p = 0, w = 0;

	putchar(dds_buffer[p] >> w & 0x000000FF);

	w += 8;
	if(w == sizeof(dds_t)*8) {
		w = 0;
		p++;
		if(p == DDS_BUFF_SIZE) {
			p = 0;
		}
	}
}

int main(void)
{
	const float fs = 44100.0;

	unsigned tw_list[max_channels];

	memset(tw_list, 0, sizeof(tw_list));
	
	while(1) {
		struct sequencer seq;
		sequencer_init(&seq, fs);

		uint32_t time = 0;

		while(seq.cur_event < events_num) {

			seq.cur_time = events[seq.cur_event].time;

			int n;
			for(n = 0; n < (seq.cur_time - time)*fs/1000.0; n++) {
				output_byte();
			}
			time = seq.cur_time;

			sequencer_next(&seq, &output, dds_buffer, sizeof(dds_buffer));
		}
	}
}
