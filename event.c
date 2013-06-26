#include <stdio.h>

#include "event.h"

void sequencer_init(struct sequencer* seq, float fs)
{
	seq->cur_event = 0;
	seq->cur_time = 0;

	seq->fs = fs;

	seq->tw_list = calloc(max_channels, sizeof(*seq->tw_list));
	seq->attn_list = calloc(max_channels, sizeof(*seq->attn_list));
}

void sequencer_next(struct sequencer* seq, dds_t* dds_buffer, size_t dds_buffer_len, unsigned dsmul)
{
	while(events[seq->cur_event].time == seq->cur_time && seq->cur_event < events_num) {
		unsigned tw = vss_dds_get_tuning_word(seq->fs, events[seq->cur_event].freq);

		unsigned i = 0;
		if(events[seq->cur_event].type) {
			while(seq->tw_list[i] != 0 && i < max_channels -1) i++;
			seq->tw_list[i] = tw;
			seq->attn_list[i] = events[seq->cur_event].attn;
		} else {
			while(seq->tw_list[i] != tw && i < max_channels -1) i++;
			seq->tw_list[i] = 0;
		}

		//fprintf(stderr, "ev %u -> %u\n", tw, i);
		seq->cur_event++;
	}

	vss_dds_fill_poly(dds_buffer, dds_buffer_len, seq->tw_list, seq->attn_list, max_channels, dsmul, 1);
}
