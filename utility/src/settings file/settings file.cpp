/**********
 * Author: Sheldon Bachstein
 * Date: Dec 29, 2010
 * Description: See settings file.h for details.
 **********/

#include"settings file.h"
#include"..\assert\assert.h"
#include"..\read write error\read write error.h"
#include<cctype>
#include<cstdlib>
#include<iostream>




namespace avl
{
namespace utility
{

	// Attempts to open the specified file and read in formatted data. If an error occurs
	// while attempting to read from the file, will throw a avl::utility::ReadWriteError. If there is a syntax
	// error, will throw a avl::utility::SyntaxError with one of these types:
	// avl::utility::SyntaxError::BAD_VARIABLE_NAME if there is a problem with the variable name;
	// avl::utility::SyntaxError::BAD_VALUE if there is a problem with a variable's value;
	// or avl::utulity::SyntaxError::OUT_OF_RANGE_INTEGER if the integer value for a variable is out of the valid
	// range of values.tingsFile::SettingsFile(const std::string& file_name)
	SettingsFile::SettingsFile(const std::string& file_name)
	{
		// First, tell the file not to throw any exceptions.
		file.exceptions(std::ios_base::goodbit);
		// Now attempt to open the specified file name for reading.
		file.open(file_name.c_str(), std::ios_base::in);

		// Check to see if the file was successfully opened. If not, then throw an avl::utility::ReadWriteError.
		if(file.fail() == true)
		{
			throw avl::utility::ReadWriteError();
		}

		// At this point, the file should be ready for reading. Attempt reading the file for formatted data.
		this->LoadSettings();
	}




	// Destructor. If the data file is open, will close it. Also iterates through the settings map and deletes
	// the avl::utility::SettingValue for each key since they've been created on the heap.
	SettingsFile::~SettingsFile()
	{
		// Close the data file.
		file.close();

		// Iterate through the settings map and delete each value.
		for(std::map<std::string, SettingValue* const>::iterator i = settings.begin();
																					i != settings.end(); ++i)
		{
			delete i->second;
		}
	}




	// If the specified variable has an associated integer value, it will be assigned to the value parameter and
	// true will be returned. If the variable does not exist or the variable does not have an associated integer
	// value, the value parameter will be left unchanged and false will be returned.
	const bool SettingsFile::GetIntegerValue(const std::string& variable, int& value) const
	{
		// First, check to see if a setting with the specified variable name exists. If not, return false.
		std::map<std::string, SettingValue* const>::const_iterator i = settings.find(variable);
	
		if(i == settings.end())
		{
			return false;
		}
		// If the variable exists, but is not an integer, return false.
		else if(i->second->GetValueType() != SettingValue::INTEGER_VALUE)
		{
			return false;
		}
		// Otherwise the variable must be an integer, so assign its value to the value parameter and return
		// true.
		else
		{
			value = *static_cast<const int* const>(i->second->GetValue());
			return true;
		}
	}




	// If the specified variable has an associated string value, it will be assigned to the value parameter and
	// true will be returned. If the variable does not exist or the variable does not have an associated string
	// value, the value parameter will be left unchanged and false will be returned.
	const bool SettingsFile::GetStringValue(const std::string& variable, std::string& value) const
	{
		// First, check to see if a setting with the specified variable name exists. If not, return false.
		std::map<std::string, SettingValue* const>::const_iterator i = settings.find(variable);
	
		if(i == settings.end())
		{
			return false;
		}
		// If the variable exists, but is not a string, return false.
		else if(i->second->GetValueType() != SettingValue::STRING_VALUE)
		{
			return false;
		}
		// Otherwise the variable must be a string, so assign its value to the value parameter and return
		// true.
		else
		{
			value = *static_cast<const std::string* const>(i->second->GetValue());
			return true;
		}
	}




