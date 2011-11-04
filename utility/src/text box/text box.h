#ifndef __AVL_UTILITY_TEXT_BOX__
#define __AVL_UTILITY_TEXT_BOX__
/**********
 * Author: Sheldon Bachstein
 * Date: Sep 25, 2011
 * Description: Implements the behavior of a dynamic box of text.
 **********/


// Included for the typedefs in utility::Sprite.
#include"..\sprite\sprite.h"

#include<vector>
#include<list>
#include<deque>
#include<string>


namespace avl
{
namespace utility
{
	// See the beginning of the file for details.
	class TextBox
	{
	private:
		// Forward declaration. See definition below for details.
		class Row;
	public:
		// Behavior flags for how the text box should react to
		// overstepping the maximum number of possible characters in the box.
		// Behaviors are as follows:
		//
		//	NO_OVERWRITE - If the user reaches the maximum number of characters, any
		//				   new characters will be ignored.
		//	OVERWRITE_ROWS - If the user reaches the maximum number of characters, any
		//					 additional characters will, if necessary, remove the top
		//					 row of text and scoot all rows up one position, and then
		//					 begin a new row at the bottom.
		//	AUTO_ADJUST_SIZE - If the user reaches the maximum number of characters,
		//					   any additional characters will force the text box to
		//					   resize itself to accomodate the new characters. Note
		//					   that, with this behavior, the chars_per_row and rows
		//					   parameters are ignored in the constructor.
		//
		// Note: When using any of these behaviors, if the user attempts to display more
		// characters in a textbox than is possible, all of the original characters and
		// the characters attempted to be displayed by the user after the maximum size
		// is reached will be stored and, in the event of the user specifying new
		// dimensions for the text box, it will dynamically change how much of the text
		// it displays to match the new dimensions.
		enum OverwriteBehavior{NO_OVERWRITE, OVERWRITE_CHARS, OVERWRITE_ROWS, AUTO_ADJUST_SIZE};


		// Constructors:
		// 
		TextBox(const float& left, const float& top, const float& right, const float& bottom, const float& z, 
				const unsigned int& chars_per_row, const unsigned int& number_of_rows,
				const OverwriteBehavior& behavior, const utility::Sprite::TextureHandle& texture_handle,
				const unsigned int& chars_per_texture_row);
		// Destructor.
		~TextBox();

		// Accessors:
		// Returns true if the textbox is currently visible, and false
		// otherwise.
		const bool IsVisible() const;

		// Returns the text box's current text.
		const std::string& GetText() const;

		// Returns the text box's number of characters per row.
		const unsigned int GetCharsPerRow() const;
		// Returns the text box's number of rows.
		const unsigned int GetRows() const;

		// Returns the left boundary of the box.
		const float& GetLeft() const;
		// Returns the upper boundary of the box.
		const float& GetTop() const;
		// Returns the right boundary of the box.
		const float& GetRight() const;
		// Returns the lower boundary of the box.
		const float& GetBottom() const;
		// Returns the z-depth of the box.
		const float& GetZ() const;

		// Returns the current texture handle.
		const utility::Sprite::TextureHandle& GetTextureHandle() const;

		// Returns a list of all the sprites composing this text box. Use for rendering.
		utility::Sprite::SpriteList GetSprites() const;



		// Mutators:
		// Set the visibility of the textbox.
		void SetVisiblity(const bool new_visbility);

		// Change the current text for the text box.
		void SetText(const std::string& new_text);
		// Append a string to the current text.
		void AddText(const std::string& new_text);
		// Clears the text box.
		void ClearText();

		// Change the dimensions of the text box.
		void SetDimensions(const unsigned int& new_chars_per_row, const unsigned int& new_rows);

		// Change the text box's overwrite behavior. See the OverWrite behavior enum above.
		void SetOverwriteBehavior(const OverwriteBehavior& new_behavior);

		// Change the position of the text box.
		void SetPosition(const float& new_left, const float& new_top, const float& new_right, const float& new_bottom, const float& new_z);

		// Change the texture handle used to draw text.
		void SetTextureHandle(const utility::Sprite::TextureHandle& new_texture_handle);




	private:
		// Internal utility functions:
		// Increases the dimensions of the textbox by either one row or one column,
		// whichever more closely maintains the original proportions of the box.
		void IncreaseDimensions();
		// Decreases the dimensions of the textbox by either one row or one column,
		// whichever more closely maintains the original proportions of the box.
		void DecreaseDimensions();

		// Goes through the textbox's text, and generates substrings for each
		// different row. These substrings are based on how many characters
		// may fit in a row and on any newline characters in text.
		std::deque<const std::string> GenerateRowSubstrings() const;



		// The visibility of the text box.
		bool visibility;
		
		// The current text for the text box.
		std::string text;

		// Number of characters per row.
		unsigned int chars_per_row;
		// Number of rows.
		unsigned int number_of_rows;
		// The original proportion of width to height.
		float width_to_height_proportion;

		// The text box's overwrite behavior.
		OverwriteBehavior behavior;

		// The current position of the text box.
		float left;
		float top;
		float right;
		float bottom;
		float z;

		// Number of characters per row in the texture.
		unsigned int chars_per_texture_row;
		// The texture handle.
		utility::Sprite::TextureHandle texture_handle;

		// The rows composing this textbox.
		std::vector<Row> rows;




		// Not implemented:
		TextBox(const TextBox&);
		TextBox& operator=(const TextBox&);






		// Represents a single row within the text box.
		class Row
		{
		public:
			// Constructors:
			// Default constructor. Doesn't initialize any variables -- use at your own
			// risk!
			Row();
			// Copy constructor. Doesn't initialize any variables -- use at your own
			// risk!
			Row(const Row& original);
			// Deletes all sprites used by this row.
			~Row();

			// Accessors:
			// Returns a list of all the sprites composing this row. Use for rendering.
			utility::Sprite::SpriteList GetSprites() const;


			// Mutators:
			// Sets the visibility of this row.
			void SetVisibility(const bool new_visibility);

			// Sets the text of this row.
			void SetText(const std::string& new_text);

			// Sets the maximum number of characters for this row.
			void SetMaxCharacters(const unsigned int& new_max);

			// Sets the characters per texture row.
			void SetCharsPerTextureRow(const unsigned int& new_chars);

			// Sets the position of this row.
			void SetPosition(const float& new_left, const float& new_top, const float& new_right, const float& new_bottom, const float& new_z);

			// Sets the new texture handle for the row.
			void SetTextureHandle(const utility::Sprite::TextureHandle& new_texture_handle);

			// Assignment operator. Note -- this doesn't perform as one would
			// expect! Use at your own risk.
			Row& operator=(const Row& rhs);
			


		private:

			// Visibility of this row.
			bool visibility;

			// This row's text.
			std::string text;

			// The maximum width of this row, in characters.
			unsigned int max_characters;

			// The row's position.
			float left;
			float top;
			float right;
			float bottom;
			float z;

			// Number of characters per row in the texture.
			unsigned int chars_per_texture_row;

			// The texture handle for characters in this row.
			utility::Sprite::TextureHandle texture_handle;

			// All of the sprites forming this row.
			utility::Sprite::SpriteList sprites;

		};

	};






} //avl
} //utility
#endif // __AVL_UTILITY_TEXT_BOX__