/**********
 * Author: Sheldon Bachstein
 * Date: Dec 29, 2010
 * Description: See settings file.h for details.
 **********/

#include"settings file.h"
#include"..\assert\assert.h"
#include"..\exceptions\exceptions.h"
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
	// while attempting to read from the file, will throw a FileReadException. If there is a syntax
	// error, will throw a SettingsFileSyntaxError with one of these types:
	// SettingsFile::SyntaxError::BAD_VARIABLE_NAME if there is a problem with the variable name; or
	// SettingsFile::SyntaxError::BAD_VALUE if there is a problem with a variable's value.
	SettingsFile::SettingsFile(const std::string& file_name)
	{
		std::ifstream file;
		// First, tell the file not to throw any exceptions.
		file.exceptions(std::ios_base::goodbit);
		// Now attempt to open the specified file name for reading.
		file.open(file_name.c_str(), std::ios_base::in);
		// Check to see if the file was successfully opened. If not, then throw a FileReadException.
		if(file.fail() == true)
		{
			file.close();
			throw FileReadException(file_name);
		}
		// At this point, the file should be ready for reading. Read the file into memory.
		StringVector lines;
		LoadFileToString(file, lines, file_name);
		// Close the file.
		file.close();
		// Load the settings from the file.
		LoadSettings(lines, file_name);
	}




	// Destructor.
	SettingsFile::~SettingsFile()
	{
	}



	const bool SettingsFile::IsIntegerVariable(const std::string& variable)
	{
		return (integer_variables.find(variable) != integer_variables.end());
	}




	const bool SettingsFile::IsStringVariable(const std::string& variable)
	{
		return (string_variables.find(variable) != string_variables.end());
	}




	const long& SettingsFile::GetIntegerValue(const std::string& variable) const
	{
		// If there's no value associated with variable, throw an InvalidArgumentException.
		IntegerVariableMap::const_iterator i = integer_variables.find(variable);
		if(i == integer_variables.end())
		{
			throw InvalidArgumentException("avl::utility::SettingsFile::GetIntegerValue()", "variable", "The supplied variable is not associated with an integer value. See avl::utility::SettingsFile::IsIntegerValue().");
		}
		// Otherwise the variable must exist.
		else
		{
			return i->second;
		}
	}




	const std::string& SettingsFile::GetStringValue(const std::string& variable) const
	{
		// If there's no value associated with variable, throw an InvalidArgumentException.
		StringVariableMap::const_iterator i = string_variables.find(variable);
		if(i == string_variables.end())
		{
			throw InvalidArgumentException("avl::utility::SettingsFile::GetStringValue()", "variable", "The supplied variable is not associated with a string value. See avl::utility::SettingsFile::IsStringValue().");
		}
		// Otherwise the variable must exist.
		else
		{
			return i->second;
		}
	}





	void SettingsFile::LoadFileToString(std::ifstream& file, SettingsFile::StringVector& lines, const std::string& file_name)
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
		// If an error occurred while reading (other than reaching EOF), then throw a FileReadException.
		if(file.fail() == true && file.eof() == false)
		{
			file.close();
			throw FileReadException(file_name);
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
				throw SettingsFile::SyntaxError(SettingsFile::SyntaxError::BAD_VARIABLE_NAME, file_name, line_number);
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
		// If this variable name already exists, then this is a bad variable name.
		if(integer_variables.find(name) != integer_variables.end() || string_variables.find(name) != string_variables.end())
		{
			throw SettingsFile::SyntaxError(SettingsFile::SyntaxError::BAD_VARIABLE_NAME, file_name, line_number);
		}
		// This check prevents an std::out_of_range exception. If the separator is the last character
		// on this line, then the variable's value is malformed.
		if(separator == line.length() - 1)
		{
			throw SettingsFile::SyntaxError(SettingsFile::SyntaxError::BAD_VALUE, file_name, line_number);
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
			integer_variables.insert(std::make_pair(name, integer_value));
		}
		else
		{
			// The value is a string.
			string_variables.insert(std::make_pair(name, value));
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