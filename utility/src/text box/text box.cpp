/**********
 * Author: Sheldon Bachstein
 * Date: Sep 25, 2011
 * Description: See text box.h for details.
 **********/

#include"text box.h"
#include"..\sprite\sprite.h"
#include"..\vertex 2d\vertex 2d.h"
#include"..\exceptions\exceptions.h"
#include"..\assert\assert.h"

#include<vector>
#include<list>
#include<deque>
#include<string>
#include<cmath>


namespace avl
{
namespace utility
{


	// 
	TextBox::TextBox(const float& left, const float& top, const float& right, const float& bottom, const float& z, 
			const unsigned int& chars_per_row, const unsigned int& number_of_rows,
			const OverwriteBehavior& behavior, const utility::Sprite::TextureHandle& texture_handle,
			const unsigned int& chars_per_texture_row)
			: left(left), top(top), right(right), bottom(bottom), z(z), chars_per_row(chars_per_row), number_of_rows(number_of_rows), behavior(behavior), texture_handle(texture_handle), chars_per_texture_row(chars_per_texture_row)
	{
		// Set up the initial proportion.
		width_to_height_proportion = (float)chars_per_row / (float)number_of_rows;
	}




	// Destructor.
	TextBox::~TextBox()
	{
	}




	// Returns true if the textbox is currently visible, and false
	// otherwise.
	const bool TextBox::IsVisible() const
	{
		return visibility;
	}




	// Returns the text box's current text.
	const std::string& TextBox::GetText() const
	{
		return text;
	}




	// Returns the text box's number of characters per row.
	const unsigned int TextBox::GetCharsPerRow() const
	{
		return chars_per_row;
	}




	// Returns the text box's number of rows.
	const unsigned int TextBox::GetRows() const
	{
		return number_of_rows;
	}




	// Returns the left boundary of the box.
	const float& TextBox::GetLeft() const
	{
		return left;
	}




	// Returns the upper boundary of the box.
	const float& TextBox::GetTop() const
	{
		return top;
	}




	// Returns the right boundary of the box.
	const float& TextBox::GetRight() const
	{
		return right;
	}




	// Returns the lower boundary of the box.
	const float& TextBox::GetBottom() const
	{
		return bottom;
	}




	// Returns the z-depth of the box.
	const float& TextBox::GetZ() const
	{
		return z;
	}




	// Returns the current texture handle.
	const utility::Sprite::TextureHandle& TextBox::GetTextureHandle() const
	{
		return texture_handle;
	}




	// Returns a list of all the sprites composing this text box. Use for rendering.
	utility::Sprite::SpriteList TextBox::GetSprites() const
	{
		utility::Sprite::SpriteList list;

		std::vector<Row>::const_iterator end = rows.end();
		for(std::vector<Row>::const_iterator i = rows.begin(); i != end; ++i)
		{
			list.splice(list.end(), i->GetSprites());
		}

		return list;
	}




	// Set the visibility of the textbox.
	void TextBox::SetVisiblity(const bool new_visibility)
	{
		// Set the visibility of each row.
		std::vector<Row>::const_iterator end = rows.end();
		for(std::vector<Row>::iterator i = rows.begin(); i != end; ++i)
		{
			i->SetVisibility(new_visibility);
		}

		// Record the new visibility.
		visibility = new_visibility;
	}




