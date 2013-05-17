#include <stdio.h>
#include <errno.h>

#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/f1/scb.h>
#include <libopencm3/stm32/iwdg.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/nvic.h>
#include <libopencm3/stm32/exti.h>

#include "platform/cc.h"
#include "platform/rtc.h"

#include "dds.h"
#include "event.h"
#include "sin.h"

/* Sync word qualifier mode = No preamble/sync 
 * CRC autoflush = false 
 * Channel spacing = 199.813843 
 * Data format = Synchronous serial mode 
 * Data rate = 43.9796 
 * RX filter BW = 210.937500 
 * PA ramping = false 
 * Preamble count = 4 
 * Whitening = false 
 * Address config = No address check 
 * Carrier frequency = 779.999908 
 * Device address = 0 
 * TX power = 0 
 * Manchester enable = false 
 * CRC enable = false 
 * Deviation = 32.958984 
 * Packet length mode = Infinite packet length mode 
 * Packet length = 255 
 * Modulation format = 4-FSK 
 * Base frequency = 779.999908 
 * Modulated = true 
 * Channel number = 0 */
static const uint8_t init_seq[] = {
	CC_REG_IOCFG2,   0x0B,     // GDO2 Output Pin Configuration
	CC_REG_IOCFG0,   0x0C,     // GDO0 Output Pin Configuration
	CC_REG_FIFOTHR,  0x47,     // RX FIFO and TX FIFO Thresholds
	CC_REG_PKTCTRL0, 0x12,     // Packet Automation Control
	CC_REG_FSCTRL1,  0x06,     // Frequency Synthesizer Control
	CC_REG_FREQ2,    0x1C,     // Frequency Control Word, High Byte
	CC_REG_FREQ1,    0xE3,     // Frequency Control Word, Middle Byte
	CC_REG_FREQ0,    0x8E,     // Frequency Control Word, Low Byte
	CC_REG_MDMCFG4,  0x8A,     // Modem Configuration
	CC_REG_MDMCFG3,  0xAB,     // Modem Configuration
	CC_REG_MDMCFG2,  0x40,     // Modem Configuration
	CC_REG_MDMCFG0,  0xE5,     // Modem Configuration
	CC_REG_DEVIATN,  0x42,     // Modem Deviation Setting
	CC_REG_MCSM0,    0x18,     // Main Radio Control State Machine Configuration
	CC_REG_FOCCFG,   0x16,     // Frequency Offset Compensation Configuration
	CC_REG_WORCTRL,  0xFB,     // Wake On Radio Control
	CC_REG_FSCAL3,   0xE9,     // Frequency Synthesizer Calibration
	CC_REG_FSCAL2,   0x2A,     // Frequency Synthesizer Calibration
	CC_REG_FSCAL1,   0x00,     // Frequency Synthesizer Calibration
	CC_REG_FSCAL0,   0x1F,     // Frequency Synthesizer Calibration
	CC_REG_TEST2,    0x81,     // Various Test Settings
	CC_REG_TEST1,    0x35,     // Various Test Settings
	0xFF, 0x0FF
};

//static const unsigned output_words[] = { 3, 3, 1, 0, 2, 2, 2, 2, 0, 1, 3, 3 };
static const unsigned output_words[] = { 3, 1, 0, 2 };

static const struct vss_dds_output output = {
	.bits = 2,
	.size = sizeof(output_words)/sizeof(*output_words),
	.words = output_words
};

/* Set up all the peripherals */

static void setup_usart(void)
{
	/* GPIO pin for USART TX */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
			GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO9);

	/* Setup USART parameters. */
	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_set_mode(USART1, USART_MODE_TX);

	/* Finally enable the USART. */
	usart_enable(USART1);
}

static void setup_radio(void)
{
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
			GPIO_CNF_OUTPUT_PUSHPULL, GPIO2);

	gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
			GPIO_CNF_INPUT_FLOAT, GPIO4);

	vss_cc_init();

	vss_cc_strobe(CC_STROBE_SIDLE);
	vss_cc_wait_state(CC_MARCSTATE_IDLE);

	int n;
	for(n = 0; init_seq[n] != 0xff; n += 2) {
		uint8_t reg = init_seq[n];
		uint8_t value = init_seq[n+1];

		vss_cc_write_reg(reg, value);
	}

	/* Configure the EXTI subsystem. */
	exti_select_source(EXTI4, GPIOA);
	exti_set_trigger(EXTI4, EXTI_TRIGGER_FALLING);
	exti_enable_request(EXTI4);
	exti_reset_request(EXTI4);

	nvic_enable_irq(NVIC_EXTI4_IRQ);
}

static void setup(void)
{
	rcc_clock_setup_in_hsi_out_64mhz();

	rcc_peripheral_enable_clock(&RCC_APB2ENR,
			RCC_APB2ENR_IOPAEN |
			RCC_APB2ENR_IOPBEN |
			RCC_APB2ENR_AFIOEN |
			RCC_APB2ENR_USART1EN);

	setup_usart();

	vss_rtc_init();

	setup_radio();
}

#define DDS_BUFF_SIZE	32

dds_t dds_buffer[DDS_BUFF_SIZE];

void exti4_isr(void)
{
	static uint32_t p = 0, w = 0;
	exti_reset_request(EXTI4);

	if((dds_buffer[p] >> w) & 0x00000001) {
		gpio_set(GPIOA, GPIO2);
	} else {
		gpio_clear(GPIOA, GPIO2);
	}

	w++;
	if(w == sizeof(dds_t)*8) {
		w = 0;
		p++;
		if(p == DDS_BUFF_SIZE) {
			p = 0;
		}
	}
}

/* Provide _write syscall used by libc */
int _write(int file, char *ptr, int len)
{
	int i;

	if (file == 1) {
		for (i = 0; i < len; i++) {
			usart_send_blocking(USART1, ptr[i]);
		}
		return i;
	} else {
		errno = EIO;
		return -1;
	}
}

/* Delay execution for some arbitrary amount of time */
void delay(void)
{
	int i;

	for (i = 0; i < 800000; i++) {
		__asm__("nop");
	}
}

int main(void)
{
	const float fs = 43.9796e3;

	setup();

	unsigned tw_num = 3;
	unsigned tw_list[tw_num];

	memset(tw_list, 0, sizeof(tw_list));
	
	printf("boot\n");

	int ch = 25;

	while(1) {
		vss_cc_strobe(CC_STROBE_SIDLE);
		vss_cc_wait_state(CC_MARCSTATE_IDLE);

		vss_cc_write_reg(CC_REG_CHANNR, ch);

		vss_cc_strobe(CC_STROBE_STX);
		vss_cc_wait_state(CC_MARCSTATE_TX);

		while(1) {
			vss_rtc_reset();
			unsigned cur_event = 0;

			while(cur_event < events_num) {

				while(events[cur_event].time > vss_rtc_read());

				unsigned tw = vss_dds_get_tuning_word(fs, events[cur_event].freq);

				int i = 0;
				if(events[cur_event].type) {
					while(tw_list[i] != 0 && i < tw_num-1) i++;
					tw_list[i] = tw;
				} else {
					while(tw_list[i] != tw && i < tw_num-1) i++;
					tw_list[i] = 0;
				}

				printf("ev %d %u -> %u %u %u\n", i, tw, tw_list[0], tw_list[1], tw_list[2]);

				vss_dds_fill_poly(dds_buffer, sizeof(dds_buffer), &output, tw_list, tw_num);

				cur_event++;
			}
		}
	}
}