	// Attempts to read in the formatted settings data from the file specified upon creation. If an error occurs
	// while attempting to read from the file, will throw a avl::utility::ReadWriteError. If there is a syntax
	// error, will throw a avl::utility::SyntaxError with one of these types:
	// avl::utility::SyntaxError::BAD_VARIABLE_NAME if there is a problem with the variable name;
	// avl::utility::SyntaxError::BAD_VALUE if there is a problem with a variable's value;
	// or avl::utulity::SyntaxError::OUT_OF_RANGE_INTEGER if the integer value for a variable is out of the valid
	// range of values.
	void avl::utility::SettingsFile::LoadSettings()
	{
		ASSERT(file.is_open() == true);
		// Loop through each line in the file until the end of the file is reached. Stop if the stream is not ready
		// for reading. Keep track of the line number for error reporting.
		for(int line_number = 1; file.good() == true; ++line_number)
		{
			ReadLine(line_number);
		}

		// At this point, the end of the file must have been reached or an error must have occured while attempting
		// to read in data. If it was an error, throw an avl::utility::ReadWriteError.
		if(file.fail() == true)
		{
			throw avl::utility::ReadWriteError();
		}
	}




	// Attempts to read in formatted data from a single line.If an error occurs while attempting to read from the file,
	// will throw a avl::utility::ReadWriteError. If there is a syntax error, will throw a avl::utility::SyntaxError
	// with one of these types:
	// avl::utility::SyntaxError::BAD_VARIABLE_NAME if there is a problem with the variable name;
	// avl::utility::SyntaxError::BAD_VALUE if there is a problem with a variable's value; or
	// avl::utulity::SyntaxError::OUT_OF_RANGE_INTEGER if the integer value for a variable is out of the valid range of
	// values.
	// If successful, will add the variable name and value to settings.
	void avl::utility::SettingsFile::ReadLine(int& line_number)
	{
		// Peek at the next character.
		char c = file.peek();
	
		// If the next character is the end of the file, then return.
		if(c == EOF)
		{
			return;
		}
		// If the next character is a newline, then skip it and return.
		if(c == '\n')
		{
			// Skip the newline character.
			file.get();
			return;
		}


		// If the character is a forward slash, then the line must either be a comment (denoted by double forward
		// slashes) or else there is a syntax error. Either way, skip the slash character and then go from there.
		if(c == '/')
		{
			// Skip the forward slash.
			file.get();

			// If the next character is another forward slash, then this line is a comment and should be skipped.
			if(file.peek() == '/')
			{
				// Discard the entire line until either a newline is found or the file is no longer in a readable
				// state (including if the end of file is reached). If a newline is found, discard it.
				for(c = file.peek(); c != '\n' && file.good() == true; c = file.peek())
				{
					file.get();
				}
				// Discard the newline.
				if(c == '\n')
				{
					file.get();
				}
				return;
			}

			// At this point, the line is not a comment and so must be an error. Throw a avl::utility::SyntaxError
			// of type avl::utility::SyntaxError::BAD_VARIABLE_NAME.
			throw SyntaxError(SyntaxError::BAD_VARIABLE_NAME, line_number);
		}



		// If the character is not alphabetic, then there is a syntax error. Throw a avl::utility::SyntaxError
		// of type avl::utility::SyntaxError::BAD_VARIABLE_NAME.
		if(isalpha(c) == 0)
		{
			throw SyntaxError(SyntaxError::BAD_VARIABLE_NAME, line_number);
		}


		// At this point, the character should be the beginning of a variable name.
		const std::string variable_name = this->ReadVariableName(line_number);

		// Peek at the next character.
		c = file.peek();

		// At this point, if c is not an equal sign then there has been an error while reading from the file or
		// there is a syntax error. If the former, throw a avl::utility::ReadWriteError(). If the latter, throw
		// a avl::utility::SyntaxError of type avl::utility::SyntaxError::BAD_VARIABLE_NAME.
		if (c != '=')
		{
			// If an error occured while reading from the file, throw a avl::utility::ReadWriteError.
			if(file.fail() == true)
			{
				throw ReadWriteError();
			}
			// Otherwise there must be a syntax error. Throw a avl::utility::SyntaxError of type
			// avl::utility::SyntaxError::BAD_VARIABLE_NAME.
			else
			{
				throw SyntaxError(SyntaxError::BAD_VARIABLE_NAME, line_number);
			}
		}

		// Discard the equal sign.
		file.get();

		// So, the user has at this point supplied a valid variable name and has placed an equal sign after it.
		// Next, attempt to read the value and store whether or not the value is an integer or string.
		const void* value = NULL;
		bool is_integer = this->ReadValue(line_number, value);

		// Create the avl::utility::SettingValue on the heap for this value.
		avl::utility::SettingValue* setting_value = NULL;

		// If the value is an integer, it must first be converted from string to int and then copied to the heap.
		if(is_integer == true)
		{
			setting_value = new SettingValue(static_cast<const int* const>(value));
		}
		// Strings can just be copied to the heap.
		else
		{
			setting_value = new SettingValue(static_cast<const std::string* const>(value));
		}
	
		// Now that we have a variable name and a SettingValue for it, add it to the settings map.
		std::pair<std::map<std::string, SettingValue* const>::iterator, bool> result =
			settings.insert(std::pair<std::string, SettingValue* const>(variable_name, setting_value));

		// Check to see if the new variable and SettingValue pair was successfully added to the map. If there
		// was already such a variable as a key, then throw a avl::utility::SyntaxError of type
		// avl::utility::SyntaxError::BAD_VARIABLE_NAME.
		if (result.second == false)
		{
			throw SyntaxError(SyntaxError::BAD_VARIABLE_NAME, line_number);
		}
	}




