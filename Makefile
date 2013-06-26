BINARY = vesna-audio-synthesis

PREFIX		?= arm-none-eabi
CC		= $(PREFIX)-gcc
LD		= $(PREFIX)-gcc
OBJCOPY		= $(PREFIX)-objcopy
CFLAGS		+= -O2 -g -Wall -Wextra -I$(TOOLCHAIN_DIR)/include -I. \
		   -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD -DSTM32F1
LDSCRIPT	?= vesna.ld
LDFLAGS		+= -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group \
		   -L$(TOOLCHAIN_DIR)/lib -L$(TOOLCHAIN_DIR)/lib/stm32/f1 \
		   -T$(LDSCRIPT) -nostartfiles -Wl,--gc-sections \
		   -mthumb -march=armv7 -mfix-cortex-m3-ldrd -msoft-float
OBJS		+= main.o wavetable.o dds.o event.o platform/cc.o platform/rtc.o smile-notes.o

OPENOCD		?= openocd
OPENOCD_PARAMS  ?= -f interface/olimex-arm-usb-ocd.cfg -f target/stm32f1x.cfg

MIDICSV		= $(HOME)/src/midicsv-1.1/midicsv

all: $(BINARY).bin

%.bin: %.elf
	$(OBJCOPY) -Obinary $(*).elf $(*).bin

%.elf: $(OBJS) $(LDSCRIPT)
	$(LD) -o $(*).elf $(OBJS) -lopencm3_stm32f1 $(LDFLAGS)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -o $@ -c $<

%.csv: %.mid
	$(MIDICSV) $< $@

%-notes.c: %.csv generate-notes.py
	python generate-notes.py $< > $@

wavetable.c: generate-wavetable.py
	python generate-wavetable.py 1024 > $@

clean:
	rm -f *.o
	rm -f *.d
	rm -f *.elf
	rm -f *.bin

test:
	$(MAKE) -C tests test

%.u: %.bin
	$(OPENOCD) $(OPENOCD_PARAMS) -c "\
		reset_config trst_and_srst; \
		init; \
		reset halt; \
		poll; \
		stm32f1x mass_erase 0; \
		flash write_bank 0 $< 0; \
		reset run; \
		shutdown \
	"

.PHONY: clean test

-include $(OBJS:.o=.d)
