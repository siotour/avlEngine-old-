#pragma once
#ifndef AVL_VIEW_SET_TEXTURE_TASK__
#define AVL_VIEW_SET_TEXTURE_TASK__
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
Defines the SetTextureTask class.
@author Sheldon Bachstein
@date Aug 01, 2012
*/

#include"..\render task\render task.h"
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include<d3d9.h>

namespace avl
{
namespace view
{
namespace d3d
{
	// Forward declarations.
	struct RenderContext;

	/**
	Sets the texture for a RenderContext so that rendered primitives
	will display that texture.
	*/
	class SetTextureTask: public RenderTask
	{
	public:
		SetTextureTask(IDirect3DTexture9& initial_texture);
		SetTextureTask(SetTextureTask& original);

		/** Sets the texture of \a render_context.
		@param render_context The context for which to set the texture.
		*/
		void Execute(RenderContext& render_context);

	private:
		/// The texture which is to be used to rendering primitives.
		IDirect3DTexture9& texture;

		/// NOT IMPLEMENTED.
		SetTextureTask();
		/// NOT IMPLEMENTED.
		const SetTextureTask& operator=(const SetTextureTask&);
	};


} // d3d
} // view
} // avl
#endif // AVL_VIEW_SET_TEXTURE_TASK__
