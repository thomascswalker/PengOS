#include <keyboard.h>
#include <pic.h>
#include <stdio.h>

using namespace Terminal;

namespace Keyboard
{
	// Tracks the state of any modifier keys (Shift, Alt, Ctrl)
	static uint8_t g_modifierKey;

	void init()
	{
		debug("Initializing keyboard...");

		// Wait for the keyboard to be available.
		while (inb(KEYBOARD_STATUS_PORT) & KEYBOARD_INPUT_FULL)
		{
		}

		// Register IRQ1 (ISR33) to execute our keyboard callback
		// function.
		IDT::registerInterruptHandler(IRQ1, callback);
		success("Keyboard initialized.");
	}

	// Called when IRQ1 (ISR33) is executed.
	void callback(Registers regs)
	{
		uint8_t sc = inb(KEYBOARD_DATA_PORT); // Incoming scancode
		KeyCode kc;							  // Converted keyboard code struct

		/*
		Check if the input scancode has the 8th bit set to 1.
		If it does, this indicates the key is being released.
		Otherwise, the key is being pressed.

		---------------------------------
		| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
		---------------------------------
		  ^ 1 if released, 0 if pressed

		The remaining 7 bits dictate which key was actually
		pressed.
		*/
		if (sc & KEYBOARD_RELEASED) // Released
		{
			sc &= ~(KEYBOARD_RELEASED); // Set bit 8 to zero.
			if (getKeycode((ScanCode)sc, &kc))
			{
				onKeyReleased(&kc);
			}
		}
		else if (getKeycode((ScanCode)sc, &kc)) // Pressed
		{
			onKeyPressed(&kc);
		}
	}

	// Given the input scancode, retrieve the keycode struct
	// containing `{ scancode, lower, upper }`.
	bool getKeycode(ScanCode sc, KeyCode* kc)
	{
		uint32_t count = sizeof(keymap) / sizeof(KeyCode);
		for (uint32_t i = 0; i < count; i++)
		{
			if (keymap[i].code == sc)
			{
				*kc = keymap[i];
				return true;
			}
		}
		return false;
	}

	void onKeyReleased(KeyCode* kc)
	{
		switch (kc->code)
		{
			case SC_SHIFTLEFT:
			case SC_SHIFTRIGHT:
				{
					g_modifierKey &= ~MOD_SHIFT;
					return;
				}
			case SC_ESC:
				{
					exit();
				}
			default:
				return;
		}
	}

	void onKeyPressed(KeyCode* kc)
	{
		switch (kc->code)
		{
			case SC_ESC:
				{
					exit();
					return;
				}
			case SC_SHIFTLEFT:
			case SC_SHIFTRIGHT:
				{
					g_modifierKey |= MOD_SHIFT;
					return;
				}
			case SC_ENTER:
				{
					Terminal::insertNewLine();
					return;
				}
			case SC_BACKSPACE:
				{
					Terminal::remchar();
					Terminal::updateCursorPosition();
					return;
				}
			default:
				break;
		}

		char c = isShiftDown() ? kc->upper : kc->lower;
		Terminal::put(c);
		Terminal::updateCursorPosition();
	}

	bool isShiftDown()
	{
		return g_modifierKey & MOD_SHIFT;
	}
} // namespace Keyboard
