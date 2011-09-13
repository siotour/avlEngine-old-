/**********
 * Author: Sheldon Bachstein
 * Date: Sep 08, 2011
 * Description: See input event.h for details.
 **********/

#include"input event.h"


namespace avl
{
namespace input
{
	// Takes an integer constant which is used by clients to identify the type
	// of input event this is.
	InputEvent::InputEvent(const unsigned char& initial_type)
		: type(initial_type)
	{
	}




	// Copy constructor.
	InputEvent::InputEvent(const InputEvent& original)
		: type(original.GetType())
	{
	}




	// Destructor.
	InputEvent::~InputEvent()
	{
	}




	// Returns the input type, which is to be defined by derived classes.
	const unsigned char& InputEvent::GetType() const
	{
		return type;
	}



}
}