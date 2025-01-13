// https://www.thejat.in/learn/implement-gdt-in-multiboot-bootloader
#pragma once

#include <stdio.h>

#define GDT_ENTRY_COUNT 5

struct GDTEntry
{
	uint16_t limitLow;
	uint16_t baseLow;
	uint8_t	 baseMiddle;
	uint8_t	 access;
	uint8_t	 granularity;
	uint8_t	 baseHigh;
} __attribute__((packed));

struct GDTPtr
{
	uint16_t limit; // Size
	uint32_t base;	// Address
} __attribute__((packed));

namespace GDT
{
	static GDTEntry entries[GDT_ENTRY_COUNT];
	static GDTPtr	ptr;

	void		init();
	void		setGate(int32_t entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
	EXTERN void loadGDT(GDTPtr* gdtPtr); // Defined in gdt.s
} // namespace GDT
