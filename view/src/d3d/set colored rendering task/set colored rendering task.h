#pragma once
#ifndef AVL_VIEW_SET_COLORED_RENDERING_TASK__
#define AVL_VIEW_SET_COLORED_RENDERING_TASK__
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
Defines the SetColoredRenderingTask class.
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
	Prepares a RenderContext for rendering colored vertices.
	*/
	class SetColoredRenderingTask: public RenderTask
	{
	public:
		SetColoredRenderingTask();
		SetColoredRenderingTask(const SetColoredRenderingTask& original);
		~SetColoredRenderingTask();

		/** Prepares \a render_context for rendering colored vertices.
		@param render_context The context to be prepared for rendering
		colored vertices.
		*/
		void Execute(RenderContext& render_context);

	private:
		/** Sets the flexible vertex format for \a device to contain position
		coordinates and a diffuse color value.
		*/
		static void SetColoredFVF(IDirect3DDevice9& device);

		/// NOT IMPLEMENTED.
		const SetColoredRenderingTask& operator=(const SetColoredRenderingTask&);
	};



} // d3d
} // view
} // avl
#endif // AVL_VIEW_SET_COLORED_RENDERING_TASK__
