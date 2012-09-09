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
Implementation for the graphic batch component. See "graphic batch.h" for details.
@author Sheldon Bachstein
@date Aug 01, 2012
*/

#include"graphic batch.h"
#include"..\draw primitives task list\draw primitives task list.h"
#include"..\render task sequence\render task sequence.h"
#include"..\wrapper functions\wrapper functions.h"
#include"..\render context\render context.h"
#include"..\texture context\texture context.h"
#include"..\d3d error\d3d error.h"
#include"..\render task\render task.h"
#include"..\..\..\..\utility\src\graphic\graphic.h"
#include<memory>
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
	GraphicBatch::GraphicBatch(const utility::GraphicList& graphics, TexHandleToTexContext& textures)
	{
		
		DrawPrimitivesTaskList draw_primitive_tasks(graphics, textures);
		draw_primitive_tasks.ExtractVertexData(textured_vertices, colored_vertices);
		draw_primitive_tasks.ExtractIndexData(indices);
		render_tasks.reset(new(std::nothrow) RenderTaskSequence(draw_primitive_tasks));
	}

	// See method declaration for details.
	GraphicBatch::~GraphicBatch()
	{
	}

	// See method declaration for details.
	void GraphicBatch::Render(RenderContext& render_context)
	{
		FillVertexBuffer(render_context.textured_vertex_buffer, textured_vertices);
		FillVertexBuffer(render_context.colored_vertex_buffer, colored_vertices);
		FillIndexBuffer(render_context.index_buffer, indices);
		if(MustSetDeviceState(render_context.device) == true)
		{
			SetDeviceState(render_context.device);
		}
		render_tasks->Execute(render_context);
	}

	// See method declaration for details.
	const bool GraphicBatch::MustSetDeviceState(IDirect3DDevice9& device)
	{
		IDirect3DVertexShader9* shader;
		device.GetVertexShader(&shader);
		if(shader != nullptr)
		{
			return true;
		}
		DWORD state;
		device.GetRenderState(D3DRS_ALPHATESTENABLE, &state);
		if(state == FALSE)
		{
			return true;
		}
		device.GetRenderState(D3DRS_ALPHAREF, &state);
		if(state != (DWORD)0x00000000)
		{
			return true;
		}
		device.GetRenderState(D3DRS_ALPHAFUNC, &state);
		if(state != D3DCMP_GREATER)
		{
			return true;
		}
		device.GetRenderState(D3DRS_SRCBLEND, &state);
		if(state != D3DBLEND_SRCALPHA)
		{
			return true;
		}
		device.GetRenderState(D3DRS_DESTBLEND, &state);
		if(state != D3DBLEND_INVSRCALPHA)
		{
			return true;
		}
		device.GetRenderState(D3DRS_BLENDOP, &state);
		if(state != D3DBLENDOP_ADD)
		{
			return true;
		}
		return false;
	}

	// See method declaration for details.
	void GraphicBatch::SetDeviceState(IDirect3DDevice9& device)
	{
		HRESULT result = device.SetVertexShader(nullptr);
		if(FAILED(result) == true)
		{
			throw D3DError("avl::view::d3d::GraphicBatch::SetDeviceState()", "Unable to set the vertex shader.", result);
		}
		result = device.SetRenderState(D3DRS_ALPHATESTENABLE, true);
		if(FAILED(result) == true)
		{
			throw D3DError("avl::view::d3d::GraphicBatch::SetDeviceState()", "Unable to enable alpha testing.", result);
		}
		result = device.SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000000);
		if(FAILED(result) == true)
		{
			throw D3DError("avl::view::d3d::GraphicBatch::SetDeviceState()", "Unable to set the alpha reference.", result);
		}
		result = device.SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		if(FAILED(result) == true)
		{
			throw D3DError("avl::view::d3d::GraphicBatch::SetDeviceState()", "Unable to set the alpha function.", result);
		}
		result = device.SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		if(FAILED(result) == true)
		{
			throw D3DError("avl::view::d3d::GraphicBatch::SetDeviceState()", "Unable to set the source blend method.", result);
		}
		result = device.SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		if(FAILED(result) == true)
		{
			throw D3DError("avl::view::d3d::GraphicBatch::SetDeviceState()", "Unable to set the destination blend method.", result);
		}
		result = device.SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		if(FAILED(result) == true)
		{
			throw D3DError("avl::view::d3d::GraphicBatch::SetDeviceState()", "Unable to set the blend operation.", result);
		}
	}


	




} // d3d
} // view
} // avl
