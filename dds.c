#include <string.h>

#include "dds.h"
#include "wavetable.h"

/** @brief How many output words in a dds_t value? */
static unsigned get_dds_word_size(unsigned bits)
{
	return sizeof(dds_t) * 8 / bits;
}

/** @brief How many output words in size bytes? */
static unsigned get_dds_size(size_t size, unsigned bits)
{
	return size * 8 / bits;
}

/** @brief Calculate tuning word.
 *
 * @param bits Number of bits per output word.
 * @param fs Sampling frequency in hertz.
 * @param f Desired synthesizer frequency. */
unsigned vss_dds_get_tuning_word(float fs, float f)
{
	return wavetable_len * f / fs;
}

/** @brief Fill direct digital synthesis buffer.
 *
 * @param buffer Pointer to the buffer to fill.
 * @param size Size of the buffer array in bytes.
 * @param bits Number of bits per output word.
 * @param tw Tuning word. */
void vss_dds_fill(dds_t* buffer, size_t size, const struct vss_dds_output* output, unsigned tw)
{
	memset(buffer, 0, size);

	const unsigned dds_size = get_dds_size(size, output->bits);
	const unsigned dds_word_size = get_dds_word_size(output->bits);

	unsigned phase = 0, n;
	for(n = 0; n < dds_size; n++) {

		const unsigned p = n / dds_word_size;
		const unsigned w = n % dds_word_size * output->bits;

		const unsigned i = phase * output->size / dds_size;
		const unsigned ow = output->words[i];

		buffer[p] |= (ow << w);

		phase = (phase + tw) % dds_size;
	}
}

unsigned vss_dds_quant(int acc, unsigned ch_num, unsigned bits)
{
	int q = (acc + 256 * ((int) ch_num)/2 - 1) * ((int) bits) / ((int) ch_num) / 256;
	if(q < 0) {
		return 0;
	} else if(q > ((int) bits - 1)) {
		return bits - 1;
	} else {
		return q;
	}
}

void vss_dds_fill_poly(dds_t* buffer, size_t size, unsigned* tw_list, int* attn_list, size_t tw_num, unsigned dsmul)
{
	unsigned phase[tw_num];
	unsigned p;

	memset(phase, 0, sizeof(phase));

	p = 0;
	while(p < size) {
		unsigned m;
		int acc = 0;

		for(m = 0; m < tw_num; m++) {
			if(tw_list[m] > 0) {
				//printf("ph = %u\n", phase[m]);
				acc += wavetable[phase[m]]; // / attn_list[m];
				//printf("acc = %d\n", (int) acc);
				phase[m] = (phase[m] + tw_list[m]) % wavetable_len;
			}
		}

		buffer[p] = vss_dds_quant(acc, 4, dsmul+1);
		p++;
	}
}
