#ifndef __AVL_UTILITY_SETTINGS_FILE__
#define __AVL_UTILITY_SETTINGS_FILE__
/**
@file
Loads formatted settings data from a text file.
@author Sheldon Bachstein
@date Dec 29, 2010
@todo Properly document syntax for settings files and document the fact that
variable names are converted to lowercase when they're mapped.
@todo Completely renovate this component so that it supports editing a settings file
(requires changing the size of the file using _chsize_s()). Will require a complete overhaul
and increase complexity dramatically.
*/


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




	/** Loads formatted settings data from a text file. Is able to process comments,
	integer variables, and string variables.
	@todo The read utility functions need to be refactored into more manageable sized functions.
	*/
	class SettingsFile
	{
	public:
		/** Attempts to open the file \a file_name and read in any formatted data.
		@param file_name The name of the file to read settings from.
		@throws avl::utility::ReadWriteError If unable to open or ready from \a file_name.
		@throws avl::utility::SyntaxError If there is a problem with the syntax.
		*/
		SettingsFile(const std::string& file_name);
		/** Basic destructor.*/
		~SettingsFile();

		/** Accesses an integer setting.
		@pre \a variable is the name of an integer variable.
		@post value will contain the value associated with the variable named \a variable.
		@param variable [IN] The name of the variable to access.
		@param value [OUT] If \a variable is an integer variable, the value associated with it.
		@return True if \a variable exists and is an integer variable. False if \a variable does
		not exist or is not an integer variable.
		*/
		const bool GetIntegerValue(const std::string& variable, int& value) const;
		/** Accesses a string setting.
		@pre \a variable is the name of a string variable.
		@post value will contain the value associated with the variable named \a variable.
		@param variable [IN] The name of the variable to access.
		@param value [OUT] If \a variable is a string variable, the value associated with it.
		@return True if \a variable exists and is a string variable. False if \a variable does
		not exist or is not a string variable.
		*/
		const bool GetStringValue(const std::string& variable, std::string& value) const;
	private:
		/** Attempts to read in the formatted settings data from \ref file.
		@post \ref settings will be populated with the variable name/value pairs stored in \ref file.
		@throws avl::utility::ReadWriteError If unable to read from \ref file.
		@throws avl::utility::SyntaxError If there is a syntax error. Will indicate the error type.
		*/
		void LoadSettings();
		/** Attempts to read in formatted data from a single line.
		@post Will add a new variable name/value pair to \ref settings.
		@throws avl::utility::ReadWriteError If an error occurs while reading from \ref file.
		@throws avl::utility::SyntaxError If there is a syntax error in \ref file. Will indicate the
		type of syntax error.
		*/
		void ReadLine(int& line_number);
		/** Attempts to read in a variable name.
		@param line_number The line number which is currently being analyzed in the file.
		This is used to generate a \ref SyntaxError with the correct line number if an
		error occurs.
		@return  The variable name.
		@throws avl::utility::ReadWriteError If an error occurs while reading \ref file.
		@throws avl::utility::SyntaxError If there is a syntax error with a variable name.
		line_number should be the
		*/
		const std::string ReadVariableName(int& line_number);
		/** Attempts to read in a value.
		@pre \a value must be NULL.
		@param line_number [IN] The line number which is currently being analyzed in the file.
		This is used to generate a \ref SyntaxError with the correct line number if an
		error occurs.
		@param value [OUT] The new value.
		@return True if the value is an integer, and false if it's a string.
		@throws avl::utility::ReadWriteError If an error occurs while reading from \ref file.
		@throws avl::utility::SyntaxError If there is a syntax error with a setting value.
		@todo avl::utility::SettingValue should be made into a union, and this function should return
		a avl::utility::SettingValue.
		*/
		const bool ReadValue(int& line_number, const void*& value);

		/// Pointer to the file which contains formatted setting information.
		std::ifstream file;
		/// Map of variable names to SettingValue objects.
		std::map<std::string, avl::utility::SettingValue* const> settings;

		/// NOT IMPLEMENTED.
		SettingsFile(const SettingsFile&);
		/// NOT IMPLEMENTED.
		const SettingsFile& operator=(const SettingsFile&);
	};




	/** Contains information regarding a variable's value, and is intended
	to be mapped to a variable name. It contains the type of value and the actual value.
	@todo Should be made into a union and nested within the avl::utility::SettingsFile class.
	*/
	class SettingValue
	{
	public:
		/**Describes what type of value is being stored.*/
		enum ValueType {INTEGER_VALUE, STRING_VALUE};

		/** Constructor for integer values. Note that \a value will be deleted when this object is
		destroyed.
		@post \ref value_type will be set to \ref INTEGER_VALUE.
		@param value The value represented by this object.
		*/
		SettingValue(const int* const value);
		/** Constructor for string values. Note that value will be deleted when this object is
		destroyed.
		@post \ref value_type will be set to \ref STRING_VALUE.
		@param value The value represented by this object.
		*/
		SettingValue(const std::string* const value);
		/** Deletes \ref value.*/
		~SettingValue();

		/** Returns \ref value_type.
		@return The value of \ref value_type.
		*/
		const avl::utility::SettingValue::ValueType& GetValueType() const;
		/** Returns \ref value.
		@return \ref value.
		*/
		const void* const GetValue() const;
	private:
		/// The type of value.
		avl::utility::SettingValue::ValueType value_type;
		/// A void pointer to the value.
		const void* value;

		/// NOT IMPLEMENTED.
		SettingValue(const SettingValue&);
		/// NOT IMPLEMENTED.
		const SettingValue& operator=(const SettingValue&);
	};




	/** Indicates that there was a syntax error in a settings file. Includes the line number
	where the error was found and an error code describing what kind of error occured.
	*/
	class SyntaxError: public Exception
	{
	public:
		/** Describes what kind of syntax error has occurred.*/
		enum ErrorType{BAD_VARIABLE_NAME, BAD_VALUE, OUT_OF_RANGE_INTEGER};
		
		/** Full-spec constructor.
		@param error_type The type of syntax error which was found.
		@param line_number the line on which the syntax error was detected.
		*/
		SyntaxError(const ErrorType& error_type, const int& line_number);
		/** Basic destructor.*/
		~SyntaxError();
		/** Required in order to throw/catch by value.
		@param original The object being copied.
		*/
		SyntaxError(const SyntaxError& original);

		/** Gets the error type.
		@return The type of syntax error found.
		*/
		const ErrorType& GetErrorType() const;
		/** Gets the line on which the error occured.
		@return The line on which the error was detected.
		*/
		const int& GetLineNumber() const;
	private:
		/// The error type.
		const ErrorType error_type;
		/// The line number on which the error occured.
		const int line_number;

		/// NOT IMPLEMENTED.
		const SyntaxError& operator=(const SyntaxError&);
	};





} //avl
} //utility
#endif // __AVL_UTILITY_SETTINGS_FILE__