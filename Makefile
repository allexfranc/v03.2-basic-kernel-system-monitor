# Tools
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

# Flags
CFLAGS = -mcpu=cortex-m4 -mthumb -nostdlib -g -Wall

# --- Source & Object Files ---
# Gets all the .c files in the folder
SRCS = $(wildcard *.c)

# This line automatically generates the list of object files (.o)
# from the list of source files (.c)
OBJS = $(SRCS:.c=.o)

# --- Rules ---
# Default target (first rule is default)
all: firmware.bin

# Rule: how to make .o from .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule: link objects into ELF
firmware.elf: $(OBJS) link.ld
	$(LD) -T link.ld $(OBJS) -o firmware.elf -Map=firmware.map

# Rule: extract binary from ELF
firmware.bin: firmware.elf
	$(OBJCOPY) -O binary firmware.elf firmware.bin

# Pseudo-target: clean up
clean:
	rm -f *.o *.elf *.bin *.map