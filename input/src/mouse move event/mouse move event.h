#ifndef __AVL_INPUT_MOUSE_MOVE_EVENT__
#define __AVL_INPUT_MOUSE_MOVE_EVENT__
/**
@file
Implements the MouseMoveEvent, which represents a movement input event
from a mouse device.
@author Sheldon Bachstein
@date Sep 08, 2011
*/


#include"..\input event\input event.h"


namespace avl
{
namespace input
{
	/**
	Encapsulates a movement input event from a mouse device.
	Includes the change in x and y coordinates.
	*/
	class MouseMoveEvent: public InputEvent
	{
	public:
		/** A constant used to initialize the InputEvent base class. This constant
		allows clients to identify an event as a MouseMoveEvent.*/
		static const unsigned char MOUSE_MOVE_TYPE;


		/** Constructs a new event based on \a x and \a y.
		@param x Movement along the vertical axis.
		@param y Movement along the horizontal axis.
		*/
		MouseMoveEvent(const short& x, const short& y);
		/** Copy constructor.
		@param original The object being copied.
		*/
		MouseMoveEvent(const MouseMoveEvent& original);
		/** Basic  destructor.*/
		~MouseMoveEvent();

		
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