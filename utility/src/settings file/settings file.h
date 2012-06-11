#pragma once
#ifndef AVL_UTILITY_SETTINGS_FILE__
#define AVL_UTILITY_SETTINGS_FILE__
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
Loads formatted settings data from a text file.
@author Sheldon Bachstein
@date Dec 29, 2010
@todo Completely renovate this component so that it supports editing a settings file
(requires changing the size of the file using _chsize_s()). Will require a complete overhaul
and increase complexity dramatically.
*/


#include"..\exceptions\exceptions.h"
#include<string>
#include<map>
#include<fstream>
#include<vector>


namespace avl
{
namespace utility
{

	/** Parses a text file and loads in variable names and variable values within that text file.
	@todo Document the syntax and so on.
	*/
	class SettingsFile
	{
	public:
		/** Attempts to open the file \a file_name and read in any formatted data.
		@param file_name The name of the file to read settings from.
		@throws FileReadException If unable to open or read from \a file_name.
		@throws SyntaxError If there is a problem with the syntax in \a file_name.
		@throws OutOfMemoryError If unable to allocate space for a new string value.
		*/
		SettingsFile(const std::string& file_name);

		/** Basic destructor.*/
		~SettingsFile();


		/** Checks to see if a variable name exists and stores an integer value.
		@param variable The name of the variable to check.
		@return True if \a variable is a variable name and is mapped to an integer value.
		*/
		const bool IsIntegerVariable(const std::string& variable);

		/** Checks to see if a variable name exists and stores a string value.
		@param variable The name of the variable to check.
		@return True if \a variable is a variable name and is mapped to a string value.
		*/
		const bool IsStringVariable(const std::string& variable);

		/** Attempts to access an integer setting value.
		@attention Use \ref IsIntegerVariable() to tell whether or not a variable name is associated
		with an integer value.
		@sa IsIntegerValue()
		@param variable The name of the variable to access.
		@return The value associated with the \a variable.
		@throws InvalidArgumentException If \a variable does not exist or does not have an associated
		integer value.
		*/
		const long& GetIntegerValue(const std::string& variable) const;
		
		/** Attempts to access a string setting value.
		@attention Use \ref IsStringVariable() to tell whether or not a variable name is associated
		with a string value.
		@sa IsStringVariable()
		@param variable The name of the variable to access.
		@return The value associated with the \a variable.
		@throws InvalidArgumentException If \a variable does not exist or does not have an associated
		string value.
		*/
		const std::string& GetStringValue(const std::string& variable) const;

	private:
		/** Merely a convenience.*/
		typedef std::vector<const std::string> StringVector;

		/** Attempts to load the contents in the file \a file to a string.
		@post Each line of \a file will be pushed onto the back of \a lines as a string.
		@param file [IN] The file from which to read.
		@param lines [OUT] A vector which is to contain the contents of \a file separated
		into strings for each line.
		@param file_name The name of the file which is to be read.
		@return The contents of \a file separated into strings for each line.
		@throws FileWriteException If unable to load the data from \a file.
		*/
		void LoadFileToString(std::ifstream& file, StringVector& lines, const std::string& file_name);

		/** Attempts to parse \a lines into a series of variable name/value pairs and add them
		to \ref integer_variables and \ref string_variables.
		@pre \a lines obeys the rules of syntax described in \ref avl::utility::SettingsFile.
		@param lines The lines from which the settings are to be loaded. Each string should
		contain a full line from the settings file.
		@param file_name The name of the settings file.
		@throws SyntaxError If there is a syntactical error in \a lines.
		*/
		void LoadSettings(StringVector& lines, const std::string& file_name);

		/** Parses a single line of text data to determine if it is a comment, blank line, or
		a substantial line. If it's a substantial line, then we will attempt to parse it into
		a variable name/value pair.
		@pre \a line obeys the syntactical rules laid out in \ref avl::utility::SettingsFile.
		@param line The line to be processed.
		@param file_name The name of the settings file.
		@param line_number The number of the line currently being parsed. Used to report which line
		syntax errors occur on.
		@throws SyntaxError If there is a syntactical error in the specified line of text
		data.
		*/
		void ProcessLine(const std::string& line, const std::string& file_name, const unsigned int& line_number);


