#ifndef __AVL_UTILITY_SETTINGS_FILE__
#define __AVL_UTILITY_SETTINGS_FILE__
/**********
 * Author: Sheldon Bachstein
 * Date: Dec 29, 2010
 * Description: Loads formatted settings data from a text file. Is able to process comments,
 * integer variables, and string variables. See Example.txt for details on formatting.
 * TODO: Properly document syntax for settings files and document the fact that
 * variable names are converted to lowercase when they're mapped.
 * TODO: Completely renovate this component so that it supports editing a settings file
 * (requires changing the size of the file using _chsize_s()). Will require a complete overhaul
 * and increase complexity dramatically.
 **********/


#include"..\exception\exception.h"
#include<string>
#include<fstream>
#include<map>


namespace avl
{
namespace utility
{
	// Forward declarations.
	class SettingValue;




	// See the beginning of the file for details.
	class SettingsFile
	{
	public:
		// Constructors:
		// Attempts to open the specified file and read in any formatted data. If unable to open
		// the file, will throw a avl::utility::ReadWriteError. If there is a problem with the
		// syntax, will throw a avl::utility::SyntaxError indicating the syntax error type and
		// the line on which the syntax error occured.
		SettingsFile(const std::string& file_name);
		// Destructor.
		~SettingsFile();

		// Accessors:
		// If the specified variable has an associated integer value, it will be assigned to
		// the value parameter and true will be returned. If the variable does not exist or the
		// variable does not have an associated integer value, the value parameter will be left
		// unchanged and false will be returned.
		const bool GetIntegerValue(const std::string& variable, int& value) const;
		// If the specified variable has an associated string value, it will be assigned to
		// the value parameter and true will be returned. If the variable does not exist or the
		// variable does not have an associated string value, the value parameter will be left
		// unchanged and false will be returned.
		const bool GetStringValue(const std::string& variable, std::string& value) const;
	private:
		// Attempts to read in the formatted settings data from the file specified. If an error
		// occurs while attempting to read from the file, will throw a
		// avl::utility::ReadWriteError. If there is a syntax error, will throw a
		// avl::utility::SyntaxError with one of these types:
		// avl::utility::SyntaxError::BAD_VARIABLE_NAME if there is a problem with the variable name;
		// avl::utility::SyntaxError::BAD_VALUE if there is a problem with a variable's value;
		// or avl::utulity::SyntaxError::OUT_OF_RANGE_INTEGER if the integer value for a variable is
		// out of the valid range of values.
		void LoadSettings();
		// Attempts to read in formatted data from a single line.
		// If an error occurs while attempting to read from the file, will throw a
		// avl::utility::ReadWriteError. If there is a syntax error, will throw a
		// avl::utility::SyntaxError with one of these types:
		// avl::utility::SyntaxError::BAD_VARIABLE_NAME if there is a problem with the variable name;
		// avl::utility::SyntaxError::BAD_VALUE if there is a problem with a variable's value;
		// or avl::utulity::SyntaxError::OUT_OF_RANGE_INTEGER if the integer value for a variable is
		// out of the valid range of values. If successful, will add the variable name and value to
		// settings.
		void ReadLine(int& line_number);
		// Attempts to read in a variable name. Will throw a avl::utility::ReadWriteError if an
		// error occurs while reading from the file. Will throw a avl::utility::SyntaxError
		// of type avl::utility::SyntaxError::BAD_VARIABLE_NAME if there is a syntax error
		// in the file. Returns the variable name if successful. line_number should be the
		// number of the line which is currently being analyzed in the file so that error
		// reporting accurately reports the line which is erroneous.
		const std::string ReadVariableName(int& line_number);
		// Attempts to read in a value. Will throw a avl::utility::ReadWriteError if an error
		// occurs while reading from the file. Will throw a avl::utility::SyntaxError of type
		// avl::utility::SyntaxError::BAD_VALUE if there is a syntax error. Will throw a
		// avl::utility::SyntaxError of type avl::utility::SyntaxError::OUT_OF_RANGE_INTEGER
		// if an integer value is out of the valid range for integers. line_number should be the
		// number of the line which is currently being analyzed in the file. value must be a NULL
		// pointer, and the return value indicates whether or not the value is integer (as opposed
		// to a string).
		const bool ReadValue(int& line_number, const void*& value);

		// Pointer to the file which contains formatted setting information.
		std::ifstream file;
		// Map of variable names to SettingValue objects.
		std::map<std::string, avl::utility::SettingValue* const> settings;

		// NOT IMPLEMENTED:
		SettingsFile(const SettingsFile&);
		const SettingsFile& operator=(const SettingsFile&);
	};




	// This object contains information regarding a variable's value, and is intended
	// to be mapped to a variable name. It contains the type of value and the actual value.
	class SettingValue
	{
	public:
		// Describes what type of value is being stored.
		enum ValueType {INTEGER_VALUE, STRING_VALUE};

		// Constructors:
		// Constructor for integer values. Note that value will be deleted when this object is
		// destroyed or if its value is ever reassigned.
		SettingValue(const int* const value);
		// Constructor for string values. Note that value will be deleted when this object is
		// destroyed or if its value is ever reassigned.
		SettingValue(const std::string* const value);
		// Deletes the data value it was given when created.
		~SettingValue();

		// Accessors:
		// Returns the type of value.
		const avl::utility::SettingValue::ValueType& GetValueType() const;
		// Returns the value as a void pointer.
		const void* const GetValue() const;
	private:
		// The type of value.
		avl::utility::SettingValue::ValueType value_type;
		// A void pointer to the value.
		const void* value;

		// NOT IMPLEMENTED:
		SettingValue(const SettingValue&);
		const SettingValue& operator=(const SettingValue&);
	};




	// This exception indicates that there was a syntax error in the settings file when
	// attempting to load it. Includes the line number where the error was found and an error
	// code describing what kind of error occured.
	class SyntaxError: public Exception
	{
	public:
		// Describes what kind of syntax error has occurred.
		enum ErrorType{BAD_VARIABLE_NAME, BAD_VALUE, OUT_OF_RANGE_INTEGER};
		
		// Constructors:
		SyntaxError(const ErrorType& error_type, const int& line_number);
		~SyntaxError();
		// Copy constructor so that this object may be caught (using throw/catch) by value.
		SyntaxError(const SyntaxError& original);

		// Accessors:
		// Returns the error type.
		const ErrorType& GetErrorType() const;
		// Returns the line on which the error occured.
		const int& GetLineNumber() const;
	private:
		// The error type.
		const ErrorType error_type;
		// The line number on which the error occured.
		const int line_number;

		// NOT IMPLEMENTED
		const SyntaxError& operator=(const SyntaxError&);
	};





} //avl
} //utility
#endif // __AVL_UTILITY_SETTINGS_FILE__