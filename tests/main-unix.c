#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

#include "dds.h"
#include "event.h"
#include "sin.h"

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
		uint32_t time = 0;

		unsigned cur_event = 0;

		while(cur_event < events_num) {

			unsigned cur_time = events[cur_event].time;

			int n;
			for(n = 0; n < (cur_time - time)*fs/1000.0; n++) {
				output_byte();
			}
			time = cur_time;

			while(events[cur_event].time == cur_time && cur_event < events_num) { 
				unsigned tw = vss_dds_get_tuning_word(fs, events[cur_event].freq);

				unsigned i = 0;
				if(events[cur_event].type) {
					while(tw_list[i] != 0 && i < max_channels -1) i++;
					tw_list[i] = tw;
				} else {
					while(tw_list[i] != tw && i < max_channels -1) i++;
					tw_list[i] = 0;
				}

				fprintf(stderr, "ev %d %u -> %u %u %u\n",
						i, tw, tw_list[0], tw_list[1], tw_list[2]);

				cur_event++;
			}

			vss_dds_fill_poly(dds_buffer, sizeof(dds_buffer), &output, tw_list, max_channels);
		}
	}
}
