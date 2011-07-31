#ifndef __AVL_UTILITY_ASSERT__
#define __AVL_UTILITY_ASSERT__
/**********
 * Author: Sheldon Bachstein
 * Date: January 13, 2010
 * Description: Conditionally-compiling ASSERT and VERIFY macros. In debug mode, these macros will throw an
 * AssertionFailure/VerificationFailure if the condition evaluates to false, but will do nothing in non-
 * debug mode.
 **********/

#include<string>
#include"..\exception\exception.h"



namespace avl
{
namespace utility
{
	class AssertionFailure;
	class VerificationFailure;
}
}

#ifdef _DEBUG

	// If x evaluates to false, throws an AssertionFailure with the current file name and the current line.
	#define ASSERT(x) if(!(x)) throw(avl::utility::AssertionFailure(__FILE__, __LINE__))

	// If x evaluates to false, throws a VerificationFailure with the current file name and the current line.
	#define VERIFY(x) if(!(x)) throw(avl::utility::VerificationFailure(__FILE__, __LINE__))

#else

	#define ASSERT(x)

	// During non-debug compilations, keeps x to maintain consistency.
	#define VERIFY(x) x

#endif



namespace avl
{
namespace utility
{

	// AssertionFailure represents the occurrence of a failed assertion. It is meant to be thrown when an assertion fails, and accepts the file-name and
	// line-number of the failed assertion. Generally used by the ASSERT macro defined above.
	class AssertionFailure: public avl::utility::Exception
	{
	public:
		// Constructors.
		// _file is the name of the file in which the assertion failed and _line is the line in that file on which assertion failed.
		AssertionFailure(const std::string& _file, const int& _line);
		~AssertionFailure();
		// Copy constructor required in order to catch AssertionFailure by value.
		AssertionFailure(const AssertionFailure& original);

		// Accessors.
		// Returns the name of the file on which the assertion failed.
		const std::string& GetFile() const;
		// Returns the line on which the assertion failed.
		const int GetLine() const;

	private:
		// File in which the assertion failed.
		const std::string file;
		// Line on which the assertion failed.
		const int line;

		// NOT IMPLEMENTED.
		AssertionFailure();
		AssertionFailure& operator=(const AssertionFailure&);
	};



	// VerificationFailure represents the occurrence of a failed verification. It is meant to be thrown when a verification fails, and accepts the file-name and
	// line-number of the failed verification. Generally used by the VERIFY macro defined above.
	class VerificationFailure: public avl::utility::Exception
	{
	public:
		// Constructors.
		// _file is the name of the file in which the verification failed and _line is the line on which the verification failed.
		VerificationFailure(const std::string& _file, const int& _line);
		~VerificationFailure();
		// Copy constructor required in order to catch VerificationFailure by value.
		VerificationFailure(const VerificationFailure& original);

		// Accessors.
		// Returns the name of the file in which the verification failed.
		const std::string& GetFile() const;
		// Returns the line on which the verification failed.
		const int GetLine() const;

	private:
		// File in which the verification failed.
		const std::string file;
		// Line on which the verification failed.
		const int line;

		// NOT IMPLEMENTED.
		VerificationFailure();
		VerificationFailure& operator=(const VerificationFailure&);
	};

}
}
#endif