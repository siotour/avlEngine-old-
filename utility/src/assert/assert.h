#ifndef __AVL_UTILITY_ASSERT__
#define __AVL_UTILITY_ASSERT__
/**
@file
Conditionally-compiling ASSERT and VERIFY macros. In debug mode, these macros will throw an
avl::utility::AssertionFailure / avl::utility::VerificationFailure if the condition evaluates to false, but will do
nothing in non-debug mode.
@author Sheldon Bachstein
@date January 13, 2010
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
		\param file Name of the file in which the assertion or verification failed.
		\param line The line in \a file on which the assertion or verification failed.
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
#endif