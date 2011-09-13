#ifndef __AVL_INPUT_MOUSE_BUTTON_EVENT__
#define __AVL_INPUT_MOUSE_BUTTON_EVENT__
/**********
 * Author: Sheldon Bachstein
 * Date: Sep 08, 2011
 * Description: The MouseButtonEvent encapsulates an input event from the mouse
 * buttons. This includes which button the event concerns, and whether the button
 * was pressed or released.
 **********/


#include"..\input event\input event.h"
#include"..\key codes\key codes.h"


namespace avl
{
namespace input
{
	// See the beginning of the file for details.
	class MouseButtonEvent: public InputEvent
	{
	public:
		// A constant used to initialize the InputEvent base class. This constant
		// allows clients to identify an event as a MouseButtonEvent.
		static const unsigned char MOUSE_BUTTON_TYPE;


		// Constructors:
		// Takes a button and whether the button was pressed or released.
		MouseButtonEvent(const key_codes::MouseButton& initial_button, const bool is_pressed);
		// Copy constructor.
		MouseButtonEvent(const MouseButtonEvent& original);
		// Destructor.
		~MouseButtonEvent();


		// Accessors:
		// Returns the button which was pressed.
		const key_codes::MouseButton& GetButton() const;
		// Returns true if the button was pressed, and false if it was released.
		const bool IsPressed() const;


	private:
		// The button which was pressed/released.
		const key_codes::MouseButton button;
		// Was it pressed or released?
		const bool pressed;

	};





} //avl
} //input
#endif // __AVL_INPUT_MOUSE_BUTTON_EVENT__