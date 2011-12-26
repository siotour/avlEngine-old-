#ifndef __AVL_INPUT_MOUSE_SCROLL_EVENT__
#define __AVL_INPUT_MOUSE_SCROLL_EVENT__
/**
@file
Defines the \ref avl::input::MouseScrollEvent class.
@author: Sheldon Bachstein
@date: Sep 08, 2011
*/


#include"..\input event\input event.h"


namespace avl
{
namespace input
{
	/**
	Used to communicate the user scrolling the mouse wheel. This includes a
	signed integer representing the scroll amount and direction.
	*/
	class MouseScrollEvent: public InputEvent
	{
	public:
		/** A constant used to initialize the InputEvent base class. This constant
		allows clients to identify an event as a MouseScrollEvent.*/
		static const unsigned char MOUSE_SCROLL_TYPE;


		/** Takes a signed short representing the amount and direction of
		the scroll event.
		@param initial_delta The direction and amount in which the mouse
		wheel has been scrolled.
		*/
		MouseScrollEvent(const short& initial_delta);
		/** Copy constructor.
		@param original The object being copied.
		*/
		MouseScrollEvent(const MouseScrollEvent& original);
		/** Basic destructor.*/
		~MouseScrollEvent();


		
		/** Returns the delta scroll value.
		@return The direction and amount in which the mouse wheel has been
		scrolled.
		*/
		const short& GetDelta() const;


	private:
		/// The delta scroll value.
		const short delta;

	};




} //avl
} //input
#endif // __AVL_INPUT_MOUSE_SCROLL_EVENT__