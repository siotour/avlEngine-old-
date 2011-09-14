/**********
 * Author: Sheldon Bachstein
 * Date: Sep 08, 2011
 * Description: See keyboard event.h for details.
 **********/

#include"keyboard event.h"
#include"..\input event\input event.h"
#include"..\key codes\key codes.h"


namespace avl
{
namespace input
{
	// This constant is used to initialize the InputEvent base class. It can be used
	// to identify KeyboardEvent objects.
	const unsigned char KeyboardEvent::KEYBOARD_TYPE = 0x01;




	// Takes a key which this event concerns, and a bool indicated whether it was
	// pressed or released.
	KeyboardEvent::KeyboardEvent(const key_codes::KeyboardKey::KeyboardKeyCodes& initial_key, const bool pressed)
		: InputEvent(KEYBOARD_TYPE), key(initial_key), is_pressed(pressed)
	{
	}




	// Copy constructor.
	KeyboardEvent::KeyboardEvent(const KeyboardEvent& original)
		: InputEvent(original.GetType()), key(original.GetKey()), is_pressed(original.IsPressed())
	{
	}




	// Destructor.
	KeyboardEvent::~KeyboardEvent()
	{
	}




	// Returns the key which was pressed.
	const key_codes::KeyboardKey::KeyboardKeyCodes& KeyboardEvent::GetKey() const
	{
		return key;
	}




	// Returns true if the key was pressed, and false if it was released.
	const bool KeyboardEvent::IsPressed() const
	{
		return is_pressed;
	}



}
}