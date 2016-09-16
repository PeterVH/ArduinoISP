CC = sdcc
#CFLAGS = -V
MFLAGS = --model-small --debug
#MFLAGS = --model-large
LFLAGS = --xram-loc 0x4000 --code-loc 0x0000

all: blink.ihx knight-rider.ihx melody.ihx

OBJS_blink = blink.rel delay.rel
OBJS_knight_rider = knight-rider.rel delay.rel
OBJS_uart = uart.rel delay.rel
OBJS_melody = melody.rel tone.rel delay.rel

melody.ihx: $(OBJS_melody)
	$(CC) $(MFLAGS) $(LFLAGS) $(OBJS_melody)

blink.ihx: $(OBJS_blink)
	$(CC) $(MFLAGS) $(LFLAGS) $(OBJS_blink)

knight-rider.ihx: $(OBJS_knight_rider)
	$(CC) $(MFLAGS) $(LFLAGS) $(OBJS_knight_rider)

uart.ihx: $(OBJS_auart)
	$(CC) $(MFLAGS) $(LFLAGS) $(OBJS_blink)

clean:
	rm -f core *~ \#* *.asm *.cdb *.rel *.hex *.ihx *.lst *.map \
		*.rst *.sym *.lnk *.lib *.bin *.adb *.mem *.omf *.lk

%.rel: %.c hw.h
	$(CC) -c $(CFLAGS) $(MFLAGS) $<

%.bin: %.ihx
	hex2bin <$< >$@
