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
#include<sstream>
#include<fstream>
#include<vector>




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
	// range of values.
	SettingsFile::SettingsFile(const std::string& file_name)
	{
		std::ifstream file;
		// First, tell the file not to throw any exceptions.
		file.exceptions(std::ios_base::goodbit);
		// Now attempt to open the specified file name for reading.
		file.open(file_name.c_str(), std::ios_base::in);
		// Check to see if the file was successfully opened. If not, then throw an avl::utility::ReadWriteError.
		if(file.fail() == true)
		{
			file.close();
			throw ReadWriteError();
		}
		// At this point, the file should be ready for reading. Read the file into memory.
		StringVector lines;
		LoadFileToString(file, lines);
		// Close the file.
		file.close();
		// Load the settings from the file.
		LoadSettings(lines, file_name);
	}




	// Destructor.
	SettingsFile::~SettingsFile()
	{
	}




	// If the specified variable has an associated integer value, it will be assigned to the value parameter and
	// true will be returned. If the variable does not exist or the variable does not have an associated integer
	// value, the value parameter will be left unchanged and false will be returned.
	const bool SettingsFile::GetIntegerValue(const std::string& variable, long& value) const
	{
		// If there's no value associated with variable, or if that value is a string, return false.
		std::map<const std::string, const SettingsFile::SettingValue>::const_iterator i = settings.find(variable);
		if(i == settings.end() || i->second.GetValueType() == SettingsFile::SettingValue::STRING_VALUE)
		{
			return false;
		}
		// Otherwise the variable must exist and be an integer, so assign its value to the value parameter
		// and return true.
		else
		{
			value = i->second.GetIntegerValue();
			return true;
		}
	}




	// If the specified variable has an associated string value, it will be assigned to the value parameter and
	// true will be returned. If the variable does not exist or the variable does not have an associated string
	// value, the value parameter will be left unchanged and false will be returned.
	const bool SettingsFile::GetStringValue(const std::string& variable, std::string& value) const
	{
		// If there's no value associated with variable, or if that value is a an integer value, return false.
		std::map<const std::string, const SettingsFile::SettingValue>::const_iterator i = settings.find(variable);
		if(i == settings.end() || i->second.GetValueType() == SettingsFile::SettingValue::INTEGER_VALUE)
		{
			return false;
		}
		// Otherwise the variable must exist and is a string, so assign its value to the value parameter
		// and return true.
		else
		{
			value = i->second.GetStringValue();
			return true;
		}
	}





	void SettingsFile::LoadFileToString(std::ifstream& file, SettingsFile::StringVector& lines)
	{
		// First, tell the file not to throw any exceptions.
		file.exceptions(std::ios_base::goodbit);
		// Now load each line from the file into a string.
		while(file.good() == true)
		{
			std::string line;
			std::getline(file, line);
			lines.push_back(line);
		}
		// If an error occurred while reading (other than reaching EOF), then throw a ReadWriteError.
		if(file.fail() == true && file.eof() == false)
		{
			file.close();
			throw new ReadWriteError();
		}
	}





	void SettingsFile::LoadSettings(SettingsFile::StringVector& lines, const std::string& file_name)
	{
		// The line number.
		unsigned int line_number = 1;
		// Process each line...
		StringVector::const_iterator end = lines.end();
		for(StringVector::const_iterator i = lines.begin(); i != end; ++i)
		{
			// Process this line.
			ProcessLine(*i, file_name, line_number);
			// Move to the next line.
			++line_number;
		}
	}





	void SettingsFile::ProcessLine(const std::string& line, const std::string& file_name, const unsigned int& line_number)
	{
		// Is the line empty?
		if(line.empty() == true)
		{
			return;
		}
		// If the line is only 1 character long, then that one character must be whitespace. Otherwise
		// the line is malformed.
		if(line.length() == 1)
		{
			if(isspace(line[0]) == 0)
			{
				throw new SettingsFile::SyntaxError(SettingsFile::SyntaxError::BAD_VARIABLE_NAME, file_name, line_number);
			}
		}
		// Otherwise there are at least 2 characters.
		else
		{
			// Is this line a comment?
			if(line[0] == '/' && line[1] == '/')
			{
				return;
			}
			// Otherwise this line isn't a comment.
			else
			{
				ProcessVariable(line, file_name, line_number);
			}
		}
	}





	void SettingsFile::ProcessVariable(const std::string& line, const std::string& file_name, const unsigned int& line_number)
	{
		ASSERT(line.length() >= 2);
		// Looks for an equals sign ('=') on this line.
		const unsigned int separator = line.find_first_of('=');
		// Get the raw variable name.
		std::string name = line.substr(0, separator);
		// Make sure that the variable name is valid.
		CheckVariableName(name, file_name, line_number);
		// If this variable name already exists in the map, then this is a bad variable name.
		if(settings.find(name) != settings.end())
		{
			throw SettingsFile::SyntaxError(SettingsFile::SyntaxError::BAD_VARIABLE_NAME, file_name, line_number);
		}
		// This check prevents an std::out_of_range exception. If the separator is the last character
		// on this line, then the variable's value is malformed.
		if(separator == line.length() - 1)
		{
			throw new SettingsFile::SyntaxError(SettingsFile::SyntaxError::BAD_VALUE, file_name, line_number);
		}
		// Get the raw variable value.
		std::string value = line.substr(separator + 1, line.npos);
		// Make sure that the variable value is valid.
		CheckVariableValue(value, file_name, line_number);
		// Add the variable name/pair to the map of settings.
		long integer_value = 0;
		if(IsIntegerValue(value, integer_value) == true)
		{
			// The value is an integer.
			settings.insert(std::make_pair(name, SettingValue(integer_value)));
		}
		else
		{
			// The value is a string.
			settings.insert(std::make_pair(name, SettingValue(value)));
		}
	}



	
	

	void SettingsFile::CheckVariableName(std::string& name, const std::string& file_name, const unsigned int& line_number)
	{
		// Trim any whitespace from the name.
		PruneWhitespace(name);
		// Empty names are invalid.
		if(name.empty() == true)
		{
			throw SettingsFile::SyntaxError(SettingsFile::SyntaxError::BAD_VARIABLE_NAME, file_name, line_number);
		}
		// Go through each character in the name.
		for(std::string::const_iterator i = name.begin(); i != name.end(); ++i)
		{
			// Names may only have alphanumeric characters.
			if(isgraph(*i) == 0)
			{
				throw SettingsFile::SyntaxError(SettingsFile::SyntaxError::BAD_VARIABLE_NAME, file_name, line_number);
			}
		}
	}





	void SettingsFile::CheckVariableValue(std::string& value, const std::string& file_name, const unsigned int& line_number)
	{
		// Trim any whitespace from the value.
		PruneWhitespace(value);
		// Empty values are invalid.
		if(value.empty() == true)
		{
			throw SettingsFile::SyntaxError(SettingsFile::SyntaxError::BAD_VALUE, file_name, line_number);
		}
		// Go through each character in the name.
		for(std::string::const_iterator i = value.begin(); i != value.end(); ++i)
		{
			// Values may only have printable characters and whitespace characters.
			if(isprint(*i) == 0 && isspace(*i) == 0)
			{
				throw SettingsFile::SyntaxError(SettingsFile::SyntaxError::BAD_VALUE, file_name, line_number);
			}
		}
	}





	bool SettingsFile::IsIntegerValue(std::string& value, long& integer_value)
	{
		// If value contains nothing but '0' characters, then it is the integer value zero.
		if(value.find_first_not_of('0') == value.npos)
		{
			integer_value = 0;
			return true;
		}
		// Otherwise, value is either a non-zero integer or it is not an integer value.
		else
		{
			// Attempt to parse value to an integer.
			integer_value = atol(value.c_str());
			// At this point value can't be an integer of 0. If the parsing resulted in
			// 0, then the parsing failed.
			if(integer_value != 0)
			{
				return true;
			}
		}
		return false;
	}




	void SettingsFile::PruneWhitespace(std::string& bush)
	{
		// Trim off any whitespace at the beginning or end of the bush.
		while(bush.empty() == false && isspace(*bush.begin()) != 0)
		{
			bush.erase(bush.begin());
		}
		while(bush.empty() == false && isspace(*(--bush.end())) != 0)
		{
			bush.erase(*(--bush.end()));
		}
	}





	// Creates an integer SettingValue with the specified long value.
	SettingsFile::SettingValue::SettingValue(const long& the_value)
	: value_type(SettingValue::INTEGER_VALUE)
	{
		value.integer_value = the_value;
	}



	// Creates a string SettingValue with the specified string value. The string
	// is deleted in the destructor.
	SettingsFile::SettingValue::SettingValue(const std::string& the_value)
	: value_type(SettingValue::STRING_VALUE)
	{
		value.string_value = new std::string(the_value);
	}




	SettingsFile::SettingValue::SettingValue(const SettingValue& original)
		: value_type(original.GetValueType())
	{
		if(value_type == SettingValue::INTEGER_VALUE)
		{
			value.integer_value = original.GetIntegerValue();
		}
		else
		{
			value.string_value = new std::string(original.GetStringValue());
		}
	}



	// If this value represents a string value, then the internal string pointer
	// is deleted.
	SettingsFile::SettingValue::~SettingValue()
	{
		// If value is a string value, then delete the string pointer.
		if(value_type == SettingValue::STRING_VALUE)
		{
			ASSERT(value.string_value != NULL);
			delete value.string_value;
		}
	}



	// Returns the value type.
	const SettingsFile::SettingValue::ValueType& SettingsFile::SettingValue::GetValueType() const
	{
		return value_type;
	}




	// Returns value as a long.
	const long& SettingsFile::SettingValue::GetIntegerValue() const
	{
		ASSERT(value_type == SettingValue::INTEGER_VALUE);
		return value.integer_value;
	}


	// Returns value as a string.
	const std::string& SettingsFile::SettingValue::GetStringValue() const
	{
		ASSERT(value_type == SettingValue::STRING_VALUE);
		return *value.string_value;
	}



	// Takes a SyntaxError::ErrorType as the type of syntax error which occured and
	// an integer specifying the line on which the error occured.
	SettingsFile::SyntaxError::SyntaxError(const SyntaxError::ErrorType& error_type, const std::string& file_name, const int& line_number)
	: Exception(), error_type(error_type), file_name(file_name), line_number(line_number)
	{
		description = "avl::utility::SettingsFile::SyntaxError -- Unable to parse line ";
		std::stringstream line_number_string;
		line_number_string << line_number;
		description += line_number_string.str();
		description += " in settings file ";
		description += file_name;
		description += ": ";
		switch(error_type)
		{
		case SettingsFile::SyntaxError::BAD_VARIABLE_NAME:
			description += "The variable name is invalid.";
			break;
		case SettingsFile::SyntaxError::BAD_VALUE:
			description += "The value is invalid.";
			break;
		}
	}




	// Basic destructor.
	SettingsFile::SyntaxError::~SyntaxError()
	{
	}




	// Copy constructor.
	SettingsFile::SyntaxError::SyntaxError(const SyntaxError& original)
	: Exception(original), error_type(original.GetErrorType()), file_name(original.GetFileName()), line_number(original.GetLineNumber())
	{
	}




	// Returns the error type.
	const SettingsFile::SyntaxError::ErrorType& SettingsFile::SyntaxError::GetErrorType() const
	{
		return error_type;
	}



	// Returns the file name in which the error occurred.
	const std::string& SettingsFile::SyntaxError::GetFileName() const
	{
		return file_name;
	}




	// Returns the number of the line on which the error occured.
	const int& SettingsFile::SyntaxError::GetLineNumber() const
	{
		return line_number;
	}



}
}