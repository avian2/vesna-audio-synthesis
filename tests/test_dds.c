/* Copyright (C) 2013 SensorLab, Jozef Stefan Institute
 * http://sensorlab.ijs.si
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */

/* Author: Tomaz Solc, <tomaz.solc@ijs.si> */
#include <string.h>

#include "unity.h"
#include "dds.h"
#include "wavetable.h"

const unsigned out_1bit_words[] = { 1, 0, 0, 1 };

static const struct vss_dds_output out_1bit = {
	.bits = 1,
	.size = sizeof(out_1bit_words)/sizeof(*out_1bit_words),
	.words = out_1bit_words
};

const unsigned out_2bit_words[] = { 3, 3, 2, 1, 0, 0, 0, 0, 1, 2, 3, 3 };

static const struct vss_dds_output out_2bit = {
	.bits = 2,
	.size = sizeof(out_2bit_words)/sizeof(*out_2bit_words),
	.words = out_2bit_words
};

void setUp(void)
{
}

void tearDown(void)
{
}

void test_get_tuning_word_min(void)
{
	unsigned tw = vss_dds_get_tuning_word(1.0, 0.5);
	TEST_ASSERT_EQUAL(wavetable_len/2, tw);
}

void test_get_tuning_word_max(void)
{
	unsigned tw = vss_dds_get_tuning_word(1.0, 1.0/wavetable_len);
	TEST_ASSERT_EQUAL(1, tw);
}

void test_fill_min_1(void)
{
	dds_t buffer[2];

	vss_dds_fill(buffer, sizeof(buffer), &out_1bit, 1);

	TEST_ASSERT_EQUAL_HEX((dds_t) 0x0F, buffer[0]);
	TEST_ASSERT_EQUAL_HEX((dds_t) 0xF0, buffer[1]);
}

void test_fill_max_1(void)
{
	dds_t buffer[2];

	vss_dds_fill(buffer, sizeof(buffer), &out_1bit, sizeof(buffer)*8/2);

	TEST_ASSERT_EQUAL_HEX((dds_t) 0x55, buffer[0]);
	TEST_ASSERT_EQUAL_HEX((dds_t) 0x55, buffer[1]);
}

void test_fill_min_2(void)
{
	dds_t buffer[8];

	vss_dds_fill(buffer, sizeof(buffer), &out_2bit, 1);

	TEST_ASSERT_EQUAL_HEX((dds_t) 0xFF, buffer[0]);
	TEST_ASSERT_EQUAL_HEX((dds_t) 0xAF, buffer[1]);
	TEST_ASSERT_EQUAL_HEX((dds_t) 0x15, buffer[2]);
	TEST_ASSERT_EQUAL_HEX((dds_t) 0x00, buffer[3]);

	TEST_ASSERT_EQUAL_HEX((dds_t) 0x00, buffer[4]);
	TEST_ASSERT_EQUAL_HEX((dds_t) 0x50, buffer[5]);
	TEST_ASSERT_EQUAL_HEX((dds_t) 0xEA, buffer[6]);
	TEST_ASSERT_EQUAL_HEX((dds_t) 0xFF, buffer[7]);
}

void test_fill_max_2(void)
{
	dds_t buffer[8];

	vss_dds_fill(buffer, sizeof(buffer), &out_2bit, sizeof(buffer)*8/4);

	TEST_ASSERT_EQUAL_HEX((dds_t) 0x33, buffer[0]);
	TEST_ASSERT_EQUAL_HEX((dds_t) 0x33, buffer[1]);
	TEST_ASSERT_EQUAL_HEX((dds_t) 0x33, buffer[2]);
	TEST_ASSERT_EQUAL_HEX((dds_t) 0x33, buffer[3]);

	TEST_ASSERT_EQUAL_HEX((dds_t) 0x33, buffer[4]);
	TEST_ASSERT_EQUAL_HEX((dds_t) 0x33, buffer[5]);
	TEST_ASSERT_EQUAL_HEX((dds_t) 0x33, buffer[6]);
	TEST_ASSERT_EQUAL_HEX((dds_t) 0x33, buffer[7]);
}

void test_fill_poly_zero(void)
{
	dds_t buffer[4];

	unsigned tw = 0;
	int attn = 1;

	vss_dds_fill_poly(buffer, sizeof(buffer), &tw, &attn, 1, 255, 0);

	TEST_ASSERT_EQUAL(127, buffer[0]);
	TEST_ASSERT_EQUAL(127, buffer[1]);
	TEST_ASSERT_EQUAL(127, buffer[2]);
	TEST_ASSERT_EQUAL(127, buffer[3]);
}

void test_fill_poly_max(void)
{
	dds_t buffer[4];

	unsigned tw = wavetable_len/2;
	int attn = 1;

	vss_dds_fill_poly(buffer, sizeof(buffer), &tw, &attn, 1, 255, 0);

	TEST_ASSERT_EQUAL((dds_t) (127 + wavetable[0]), buffer[0]);
	TEST_ASSERT_EQUAL((dds_t) (127 + wavetable[512]), buffer[1]);
	TEST_ASSERT_EQUAL((dds_t) (127 + wavetable[0]), buffer[2]);
	TEST_ASSERT_EQUAL((dds_t) (127 + wavetable[512]), buffer[3]);
}

void test_fill_poly_some(void)
{
	unsigned dsmul = 8;
	unsigned buffer_len = 128;

	dds_t buffer[buffer_len];

	unsigned tw = wavetable_len/32;
	int attn = 1;

	vss_dds_fill_poly(buffer, sizeof(buffer), &tw, &attn, 1, dsmul, 0);

	int words_used[dsmul+1];
	memset(words_used, 0, sizeof(words_used));

	int p;
	for(p = 0; p < buffer_len; p++) {
		words_used[(unsigned) buffer[p]]++;
	}

	for(p = 0; p < dsmul + 1; p++) {
		TEST_ASSERT_TRUE(words_used[p]);
	}
}

void test_fill_poly_two(void)
{
	unsigned dsmul = 8;
	unsigned buffer_len = 256;

	dds_t buffer[buffer_len];

	unsigned tw[] = {
		wavetable_len/32,
		wavetable_len/17 };

	int attn[] = { 1, 1 };

	vss_dds_fill_poly(buffer, sizeof(buffer), tw, attn, 2, dsmul, 0);

	int words_used[dsmul+1];
	memset(words_used, 0, sizeof(words_used));

	int p;
	for(p = 0; p < buffer_len; p++) {
		words_used[(unsigned) buffer[p]]++;
	}

	for(p = 0; p < dsmul + 1; p++) {
		TEST_ASSERT_TRUE(words_used[p]);
	}
}

void test_quantization_1(void)
{
	int acc;
	for(acc = -127; acc < 128; acc++) {

		int i;
		if(acc > 64) {
			i = 3;
		} else if(acc > 0) {
			i = 2;
		} else if(acc > -64) {
			i = 1;
		} else {
			i = 0;
		}

		TEST_ASSERT_EQUAL(i, vss_dds_quant(acc, 1, 4));
	}
}

void test_quantization_2(void)
{
	int acc;
	for(acc = -127*2; acc < 128*2; acc++) {

		int i;
		if(acc > 128) {
			i = 3;
		} else if(acc > 0) {
			i = 2;
		} else if(acc > -128) {
			i = 1;
		} else {
			i = 0;
		}

		TEST_ASSERT_EQUAL(i, vss_dds_quant(acc, 2, 4));
	}
}
