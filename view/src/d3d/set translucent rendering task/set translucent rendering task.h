#pragma once
#ifndef AVL_VIEW_SET_TRANSLUCENT_RENDERING_TASK__
#define AVL_VIEW_SET_TRANSLUCENT_RENDERING_TASK__
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
Defines the SetTranslucentRenderingTask class.
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
	Prepares a RenderContext for rendering translucent primitives.
	*/
	class SetTranslucentRenderingTask: public RenderTask
	{
	public:
		SetTranslucentRenderingTask();
		SetTranslucentRenderingTask(const SetTranslucentRenderingTask& original);
		~SetTranslucentRenderingTask();

		/** Prepares \a render_context for rendering translucent primitives.
		@param render_context The context on which to operate on.
		*/
		void Execute(RenderContext& render_context);

	private:
		/** Enables alpha blending and disables write operations to the z-buffer
		for \a device.
		@param device The device on which to operate.
		*/
		static void SetTranslucentRendering(IDirect3DDevice9& device);

		/// NOT IMPLEMENTED.
		const SetTranslucentRenderingTask& operator=(const SetTranslucentRenderingTask&);
	};


} // d3d
} // view
} // avl
#endif // AVL_VIEW_SET_TRANSLUCENT_RENDERING_TASK__
