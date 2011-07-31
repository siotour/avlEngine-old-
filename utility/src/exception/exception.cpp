/**********
 * Author: Sheldon Bachstein
 * Date: January 16, 2010
 * Description: See exception.h for details.
 **********/

#include"exception.h"
#include<string>


namespace avl
{
namespace utility
{

	// initial_description should describe the exception that occurred.
	Exception::Exception(const std::string& initial_description)
		: description(initial_description)
	{
	}



	Exception::Exception(const avl::utility::Exception& original)
		: description(original.GetDescription())
	{
	}



	Exception::~Exception()
	{
	}



	// Returns a string describing the exception that occurred.
	const std::string& Exception::GetDescription() const
	{
		return description;
	}



}
}