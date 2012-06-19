/* Copyright 2012 Sheldon Bachstein
This file is part of the avl Library.

The avl Library is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The avl Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the avl Library.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
@file
Implementation for the assert component. See "assert.h" for details.
@author Sheldon Bachstein
@ate Jan 13, 2010
*/

#include"assert.h"
#include"..\exceptions\exceptions.h"
#include<string>

namespace avl
{
namespace utility
{

	// See method declaration for details.
	AssertVerifyFailure::AssertVerifyFailure(const std::string& file, const int& line)
		: file(file), line(line)
	{
		description = "An assertion/verification failed on line ";
		description += line;
		description += " of source file ";
		description += file;
		description += '.';
	}

	// See method declaration for details.
	AssertVerifyFailure::~AssertVerifyFailure()
	{
	}

	// See method declaration for details.
	AssertVerifyFailure::AssertVerifyFailure(const avl::utility::AssertVerifyFailure& original)
		: Exception(original.GetDescription()), file(original.GetFile()), line(original.GetLine())
	{
	}

	// See method declaration for details.
	const std::string& AssertVerifyFailure::GetFile() const
	{
		return file;
	}

	// See method declaration for details.
	const int AssertVerifyFailure::GetLine() const
	{
		return line;
	}

}
}