/**********
 * Author: Sheldon Bachstein
 * Date: Sep 08, 2011
 * Description: See mouse move event.h for details.
 **********/

#include"mouse move event.h"
#include"..\..\..\utility\src\vertex 2d\vertex 2d.h"


namespace avl
{
namespace input
{

	// A constant used to initialize the InputEvent base class. This constant
	// allows clients to identify an event as a MouseMoveEvent.
	const unsigned char MouseMoveEvent::MOUSE_MOVE_TYPE = 0x03;


	// Constructors:
	// Takes a 2-dimensional vector representing the change in x and y
	// coordinates.
	MouseMoveEvent::MouseMoveEvent(const short& x, const short& y)
		: InputEvent(MOUSE_MOVE_TYPE), delta_x(x), delta_y(y)
	{
	}




	// Copy constructor.
	MouseMoveEvent::MouseMoveEvent(const MouseMoveEvent& original)
		: InputEvent(original.GetType()), delta_x(original.GetDeltaX()), delta_y(original.GetDeltaY())
	{
	}




	// Destructor.
	MouseMoveEvent::~MouseMoveEvent()
	{
	}




	// Returns the change in x coordinates.
	const short MouseMoveEvent::GetDeltaX() const
	{
		return delta_x;
	}




	// Returns the change in y coordinates.
	const short MouseMoveEvent::GetDeltaY() const
	{
		return delta_y;
	}


}
}