.PHONY: all clean program reset halgen term

DEVICE := atxmega128a1
DUDE_DEV := x128a1
CLOCK := 25000000
BITCLK_US := 1
OBJS := main.o

CC := avr-gcc
CFLAGS := -std=c11 -g3 -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)
CFLAGS += -Wall -Wmissing-prototypes -Wstrict-prototypes -Werror=implicit-function-declaration -Werror=format -Wshadow

all: main main.bin

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

reset:
	avrdude -c avrispmkII -B $(BITCLK_US) -p $(DUDE_DEV) 

program: main.hex
	avrdude -c avrispmkII -B $(BITCLK_US) -p $(DUDE_DEV) -U flash:w:main.hex:i

term:
	picocom --baud 115200 /dev/ttyUSB0

clean:
	rm -f main.hex main.bin main $(OBJS)

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

main.hex: main
	avr-objcopy -j .text -j .data -O ihex main $@

main.bin: main
	avr-objcopy -j .text -j .data -O binary main $@

halgen: hal.xml
	../HALGen/halgen -a xmega -o hal.h hal.xml
