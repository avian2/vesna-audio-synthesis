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
#include <libopencm3/stm32/f1/rtc.h>

#include "vss.h"
#include "rtc.h"

int vss_rtc_init(void)
{
	rtc_awake_from_off(LSE);
	rtc_set_prescale_val(15);

	return VSS_OK;
}

int vss_rtc_reset(void)
{
	rtc_set_counter_val(0);

	return VSS_OK;
}

uint32_t vss_rtc_read(void)
{
	uint32_t val = rtc_get_counter_val();
	/* LSE clock is 32768 Hz. Prescaler is set to 16.
	 *
	 *                 rtc_counter * 16
	 * t [ms] = 1000 * ----------------
	 *                       32768
	 */
	return ((long long) val) * 1000 / 2048;
}
