#ifndef __AVL_INPUT_MOUSE_MOVE_EVENT__
#define __AVL_INPUT_MOUSE_MOVE_EVENT__
/**********
 * Author: Sheldon Bachstein
 * Date: Sep 08, 2011
 * Description: The MouseMoveEvent encapsulates a movement input event from the
 * mouse. This includes the change in x and y coordinates.
 **********/


#include"..\input event\input event.h"
#include"..\..\..\utility\src\vertex 2d\vertex 2d.h"


namespace avl
{
namespace input
{
	// See the beginning of the file for details.
	class MouseMoveEvent: public InputEvent
	{
	public:
		// A constant used to initialize the InputEvent base class. This constant
		// allows clients to identify an event as a MouseMoveEvent.
		static const unsigned char MOUSE_MOVE_TYPE;


		// Constructors:
		// Takes a 2-dimensional vector representing the change in x and y
		// coordinates.
		MouseMoveEvent(const short& x, const short& y);
		// Copy constructor.
		MouseMoveEvent(const MouseMoveEvent& original);
		// Destructor.
		~MouseMoveEvent();

		// Accessors:
		// Returns the mouse delta coordinates.
		const utility::Vertex2D& GetDelta() const;
		// Returns the change in x coordinates.
		const short GetDeltaX() const;
		// Returns the change in y coordinates.
		const short GetDeltaY() const;


	private:
		// The delta coordinates.
		const short delta_x;
		const short delta_y;


	};



} //avl
} //input
#endif // __AVL_INPUT_MOUSE_MOVE_EVENT__