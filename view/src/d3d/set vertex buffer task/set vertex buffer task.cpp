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
Implementation for the set vertex buffer task component. See "set vertex buffer task.h" for details.
@author Sheldon Bachstein
@date Aug 01, 2012
*/

#include"set vertex buffer task.h"
#include"..\render context\render context.h"
#include"..\d3d error\d3d error.h"
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

	// See method declaration for details.
	SetVertexBufferTask::SetVertexBufferTask(const UINT size_of_vertex, const bool use_textured_buffer)
		: vertex_size(size_of_vertex), use_textured_vertex_buffer(use_textured_buffer)
	{
	}

	// See method declaration for details.
	SetVertexBufferTask::SetVertexBufferTask(const SetVertexBufferTask& original)
		: vertex_size(original.vertex_size), use_textured_vertex_buffer(original.use_textured_vertex_buffer)
	{
	}

	// See method declaration for details.
	void SetVertexBufferTask::Execute(RenderContext& render_context)
	{
		HRESULT result;
		if(use_textured_vertex_buffer == true)
		{
			result = render_context.device.SetStreamSource(0, &render_context.textured_vertex_buffer, 0, vertex_size);
		}
		else
		{
			result = render_context.device.SetStreamSource(0, &render_context.colored_vertex_buffer, 0, vertex_size);
		}
		if(FAILED(result))
		{
			throw D3DError("avl::view::d3d::SetVertexBufferTask::Execute()", "Unable to set the vertex buffer.", result);
		}
	}


} // d3d
} // view
} // avl
