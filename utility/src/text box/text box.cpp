/**********
 * Author: Sheldon Bachstein
 * Date: Sep 25, 2011
 * Description: See text box.h for details.
 **********/

#include"text box.h"
#include"..\sprite\sprite.h"
#include"..\vertex 2d\vertex 2d.h"
#include"..\exception\exception.h"
#include"..\assert\assert.h"

#include<vector>
#include<string>
#include<cmath>


namespace avl
{
namespace utility
{


	// 
	TextBox::TextBox(const float& left, const float& top, const float& right, const float& bottom, const float& z, 
			const unsigned int& chars_per_row, const unsigned int& rows,
			const OverwriteBehavior& behavior, const utility::Sprite::TextureHandle& texture_handle,
			const unsigned int& chars_per_texture_row)
			: left(left), top(top), right(right), bottom(bottom), z(z), chars_per_row(chars_per_row), rows(rows), behavior(behavior), texture_handle(texture_handle), chars_per_texture_row(chars_per_texture_row)
	{
		// Set up the initial proportion.
		width_to_height_proportion = (float)chars_per_row / (float)rows;
	}




	// Destructor.
	TextBox::~TextBox()
	{
		// Delete all the sprites composing this text box.
		while(sprites.empty() == false)
		{
			delete sprites.back();
			sprites.pop_back();
		}
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
		return rows;
	}




	// Returns the current number of characters in the text box.
	const unsigned int TextBox::GetSize() const
	{
		return sprites.size();
	}




	// Returns the maximum number of characters that will fit in the text box.
	const unsigned int TextBox::GetMaxSize() const
	{
		return chars_per_row * rows;
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

		std::vector<utility::Sprite* const>::const_iterator end = sprites.end();
		for(std::vector<utility::Sprite* const>::const_iterator i = sprites.begin(); i != end; ++i)
		{
			list.push_back(*i);
		}

		return list;
	}




