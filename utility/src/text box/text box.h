#pragma once
#ifndef AVL_UTILITY_TEXT_BOX__
#define AVL_UTILITY_TEXT_BOX__
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
Defines the utility::TextBox class, which provides the behavior of a dynamic box of text.
@author Sheldon Bachstein
@date Sep 25, 2011
@todo This file description needs a more detailed comment.
*/

#include"..\graphic\graphic.h"
#include"..\render primitive\render primitive.h"
#include"..\textured quad\textured quad.h"
#include<vector>
#include<deque>
#include<string>
#include<memory>


namespace avl
{
namespace utility
{
	/** @todo Properly comment this class definition.
	*/
	class TextBox: public Graphic
	{
	private:
		// Forward declaration. See definition below for details.
		class Row;
	public:
		/** Behavior flags for how the text box should react to
		overstepping the maximum number of displayable characters in the box.
		@note When using any of these behaviors, if the user attempts to display more
		characters in a textbox than is possible, all of the original characters and
		the characters attempted to be displayed by the user after the maximum size
		is reached will be stored and, in the event of the user specifying new
		dimensions for the text box, it will dynamically change how much of the text
		it displays to match the new dimensions. In other words: Excess text data will
		not be discarded.
		*/
		enum OverwriteBehavior{
			/** If the user reaches the maximum number of characters, any
			new characters will be ignored.*/
			NO_OVERWRITE,
			/** If the user reaches the maximum number of characters, any
			additional characters will, if necessary, remove the top
			row of text and scoot all rows up one position, and then
			begin a new row at the bottom.*/
			OVERWRITE_ROWS,
			/** If the user reaches the maximum number of characters,
			any additional characters will force the text box to
			resize itself to accomodate the new characters. Note
			that, with this behavior, the \a chars_per_row and \a rows
			parameters are ignored in the constructor.*/
			AUTO_ADJUST_SIZE
		};


		/** Full-spec constructor.
		@pre \a texture_handle must be the handle to a font image with the following properties:
			\li It contains all 256 extended ASCII characters in sequence.
			\li Each character has the same pixel dimensions.
			\li There are \a chars_per_texture_row characters per row.
			\li There is no padding around the edges of the image or between characters.
		@param left The left boundary of the text box.
		@param top The top boundary of the text box.
		@param right The right boundary of the text box.
		@param bottom The bottom boundary of the text box.
		@param z The z-depth of the text box.
		@param chars_per_row The number of characters per row.
		@param number_of_rows The number of rows in the box.
		@param behavior The overwrite behavior of the box.
		@param texture_handle The handle to the font image.
		@param chars_per_texture_row The number of characters per row in the font image.
		*/
		TextBox(const float& left, const float& top, const float& right, const float& bottom, const float& z, 
				const unsigned int& chars_per_row, const unsigned int& number_of_rows,
				const OverwriteBehavior& behavior, const utility::TexturedQuad::TextureHandle& texture_handle,
				const unsigned int& chars_per_texture_row);
		/** Basic destructor.*/
		~TextBox();

		/** Returns the text box's visibility.
		@return True if the box is visible, and false if not.
		*/
		const bool IsVisible() const;

		/** Returns the text box's current text, unmodified due to the overwrite behavior.
		@return The box's current text.
		*/
		const std::string& GetText() const;

		/** Returns the maximum number of characters displayed per row.
		@return The maximum number of characters displayed per row.
		*/
		const unsigned int GetCharsPerRow() const;
		/** Returns the maximum number of rows display.
		@return The maximum number of rows displayed.
		*/
		const unsigned int GetRows() const;

		/** Gets the box's left boundary.
		@return The left boundary of the box.
		*/
		const float& GetLeft() const;
		/** Gets the box's left boundary.
		@return The left boundary of the box.
		*/
		const float& GetTop() const;
		/** Gets the box's upper boundary.
		@return The upper boundary of the box.
		*/
		const float& GetRight() const;
		/** Gets the box's right boundary.
		@return The right boundary of the box.
		*/
		const float& GetBottom() const;
		/** Gets the box's z depth.
		@return The z depth of the box.
		*/
		const float& GetZ() const;

		/** Gets the box's texture handle.
		@return The current texture handle.
		*/
		const utility::TexturedQuad::TextureHandle& GetTextureHandle() const;

		/** Gets the render primitives used to render this object.
		@return The render primitives used to render this object.
		*/
		const RenderPrimitiveList& GetRenderPrimitives() const;



		/** Sets the box's visiblity.
		@param new_visibility The new visibility of the box.
		*/
		void SetVisiblity(const bool new_visibility);

		/** Sets the box's text.
		@todo Are the overwrite behaviors properly implemented? See this method's
		definition for details.
		@post \a new_text will be displayed in the text box according to the
		box's current overwrite behavior.
		@param new_text The new text to be displayed in the box.
		@see OverwriteBehavior
		*/
		void SetText(const std::string& new_text);
		/** Appends text to the end of the box.
		@param new_text The text to be appended to the box.
		*/
		void AddText(const std::string& new_text);
		/** Clears the box of any text.
		*/
		void ClearText();

		/** Changes the dimensions of the box.
		@post The dimensions of the text box will be changed to display the new specifications. However,
		the overwrite behavior of this text box will still be in effect.
		@param new_chars_per_row The new number of characters to be displayed per row.
		@param new_rows The new number of rows to be displayed.
		@see OverwriteBehavior
		*/
		void SetDimensions(const unsigned int& new_chars_per_row, const unsigned int& new_rows);

