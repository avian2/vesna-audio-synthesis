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
#ifndef HAVE_CC_H
#define HAVE_CC_H

#include <stdint.h>

/* Configuration Registers */
#define CC_REG_IOCFG2           0x00        // GDO2 output pin configuration
#define CC_REG_IOCFG1           0x01        // GDO1 output pin configuration
#define CC_REG_IOCFG0           0x02        // GDO0 output pin configuration
#define CC_REG_FIFOTHR          0x03        // RX FIFO and TX FIFO thresholds
#define CC_REG_SYNC1            0x04        // Sync word, high byte
#define CC_REG_SYNC0            0x05        // Sync word, low byte
#define CC_REG_PKTLEN           0x06        // Packet length
#define CC_REG_PKTCTRL1         0x07        // Packet automation control
#define CC_REG_PKTCTRL0         0x08        // Packet automation control
#define CC_REG_ADDR             0x09        // Device address
#define CC_REG_CHANNR           0x0A        // Channel number
#define CC_REG_FSCTRL1          0x0B        // Frequency synthesizer control
#define CC_REG_FSCTRL0          0x0C        // Frequency synthesizer control
#define CC_REG_FREQ2            0x0D        // Frequency control word, high byte
#define CC_REG_FREQ1            0x0E        // Frequency control word, middle byte
#define CC_REG_FREQ0            0x0F        // Frequency control word, low byte
#define CC_REG_MDMCFG4          0x10        // Modem configuration
#define CC_REG_MDMCFG3          0x11        // Modem configuration
#define CC_REG_MDMCFG2          0x12        // Modem configuration
#define CC_REG_MDMCFG1          0x13        // Modem configuration
#define CC_REG_MDMCFG0          0x14        // Modem configuration
#define CC_REG_DEVIATN          0x15        // Modem deviation setting
#define CC_REG_MCSM2            0x16        // Main Radio Cntrl State Machine config
#define CC_REG_MCSM1            0x17        // Main Radio Cntrl State Machine config
#define CC_REG_MCSM0            0x18        // Main Radio Cntrl State Machine config
#define CC_REG_FOCCFG           0x19        // Frequency Offset Compensation config
#define CC_REG_BSCFG            0x1A        // Bit Synchronization configuration
#define CC_REG_AGCCTRL2         0x1B        // AGC control
#define CC_REG_AGCCTRL1         0x1C        // AGC control
#define CC_REG_AGCCTRL0         0x1D        // AGC control
#define CC_REG_WOREVT1          0x1E        // High byte Event 0 timeout
#define CC_REG_WOREVT0          0x1F        // Low byte Event 0 timeout
#define CC_REG_WORCTRL          0x20        // Wake On Radio control
#define CC_REG_FREND1           0x21        // Front end RX configuration
#define CC_REG_FREND0           0x22        // Front end TX configuration
#define CC_REG_FSCAL3           0x23        // Frequency synthesizer calibration
#define CC_REG_FSCAL2           0x24        // Frequency synthesizer calibration
#define CC_REG_FSCAL1           0x25        // Frequency synthesizer calibration
#define CC_REG_FSCAL0           0x26        // Frequency synthesizer calibration
#define CC_REG_RCCTRL1          0x27        // RC oscillator configuration
#define CC_REG_RCCTRL0          0x28        // RC oscillator configuration
#define CC_REG_FSTEST           0x29        // Frequency synthesizer cal control
#define CC_REG_PTEST            0x2A        // Production test
#define CC_REG_AGCTEST          0x2B        // AGC test
#define CC_REG_TEST2            0x2C        // Various test settings
#define CC_REG_TEST1            0x2D        // Various test settings
#define CC_REG_TEST0            0x2E        // Various test settings
#define CC_REG_RCCTRL1_STATUS   (0x3C | 0xc0)
#define CC_REG_RCCTRL0_STATUS   (0x3D | 0xc0)

/* Status registers */
#define CC_REG_PARTNUM          (0x30 | 0xc0)        // Part number
#define CC_REG_VERSION          (0x31 | 0xc0)        // Current version number
#define CC_REG_FREQEST          (0x32 | 0xc0)       // Frequency offset estimate
#define CC_REG_LQI              (0x33 | 0xc0)        // Demodulator estimate for link quality
#define CC_REG_RSSI             (0x34 | 0xc0)        // Received signal strength indication
#define CC_REG_MARCSTATE        (0x35 | 0xc0)        // Control state machine state
#define CC_REG_WORTIME1         (0x36 | 0xc0)        // High byte of WOR timer
#define CC_REG_WORTIME0         (0x37 | 0xc0)       // Low byte of WOR timer
#define CC_REG_PKTSTATUS        (0x38 | 0xc0)        // Current GDOx status and packet status
#define CC_REG_VCO_VC_DAC       (0x39 | 0xc0)        // Current setting from PLL cal module
#define CC_REG_TXBYTES          (0x3A | 0xc0)        // Underflow and # of bytes in TXFIFO
#define CC_REG_RXBYTES          (0x3B | 0xc0)        // Overflow and # of bytes in RXFIFO

