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
Implementation for the set translucent rendering task component. See "set translucent rendering task.h" for details.
@author Sheldon Bachstein
@date Aug 01, 2012
*/

#include"set translucent rendering task.h"
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
	SetTranslucentRenderingTask::SetTranslucentRenderingTask()
	{
	}

	// See method declaration for details.
	SetTranslucentRenderingTask::SetTranslucentRenderingTask(const SetTranslucentRenderingTask& original)
	{
	}

	// See method declaration for details.
	SetTranslucentRenderingTask::~SetTranslucentRenderingTask()
	{
	}

	// See method declaration for details.
	void SetTranslucentRenderingTask::Execute(RenderContext& render_context)
	{
		SetTranslucentRendering(render_context.device);
	}

	// See method declaration for details.
	void SetTranslucentRenderingTask::SetTranslucentRendering(IDirect3DDevice9& device)
	{
		HRESULT result = device.SetRenderState(D3DRS_ZWRITEENABLE, false);
		if(FAILED(result) == true)
		{
			throw D3DError("avl::view::d3d::SetTranslucentRenderingTask::SetTranslucentRendering()", "Unable to disable writing to the z buffer.", result);
		}
		result = device.SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		if(FAILED(result) == true)
		{
			throw D3DError("avl::view::d3d::SetTranslucentRenderingTask::SetTranslucentRendering()", "Unable to enable alpha blending.", result);
		}
	}



} // d3d
} // view
} // avl
