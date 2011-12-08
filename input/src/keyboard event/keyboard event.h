#ifndef __AVL_INPUT_KEYBOARD_EVENT__
#define __AVL_INPUT_KEYBOARD_EVENT__
/**
@file
Implements the KeyboardEvent class, which represents an input event
from a keyboard device.
@author Sheldon Bachstein
@date Sep 08, 2011
*/


#include"..\input event\input event.h"
#include"..\key codes\key codes.h"


namespace avl
{
namespace input
{

	/**
	Encapsulates a single input event from
	the keyboard. Includes the key which the event concerns, and whether
	that key was pressed or released.
	*/
	class KeyboardEvent: public InputEvent
	{
	public:
		/** This constant is used to initialize the InputEvent base class. It can be used
		to identify KeyboardEvent objects.*/
		static const unsigned char KEYBOARD_TYPE;

		/** Takes a key which this event concerns, and a bool indicated of whether it was
		pressed or released.
		@param initial_key The key which has been pressed or released.
		@param pressed True if \a initialKey was pressed, or false if it was released.
		*/
		KeyboardEvent(const key_codes::KeyboardKey::KeyboardKeyCodes& initial_key, const bool pressed);
		/** Copy constructor.
		@param original The object being copied.
		*/
		KeyboardEvent(const KeyboardEvent& original);
		/** Basic destructor.*/
		~KeyboardEvent();


		
		/** Returns the key which was pressed or released.
		@return The key which was pressed or released.
		*/
		const key_codes::KeyboardKey::KeyboardKeyCodes& GetKey() const;
		/** Returns true if the key was pressed, and false if it was released.
		@return True if the key was pressed, and false if it was released.
		*/
		const bool IsPressed() const;


	private:
		/// The key which was pressed/released.
		const key_codes::KeyboardKey::KeyboardKeyCodes key;
		/// Was \ref key pressed or released?
		const bool is_pressed;

	};



} //avl
} //input
#endif // __AVL_INPUT_KEYBOARD_EVENT__