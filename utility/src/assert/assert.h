#pragma once
#ifndef AVL_UTILITY_ASSERT__
#define AVL_UTILITY_ASSERT__
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
Conditionally-compiling ASSERT and VERIFY macros. In debug mode, these macros will throw an
avl::utility::AssertionFailure / avl::utility::VerificationFailure if the condition evaluates to false, but will do
nothing in non-debug mode.
@author Sheldon Bachstein
@date January 13, 2010
@todo One possible solution to the compiler issuing warnings in Release build for each
verify statement is to have the verify statement expand to a more complex expression
in Release mode. Something like "a == b ? true : false."
*/

#include<string>
#include"..\exceptions\exceptions.h"



namespace avl
{
namespace utility
{
	class AssertVerifyFailure;

#ifdef _DEBUG
	/**
	If \a x evaluates to false and _DEBUG is defined, throws an \ref avl::utility::AssertVerifyFailure with the current file name and the current line.
	If _DEBUG is not defined, \a x will be discarded and not evaluated.
	@hideinitializer
	*/
	#define ASSERT(x) if(!(x)) throw(avl::utility::AssertVerifyFailure(__FILE__, __LINE__))

	/**
	If \a x evaluates to false and _DEBUG is defined, throws an \ref avl::utility::AssertVerifyFailure with the current file name and the current line.
	If _DEBUG is not defined, \a x will still be evaluated to maintain consistency between compile modes, but nothing happens if it evaluates to false.
	@hideinitializer
	*/
	#define VERIFY(x) if(!(x)) throw(avl::utility::AssertVerifyFailure(__FILE__, __LINE__))

#else
	/**
	If \a x evaluates to false and _DEBUG is defined, throws an \ref avl::utility::AssertVerifyFailure with the current file name and the current line.
	If _DEBUG is not defined, \a x will be discarded and not evaluated.
	@hideinitializer
	*/
	#define ASSERT(x)

	/**
	If \a x evaluates to false and _DEBUG is defined, throws an \ref avl::utility::AssertVerifyFailure with the current file name and the current line.
	If _DEBUG is not defined, \a x will still be evaluated to maintain consistency between compile modes, but nothing happens if it evaluates to false.
	@hideinitializer
	*/
	#define VERIFY(x) x
#endif

}
}




namespace avl
{
namespace utility
{
	/**
	Represents the occurrence of a failed assertion or verification. Thrown when an assertion or verification fails,
	and accepts the file-name and line-number of the failure. Generally used by the \ref ASSERT and \ref VERIFY macros.
	@sa ASSERT
	@sa VERIFY
	*/
	class AssertVerifyFailure: public Exception
	{
	public:
		/** Constructs an AssertVerifyFailure from a file name and line number.
		@param file Name of the file in which the assertion or verification failed.
		@param line The line in \a file on which the assertion or verification failed.
		@todo \a line must be parsed to a string, not simply added to the end of the string. 
		*/
		AssertVerifyFailure(const std::string& file, const int& line);
		/** Basic destructor.*/
		~AssertVerifyFailure();
		/** Copy constructor.
		\param original Object being copied.
		*/
		AssertVerifyFailure(const AssertVerifyFailure& original);


		/** Returns the name of the file in which the assertion or verification failed.
		\return Name of the file of the failed assertion of verification.
		*/
		const std::string& GetFile() const;
		/** Returns the line of the failed assertion or verification.
		\return The line number of the failed assertion or verification.
		*/
		const int GetLine() const;

	private:
		/// File in which the assertion or verification failed.
		const std::string file;
		/// Line on which the assertion or verification failed.
		const int line;

		/// NOT IMPLEMENTED.
		AssertVerifyFailure();
		/// NOT IMPLEMENTED.
		AssertVerifyFailure& operator=(const AssertVerifyFailure&);
	};



}
}
#endif // AVL_UTILITY_ASSERT__