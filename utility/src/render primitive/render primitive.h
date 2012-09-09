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

	/** Non-memory managed container type for RenderPrimitives.
	*/
	typedef std::list<const RenderPrimitive* const> RenderPrimitiveList;

	/**
	Base class for all primitive renderable objects.
	*/
	class RenderPrimitive
	{
	public:

		/** Enumerates the different types of render primitves. */
		enum PrimitiveType{
			TEXTURED_QUAD,
			FILLED_QUAD,
			FILLED_CIRCLE,
			LINE_SEGMENT
		};

		/** @post This primitive will be visible.*/
		RenderPrimitive(const PrimitiveType primitive_type, const float z);
		RenderPrimitive(const RenderPrimitive& original);
		virtual ~RenderPrimitive();

		const PrimitiveType GetType() const;
		const bool IsVisible() const;
		const float GetZ() const;

		void SetVisibility(const bool visibility);
		void SetZ(const float new_z);

		RenderPrimitive& operator=(const RenderPrimitive& rhs);

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
