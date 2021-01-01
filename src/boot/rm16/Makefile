TARGET = demo.img
OBJECTS = stage1.o main.o
OBJECTS += stdio.o string.o serial.o conio.o kbd_US.o interrupt.o time.o
LIBC = ../../libc
LIBSYS = ../../libsys
INCLUDE = ../../../include
CC = gcc
CFLAGS = -g -std=c99 -pedantic -Wall
CFLAGS += -m16 -ffreestanding
CFLAGS += -fno-pic -fno-exceptions -fno-asynchronous-unwind-tables
CFLAGS += -nostdinc -I $(addsuffix /,$(INCLUDE))
ASFLAGS = -g -F dwarf -f elf
LDFLAGS = -m elf_i386 --nmagic  # --nmagic eliminates gaps between sections
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
$(TARGET): stage0.bin stage1.bin
	./mkboot $@ $^
%.bin: %.elf
	objcopy -O binary $< $@
stage0.elf: stage0.o
	$(LD) $(LDFLAGS) -T stage0.ld -o $@ $^
stage1.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) -T stage1.ld -o $@ $^
%.o: %.asm
	nasm $(ASFLAGS) -o $@ -l $(addsuffix .lst,$(basename $@)) $<
%.o: %.c
	$(CC) $(CFLAGS) -c $<
clean:
	@$(RM) $(TARGET) *.elf *.o *.lst *.bin
run: $(TARGET)
	@./qemu -display curses
debug: $(TARGET)
	gdb -x start16.gdb
