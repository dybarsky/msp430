SOURCES = src/sources
HEADERS = src/headers
OUT = build

export OBJ
OBJ += $(OUT)/led.o
OBJ += $(OUT)/button.o
OBJ += $(OUT)/servo.o
OBJ += $(OUT)/main.o


CC = msp430-gcc
CFLAGS = -g -mmcu=msp430g2553
INCFLAGS = -I/usr/local/include -I$(HEADERS)

help:
	@echo "Targets: clean, build, flash" 

clean:
	@echo "Clean"
	rm -rf build

build-dir:
	mkdir -p build

.SILENT: clean build-dir
.PHONY: clean build-dir flash build

flash: $(OUT)/firmware.elf
	@echo "Flashing";\
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

$(OUT)/button.o: $(SOURCES)/button.c $(HEADERS)/button.h
	@echo "Compile $<";\
	$(CC) $(CFLAGS) $(INCFLAGS) -c $(SOURCES)/button.c -o $(OUT)/button.o

$(OUT)/servo.o: $(SOURCES)/servo.c $(HEADERS)/servo.h
	@echo "Compile $<";\
	$(CC) $(CFLAGS) $(INCFLAGS) -c $(SOURCES)/servo.c -o $(OUT)/servo.o

