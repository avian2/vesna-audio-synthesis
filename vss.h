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
/** @file
 * @brief Top-level spectrum sensor definitions.
 */
#ifndef HAVE_VSS_H
#define HAVE_VSS_H

/** @name Function result codes. */

/** @{ */

/** @brief Success. */
#define VSS_OK		0
/** @brief Unspecified error. */
#define VSS_ERROR	-1
/** @brief Stop iteration. */
#define VSS_STOP	-2
/** @brief Not enough memory or too many iteration requested. */
#define VSS_TOO_MANY	-3

/** @} */

#endif
