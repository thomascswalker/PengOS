/*
Main entry point into PengOS. Inializes the kernel.
*/

#include <io.h>

// Entry point
void kernel_main(void)
{
	init_terminal();
	println("Welcome to PengOS!");
	println("Made by Thomas Walker.");
}