/* Multi byte memory locations */
#define CC_REG_PATABLE          0x3E
#define CC_REG_TXFIFO           0x3F
#define CC_REG_RXFIFO           0x3F

/* Definitions for burst/single access to registers */
#define CC_REG_WRITE_BURST      0x40
#define CC_REG_READ_SINGLE      0x80
#define CC_REG_READ_BURST       0xC0

/* Strobe commands */
#define CC_STROBE_SRES             0x30        // Reset chip.
#define CC_STROBE_SFSTXON          0x31        // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
// If in RX/TX: Go to a wait state where only the synthesizer is
// running (for quick RX / TX turnaround).
#define CC_STROBE_SXOFF            0x32        // Turn off crystal oscillator.
#define CC_STROBE_SCAL             0x33        // Calibrate frequency synthesizer and turn it off
// (enables quick start).
#define CC_STROBE_SRX              0x34        // Enable RX. Perform calibration first if coming from IDLE and
// MCSM0.FS_AUTOCAL=1.
#define CC_STROBE_STX              0x35        // In IDLE state: Enable TX. Perform calibration first if
// MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:
// Only go to TX if channel is clear.
#define CC_STROBE_SIDLE            0x36        // Exit RX / TX, turn off frequency synthesizer and exit
// Wake-On-Radio mode if applicable.
#define CC_STROBE_SAFC             0x37        // Perform AFC adjustment of the frequency synthesizer
#define CC_STROBE_SWOR             0x38        // Start automatic RX polling sequence (Wake-on-Radio)
#define CC_STROBE_SPWD             0x39        // Enter power down mode when CSn goes high.
#define CC_STROBE_SFRX             0x3A        // Flush the RX FIFO buffer.
#define CC_STROBE_SFTX             0x3B        // Flush the TX FIFO buffer.
#define CC_STROBE_SWORRST          0x3C        // Reset real time clock.
#define CC_STROBE_SNOP             0x3D        // No operation. May be used to pad strobe commands to two
// bytes for simpler software.


/* Chip status byte */

/* Bit fields in the chip status byte */
#define CC_STATUS_CHIP_RDYn_BM             0x80
#define CC_STATUS_STATE_BM                 0x70
#define CC_STATUS_FIFO_BYTES_AVAILABLE_BM  0x0F

/* Chip states */
#define CC_STATE_IDLE                      0x00
#define CC_STATE_RX                        0x10
#define CC_STATE_TX                        0x20
#define CC_STATE_FSTXON                    0x30
#define CC_STATE_CALIBRATE                 0x40
#define CC_STATE_SETTLING                  0x50
#define CC_STATE_RX_OVERFLOW               0x60
#define CC_STATE_TX_UNDERFLOW              0x70

/* possible values of MACRSTATE register */
#define CC_MARCSTATE_SLEEP                   0x00
#define CC_MARCSTATE_IDLE                    0x01
#define CC_MARCSTATE_XOFF                    0x02
#define CC_MARCSTATE_VCOON_MC                0x03
#define CC_MARCSTATE_REGON_MC                0x04
#define CC_MARCSTATE_MANCAL                  0x05
#define CC_MARCSTATE_VCOON                   0x06
#define CC_MARCSTATE_REGON                   0x07
#define CC_MARCSTATE_STARTCAL                0x08
#define CC_MARCSTATE_BWBOOST                 0x09
#define CC_MARCSTATE_FS_LOCK                 0x0A
#define CC_MARCSTATE_IFADCON                 0x0B
#define CC_MARCSTATE_ENDCAL                  0x0C
#define CC_MARCSTATE_RX                      0x0D
#define CC_MARCSTATE_RX_END                  0x0E
#define CC_MARCSTATE_RX_RST                  0x0F
#define CC_MARCSTATE_TXRX_SWITCH             0x10
#define CC_MARCSTATE_RXFIFO_OVERFLOW         0x11
#define CC_MARCSTATE_FSTXON                  0x12
#define CC_MARCSTATE_TX                      0x13
#define CC_MARCSTATE_TX_END                  0x14
#define CC_MARCSTATE_RXTX_SWITCH             0x15
#define CC_MARCSTATE_TXFIFO_UNDERFLOW        0x16

/* Other register bit fields */
#define CC_REG_LQI_CRC_OK_BM                   0x80
#define CC_REG_LQI_EST_BM                      0x7F
#define CC_REG_PKTSTATUS_SFD		       0x08

int vss_cc_init(void);
int vss_cc_reset(void);
int vss_cc_read_reg(uint8_t reg, uint8_t* value);
int vss_cc_write_reg(uint8_t reg, uint8_t value);
int vss_cc_strobe(uint8_t strobe);
int vss_cc_wait_state(uint8_t state);

#endif
