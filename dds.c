#include <string.h>

#include "dds.h"
#include "sin.h"

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
	return sin_data_len * f / fs;
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

void vss_dds_fill_poly(dds_t* buffer, size_t size, const struct vss_dds_output* output, unsigned* tw_list, size_t tw_num)
{
	memset(buffer, 0, size);

	const unsigned dds_size = get_dds_size(size, output->bits);
	const unsigned dds_word_size = get_dds_word_size(output->bits);

	unsigned phase[tw_num];
	unsigned n;

	memset(phase, 0, sizeof(phase));

	for(n = 0; n < dds_size; n++) {

		unsigned m;
		int acc = 0;

		for(m = 0; m < tw_num; m++) {
			//printf("ph = %u\n", phase[m]);
			acc += sin_data[phase[m]];
			//printf("acc = %d\n", (int) acc);
			phase[m] = (phase[m] + tw_list[m]) % sin_data_len;
		}

		acc /= tw_num;
		//printf("%d\n", (int) acc);
		
		int i;
		if(acc > 64) {
			i = 0;
		} else if(acc > 0) {
			i = 1;
		} else if(acc > -63) {
			i = 2;
		} else {
			i = 3;
		}

		//printf("%d %d\n", acc, i);
		
		const unsigned ow = output->words[i];

		const unsigned p = n / dds_word_size;
		const unsigned w = n % dds_word_size * output->bits;

		buffer[p] |= (ow << w);
	}
}