	// Change the current text for the text box.
	void TextBox::SetText(const std::string& new_text)
	{
		// Store the new text.
		text = new_text;
		
		// If the behavior is AUTO_ADJUST_SIZE and the new text demands a resize,
		// resize the text box.
		if((behavior == AUTO_ADJUST_SIZE) && (text.size() > chars_per_row * rows))
		{
			Resize(text.size());
		}

		

		// If necessary, resize the sprites vector so that we have exactly the number
		// of characters that we need.
		const unsigned int required_sprites = FindRequiredSprites();

		// Do we need more sprites?
		while(sprites.size() < required_sprites)
		{
			// If we don't have the memory to make a new sprite, throw a
			// utility::Exception.
			utility::Sprite* const new_sprite = new utility::Sprite();
			if(new_sprite == NULL)
			{
				throw utility::Exception("avl::utility::TextBox::SetText() -- Unable to allocate memory for a new utility::Sprite.");
			}
			sprites.push_back(new_sprite);
		}

		// Do we need to get rid of some sprites?
		// Delete the last sprite in the vector until we have trimmed off all unnecessary sprites.
		while(sprites.size() > required_sprites)
		{
			delete sprites.back();
			sprites.pop_back();
		}


		// Get an iterator to the position in text which the first sprite must represent.
		// This is necessary because text boxes with different behaviors may display
		// different substrings even if they are both set to the same string.
		std::string::const_iterator current_char = FindFirstCharacter();

		// The width of each sprite.
		const float sprite_width = (right - left) / (float)chars_per_row;
		// The height of each sprite.
		const float sprite_height = (top - bottom) / (float)rows;
		// The width of each character in the texture.
		const float character_width = 1.0f / (float)chars_per_texture_row;
		// The height of each character in the texture.
		const float character_height = 1.0f / (float)(256 / chars_per_texture_row);
		
		// Temp storage:
		float sprite_left;
		float sprite_top;
		float sprite_right;
		float sprite_bottom;
		float tex_left;
		float tex_top;
		float tex_right;
		float tex_bottom;

		// Set up each sprite. Stop once all the sprites have been set.
		for(unsigned int current_sprite = 0; current_sprite < sprites.size(); ++current_sprite, ++current_char)
		{
			ASSERT(current_char != text.end());

			utility::Sprite& sprite = *sprites.at(current_sprite);

			// Calculate this sprite's position.
			sprite_left = left + sprite_width * (current_sprite % chars_per_row);
			sprite_top = top - sprite_height * (current_sprite / chars_per_row);
			sprite_right = sprite_left + sprite_width;
			sprite_bottom = sprite_top - sprite_height;

			sprite.ResetPosition(sprite_left, sprite_top, sprite_right, sprite_bottom, z);

			// Calculate this sprite's texture coordinates.
			tex_left = character_width * (*current_char % chars_per_texture_row);
			tex_top = 1.0f - character_height * (*current_char / chars_per_texture_row);
			tex_right = tex_left + character_width;
			tex_bottom = tex_top - character_height;

			sprite.SetQ1(utility::Vertex2D(tex_left, tex_bottom));
			sprite.SetQ2(utility::Vertex2D(tex_left, tex_top));
			sprite.SetQ3(utility::Vertex2D(tex_right, tex_top));
			sprite.SetQ4(utility::Vertex2D(tex_right, tex_bottom));

			// Set the sprite's texture handle.
			sprite.SetTextureHandle(texture_handle);
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
		SetText(std::string());
	}




	// Change the dimensions of the text box.
	void TextBox::SetDimensions(const unsigned int& new_chars_per_row, const unsigned int& new_rows)
	{
		// Update dimension variables.
		chars_per_row = new_chars_per_row;
		rows = new_rows;
		// Dimensions of 0 are invalid. Boost to 1.
		if(chars_per_row < 1)
		{
			chars_per_row = 1;
		}
		if(rows < 1)
		{
			rows = 1;
		}
		// Update the proportion.
		width_to_height_proportion = (float)chars_per_row / (float)rows;
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
		// Update the texture handle of each text character.
		std::vector<utility::Sprite* const>::const_iterator end = sprites.end();
		for(std::vector<utility::Sprite* const>::iterator i = sprites.begin(); i != end; ++i)
		{
			(*i)->SetTextureHandle(texture_handle);
		}
	}




	// If necessary, resizes the dimensions of the textbox until the textbox can
	// efficiently store new_size characters. Used when behavior is set to
	// AUTO_ADJUST_SIZE.
	void TextBox::Resize(const unsigned int new_size)
	{
		// If the textbox is too large, decrease its size until it efficiently
		// stores the text. This loop will likely decrease the dimensions once
		// too many, but that will be corrected in the second loop. This turned
		// out to be the easiest approach.
		while(chars_per_row * rows > new_size)
		{
			DecreaseDimensions();
		}

		// If the textbox is too small, increase it's size until it's big enough.
		// This corrects the possible error from the above loop.
		while(new_size > chars_per_row * rows)
		{
			IncreaseDimensions();
		}
	}




	// Increases the dimensions of the textbox by either one row or one column,
	// whichever more closely maintains the original proportions of the box.
	void TextBox::IncreaseDimensions()
	{
		// Should we make increase it width-wise or height-wise?
		const float wider_proportion_difference = width_to_height_proportion - (float)(chars_per_row + 1) / (float)rows;
		const float higher_proportion_difference = width_to_height_proportion - (float)chars_per_row / (float)(rows + 1);

		if(fabs(wider_proportion_difference) <= fabs(higher_proportion_difference))
		{
			++chars_per_row;
		}
		else
		{
			++rows;
		}
	}




	// Decreases the dimensions of the textbox by either one row or one column,
	// whichever more closely maintains the original proportions of the box.
	void TextBox::DecreaseDimensions()
	{
		// Should we make decrease it width-wise or height-wise?
		const float less_wide_proportion_difference = width_to_height_proportion - (float)(chars_per_row - 1) / (float)rows;
		const float less_tall_proportion_difference = width_to_height_proportion - (float)chars_per_row / (float)(rows - 1);

		if(fabs(less_wide_proportion_difference) <= fabs(less_tall_proportion_difference))
		{
			--chars_per_row;
		}
		else
		{
			--rows;
		}
	}




	// Based on the text box's behavior, characters per row, and number of rows,
	// will return how many sprites are required to display the appropriate
	// text characters.
	const unsigned int TextBox::FindRequiredSprites()
	{
		// If behavior is AUTO_ADJUST_SIZE, then we need enough sprites to fit all
		// the characters of the user's text.
		if(behavior == AUTO_ADJUST_SIZE)
		{
			return text.size();
		}
		// If the behavior is OVERWRITE_ROWS, then we need enough sprites to fill
		// all the rows but the bottom, and then enough sprites for each character
		// on the bottom row (which could have any number of characters).
		else if(behavior == OVERWRITE_ROWS)
		{
			// Is the bottom row full?
			if(text.size() % chars_per_row == 0)
			{
				return chars_per_row * rows;
			}
			// Or is it not full?
			{
				return chars_per_row * (rows - 1) + text.size() % chars_per_row;
			}
		}
		// For OVERWRITE_CHARS and NO_OVERWRITE, return the maximum size.
		else
		{
			return chars_per_row * rows;
		}
	}
	
	
	
	
	// Based on the text box's behavior, characters per row, and number of rows,
	// will return the position in the member text which is to be displayed by
	// the top-left sprite composing the text box.
	std::string::const_iterator TextBox::FindFirstCharacter()
	{
		// If the textbox isn't full, the behavior is AUTO_ADJUST_SIZE, or if
		// the behavior is NO_OVERWRITE, simply return the first position of text.
		if(chars_per_row * rows > text.size() || behavior == AUTO_ADJUST_SIZE || behavior == NO_OVERWRITE)
		{
			return text.begin();
		}
		// If the behavior is OVERWRITE_ROWS, things are a little complex. If
		// the last row should be full, then simply return the last (max size)
		// positions in text. Otherwise
		else if(behavior == OVERWRITE_ROWS)
		{
			unsigned int last_row_length = text.size() % chars_per_row;
			// If the last row is going to be full, then just return the max size.
			if(last_row_length == 0)
			{
				return text.end() - (chars_per_row * rows);
			}
			// Otherwise we want to return the capacity of all the rows but the
			// last row, plus how many characters will actually be on the last row.
			else
			{
				return text.end() - (chars_per_row * (rows - 1) + last_row_length);
			}
		}
		// If the behavior is OVERWRITE_CHARS, return as many characters as can
		// displayed starting from the back of the text string.
		else
		{
			return text.end() - chars_per_row * rows;
		}
	}













}
}