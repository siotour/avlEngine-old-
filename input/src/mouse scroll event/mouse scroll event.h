#ifndef __AVL_INPUT_MOUSE_SCROLL_EVENT__
#define __AVL_INPUT_MOUSE_SCROLL_EVENT__
/**********
 * Author: Sheldon Bachstein
 * Date: Sep 08, 2011
 * Description: The MouseScrollEvent is used to communicate the user scrolling
 * the mouse wheel. This includes a signed integer representing the scroll amount
 * and direction.
 **********/


#include"..\input event\input event.h"


namespace avl
{
namespace input
{
	// See the beginning of the file for details.
	class MouseScrollEvent: public InputEvent
	{
	public:
		// A constant used to initialize the InputEvent base class. This constant
		// allows clients to identify an event as a MouseScrollEvent.
		static const unsigned char MOUSE_SCROLL_TYPE;


		// Constructors:
		// Takes a signed short representing the amount and direction of the scroll
		// event.
		MouseScrollEvent(const short& initial_delta);
		// Copy constructor.
		MouseScrollEvent(const MouseScrollEvent& original);
		// Destructor.
		~MouseScrollEvent();


		// Accessors:
		// Returns the delta scroll value.
		const short& GetDelta() const;


	private:
		// The delta scroll value.
		const short delta;

	};




} //avl
} //input
#endif // __AVL_INPUT_MOUSE_SCROLL_EVENT__