		/** Changes the way that this text box behaves when the user attempts to display more characters
		than will fit within the text box's dimensions.
		@param new_behavior The new overwrite behavior.
		@see OverwriteBehavior
		*/
		void SetOverwriteBehavior(const OverwriteBehavior& new_behavior);

		/** Changes the position and/or size of the box.
		@param new_left The new left boundary.
		@param new_top The new upper boundary.
		@param new_right The new right boundary.
		@param new_bottom The new lower boundary.
		@param new_z The new z-depth.
		*/
		void SetPosition(const float& new_left, const float& new_top, const float& new_right, const float& new_bottom, const float& new_z);

		// Change the texture handle used to draw text.
		void SetTextureHandle(const utility::TexturedQuad::TextureHandle& new_texture_handle);




	private:

		/** Increases the dimensions of the textbox by either one row or one column,
		whichever more closely maintains the original proportions of the box.
		@post The text box will be increased by either 1 row or 1 character per row,
		whichever more accurately maintains the original proportions of the box.
		*/
		void IncreaseDimensions();
		/** Decreases the dimensions of the textbox by either one row or one column,
		whichever more closely maintains the original proportions of the box.
		@post The text box will be decreased by either 1 row or 1 character per row,
		whichever more accurately maintains the original proportions of the box.
		*/
		void DecreaseDimensions();

		/** Generates the substrings which will be displayed on each row of the
		text box. These substrings are based on how many characters may fit in a
		row and on any newline characters in the text.
		@return The substrings for each row, with the upper row at the front and
		the lower row at the back.
		*/
		std::deque<const std::string> GenerateRowSubstrings() const;



		/// The visibility of the text box.
		bool visibility;
		
		/// The current text for the text box.
		std::string text;

		/// Number of characters per row.
		unsigned int chars_per_row;
		/// Number of rows.
		unsigned int number_of_rows;
		/// The original proportion of width to height.
		float width_to_height_proportion;

		/// The text box's overwrite behavior.
		OverwriteBehavior behavior;

		/// Left boundary.
		float left;
		/// Top boundary.
		float top;
		/// Right boundary.
		float right;
		/// Bottom boundary.
		float bottom;
		/// Z-depth.
		float z;

		/// Number of characters per row in the texture.
		unsigned int chars_per_texture_row;
		/// The texture handle.
		utility::TexturedQuad::TextureHandle texture_handle;

		/// The rows composing this textbox.
		std::vector<Row> rows;




		/// NOT IMPLEMENTED.
		TextBox(const TextBox&);
		/// NOT IMPLEMENTED.
		TextBox& operator=(const TextBox&);




		/** Represents a single row of text within the text box.
		*/
		class Row
		{
		public:
			/** Default constructor.
			@warning Doesn't initialize any variables -- use at your own risk!
			*/
			Row();
			/** Copy constructor.
			@warning Doesn't initialize any variables -- use at your own risk!
			@param original The object being copied.
			*/
			Row(const Row& original);
			/** Basic destructor.
			*/
			~Row();


			/** Gets the render primitives used to render this object.
			@return The render primitives used to render this object.
			*/
			const std::list<TexturedQuad>& GetQuads() const;


			/** Sets the visibility of this row.
			@param new_visibility The new visibility of the row.
			*/
			void SetVisibility(const bool new_visibility);

			/** Sets the text of this row.
			@param new_text The row's new text.
			@throws OutOfMemoryError If unable to allocate required memory.
			*/
			void SetText(const std::string& new_text);

			/** Sets the maximum number of characters for this row.
			@param new_max The new number of max characters in this row.
			*/
			void SetMaxCharacters(const unsigned int& new_max);

			/** Sets the number of characters per row in the texture.
			@param new_chars The new number of characters per row in the texture.
			*/
			void SetCharsPerTextureRow(const unsigned int& new_chars);

			/** Sets position of this row.
			@param new_left The new left boundary.
			@param new_top The new upper boundary.
			@param new_right The new right boundary.
			@param new_bottom The new lower boundary.
			@param new_z The new z-depth.
			*/
			void SetPosition(const float& new_left, const float& new_top, const float& new_right, const float& new_bottom, const float& new_z);

			/** Sets the new texture handle for the row.
			@param new_texture_handle The new texture handle for this row.
			*/
			void SetTextureHandle(const utility::TexturedQuad::TextureHandle& new_texture_handle);

			/** Assignment operator.
			@warning Note -- this doesn't perform as one would expect! Use at your own risk.
			@todo This stub was inserted simply to stop a linker error. When this class was used
			within a STL container, the linker issued a warning about a templated type not having
			an assignment operator, so I added this to shut it up. This class needs to be rewritten
			so that it has accessors and a legitimate assignment operator. And also a legitimate
			full-spec constructor.
			*/
			Row& operator=(const Row& rhs);
			


		private:

			/// Visibility of this row.
			bool visibility;

			/// This row's text.
			std::string text;

			// The maximum width of this row, in characters.
			unsigned int max_characters;

			/// Left boundary.
			float left;
			/// Upper boundary.
			float top;
			/// Right boundary.
			float right;
			/// Lower boundary.
			float bottom;
			/// Z-depth.
			float z;

			/// The number of characters per row in the texture.
			unsigned int chars_per_texture_row;

			/// The texture handle for characters in this row.
			utility::TexturedQuad::TextureHandle texture_handle;

			/// All of the textured quads forming this row.
			std::list<TexturedQuad> quads;

		};

	};






} //avl
} //utility
#endif // AVL_UTILITY_TEXT_BOX__