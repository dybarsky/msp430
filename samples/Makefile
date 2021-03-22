SOURCES = src/sources
HEADERS = src/headers
OUT = build

export OBJ
OBJ += $(OUT)/led.o
OBJ += $(OUT)/main.o
#OBJ += $(OUT)/uart.o
#OBJ += $(OUT)/i2c.o

CC = msp430-gcc
CFLAGS = -g -mmcu=msp430g2553
INCFLAGS = -I/usr/local/include -I$(HEADERS)

#LDFLAGS += -T /usr/local/Cellar/msp430-mcu/20130321/msp430/lib/ldscripts/msp430g2553/memory.x

flash: $(OUT)/firmware.elf
	@echo "Flashing";\
	@echo $(IDFLAGS)
	mspdebug rf2500 "prog $(OUT)/firmware.elf"

build: $(OUT)/firmware.elf
	@echo "SUCCESSFUL";\

$(OUT)/firmware.elf: | build-dir $(OBJ)
	@echo "Link firmware";\
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $(OUT)/firmware.elf

$(OUT)/main.o: $(SOURCES)/main.c $(HEADERS)/*.h
	@echo "Compile $<";\
	$(CC) $(CFLAGS) $(INCFLAGS) -c $(SOURCES)/main.c -o $(OUT)/main.o

$(OUT)/led.o: $(SOURCES)/led.c $(HEADERS)/led.h
	@echo "Compile $<";\
	$(CC) $(CFLAGS) $(INCFLAGS) -c $(SOURCES)/led.c -o $(OUT)/led.o

#$(OUT)/servo.o:	$(SOURCES)/servo.c $(HEADERS)/servo.h
#	@echo "Compile $<";\
#	$(CC) $(CFLAGS) $(INCFLAGS) -c $(SOURCES)/servo.c -o $(OUT)/servo.o

#$(OUT)/uart.o: $(SOURCES)/uart.c $(HEADERS)/uart.h
#	@echo "Compile $<";\
#	$(CC) $(CFLAGS) $(INCFLAGS) -c $(SOURCES)/uart.c -o $(OUT)/uart.o

#$(OUT)/i2c.o: $(SOURCES)/i2c.c $(HEADERS)/*.h
#	@echo "Compile $<";\
#	$(CC) $(CFLAGS) $(INCFLAGS) -c $(SOURCES)/i2c.c -o $(OUT)/i2c.o

clean:
	rm -rf build

build-dir:
	mkdir -p build

.SILENT: clean build-dir
.PHONY: clean build-dir flash build
