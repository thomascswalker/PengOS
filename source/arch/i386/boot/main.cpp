/*
Main entry point into PengOS.
*/

#include <filesystem.h>
#include <gdt.h>
#include <ide.h>
#include <keyboard.h>
#include <memory.h>
#include <multiboot.h>
#include <pit.h>
#include <string.h>

EXTERN void kmain(MultibootInfo* info, uint32_t magic)
{
	VGA::init();
	println("Initializing PengOS...");
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		panic("Invalid Multiboot magic value.");
	}
	uint32_t start = 0;
	uint32_t size = 0;
	Multiboot::init(info, &start, &size);

	GDT::init();
	IDT::init();
	PIT::init();
	Keyboard::init();

	// Once everything is initialized, enable interrupts.
	enableInterrupts();
	println("Welcome to PengOS!");

	Memory::init(start, size);

	IDE::init();
	// FileSystem::init();

	while (1)
	{
	}
}