	// Change the current text for the text box.
	void TextBox::SetText(const std::string& new_text)
	{
		// Store the new text.
		text = new_text;
		
		// Generate substrings for each row.
		std::deque<const std::string> row_strings = GenerateRowSubstrings();

		// If the behavior is AUTO_ADJUST_SIZE and the new text demands a resize,
		// resize the text box.
		if(behavior == AUTO_ADJUST_SIZE && row_strings.size() != number_of_rows)
		{
			// Decrease size to just slightly below what we need.
			while(number_of_rows > row_strings.size())
			{
				DecreaseDimensions();
				row_strings = GenerateRowSubstrings();
			}

			// Increase size up to what we need.
			while(number_of_rows < row_strings.size())
			{
				IncreaseDimensions();
				row_strings = GenerateRowSubstrings();
			}
		}

		
		/////////// Does this actually work? GenerateSubstrings() should be inserting
		/////////// older rows at the front and newer rows at the back.
		// If the behavior is NO_OVERWRITE, then we need to discard the newest
		// rows substrings until only number_of_rows row substrings remain.
		if(behavior == OVERWRITE_ROWS)
		{
			while(row_strings.size() > number_of_rows)
			{
				row_strings.pop_front();
			}
		}


		/////////// Does this actually work? GenerateSubstrings() should be inserting
		/////////// older rows at the front and newer rows at the back.
		// If the behavior is OVERWRITE_ROWS, then we need to discard the oldest
		// rows substrings until only number_of_rows row substrings remain.
		if(behavior == OVERWRITE_ROWS)
		{
			while(row_strings.size() > number_of_rows)
			{
				row_strings.pop_back();
			}
		}



		// Make sure that we have the correct number of rows.
		if(rows.size() > row_strings.size())
		{
			// Erase rows until we have the correct number.
			rows.erase(rows.begin(), rows.begin() + (rows.size() - row_strings.size()));
		}
		else if(rows.size() < row_strings.size())
		{
			// Add rows until we have the correct number.
			rows.insert(rows.end(), row_strings.size() - rows.size(), Row());
		}




		// The height of a row.
		const float row_height = (top - bottom) / (float)number_of_rows;
		// The current row.
		unsigned int current_row = 0;
		// Temp storage.
		float row_top;
		float row_bottom;

		// Set up the rows with the substrings.
		std::vector<Row>::const_iterator end = rows.end();
		for(std::vector<Row>::iterator i = rows.begin(); i != end; ++i)
		{
			// Set row position.
			row_top = top - (float)current_row * row_height;
			row_bottom = row_top - row_height;
			i->SetPosition(left, row_top, right, row_bottom, z);
			// Set texture handle.
			i->SetTextureHandle(texture_handle);
			// Set row length.
			i->SetMaxCharacters(chars_per_row);
			// Set row visibility.
			i->SetVisibility(visibility);
			// Set chars per texture row.
			i->SetCharsPerTextureRow(chars_per_texture_row);
			// Set row text.
			i->SetText(row_strings.front());
			row_strings.pop_front();
			// Move on to the next row.
			++current_row;
		}
	}




	// Append a string to the current text.
	void TextBox::AddText(const std::string& new_text)
	{
		SetText(text + new_text);
	}




	// Clears the text box.
	void TextBox::ClearText()
	{
		// Clear the internal representation of the text.
		text.clear();

		// Clear each row.
		std::vector<Row>::const_iterator end = rows.end();
		for(std::vector<Row>::iterator i = rows.begin(); i != end; ++i)
		{
			i->SetText("");
		}
	}




	// Change the dimensions of the text box.
	void TextBox::SetDimensions(const unsigned int& new_chars_per_row, const unsigned int& new_rows)
	{
		// Update dimension variables.
		chars_per_row = new_chars_per_row;
		number_of_rows = new_rows;
		// Dimensions of 0 are invalid. Boost to 1.
		if(chars_per_row < 1)
		{
			chars_per_row = 1;
		}
		if(number_of_rows < 1)
		{
			number_of_rows = 1;
		}
		// Update the proportion.
		width_to_height_proportion = (float)chars_per_row / (float)number_of_rows;
		// Redraw the text.
		SetText(text);
	}




	// Change the text box's overwrite behavior. See the OverWrite behavior enum above.
	void TextBox::SetOverwriteBehavior(const OverwriteBehavior& new_behavior)
	{
		// Update behavior variable.
		behavior = new_behavior;
		// Redraw text.
		SetText(text);
	}




	// Change the position of the text box.
	void TextBox::SetPosition(const float& new_left, const float& new_top, const float& new_right, const float& new_bottom, const float& new_z)
	{
		// Update position variables.
		left = new_left;
		top = new_top;
		right = new_right;
		bottom = new_bottom;
		z = new_z;
		// Redraw text.
		SetText(text);
	}




	// Change the texture handle used to draw text.
	void TextBox::SetTextureHandle(const utility::Sprite::TextureHandle& new_texture_handle)
	{
		// Update texture handle variable.
		texture_handle = new_texture_handle;
		// Update the texture handle of each row.
		std::vector<Row>::const_iterator end = rows.end();
		for(std::vector<Row>::iterator i = rows.begin(); i != end; ++i)
		{
			i->SetTextureHandle(new_texture_handle);
		}
	}




