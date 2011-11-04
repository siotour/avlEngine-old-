/**********
 * Author: Sheldon Bachstein
 * Date: January 13, 2010
 * Description: See assert.h for details.
 **********/

#include"assert.h"
#include"..\exception\exception.h"
#include<string>


namespace avl
{
namespace utility
{



	AssertionFailure::AssertionFailure(const std::string& file, const int& line)
		: Exception("avl::utility::AssertionFailure -- An assertion has failed."), file(file), line(line)
	{
	}



	AssertionFailure::~AssertionFailure()
	{
	}



	AssertionFailure::AssertionFailure(const avl::utility::AssertionFailure& original)
		: Exception(original), file(original.GetFile()), line(original.GetLine())
	{
	}



	const std::string& AssertionFailure::GetFile() const
	{
		return file;
	}



	const int AssertionFailure::GetLine() const
	{
		return line;
	}





	VerificationFailure::VerificationFailure(const std::string& file, const int& line)
		: Exception("avl::utility::VerificationFailure -- A verification has failed."), file(file), line(line)
	{
	}



	VerificationFailure::~VerificationFailure()
	{
	}



	VerificationFailure::VerificationFailure(const avl::utility::VerificationFailure& original)
		: Exception(original), file(original.GetFile()), line(original.GetLine())
	{
	}



	const std::string& VerificationFailure::GetFile() const
	{
		return file;
	}



	const int VerificationFailure::GetLine() const
	{
		return line;
	}


}
}