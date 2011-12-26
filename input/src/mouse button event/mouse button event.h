#ifndef __AVL_INPUT_MOUSE_BUTTON_EVENT__
#define __AVL_INPUT_MOUSE_BUTTON_EVENT__
/**
@file
Implements the \ref avl::input::MouseButtonEvent.
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
	Encapsulates an input event from the mouse
	buttons. Includes which button the event concerns, and whether the button
	was pressed or released.
	*/
	class MouseButtonEvent: public InputEvent
	{
	public:
		/** A constant used to initialize the InputEvent base class. This constant
		allows clients to identify an event as a MouseButtonEvent.*/
		static const unsigned char MOUSE_BUTTON_TYPE;


		
		/** Takes a button and whether the button was pressed or released.
		@param initial_button The button which was pressed or released.
		@param is_pressed True if \a initial_button was pressed, and false if it
		was released.
		*/
		MouseButtonEvent(const key_codes::MouseButton::MouseButtonCodes& initial_button, const bool is_pressed);
		/** Copy constructor.
		@param original The object being copied.
		*/
		MouseButtonEvent(const MouseButtonEvent& original);
		/** Basic destructor.*/
		~MouseButtonEvent();


		
		/** Returns the button which was pressed or released.
		@return The button which was pressed or released.
		*/
		const key_codes::MouseButton::MouseButtonCodes& GetButton() const;
		/** Returns true if the button was pressed, and false if it was released.
		@return True if \ref button was pressed, and false if it was released.
		*/
		const bool IsPressed() const;


	private:
		/// The button which was pressed/released.
		const key_codes::MouseButton::MouseButtonCodes button;
		/// Was \ref button pressed or released?
		const bool pressed;

	};





} //avl
} //input
#endif // __AVL_INPUT_MOUSE_BUTTON_EVENT__