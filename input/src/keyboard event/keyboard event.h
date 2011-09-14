#ifndef __AVL_INPUT_KEYBOARD_EVENT__
#define __AVL_INPUT_KEYBOARD_EVENT__
/**********
 * Author: Sheldon Bachstein
 * Date: Sep 08, 2011
 * Description: The KeyboardEvent class encapsulates a single input event from
 * the keyboard. This includes the key which the event concerns, and whether
 * that key was pressed or released.
 **********/


#include"..\input event\input event.h"
#include"..\key codes\key codes.h"


namespace avl
{
namespace input
{

	// See the beginning of the file for details.
	class KeyboardEvent: public InputEvent
	{
	public:
		// This constant is used to initialize the InputEvent base class. It can be used
		// to identify KeyboardEvent objects.
		static const unsigned char KEYBOARD_TYPE;
		// Constructors:
		// Takes a key which this event concerns, and a bool indicated whether it was
		// pressed or released.
		KeyboardEvent(const key_codes::KeyboardKey::KeyboardKeyCodes& initial_key, const bool pressed);
		// Copy constructor.
		KeyboardEvent(const KeyboardEvent& original);
		// Destructor.
		~KeyboardEvent();


		// Accessors:
		// Returns the key which was pressed.
		const key_codes::KeyboardKey::KeyboardKeyCodes& GetKey() const;
		// Returns true if the key was pressed, and false if it was released.
		const bool IsPressed() const;


	private:
		// The key which was pressed/released.
		const key_codes::KeyboardKey::KeyboardKeyCodes key;
		// Was it pressed or released?
		const bool is_pressed;

	};



} //avl
} //input
#endif // __AVL_INPUT_KEYBOARD_EVENT__