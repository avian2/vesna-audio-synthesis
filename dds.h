#ifndef HAVE_DDS_H
#define HAVE_DDS_H

#include <stdlib.h>

typedef char dds_t;

struct vss_dds_output {
	unsigned bits;
	size_t size;

	const unsigned *words;
};

unsigned vss_dds_get_tuning_word(float fs, float f);
void vss_dds_fill(dds_t* buffer, size_t size, const struct vss_dds_output* output, unsigned tw);
void vss_dds_fill_poly(dds_t* buffer, size_t size, unsigned* tw_list, int* attn_list, size_t tw_num, unsigned dsmul, unsigned ampl_corr);
unsigned vss_dds_quant(int acc, unsigned ch_num, unsigned bits);

#endif
