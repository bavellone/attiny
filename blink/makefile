# -------------------
# User-defined config
# -------------------

CC=avr-gcc
MCU=attiny85
TGT=t85
CFLAGS=-Wall -Os -g -mmcu=$(MCU)
DUMP_FLAGS=--headers -l --source -j .text -j .data
PROGRAMMER=usbtiny
PRG=blink
SRC=src
DIST=dist
OBJ=$(DIST)/$(PRG).o
ELF=$(DIST)/$(PRG).elf
HEX=$(DIST)/$(PRG).hex

# -------------------
# Targets
# -------------------

default: init $(HEX) dump asm size

$(OBJ): $(SRC)/$(PRG).c $(SRC)/$(PRG).h
	$(CC) $(CFLAGS) -o $(OBJ) -c $(SRC)/$(PRG).c

$(ELF): $(OBJ)
	$(CC) $(CFLAGS) -o $(ELF) $(OBJ)

$(HEX): $(ELF)
	avr-objcopy -j .text -j .data -O ihex $(ELF) $(HEX)
	
upload: $(HEX) asm dump
	avrdude -c $(PROGRAMMER) -p $(TGT) -U flash:w:$(HEX):i

download:
	avrdude -c $(PROGRAMMER) -p $(TGT) -U flash:r:$(DIST)/$(PRG).dump.hex:i

dump: $(ELF)
	avr-objdump $(DUMP_FLAGS) $(ELF) > $(DIST)/$(PRG).lst

asm: 
	$(CC) $(CFLAGS) -S -o $(DIST)/$(PRG).asm -c $(SRC)/$(PRG).c

size: $(ELF)
	avr-size -C --mcu=$(MCU) $(ELF)

init:
	mkdir -p dist

clean: 
	rm -rf dist/
