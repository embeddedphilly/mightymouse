MSG_FLASH=Creating final hex file and flashing to Teensy
MCU=at90usb1286
FORMAT=ihex
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
TARGET = demo
SRC =	src/main.c \
		src/MM_MotorDriver.c \
			lib/libavr/src/AvrGPIO.c \
			lib/libavr/src/AvrPWM.c \
			lib/libavr/src/AvrChipControl.c \

OBJDIR = ./Build
OBJ = $(SRC:src/%.c=$(OBJDIR)/%.o)

F_CPU = 8000000
DEBUG = dwarf-2

CFLAGS = -g$(DEBUG)
CFLAGS += -DF_CPU=$(F_CPU)UL
CFLAGS += -Os
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -ffunction-sections
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -Wall
CFLAGS += -Wstrict-prototypes
CFLAGS += -std=gnu99
CFLAGS += -Isrc/
CFLAGS += -Ilib/libavr/src/
CFLAGS += -Ilib/libavr/lib/butilities/src/
CFLAGS += -Ilib/libavr/lib/butilities/lib/halinterface/headers/


#---------------- Linker Options ----------------
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
EXTRALIBDIRS = /usr/local/lib \
			   /usr/lib/ \

LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
LDFLAGS += -Wl,--relax
LDFLAGS += -Wl,--gc-sections
LDFLAGS += $(patsubst %,-L%,$(EXTRALIBDIRS))

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MMD -MP -MF .dep/$(@F).d

ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS)

default: $(TARGET).hex

# Link: create ELF output file from object files.
%.elf: $(OBJ) $(AVROBJ) $(BUTILOBJ)
	@echo
	$(CC) $(ALL_CFLAGS) $^ --output $@ $(LDFLAGS)

# Compile: create object files from C source files.
$(OBJDIR)/%.o : src/%.c
	@echo
	$(CC) -c $(ALL_CFLAGS) -Wa,-adhlns=$(<:src/%.c=$(OBJDIR)/%.lst) $< -o $@

$(OBJDIR)/%.o : lib/libavr/src/%.c
	@echo
	$(CC) -c $(ALL_CFLAGS) -Wa,-adhlns=$(<:lib/libavr/src/%.c=$(OBJDIR)/%.lst) $< -o $@

$(OBJDIR)/%.o : lib/libavr/lib/butilities/src/%.c
	@echo
	$(CC) -c $(ALL_CFLAGS) -Wa,-adhlns=$(<:lib/libavr/lib/butilities/src/%.c=$(OBJDIR)/%.lst) $< -o $@


# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	@echo
	$(OBJCOPY) -O $(FORMAT) -R .eeprom -R .fuse -R .lock -R .signature $< $@

flash: $(TARGET).hex
	teensy_loader_cli -mmcu=$(MCU) -w -v $(TARGET).hex

clean:
	rm -rf bin/*.o
	rm -rf $(TARGET).hex
	rm -rf bin/*.lst
	rm -rf $(TARGET).map
