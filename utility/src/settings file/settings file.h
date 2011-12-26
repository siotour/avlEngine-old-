#ifndef __AVL_UTILITY_SETTINGS_FILE__
#define __AVL_UTILITY_SETTINGS_FILE__
/**
@file
Loads formatted settings data from a text file.
@author Sheldon Bachstein
@date Dec 29, 2010
@todo Completely renovate this component so that it supports editing a settings file
(requires changing the size of the file using _chsize_s()). Will require a complete overhaul
and increase complexity dramatically.
*/


#include"..\exception\exception.h"
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
		@throws avl::utility::ReadWriteError If unable to open or read from \a file_name.
		@throws avl::utility::SyntaxError If there is a problem with the syntax in \a file_name.
		*/
		SettingsFile(const std::string& file_name);

		/** Basic destructor.*/
		~SettingsFile();

		/** Attempts to access an integer setting value.
		@post If \a variable does not exist or is not an integer variable, then \a value will
		not be modified.
		@param variable [IN] The name of the variable to access.
		@param value [OUT] If \a variable exists and is an integer variable, the value associated with it.
		@return True if \a variable exists and is an integer variable. False if \a variable does
		not exist or is not an integer variable.
		*/
		const bool GetIntegerValue(const std::string& variable, long& value) const;
		
		/** Attempts to access a string setting value.
		@post If \a variable does not exist or is not a string variable, then \a value will
		not be modified.
		@param variable [IN] The name of the variable to access.
		@param value [OUT] If \a variable exists and is a string variable, the value associated with it.
		@return True if \a variable exists and is a string variable. False if \a variable does
		not exist or is not a string variable.
		*/
		const bool GetStringValue(const std::string& variable, std::string& value) const;
	private:
		/** Merely a convenience.*/
		typedef std::vector<const std::string> StringVector;

		/** Attempts to load the contents in the file \a file to a string.
		@post Each line of \a file will be pushed onto the back of \a lines as a string.
		@param file [IN] The file from which to read.
		@param lines [OUT] A vector which is to contain the contents of \a file separated
		into strings for each line.
		@return The contents of \a file separated into strings for each line.
		@throws ReadWriteError If unable to load the data from \a file.
		*/
		void LoadFileToString(std::ifstream& file, StringVector& lines);

		/** Attempts to parse \a lines into a map of variable name/value pairs and add them
		to \ref settings.
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
		@throws avl::utility::SyntaxError If there is a syntactical error in the specified line of text
		data.
		*/
		void ProcessLine(const std::string& line, const std::string& file_name, const unsigned int& line_number);


		/** Attempts to parse a non-comment, non-blank line into a variable name/value pair and adds
		it to \ref settings.
		@pre \a line obeys the syntactical rules laid out in \ref avl::utility::SettingsFile, and is
		neither a blank line (whitespace not considered substantial) or a comment.
		@param line The line to be processed.
		@param file_name The name of the settings file.
		@param line_number The number of the line currently being parsed. Used to report which line
		syntax errors occur on.
		@throws avl::utility::SyntaxError If there is a syntactical error in the specified line of text
		data.
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

		/// Map of variable names to SettingValue objects.
		std::map<const std::string, const SettingsFile::SettingValue> settings;

		/// NOT IMPLEMENTED.
		SettingsFile(const SettingsFile&);
		/// NOT IMPLEMENTED.
		const SettingsFile& operator=(const SettingsFile&);



		/** Contains information regarding a variable's value, and is intended
		to be mapped to a variable name. It contains the type of value and the actual value.
		*/
		class SettingValue
		{
		public:
			/**Describes what type of value is being stored.*/
			enum ValueType {INTEGER_VALUE, STRING_VALUE};

			/** Constructor for integer values.
			@post \ref value_type will be set to \ref INTEGER_VALUE.
			@param the_value The value represented by this object.
			*/
			SettingValue(const long& the_value);

			/** Constructor for string values.
			@post \ref value_type will be set to \ref STRING_VALUE.
			@param the_value The value represented by this object.
			*/
			SettingValue(const std::string& the_value);

			/** Copy constructor.
			@param original The object being copied.
			*/
			SettingValue(const SettingValue& original);

			/** If this value is a string value, then the internal
			pointer containing that string is deleted.
			*/
			~SettingValue();

			/** Returns \ref value_type.
			@return The value of \ref value_type.
			*/
			const SettingValue::ValueType& GetValueType() const;

			/** Returns \c value.long_value.
			@return \c value.long_value.
			@sa value
			*/
			const long& GetIntegerValue() const;

			/** Returns \c value.string_value.
			@return \c value.string_value.
			@sa value
			*/
			const std::string& GetStringValue() const;
		private:
			/// The type of value.
			SettingValue::ValueType value_type;

			/** Combines a long or string pointer.
			*/
			union LongStringUnion{
				long integer_value;
				std::string* string_value;
			};

			/// The value.
			LongStringUnion value;

			/// NOT IMPLEMENTED.
			const SettingValue& operator=(const SettingValue&);
		};




		public:
		/** Indicates that there was a syntax error in a settings file. Includes the line number
		where the error was found and an error code describing what kind of error occured.
		@todo Record the name of the settings file in which exceptions occur.
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
#endif // __AVL_UTILITY_SETTINGS_FILE__