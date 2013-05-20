#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

#include "dds.h"
#include "event.h"
#include "wavetable.h"

//static const unsigned output_words[] = { 0, 85, 170, 255 };
static const unsigned output_words[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255 };

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
