# Nuke built-in rules and variables.
override MAKEFLAGS += -rR

# This is the name that our final kernel executable will have.
# Change as needed.
override KERNEL := mukeos

# Convenience macro to reliably declare user overridable variables.
define DEFAULT_VAR =
    ifeq ($(origin $1),default)
        override $(1) := $(2)
    endif
    ifeq ($(origin $1),undefined)
        override $(1) := $(2)
    endif
endef

override DEFAULT_KCC := cc
$(eval $(call DEFAULT_VAR,KCC,$(DEFAULT_KCC)))

override DEFAULT_KCXX := c++
$(eval $(call DEFAULT_VAR,KCXX,$(DEFAULT_KCXX)))

# User controllable C flags.
override DEFAULT_KCFLAGS := -g -O2 -pipe
$(eval $(call DEFAULT_VAR,KCFLAGS,$(DEFAULT_KCFLAGS)))

# User controllable C preprocessor flags. We set none by default.
override DEFAULT_KCPPFLAGS :=
$(eval $(call DEFAULT_VAR,KCPPFLAGS,$(DEFAULT_KCPPFLAGS)))

# User controllable nasm flags.
override DEFAULT_KNASMFLAGS := -F dwarf -g
$(eval $(call DEFAULT_VAR,KNASMFLAGS,$(DEFAULT_KNASMFLAGS)))

# User controllable linker flags. We set none by default.
override DEFAULT_KLDFLAGS :=
$(eval $(call DEFAULT_VAR,KLDFLAGS,$(DEFAULT_KLDFLAGS)))

# Internal C flags that should not be changed by the user.
override KCFLAGS += \
    -Wall \
    -Wextra \
    -std=gnu11 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fPIE \
    -m64 \
    -march=x86-64 \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone

# Internal C preprocessor flags that should not be changed by the user.
override KCPPFLAGS := \
    -I src \
    $(KCPPFLAGS) \
    -MMD \
    -MP

# Internal linker flags that should not be changed by the user.
override KLDFLAGS += \
    -nostdlib \
    -Wl,-m,elf_x86_64 \
    -Wl,-nostdlib \
    -Wl,-pie \
    -Wl,-z,text \
    -Wl,-z,max-page-size=0x1000 \
    -Wl,-T,linker.ld

# Internal nasm flags that should not be changed by the user.
override KNASMFLAGS += \
    -Wall \
    -f elf64

# Use "find" to glob all *.c, *.S, and *.asm files in the tree and obtain the
# object and header dependency file names.
override CFILES := $(shell cd src && find -L * -type f -name '*.c')
override CXXFILES := $(shell cd src && find -L * -type f -name '*.cxx')
override ASFILES := $(shell cd src && find -L * -type f -name '*.S')
override NASMFILES := $(shell cd src && find -L * -type f -name '*.asm')
override OBJ := $(addprefix obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(NASMFILES:.asm=.asm.o) $(CXXFILES:.cxx=.cxx.o))
override HEADER_DEPS := $(addprefix obj/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d) $(CXXFILES:.cxx=.cxx.d))

# Link rules for the final kernel executable.
bin/$(KERNEL): GNUmakefile linker.ld $(OBJ)
	mkdir -p "$$(dirname $@)"
	$(KCC) $(KCFLAGS) $(OBJ) $(KLDFLAGS) -o $@
	printf '\003' | dd of=$@ bs=1 count=1 seek=16 conv=notrunc 2>/dev/null

# Include header dependencies.
-include $(HEADER_DEPS)

# Compilation rules for *.c files.
obj/%.c.o: src/%.c GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(KCC) $(KCFLAGS) $(KCPPFLAGS) -c $< -o $@

# Compilation rules for *.S files.
obj/%.S.o: src/%.S GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(KCC) $(KCFLAGS) $(KCPPFLAGS) -c $< -o $@

# Compilation rules for *.asm (nasm) files.
obj/%.asm.o: src/%.asm GNUmakefile
	mkdir -p "$$(dirname $@)"
	nasm $(KNASMFLAGS) $< -o $@

# Compilation rules for *.cxx files.
obj/%.cxx.o: src/%.cxx GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(KCXX) $(KCFLAGS) $(KCPPFLAGS) -c $< -o $@

# Fetch limine bootloader
.PHONY: fetch-limine
fetch-limine:
	git clone https://github.com/limine-bootloader/limine.git --branch=v7.x-binary --depth=1
	make -C limine

# Build the final ISO image.
.PHONY: build
mukeos.iso: bin/$(KERNEL)
	if [ ! -d limine ]; then \
		echo "Please run 'make fetch-limine' first."; \
		exit 1; \
	fi

	mkdir -p iso_root
	mkdir -p iso_root/boot
	cp -v bin/mukeos iso_root/boot/
	mkdir -p iso_root/boot/limine
	cp -v limine.cfg limine/limine-bios.sys limine/limine-bios-cd.bin \
    	limine/limine-uefi-cd.bin iso_root/boot/limine/
	mkdir -p iso_root/EFI/BOOT
	cp -v limine/BOOTX64.EFI iso_root/EFI/BOOT/
	cp -v limine/BOOTIA32.EFI iso_root/EFI/BOOT/

	xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot boot/limine/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        iso_root -o $@
	
	./limine/limine bios-install $@
	rm -rf iso_root

# Run the kernel in QEMU.
.PHONY: run
run: mukeos.iso
	qemu-system-x86_64 -cdrom $<

# Run the kernel in QEMU with GDB.
.PHONY: debug
debug: mukeos.iso
	qemu-system-x86_64 -cdrom $< -s -S

# Default target.
.PHONY: all
all: mukeos.iso

# Remove object files and the final executable.
.PHONY: clean
clean:
	rm -rf bin obj mukeos.iso