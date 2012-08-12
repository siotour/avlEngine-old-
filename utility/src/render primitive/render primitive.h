#pragma once
#ifndef AVL_UTILITY_RENDER_PRIMITIVE__
#define AVL_UTILITY_RENDER_PRIMITIVE__
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
Defines the \ref avl::utility::RenderPrimitive class.
@author Sheldon Bachstein
@date Jul 06, 2012
*/

#include<list>

namespace avl
{
namespace utility
{
	// Forward declaration.
	class RenderPrimitive;

	/** Used to exchange groups of render primitives.
	*/
	typedef std::list<const RenderPrimitive* const> RenderPrimitiveList;

	/**
	Base class for all primitive renderable objects.
	*/
	class RenderPrimitive
	{
	public:

		/**
		@todo Document me.
		*/
		enum PrimitiveType{
			TEXTURED_QUAD,
			FILLED_QUAD,
			FILLED_CIRCLE,
			LINE_SEGMENT
		};

		/** Basic constructor.
		@post is_visible is set to true.
		@param primitive_type The type of render primitive this is.
		@param z The z-depth of this primitive.
		*/
		RenderPrimitive(const PrimitiveType primitive_type, const float z);
		/** Basic destructor.
		*/
		virtual ~RenderPrimitive();

		/** What type of primitive is this?
		@return An identifier specifying which type of render primitive
		this is.
		*/
		const PrimitiveType GetType() const;

		/** Is this object visible?
		@return True if this object is visible, and false if not.
		*/
		const bool IsVisible() const;

		/** Returns the z depth of this object.
		@return The z depth of this object.
		*/
		const float GetZ() const;

		/** Sets the visibility of this object.
		@paral visibility The new visibility for this object.
		*/
		void SetVisibility(const bool visibility);

		/** Sets the z-depth of this object.
		@param new_z The new z-depth of this object.
		*/
		void SetZ(const float new_z);

	protected:
		/// Identifies which type of primitive this is.
		PrimitiveType type;

		/// Is this object visible?
		bool is_visible;

		/// The z depth of this object.
		float z_depth;

	};


	//// See method declaration for details.
	inline const RenderPrimitive::PrimitiveType RenderPrimitive::GetType() const
	{
		return type;
	}

	// See method declaration for details.
	inline const bool RenderPrimitive::IsVisible() const
	{
		return is_visible;
	}

	// See method declaration for details.
	inline void RenderPrimitive::SetVisibility(const bool visibility)
	{
		is_visible = visibility;
	}

	// See method declaration for details.
	inline const float RenderPrimitive::GetZ() const
	{
		return z_depth;
	}

	// See method declaration for details.
	inline void RenderPrimitive::SetZ(const float new_z)
	{
		z_depth = new_z;
	}




} // utility
} // avl
#endif // AVL_UTILITY_RENDER_PRIMITIVE__
