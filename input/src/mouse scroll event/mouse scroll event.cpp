/**********
 * Author: Sheldon Bachstein
 * Date: Sep 08, 2011
 * Description: See mouse scroll event.h for details.
 **********/

#include"mouse scroll event.h"



namespace avl
{
namespace input
{

	// A constant used to initialize the InputEvent base class. This constant
	// allows clients to identify an event as a MouseScrollEvent.
	const unsigned char MouseScrollEvent::MOUSE_SCROLL_TYPE = 0x04;


	// Constructors:
	// Takes a signed short representing the amount and direction of the scroll
	// event.
	MouseScrollEvent::MouseScrollEvent(const short& initial_delta)
		: InputEvent(MOUSE_SCROLL_TYPE), delta(initial_delta)
	{
	}




	// Copy constructor.
	MouseScrollEvent::MouseScrollEvent(const MouseScrollEvent& original)
		: InputEvent(original.GetType()), delta(original.GetDelta())
	{
	}




	// Destructor.
	MouseScrollEvent::~MouseScrollEvent()
	{
	}




	// Returns the delta scroll value.
	const short& MouseScrollEvent::GetDelta() const
	{
		return delta;
	}


}
}