		/** Attempts to parse a non-comment, non-blank line into a variable name/value pair and add
		it to either \ref integer_variables or \ref string_variables
		@pre \a line obeys the syntactical rules laid out in \ref avl::utility::SettingsFile, and is
		neither a blank line (whitespace not considered substantial) or a comment.
		@param line The line to be processed.
		@param file_name The name of the settings file.
		@param line_number The number of the line currently being parsed. Used to report which line
		syntax errors occur on.
		@throws SyntaxError If there is a syntactical error in the specified line of text data.
		*/
		void ProcessVariable(const std::string& line, const std::string& file_name, const unsigned int& line_number);


		/** Checks to see if \a name is a valid variable name. Additionally, trims any
		whitespace off the beginning and end of \a name.
		@param name The variable name being checked.
		@param file_name The name of the settings file.
		@param line_number The line in the settings file on which this name occurs.
		@throws SyntaxError If \a name is not a valid variable name.
		*/
		static void CheckVariableName(std::string& name, const std::string& file_name, const unsigned int& line_number);


		/** Checks to see if \a value is a valid variable value. Additionally, trims any
		whitespace off the beginning and end of \a value.
		@param value The variable value being checked.
		@param file_name The name of the settings file.
		@param line_number The line in the settings file on which this value occurs.
		@throws SyntaxError If \a value is not a valid variable value.
		*/
		static void CheckVariableValue(std::string& value, const std::string& file_name, const unsigned int& line_number);


		/** Checks to see if \a value can be parsed to an integer value, and if so
		stores the parsed integer in \a integer_value.
		@param value [IN] The string which is being checked.
		@param integer_value [OUT] The parsed integer value of \a value, if \a value
		can be parsed to an integer.
		@return True if \a value may be parsed to an integer value, and false if not.
		*/
		static bool IsIntegerValue(std::string& value, long& integer_value);


		/** Prunes any whitespace off of the beginning and end of \a bush.
		@param bush The string to be pruned.
		*/
		static void PruneWhitespace(std::string& bush);


		// Forward declaration.
		class SettingValue;

		/// The structure in which string variables are stored.
		typedef std::map<const std::string, const std::string> StringVariableMap;

		/// Contains all string variables.
		StringVariableMap string_variables;

		/// The structure in which integer variables are stored.
		typedef std::map<const std::string, const long> IntegerVariableMap;

		/// Contains all integer variables.
		IntegerVariableMap integer_variables;


		/// NOT IMPLEMENTED.
		SettingsFile(const SettingsFile&);
		/// NOT IMPLEMENTED.
		const SettingsFile& operator=(const SettingsFile&);




		public:
		/** Indicates that there was a syntax error in a settings file. Includes the line number
		where the error was found and an error code describing what kind of error occured.
		*/
		class SyntaxError: public Exception
		{
		public:
			/** Describes what kind of syntax error has occurred.*/
			enum ErrorType{BAD_VARIABLE_NAME, BAD_VALUE};
		
			/** Full-spec constructor.
			@param error_type The type of syntax error which was found.
			@param file_name The name of the file in which the error occurred.
			@param line_number the line on which the syntax error was detected.
			*/
			SyntaxError(const ErrorType& error_type, const std::string& file_name, const int& line_number);

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

			/** Gets the file name.
			@return The name of the settings file in which the error was detected.
			*/
			const std::string& GetFileName() const;

			/** Gets the line on which the error occured.
			@return The line on which the error was detected.
			*/
			const int& GetLineNumber() const;
		private:
			/// The error type.
			const ErrorType error_type;

			/// The file name in which the error occurred.
			const std::string file_name;

			/// The line number on which the error occurred.
			const int line_number;

			/// NOT IMPLEMENTED.
			const SyntaxError& operator=(const SyntaxError&);
		};

	};





} //avl
} //utility
#endif // AVL_UTILITY_SETTINGS_FILE__