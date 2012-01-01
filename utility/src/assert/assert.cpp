/**********
 * Author: Sheldon Bachstein
 * Date: January 13, 2010
 * Description: See assert.h for details.
 **********/

#include"assert.h"
#include"..\exceptions\exceptions.h"
#include<string>


namespace avl
{
namespace utility
{



	AssertVerifyFailure::AssertVerifyFailure(const std::string& file, const int& line)
		: file(file), line(line)
	{
		description = "An assertion/verification failed on line ";
		description += line;
		description += " of source file ";
		description += file;
		description += '.';
	}



	AssertVerifyFailure::~AssertVerifyFailure()
	{
	}



	AssertVerifyFailure::AssertVerifyFailure(const avl::utility::AssertVerifyFailure& original)
		: Exception(original.GetDescription()), file(original.GetFile()), line(original.GetLine())
	{
	}



	const std::string& AssertVerifyFailure::GetFile() const
	{
		return file;
	}



	const int AssertVerifyFailure::GetLine() const
	{
		return line;
	}



}
}