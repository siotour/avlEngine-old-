#pragma once
#ifndef AVL_VIEW_SET_OPAQUE_RENDERING_TASK__
#define AVL_VIEW_SET_OPAQUE_RENDERING_TASK__
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
Defines the SetOpaqueRenderingTask class.
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
	Prepares a RenderContext for rendering opaque primitives.
	*/
	class SetOpaqueRenderingTask: public RenderTask
	{
	public:
		SetOpaqueRenderingTask();
		SetOpaqueRenderingTask(const SetOpaqueRenderingTask& original);
		~SetOpaqueRenderingTask();

		/** Prepares \a render_context for the rendering of opaque primitives.
		@param render_context The context to prepare for opaque rendering.
		*/
		void Execute(RenderContext& render_context);

	private:
		/** Disables alpha blending and enables write operations to the
		z-buffer.
		@param device The device on which to operate.
		*/
		static void SetOpaqueRendering(IDirect3DDevice9& device);

		/// NOT IMPLEMENTED.
		const SetOpaqueRenderingTask& operator=(const SetOpaqueRenderingTask&);
	};




} // d3d
} // view
} // avl
#endif // AVL_VIEW_SET_OPAQUE_RENDERING_TASK__
