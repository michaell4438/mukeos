ASM_FILES = $(filter-out macros/*, $(wildcard *.S */*.S))
C_FILES = $(wildcard *.c */*.c)
NASM_FILES = $(wildcard *.asm */*.asm)
OBJ_FILES = $(ASM_FILES:.S=.o) $(C_FILES:.c=.o) $(NASM_FILES:.asm=.o)
LD = i686-elf-ld
C_FLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra

%.o: %.S
	i686-elf-as $< -o $@

%.o: %.asm
	nasm -f elf32 $< -o $@

%.o: %.c
	i686-elf-gcc -c $< -o $@ $(C_FLAGS)

mukeos.bin: $(OBJ_FILES)
	$(LD) -T linker.ld -o $@ -O2 -nostdlib $^

mukeos.iso: mukeos.bin
	rm -rf isodir
	mkdir -p isodir/boot/grub
	cp mukeos.bin isodir/boot/mukeos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir

run: mukeos.iso
	qemu-system-i386 -cdrom $<

clean:
	rm -rf $(OBJ_FILES) mukeos.bin mukeos.iso isodir

debug: mukeos.iso
	qemu-system-i386 -m 512M -cdrom $< -s -S &
	i686-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file mukeos.bin"