	// Attempts to read in a variable name. Will throw a avl::utility::ReadWriteError if an error occurs while
	// reading from the file. Will throw a avl::utility::SyntaxError of type
	// avl::utility::SyntaxError::BAD_VARIABLE_NAME if there is a syntax error in the file. Returns the variable
	// name if successful.
	const std::string SettingsFile::ReadVariableName(int& line_number)
	{
		// Any alphabetic characters or underscores to the next equal sign are part of the variable name. If any
		// characters other than alphabetic characters or an underscore are encountered (inluding whitespace,
		// newlines, and end of file) are reached before an equal sign, then there is a syntax error. In such a
		// case, throw a avl::utility::SyntaxError of type avl::utility::SyntaxError::BAD_VARIABLE_NAME.
		// Loop until a non-alphabetic, non-underscore is found. Add each character to the variable name.
		std::string variable_name;
		for(char c = file.peek(); isalpha(c) != 0 || c == '_'; c = file.peek())
		{
			// Add the lower-case version of the char to the variable name.
			variable_name += tolower(file.get());
		}

		// Return the variable name.
		return variable_name;
	}





	// Attempts to read in a value. Will throw a avl::utility::ReadWriteError if an error occurs while reading
	// from the file. Will throw a avl::utility::SyntaxError of type avl::utility::SyntaxError::BAD_VALUE if there
	// is a syntax error. Will throw a avl::utility::SyntaxError of type
	// avl::utility::SyntaxError::OUT_OF_RANGE_INTEGER if an integer value is out of the valid range for integers.
	const bool SettingsFile::ReadValue(int& line_number, const void*& value)
	{
		ASSERT(value != NULL);
		// Used to store the value as a string. If it is an integer value, it will be converted later. 
		std::string string_value;
		// Indicates whether the value is an integer or not. If it is, it has to be converted later.
		bool is_integer = false;

		// Peek at the next character.
		char c = file.peek();

		// If the next character is numeric or a minus sign, then the value should be an integer.
		if(isdigit(c) != 0 || c == '-')
		{
			// At this point, the value should be an integer. Loop through each character until a
			// non-numeric character is found or the file is no longer readable, adding each
			// numeric character to string_value.
			string_value = file.get();
			is_integer = true;
			for(c = file.peek(); isdigit(c) != 0 && file.good() == true; c = file.peek())
			{
				// Add the character to the string_value.
				string_value += file.get();
			}



			// If the next character isn't a newline or the file is no longer readable, then either
			// a syntax error has occured or there was an error while reading the file.
			if(file.peek() != '\n' || file.good() != true)
			{
				// If an error has occured, throw a avl::utility::ReadWriteError().
				if(file.fail() == true)
				{
					throw avl::utility::ReadWriteError();
				}
				// Otherwise there must be a syntax error. Throw a avl::utility::SyntaxError of type
				// avl::utility::SyntaxError::BAD_VALUE.
				else
				{
					throw SyntaxError(SyntaxError::BAD_VALUE, line_number);
				}
			}

			// Discard the newline character.
			file.get();

			// At this point an integer value has been read.
		}


		// Since the character is not part of an integer value, it should be part of a string value.
		// However, string values are not allowed to start with whitespace. If the character is
		// whitespace, then throw a avl::utility::SyntaxError of type
		// avl::utility::SyntaxError::BAD_VALUE.
		else if(isspace(c) != 0)
		{
			throw SyntaxError(avl::utility::SyntaxError::BAD_VALUE, line_number);
		}

		// Otherwise the character must belong to a string value. Copy it and the rest of the line
		// to string_value.
		else
		{
			// Copy the character to string_value.
			string_value = file.get();

			// Process the rest of the line unless an the end of file is reached or the file is no
			// longer readable.
			for (c = file.peek(); c != '\n' && file.good() == true; c = file.peek())
			{
				// Add the character to string_value.
				string_value += file.get();
			}

			// If an error occured while attempting to read data, throw a avl::utility::ReadWriteError.
			if (file.fail() == true)
			{
				throw ReadWriteError();
			}

			// Discard the newline character.
			file.get();

		// At this point, a string value has been read.
		}

		// Now a value has been read, so it's time to create the value on the heap.
				
		// If the value is an integer, it must first be converted to an integer.
		if (is_integer == true)
		{
			// Attempt to convert value to an integer.
			int integer_value = atoi(string_value.c_str());

			// If integer_value is INT_MIN or INT_MAX, it means that the user's value was out of range.
			// If so, throw a avl::utility::SyntaxError of type
			// avl::utility::SyntaxError::OUT_OF_RANGE_INTEGER.
			if(integer_value == INT_MIN || integer_value == INT_MAX)
			{
				throw SyntaxError(avl::utility::SyntaxError::OUT_OF_RANGE_INTEGER, line_number);
			}

			// Now copy integer_value onto the heap, and store the address in value.
			value = new int(integer_value);
		}
		// Otherwise it is a string value and just has to be copied onto the heap and the address copied to value.
		else
		{
			value = new std::string(string_value);
		}

		// Now return whether or not the value is an integer.
		return is_integer;
	}




