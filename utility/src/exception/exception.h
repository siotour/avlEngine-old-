#ifndef __AVL_UTILITY_EXCEPTION__
#define __AVL_UTILITY_EXCEPTION__
/**
@file
Defines the base Exception class from which all other library exceptions inherit.
@author Sheldon Bachstein
@date January 16, 2010
 */

#include<string>



namespace avl
{
namespace utility
{
	/**
	The base class for all avl Library exceptions.
	*/
	class Exception
	{
	public:
		/** Constructs an exception given a description of the situation.
		@param description A description of the exceptional situation.
		*/
		Exception(const std::string& description);
		/** Basic destructor.*/
		virtual ~Exception();
		/** Copy constructor required in order to catch Exception by value.
		@param original The object being copied.
		*/
		Exception(const Exception& original);

		/** Gets a description of the exceptional situation.
		@return Description of the exception.
		*/
		const std::string& GetDescription() const;

	protected:
		/** Basic constructor. Only usable to subclasses.*/
		Exception();


		/// Describes the exception which occurred.
		std::string description;

		
	private:
		/// NOT IMPLEMENTED.
		const Exception& operator=(const Exception&);
	};

}// utility
}// avl
#endif // __AVL_UTILITY_EXCEPTION__