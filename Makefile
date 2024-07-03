ASM_FILES = $(filter-out macros/*, $(wildcard *.S */*.S))
OBJ_FILES = $(ASM_FILES:.S=.o)
LD = i686-elf-ld

%.o: %.S
	i686-elf-as $< -o $@

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
	qemu-system-i386 -cdrom $< -s -S &
	i686-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file mukeos.bin"