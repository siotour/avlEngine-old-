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
Implementation for the set index buffer task component. See "set index buffer task.h" for details.
@author Sheldon Bachstein
@date Aug 01, 2012
*/

#include"set index buffer task.h"
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
	SetIndexBufferTask::SetIndexBufferTask()
	{
	}

	// See method declaration for details.
	SetIndexBufferTask::SetIndexBufferTask(const SetIndexBufferTask& original)
	{
	}

	// See method declaration for details.
	SetIndexBufferTask::~SetIndexBufferTask()
	{
	}

	// See method declaration for details.
	void SetIndexBufferTask::Execute(RenderContext& render_context)
	{
		HRESULT result = render_context.device.SetIndices(&render_context.index_buffer);
		if(FAILED(result))
		{
			throw D3DError("avl::view::d3d::SetIndexBufferTask::Execute()", "Unable to set the index buffer.", result);
		}
	}


} // d3d
} // view
} // avl
