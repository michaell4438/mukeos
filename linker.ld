ENTRY (_start)

SECTIONS
{
        . = 0x00100000;
	/* The kernel will live at 3GB + 1MB in the virtual address space, */
	/* which will be mapped to 1MB in the physical address space. */
	/* Note that we page-align the sections. */

	_kernel_start = .;
        .multiboot.data : {
            *(.multiboot.data)
        }

       .multiboot.text : {
           *(.multiboot.text)
       }
	_bootloader_end = .;

	/* Add a symbol that indicates the start address of the kernel. */
	.text ALIGN (4K) :
	{
		*(.text)
	}
	.rodata ALIGN (4K) :
	{
		*(.rodata)
	}
	.data ALIGN (4K) :
	{
		*(.data)
	}
	.bss ALIGN (4K):
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}
	/* Add a symbol that indicates the end address of the kernel. */
	_kernel_end = .;
}