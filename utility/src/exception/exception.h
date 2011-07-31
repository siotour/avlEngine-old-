#ifndef __AVL_UTILITY_EXCEPTION__
#define __AVL_UTILITY_EXCEPTION__
/**********
 * Author: Sheldon Bachstein
 * Date: January 16, 2010
 * Description: Base exception class for all exceptions in the avl library. Includes the function getMessage()
 * which returns a string describing the exception which has occurred.
 **********/

#include<string>



namespace avl
{
namespace utility
{
	// See the description at the beginning of this file.
	class Exception
	{
	public:
		// Constructors.
		Exception(const std::string& initial_description);
		virtual ~Exception();
		// Copy constructor required in order to catch Exception by value.
		Exception(const Exception& original);

		// Returns a string describing the exception that occurred.
		const std::string& GetDescription() const;

	private:
		// Describes the exception which occurred.
		const std::string description;

		// NOT IMPLEMENTED.
		Exception();
		const Exception& operator=(const Exception&);
	};

}// utility
}// avl
#endif // __AVL_UTILITY_EXCEPTION__