	// Increases the dimensions of the textbox by either one row or one column,
	// whichever more closely maintains the original proportions of the box.
	void TextBox::IncreaseDimensions()
	{
		// Should we make increase it width-wise or height-wise?
		const float wider_proportion_difference = width_to_height_proportion - (float)(chars_per_row + 1) / (float)number_of_rows;
		const float higher_proportion_difference = width_to_height_proportion - (float)chars_per_row / (float)(number_of_rows + 1);

		if(fabs(wider_proportion_difference) <= fabs(higher_proportion_difference))
		{
			++chars_per_row;
		}
		else
		{
			++number_of_rows;
		}
	}




	// Decreases the dimensions of the textbox by either one row or one column,
	// whichever more closely maintains the original proportions of the box.
	void TextBox::DecreaseDimensions()
	{
		// Should we make decrease it width-wise or height-wise?
		const float less_wide_proportion_difference = width_to_height_proportion - (float)(chars_per_row - 1) / (float)number_of_rows;
		const float less_tall_proportion_difference = width_to_height_proportion - (float)chars_per_row / (float)(number_of_rows - 1);

		if(fabs(less_wide_proportion_difference) <= fabs(less_tall_proportion_difference))
		{
			--chars_per_row;
		}
		else
		{
			--number_of_rows;
		}
	}
	
	
	
	
	// Based on the text box's behavior, characters per row, and number of rows,
	// will return the position in the member text which is to be displayed by
	// the top-left sprite composing the text box.
	std::deque<const std::string> TextBox::GenerateRowSubstrings() const
	{
		// Substrings generated from text.
		std::deque<const std::string> row_strings;

		// Current position in text.
		size_t current_pos = 0;
		// Position of the next newline character.
		size_t next_newline = 0;

		// Until we've processed the entire string...
		while(current_pos < text.size())
		{
			// If we've passed the latest newline character, search for the next
			// one.
			if(next_newline <= current_pos)
			{
				next_newline = text.find_first_of('\n', current_pos);
			}
			// If the next newline won't occur on this row, simply create a new
			// row of length chars_per_row.
			if(next_newline - current_pos > chars_per_row)
			{
				row_strings.push_back(text.substr(current_pos, chars_per_row));
				current_pos += chars_per_row;
			}
			// Otherwise the newline will occur on this row. In that case, the
			// row should contain all characters up to, but not including, the
			// newline character.
			else
			{
				// Handle the special case when next_newline == current_pos.
				if(current_pos == next_newline)
				{
					// This row must have an empty string.
					row_strings.push_back("");
					// Skip this newline.
					++current_pos;
				}
				else
				{
					row_strings.push_back(text.substr(current_pos, next_newline - current_pos + 1));
					// If next_newline = text.npos, then attempting to skip that
					// newline will result in current_pos wrapping around to 0.
					if(next_newline == text.npos)
					{
						// Prevent overflow.
						current_pos = text.npos;
					}
					else
					{
						// Skip the newline character.
						current_pos = next_newline + 1;
					}
				}
			}
		}

		return row_strings;
	}






	// Default constructor. Doesn't initialize any variables -- use at your own
	// risk!
	TextBox::Row::Row()
	{
	}




	// Copy constructor. Doesn't initialize any variables -- use at your own
	// risk!
	TextBox::Row::Row(const Row& original)
	{
	}




	// Deletes all sprites used by this row.
	TextBox::Row::~Row()
	{
		// Delete all sprites.
		utility::Sprite::SpriteList::const_iterator end = sprites.end();
		for(utility::Sprite::SpriteList::const_iterator i = sprites.begin(); i != end; ++i)
		{
			delete *i;
			sprites.erase(i);
		}
	}




	// Returns a list of all the sprites composing this row. Use for rendering.
	utility::Sprite::SpriteList TextBox::Row::GetSprites() const
	{
		return sprites;
	}


	

