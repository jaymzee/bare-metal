TARGET = boot.img
OBJECTS = main.o intsetup.o handlers.o
OBJECTS += crt0.o stdio.o serial.o string.o conio.o kbd_US.o cpu.o interrupt.o
LIBC = ../../libc
LIBSYS = ../../libsys
INCLUDE = ../../../include
CC = gcc
CFLAGS = -g -Os -std=c99 -pedantic -Wall -Werror
CFLAGS = -m32 -ffreestanding
CFLAGS += -fno-pic -fno-exceptions -fno-asynchronous-unwind-tables
# -mtune=pentium3 will have gcc code-gen pop instructions instead of mov
CFLAGS += -mtune=pentium3
CFLAGS += -nostdinc -I $(INCLUDE) -include _default.h
ASFLAGS = -g -F dwarf -f elf -i $(addsuffix /asm/,$(INCLUDE))
# --nmagic eliminates gaps between sections
LDFLAGS = -m elf_i386 --nmagic
GCCVERSION := $(shell gcc -dumpversion)
ifeq ($(shell expr $(GCCVERSION) \>= 8), 1)
    # disable CET endbr32 instructions in function entry point
    CFLAGS += -fcf-protection=return
endif
vpath %.asm src
vpath %.c src
vpath %.c $(LIBC)
vpath %.c $(LIBSYS)

.PHONY: clean run debug
$(TARGET): boot.bin main.bin
	./mkboot $@ $^
%.bin: %.elf
	objcopy -O binary $< $@
boot.elf: boot.o
	$(LD) $(LDFLAGS) -T $(addsuffix .ld,$(basename $@)) -o $@ $^
main.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) -T $(addsuffix .ld,$(basename $@)) -o $@ $^
%.o: %.asm
	nasm $(ASFLAGS) -o $@ -l $(addsuffix .lst,$(basename $@)) $<
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
clean:
	@$(RM) $(TARGET) *.elf *.o *.lst *.bin
run: $(TARGET)
	@./qemu -display curses
debug: $(TARGET)
	gdb -x init.gdb
