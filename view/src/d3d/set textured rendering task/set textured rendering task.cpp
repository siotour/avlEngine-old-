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
Implementation for the set textured rendering task component. See "set textured rendering task.h" for details.
@author Sheldon Bachstein
@date Aug 01, 2012
*/

#include"set textured rendering task.h"
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
	SetTexturedRenderingTask::SetTexturedRenderingTask()
	{
	}

	// See method declaration for details.
	SetTexturedRenderingTask::SetTexturedRenderingTask(const SetTexturedRenderingTask& original)
	{
	}

	// See method declaration for details.
	SetTexturedRenderingTask::~SetTexturedRenderingTask()
	{
	}

	// See method declaration for details.
	void SetTexturedRenderingTask::Execute(RenderContext& render_context)
	{
		SetTexturedFVF(render_context.device);
	}

	// See method declaration for details.
	void SetTexturedRenderingTask::SetTexturedFVF(IDirect3DDevice9& device)
	{
		HRESULT result = device.SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
		if(FAILED(result) == true)
		{
			throw D3DError("avl::view::d3d::SetTexturedRenderingTask::SetTexturedFVF()", "Unable to set the flexible vertex format to textured mode.", result);
		}
	}



} // d3d
} // view
} // avl
