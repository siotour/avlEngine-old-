#ifndef __AVL_INPUT_INPUT_EVENT__
#define __AVL_INPUT_INPUT_EVENT__
/**********
 * Author: Sheldon Bachstein
 * Date: Sep 08, 2011
 * Description: Defines a base class for all input events. Derived classes
 * must define a constant to identify its type.
 **********/



namespace avl
{
namespace input
{
	// See the beginning of the file for details.
	class InputEvent
	{
	public:
		// Constructors:
		// Takes an integer constant which is used by clients to identify the type
		// of input event this is.
		InputEvent(const unsigned char& initial_type);
		// Copy constructor.
		InputEvent(const InputEvent& original);
		virtual ~InputEvent();

		// Accrssors:
		// Returns the input type, which is to be defined by derived classes.
		const unsigned char& GetType() const;



	private:
		// Identifies what type of input event this is.
		const unsigned char type;
	};




} //avl
} //input
#endif // __AVL_INPUT_INPUT_EVENT__