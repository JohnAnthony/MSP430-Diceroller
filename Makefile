ARCH	:= msp430
CHIP	:= g2211
CC	    := $(ARCH)-gcc
OBJCOPY := $(ARCH)-objcopy
OBJDUMP := $(ARCH)-objdump

MCU	    := -mmcu=$(ARCH)$(CHIP)
CFLAGS  := $(MCU) -O2 -Wall
OBJECTS := roller.o

.PHONY: clean

all: $(OBJECTS)
	$(CC) $(MCU) -o roller.elf roller.o
	$(OBJCOPY) -O ihex roller.elf roller.a43
	$(OBJDUMP) -dSt roller.elf > roller.lst

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm roller.a43 roller.lst roller.elf $(OBJECTS)

install:
	echo "erase\n load roller.elf\n exit" | sudo mspdebug rf2500
