/**********
 * Author: Sheldon Bachstein
 * Date: Sep 08, 2011
 * Description: See mouse button event.h for details.
 **********/

#include"mouse button event.h"
#include"..\key codes\key codes.h"




namespace avl
{
namespace input
{


	// A constant used to initialize the InputEvent base class. This constant
	// allows clients to identify an event as a MouseButtonEvent.
	const unsigned char MouseButtonEvent::MOUSE_BUTTON_TYPE = 0x02;


	// Constructors:
	// Takes a button and whether the button was pressed or released.
	MouseButtonEvent::MouseButtonEvent(const key_codes::MouseButton::MouseButtonCodes& initial_button, const bool is_pressed)
		: InputEvent(MOUSE_BUTTON_TYPE), button(initial_button), pressed(is_pressed)
	{
	}




	// Copy constructor.
	MouseButtonEvent::MouseButtonEvent(const MouseButtonEvent& original)
		: InputEvent(original.GetType()), button(original.GetButton()), pressed(original.IsPressed())
	{
	}




	// Destructor.
	MouseButtonEvent::~MouseButtonEvent()
	{
	}




	// Returns the button which was pressed.
	const key_codes::MouseButton::MouseButtonCodes& MouseButtonEvent::GetButton() const
	{
		return button;
	}




	// Returns true if the button was pressed, and false if it was released.
	const bool MouseButtonEvent::IsPressed() const
	{
		return pressed;
	}


}
}