	// Sets the visibility of this row.
	void TextBox::Row::SetVisibility(const bool new_visibility)
	{
		// Set the visibility of each sprite.
		utility::Sprite::SpriteList::const_iterator end = sprites.end();
		for(utility::Sprite::SpriteList::iterator i = sprites.begin(); i != end; ++i)
		{
			(*i)->SetVisibility(new_visibility);
		}
		// Store new visibility.
		visibility = new_visibility;
	}




	// Sets the text of this row.
	void TextBox::Row::SetText(const std::string& new_text)
	{
		// Store the new text.
		text = new_text;

		// Make sure that we have the correct number of sprites.

		// Create new sprites if we're short. If unable to allocate space, will
		// throw an OutOfMemoryError.
		while(sprites.size() < text.size())
		{
			utility::Sprite* const new_sprite = new utility::Sprite();
			if(new_sprite == NULL)
			{
				throw OutOfMemoryError();
			}
			sprites.insert(sprites.end(), new_sprite);
		}
		// Delete any excess sprites.
		while(sprites.size() > text.size())
		{
			utility::Sprite::SpriteList::iterator old = sprites.begin();
			delete *(old);
			sprites.erase(old);
		}

		// The width of each sprite.
		const float sprite_width = (right - left) / (float)max_characters;
		// The width of each character, in texture coordinates.
		float character_width = 1.0f / (float)chars_per_texture_row;
		// The height of each character, in texture coordinates.
		float character_height = 1.0f / (float)(256 / chars_per_texture_row);
		// Temporary storage.
		float sprite_left;
		float sprite_right;
		float tex_left;
		float tex_top;
		float tex_right;
		float tex_bottom;


		// Set up each sprite.
		unsigned int current_character = 0;
		utility::Sprite::SpriteList::const_iterator end = sprites.end();
		for(utility::Sprite::SpriteList::iterator i = sprites.begin(); i != end; ++i)
		{
			// Calculate this sprite's position.
			sprite_left = left + sprite_width * current_character;
			sprite_right = sprite_left + sprite_width;

			(*i)->ResetPosition(sprite_left, top, sprite_right, bottom, z);

			// Calculate this sprite's texture coordinates.
			tex_left = character_width * (text.at(current_character) % chars_per_texture_row);
			tex_top = 1.0f - character_height * (text.at(current_character) / chars_per_texture_row);
			tex_right = tex_left + character_width;
			tex_bottom = tex_top - character_height;

			(*i)->SetQ1(utility::Vertex2D(tex_left, tex_bottom));
			(*i)->SetQ2(utility::Vertex2D(tex_left, tex_top));
			(*i)->SetQ3(utility::Vertex2D(tex_right, tex_top));
			(*i)->SetQ4(utility::Vertex2D(tex_right, tex_bottom));

			// Set the sprite's texture handle.
			(*i)->SetTextureHandle(texture_handle);

			// Set the sprite's visibility.
			(*i)->SetVisibility(visibility);
			// Next character.
			++current_character;
		}
	}




	// Sets the maximum number of characters for this row.
	void TextBox::Row::SetMaxCharacters(const unsigned int& new_max)
	{
		max_characters = new_max;
	}




	// Sets the characters per texture row.
	void TextBox::Row::SetCharsPerTextureRow(const unsigned int& new_chars)
	{
		chars_per_texture_row = new_chars;
	}




	// Sets the position of this row.
	void TextBox::Row::SetPosition(const float& new_left, const float& new_top, const float& new_right, const float& new_bottom, const float& new_z)
	{
		left = new_left;
		top = new_top;
		right = new_right;
		bottom = new_bottom;
		z = new_z;
	}




	// Sets the new texture handle for the row.
	void TextBox::Row::SetTextureHandle(const utility::Sprite::TextureHandle& new_texture_handle)
	{
		// Set the texture handle of each sprite.
		utility::Sprite::SpriteList::const_iterator end = sprites.end();
		for(utility::Sprite::SpriteList::iterator i = sprites.begin(); i != end; ++i)
		{
			(*i)->SetTextureHandle(new_texture_handle);
		}
		// Store new texture handle.
		texture_handle = new_texture_handle;
	}




	// Assignment operator. Note -- this doesn't perform as one would
	// expect! Use at your own risk.
	TextBox::Row& TextBox::Row::operator=(const Row& rhs)
	{
		return *this;
	}













}
}