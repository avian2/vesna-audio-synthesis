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

const unsigned out_2bit_words_poly[] = { 0, 1, 2, 3 };

static const struct vss_dds_output out_2bit_poly = {
	.bits = 2,
	.size = sizeof(out_2bit_words_poly)/sizeof(*out_2bit_words_poly),
	.words = out_2bit_words_poly
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

	TEST_ASSERT_EQUAL_HEX(0x0000FFFF, buffer[0]);
	TEST_ASSERT_EQUAL_HEX(0xFFFF0000, buffer[1]);
}

void test_fill_max_1(void)
{
	dds_t buffer[2];

	vss_dds_fill(buffer, sizeof(buffer), &out_1bit, sizeof(buffer)*8/2);

	TEST_ASSERT_EQUAL_HEX(0x55555555, buffer[0]);
	TEST_ASSERT_EQUAL_HEX(0x55555555, buffer[1]);
}

void test_fill_min_2(void)
{
	dds_t buffer[2];

	vss_dds_fill(buffer, sizeof(buffer), &out_2bit, 1);

	TEST_ASSERT_EQUAL_HEX(0x0015AFFF, buffer[0]);
	TEST_ASSERT_EQUAL_HEX(0xFFEA5000, buffer[1]);
}

void test_fill_max_2(void)
{
	dds_t buffer[2];

	vss_dds_fill(buffer, sizeof(buffer), &out_2bit, sizeof(buffer)*8/4);

	TEST_ASSERT_EQUAL_HEX(0x33333333, buffer[0]);
	TEST_ASSERT_EQUAL_HEX(0x33333333, buffer[1]);
}

void test_fill_poly_zero(void)
{
	dds_t buffer[2];

	unsigned tw = 0;
	int attn = 1;

	vss_dds_fill_poly(buffer, sizeof(buffer), &out_2bit_poly, &tw, &attn, 1);

	TEST_ASSERT_EQUAL_HEX(0x55555555, buffer[0]);
	TEST_ASSERT_EQUAL_HEX(0x55555555, buffer[1]);
}

void test_fill_poly_max(void)
{
	dds_t buffer[2];

	unsigned tw = wavetable_len/2;
	int attn = 1;

	vss_dds_fill_poly(buffer, sizeof(buffer), &out_2bit_poly, &tw, &attn, 1);

	TEST_ASSERT_EQUAL_HEX(0x77777777, buffer[0]);
	TEST_ASSERT_EQUAL_HEX(0x77777777, buffer[1]);
}

void test_fill_poly_some(void)
{
	dds_t buffer[2];

	unsigned tw = wavetable_len/32;
	int attn = 1;

	vss_dds_fill_poly(buffer, sizeof(buffer), &out_2bit_poly, &tw, &attn, 1);

	TEST_ASSERT_EQUAL_HEX(0x55555ABF, buffer[0]);
	TEST_ASSERT_EQUAL_HEX(0xFA955555, buffer[1]);
}

void test_fill_poly_two(void)
{
	dds_t buffer[2];

	unsigned tw[] = {
		wavetable_len/32,
		wavetable_len/16 };

	int attn[] = { 1, 1 };

	vss_dds_fill_poly(buffer, sizeof(buffer), &out_2bit_poly, tw, attn, 2);

	TEST_ASSERT_EQUAL_HEX(0x955556AF, buffer[0]);
	TEST_ASSERT_EQUAL_HEX(0xEA55555A, buffer[1]);
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
