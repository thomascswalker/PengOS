#pragma once

// If on a normal desktop platform, define i386 as to not
// throw an error.
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__) || defined(__apple__)
#define __i386__
#endif

#if !defined(__i386__)
#error "This needs to be compiled with ix86-elf."
#endif

#include <string.h>
#include <sys.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// The starting point of the terminal's text buffer in memory.
#define TERMINAL_BUFFER_START (uint16_t*)0xB8000

/* Hardware text mode color constants. */
typedef enum vga_color
{
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_YELLOW = 14,
	VGA_COLOR_WHITE = 15,
	VGA_COLOR_DEFAULT = VGA_COLOR_LIGHT_GREY,
} vga_color_t;

// Terminal containing a 16-bit text buffer.
typedef struct terminal
{
	uint16_t* buffer; // Buffer of all terminal text.
	uint16_t  color;  // The current cursor color.
	int32_t	  row;	  // The current cursor row.
	int32_t	  column; // The current cursor column.
} terminal_t;

// Global terminal object.
static terminal_t g_terminal;

// VGA

static uint16_t make_entry_color(vga_color_t fore_color, vga_color_t back_color);
static uint16_t create_entry(uint8_t character, uint8_t color);

// Cursor

static uint32_t get_cursor_pos();
static void		set_cursor_pos(int8_t x, int8_t y);
static void		enable_cursor(uint8_t start, uint8_t end);
static void		disable_cursor();

// Terminal

static void clear_terminal();
static void init_terminal();
static void putchar(char c);
static void terminal_new_line();
static void write_terminal(const char* data, size_t size);
static void scroll_terminal();

// Terminal color

static void set_terminal_color(uint8_t color);
static void reset_terminal_color();
