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
#include"..\exception\exception.h"



namespace avl
{
namespace utility
{
	class AssertionFailure;
	class VerificationFailure;

#ifdef _DEBUG
	/**
	If \a x evaluates to false and _DEBUG is defined, throws an avl::utility::AssertionFailure with the current file name and the current line.
	If _DEBUG is not defined, \a x will be discarded and not evaluated.
	@hideinitializer
	*/
	#define ASSERT(x) if(!(x)) throw(avl::utility::AssertionFailure(__FILE__, __LINE__))

	/**
	If \a x evaluates to false and _DEBUG is defined, throws a avl::utility::VerificationFailure with the current file name and the current line.
	If _DEBUG is not defined, \a x will still be evaluated to maintain consistency between compile modes.
	@hideinitializer
	*/
	#define VERIFY(x) if(!(x)) throw(avl::utility::VerificationFailure(__FILE__, __LINE__))

#else
	/**
	If \a x evaluates to false and _DEBUG is defined, throws an avl::utility::AssertionFailure with the current file name and the current line.
	If _DEBUG is not defined, \a x will be discarded and not evaluated.
	@hideinitializer
	*/
	#define ASSERT(x)

	/**
	If \a x evaluates to false and _DEBUG is defined, throws a avl::utility::VerificationFailure with the current file name and the current line.
	If _DEBUG is not defined, \a x will still be evaluated to maintain consistency between compile modes.
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
	Represents the occurrence of a failed assertion. Thrown when an assertion fails, and accepts the file-name and
	line-number of the failed assertion. Generally used by the \ref ASSERT macro.
	*/
	class AssertionFailure: public avl::utility::Exception
	{
	public:
		/** Constructs an AssertionFailure from a file name and line number.
		\param file Name of the file in which the assertion failed.
		\param line The line in \a file on which assertion failed.
		*/
		AssertionFailure(const std::string& file, const int& line);
		/** Basic destructor.*/
		~AssertionFailure();
		/** Copy constructor required in order to catch AssertionFailure by value.
		\param original Object being copied.
		*/
		AssertionFailure(const AssertionFailure& original);


		/** Returns the name of the file in which the assertion failed.
		\return Name of the file in which the assertion failed.
		*/
		const std::string& GetFile() const;
		/** Returns the line on which the assertion failed.
		\return The line number on which the assertion failed.
		*/
		const int GetLine() const;

	private:
		/// File in which the assertion failed.
		const std::string file;
		/// Line on which the assertion failed.
		const int line;

		/// NOT IMPLEMENTED.
		AssertionFailure();
		/// NOT IMPLEMENTED.
		AssertionFailure& operator=(const AssertionFailure&);
	};



	/**
	Represents the occurrence of a failed verification. Thrown when a verification fails, and accepts the file-name and
	line-number of the failed verification. Generally used by the \ref VERIFY macro.
	*/
	class VerificationFailure: public avl::utility::Exception
	{
	public:
		/**
		\param file The name of the file in which the verification failed.
		\param line The line in \a file on which the verification failed.
		*/
		VerificationFailure(const std::string& file, const int& line);
		/** Basic destructor.*/
		~VerificationFailure();
		/** Copy constructor required in order to catch VerificationFailure by value.
		\param original The object being copied.
		*/
		VerificationFailure(const VerificationFailure& original);


		/** Returns the name of the file in which the verification failed.
		\return The name of the file in which the verification failed.
		*/
		const std::string& GetFile() const;
		/** Returns the line on which the verification failed.
		\return The line number on which the verification failed.
		*/
		const int GetLine() const;

	private:
		/// File in which the verification failed.
		const std::string file;
		/// Line on which the verification failed.
		const int line;

		/// NOT IMPLEMENTED.
		VerificationFailure();
		/// NOT IMPLEMENTED.
		VerificationFailure& operator=(const VerificationFailure&);
	};

}
}
#endif