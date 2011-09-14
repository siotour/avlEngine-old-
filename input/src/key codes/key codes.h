#ifndef __AVL_INPUT_KEY_CODES__
#define __AVL_INPUT_KEY_CODES__
/**********
 * Author: Sheldon Bachstein
 * Date: Sep 03, 2011
 * Description: Defines enumerations for input key/button codes. There are separate
 * enumerations for mouse buttons, keyboard modifier keys (capslock, shift, ctrl, etc.),
 * and keyboard keys.
 **********/




namespace avl
{
namespace input
{
namespace key_codes
{


	// Mouse button codes.
	struct MouseButton
	{
	enum MouseButtonCodes
	{
		mb_null				= 0,
		mb_left				= 1,
		mb_right			= 2,
		mb_middle			= 3,
	};
	};


	// Keyboard modifier codes.
	struct KeyboardModifier
	{
	enum KeyboardModifierCodes
	{
		km_none				= 0,
		km_leftshift		= 1,
		km_rightshift		= 2,
		km_leftctrl			= 4,
		km_rightctrl		= 8,
		km_leftalt			= 16,
		km_rightalt			= 32,
		km_numlock			= 64,
		km_capslock			= 128
	};
	};


	// Keyboard key codes.
	struct KeyboardKey
	{
	enum KeyboardKeyCodes
	{
		kk_null				= 0,

		// Numbers:
		kk_0				= 1,
		kk_1				= 2,
		kk_2				= 3,
		kk_3				= 4,
		kk_4				= 5,
		kk_5				= 6,
		kk_6				= 7,
		kk_7				= 8,
		kk_8				= 9,
		kk_9				= 10,

		// Letters:
		kk_a				= 11,
		kk_b				= 12,
		kk_c				= 13,
		kk_d				= 14,
		kk_e				= 15,
		kk_f				= 16,
		kk_g				= 17,
		kk_h				= 18,
		kk_i				= 19,
		kk_j				= 20,
		kk_k				= 21,
		kk_l				= 22,
		kk_m				= 23,
		kk_n				= 24,
		kk_o				= 25,
		kk_p				= 26,
		kk_q				= 27,
		kk_r				= 28,
		kk_s				= 29,
		kk_t				= 30,
		kk_u				= 31,
		kk_v				= 32,
		kk_w				= 33,
		kk_x				= 34,
		kk_y				= 35,
		kk_z				= 36,

		// Punctuation:
		kk_comma			= 37,
		kk_period			= 38,
		kk_semicolon		= 39,
		kk_apostrophe		= 40,
		kk_accent			= 41, // Key left of 1.

		// Control characters:
		kk_space			= 42,
		kk_tab				= 43,
		kk_backspace		= 44,
		kk_enter			= 45,
		kk_escape			= 46,

		// Home/end cluster:
		kk_insert			= 47,
		kk_delete			= 48,
		kk_home				= 49,
		kk_end				= 50,
		kk_pageup			= 51,
		kk_pagedown			= 52,
		
		// Symbols:
		kk_minus			= 53,
		kk_slash			= 54,
		kk_equals			= 55,
		kk_backslash		= 56,
		kk_leftbracket		= 57,
		kk_rightbracket		= 58,

		// Arrows.
		kk_up				= 59,
		kk_down				= 60,
		kk_right			= 61,
		kk_left				= 62,
	
		// Function keys
		kk_f1				= 63,
		kk_f2				= 64,
		kk_f3				= 65,
		kk_f4				= 66,
		kk_f5				= 67,
		kk_f6				= 68,
		kk_f7				= 69,
		kk_f8				= 70,
		kk_f9				= 71,
		kk_f10				= 72,
		kk_f11				= 73,
		kk_f12				= 74,

		// Modifier keys.
		kk_numlock			= 78,
		kk_capslock			= 79,
		kk_scrolllock		= 80,
		kk_rightshift		= 81,
		kk_leftshift		= 81,
		kk_rightctrl		= 83,
		kk_leftctrl			= 84,
		kk_rightalt			= 85,
		kk_leftalt			= 86,
		kk_rightwindow		= 87,
		kk_leftwindow		= 88,

		// Numpad
		kk_np_0				= 89,
		kk_np_1				= 90,
		kk_np_2				= 91,
		kk_np_3				= 92,
		kk_np_4				= 93,
		kk_np_5				= 94,
		kk_np_6				= 95,
		kk_np_7				= 96,
		kk_np_8				= 97,
		kk_np_9				= 98,
		kk_np_period		= 99,
		kk_np_slash			= 100,
		kk_np_star			= 101,
		kk_np_minus			= 102,
		kk_np_plus			= 103,
		kk_np_enter			= 104,

		// Misc.:
		kk_menu				= 105
	};
	};






}
}
}



#endif // __AVL_INPUT_KEY_CODES__