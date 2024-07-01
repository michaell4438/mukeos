ASM_FILES = $(wildcard *.asm)
OBJ_FILES = $(ASM_FILES:.asm=.o)
LD = i686-elf-ld

%.o: %.asm
	nasm -felf32 $< -o $@

mukeos.bin: $(OBJ_FILES)
	$(LD) -T linker.ld -o $@ -O2 -nostdlib $^

mukeos.iso: mukeos.bin
	mkdir -p isodir/boot/grub
	cp mukeos.bin isodir/boot/mukeos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir

run: mukeos.iso
	qemu-system-i386 -cdrom $<

clean:
	rm -rf $(OBJ_FILES) mukeos.bin mukeos.iso isodir

debug: mukeos.iso
	qemu-system-i386 -s -S -cdrom $< &
	gdb -ex "target remote localhost:1234" -ex "layout src" -ex "layout regs" -ex "break *0x7c00" -ex "continue"