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
#include<string>


namespace avl
{
namespace utility
{
	// See the beginning of the file for details.
	class TextBox
	{
	public:
		// Behavior flags for how the text box should react to
		// overstepping the maximum number of possible characters in the box.
		// Behaviors are as follows:
		//
		//	NO_OVERWRITE - If the user reaches the maximum number of characters, any
		//				   new characters will be ignored.
		//	OVERWRITE_CHARS - If the user reaches the maximum number of characters, any
		//					  additional characters will remove the first character of
		//					  the top line, and scoot all characters back on position.
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
				const unsigned int& chars_per_row, const unsigned int& rows,
				const OverwriteBehavior& behavior, const utility::Sprite::TextureHandle& texture_handle,
				const unsigned int& chars_per_texture_row);
		// Destructor.
		~TextBox();

		// Accessors:
		// Returns the text box's current text.
		const std::string& GetText() const;

		// Returns the text box's number of characters per row.
		const unsigned int GetCharsPerRow() const;
		// Returns the text box's number of rows.
		const unsigned int GetRows() const;

		// Returns the current number of characters in the text box.
		const unsigned int GetSize() const;
		// Returns the maximum number of characters that will fit in the text box.
		const unsigned int GetMaxSize() const;

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
		// If necessary, resizes the dimensions of the textbox until the textbox can
		// efficiently store new_size characters. Used when behavior is set to
		// AUTO_ADJUST_SIZE.
		void Resize(const unsigned int new_size);
		// Increases the dimensions of the textbox by either one row or one column,
		// whichever more closely maintains the original proportions of the box.
		void IncreaseDimensions();
		// Decreases the dimensions of the textbox by either one row or one column,
		// whichever more closely maintains the original proportions of the box.
		void DecreaseDimensions();

		// Based on the text box's behavior, characters per row, and number of rows,
		// will return the position in the member text which is to be displayed by
		// the top-left sprite composing the text box.
		std::string::const_iterator FindFirstCharacter();



		// The current text for the text box.
		std::string text;

		// Number of characters per row.
		unsigned int chars_per_row;
		// Number of rows.
		unsigned int rows;
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

		// All of the sprites forming this text box. Pointers are used so
		// that it's easy to return a utility::Sprite::SpriteList.
		std::vector<utility::Sprite* const> sprites;




		// Not implemented:
		TextBox(const TextBox&);
		TextBox& operator=(const TextBox&);



	};






} //avl
} //utility
#endif // __AVL_UTILITY_TEXT_BOX__