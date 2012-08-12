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
Implementation for the render context component. See "render context.h" for details.
@author Sheldon Bachstein
@date Aug 01, 2012
*/

#include"render context.h"
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
	RenderContext::RenderContext(IDirect3DDevice9& initial_device, IDirect3DIndexBuffer9& initial_index_buffer, IDirect3DVertexBuffer9& initial_textured_vertex_buffer, IDirect3DVertexBuffer9& initial_colored_vertex_buffer)
		: device(initial_device), index_buffer(initial_index_buffer), textured_vertex_buffer(initial_textured_vertex_buffer), colored_vertex_buffer(initial_colored_vertex_buffer)
	{
	}

	// See method declaration for details.
	RenderContext::~RenderContext()
	{
	}



} // d3d
} // view
} // avl
