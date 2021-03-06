#pragma once
#ifndef AVL_VIEW_SET_VERTEX_BUFFER_TASK__
#define AVL_VIEW_SET_VERTEX_BUFFER_TASK__
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
Defines the SetVertexBufferTask class.
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
	Sets the vertex buffer of a RenderContext.
	*/
	class SetVertexBufferTask: public RenderTask
	{
	public:
		/** Constructor.
		@param size_of_vertex The size, in bytes, of each vertex.
		@param use_textured_buffer True if we're to set the vertex buffer for
		textured vertices, and false if we're to set the vertex buffer for
		colored vertices.
		*/
		SetVertexBufferTask(const UINT size_of_vertex, const bool use_textured_buffer);
		SetVertexBufferTask(const SetVertexBufferTask& original);

		/** Sets the active vertex buffer for \a render context to either the textured
		or the colored vertex buffer as specified in the constructor.
		*/
		void Execute(RenderContext& render_context);
		
	private:
		/// The size of each vertex.
		const UINT vertex_size;
		/// Do we use the textured or the colored vertex buffer?
		const bool use_textured_vertex_buffer;

		/// NOT IMPLEMENTED.
		SetVertexBufferTask();
		/// NOT IMPLEMENTED.
		const SetVertexBufferTask& operator=(const SetVertexBufferTask&);
	};


} // d3d
} // view
} // avl
#endif // AVL_VIEW_SET_VERTEX_BUFFER_TASK__