	// Creates an integer SettingValue with the specified value. Note that value will be deleted when this object is
	// destroyed or if its value is ever reassigned.
	SettingValue::SettingValue(const int* const value)
	: value_type(SettingValue::INTEGER_VALUE), value(value)
	{
		ASSERT(value != NULL);
	}



	// Creates a string SettingValue with the specified value. Note that value will be deleted when this object is
	// destroyed or if its value is ever reassigned.
	SettingValue::SettingValue(const std::string* const value)
	: value_type(SettingValue::STRING_VALUE), value(value)
	{
		ASSERT(value != NULL);
	}




	// Casts the void pointer of value to the correct type and then deletes it.
	SettingValue::~SettingValue()
	{
		ASSERT(value != NULL);
		// If this is an integer value, cast to int and delete the value pointer. Otherwise it
		// must be a string, so cast to a string and delete the value pointer.
		if(value_type == SettingValue::INTEGER_VALUE)
		{
			delete static_cast<const int* const>(value);
		}
		else
		{
			delete static_cast<const std::string* const>(value);
		}
	}




	// Returns the value type.
	const SettingValue::ValueType& SettingValue::GetValueType() const
	{
		return value_type;
	}




	// Returns a value as a void pointer.
	const void* const SettingValue::GetValue() const
	{
		ASSERT(value != NULL);
		return value;
	}




	// Takes a avl::utility::SyntaxError::ErrorType as the type of syntax error which occured and
	// an integer specifying the line on which the error occured.
	SyntaxError::SyntaxError(const SyntaxError::ErrorType& error_type, const int& line_number)
	: Exception("avl::utility::SettingsFile -- A syntax error occurred while reading a settings file."),
														error_type(error_type), line_number(line_number)
	{
	}




	// Basic destructor.
	avl::utility::SyntaxError::~SyntaxError()
	{
	}




	// Copy constructor.
	SyntaxError::SyntaxError(const SyntaxError& original)
	: Exception(original), error_type(original.GetErrorType()), line_number(original.GetLineNumber())
	{
	}




	// Returns the error type.
	const SyntaxError::ErrorType& SyntaxError::GetErrorType() const
	{
		return error_type;
	}




	// Returns the number of the line on which the error occured.
	const int& SyntaxError::GetLineNumber() const
	{
		return line_number;
	}



}
}