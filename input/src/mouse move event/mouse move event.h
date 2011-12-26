#ifndef __AVL_INPUT_MOUSE_MOVE_EVENT__
#define __AVL_INPUT_MOUSE_MOVE_EVENT__
/**
@file
Implements the \ref avl::input::MouseMoveEvent.
@author Sheldon Bachstein
@date Sep 08, 2011
*/


#include"..\input event\input event.h"
#include"..\..\..\utility\src\vertex 2d\vertex 2d.h"


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



		/** Constructs a new event where \a x and \a y are the movement along the
		vertical and horizontal axes.
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

		

		/** Returns the change in x coordinates.
		@return The mouse movement along the vertical axis.
		*/
		const short GetDeltaX() const;
		/** Returns the change in y coordinates.
		@return The mouse movement along the horizontal axis.
		*/
		const short GetDeltaY() const;


	private:
		/// The mouse movement along the horizontal axis.
		const short delta_x;
		/// The mouse movement along the vertical axis.
		const short delta_y;


	};



} //avl
} //input
#endif // __AVL_INPUT_MOUSE_MOVE_EVENT__