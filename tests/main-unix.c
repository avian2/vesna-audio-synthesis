#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

#include "dds.h"
#include "event.h"
#include "wavetable.h"

#define DDS_BUFF_SIZE	1024

dds_t dds_buffer[DDS_BUFF_SIZE];

static const unsigned dsmul = 16;

/*
void output_byte(void)
{
	static uint32_t p = 0, w = 1;

	printf("%d\t%d\t", w, dds_buffer[p]);

	if(w > dds_buffer[p]) {
		printf("1\n");
	} else {
		printf("0\n");
	}

	if(w < dsmul) {
		w++;
	} else {
		w = 1;
		p = (p + 1) % DDS_BUFF_SIZE;
	}
}*/

void output_byte(void)
{
	static uint32_t p = 0;

	putchar(((int) dds_buffer[p]) * 255 / dsmul);

	p = (p + 1) % DDS_BUFF_SIZE;
}

int main(void)
{
	const float fs = 400e3/((float)dsmul);
	fprintf(stderr, "fs = %f\n", fs);

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

			sequencer_next(&seq, dds_buffer, sizeof(dds_buffer), dsmul);
		}
	}
}
