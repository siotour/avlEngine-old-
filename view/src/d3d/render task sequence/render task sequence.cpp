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
Implementation for the render task sequence component. See "render task sequence.h" for details.
@author Sheldon Bachstein
@date Aug 07, 2012
*/

#include"render task sequence.h"
#include"..\render state\render state.h"
#include"..\render context\render context.h"
#include"..\draw primitives task\draw primitives task.h"
#include"..\draw primitives task list\draw primitives task list.h"
#include"..\set colored rendering task\set colored rendering task.h"
#include"..\set index buffer task\set index buffer task.h"
#include"..\set opaque rendering task\set opaque rendering task.h"
#include"..\set texture task\set texture task.h"
#include"..\set textured rendering task\set textured rendering task.h"
#include"..\set translucent rendering task\set translucent rendering task.h"
#include"..\set vertex buffer task\set vertex buffer task.h"
#include"..\..\..\..\utility\src\exceptions\exceptions.h"
#include<vector>
#include<new>
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
	RenderTaskSequence::RenderTaskSequence(DrawPrimitivesTaskList& draw_primitives_tasks)
	{
		InsertSetIndexBufferTask();
		RenderState current_render_state = GetInverseRenderState(*draw_primitives_tasks.Begin());
		RenderState next_render_state;
		for(auto i = draw_primitives_tasks.Begin(); i != draw_primitives_tasks.End(); ++i)
		{
			next_render_state = GetRenderState(*i);
			GenerateTransitionTasksToNewRenderState(current_render_state, next_render_state);
			InsertDrawPrimitivesTask(*i);
			current_render_state = next_render_state;
		}
	}

	// See method declaration for details.
	RenderTaskSequence::~RenderTaskSequence()
	{
		DeleteRenderTasks();
	}

	// See method declaration for details.
	void RenderTaskSequence::Execute(RenderContext& render_context)
	{
		for(auto i = render_tasks.begin(); i != render_tasks.end(); ++i)
		{
			(*i)->Execute(render_context);
		}
	}


	// See method declaration for details.
	void RenderTaskSequence::InsertSetVertexBufferTask(const bool is_textured)
	{
		try
		{
			if(is_textured == true)
			{
				render_tasks.push_back(new(std::nothrow) SetVertexBufferTask(5 * sizeof(FLOAT), true));
			}
			else
			{
				render_tasks.push_back(new(std::nothrow) SetVertexBufferTask(4 * sizeof(FLOAT), false));
			}
		}
		catch(const std::bad_alloc&)
		{
			throw utility::OutOfMemoryError();
		}
	}

	// See method declaration for details.
	void RenderTaskSequence::InsertSetIndexBufferTask()
	{
		try
		{
			render_tasks.push_back(new(std::nothrow) SetIndexBufferTask());
		}
		catch(const std::bad_alloc&)
		{
			throw utility::OutOfMemoryError();
		}
	}

	// See method declaration for details.
	void RenderTaskSequence::InsertSetTranslucencyRenderingTask(const bool is_translucent)
	{
		try
		{
			if(is_translucent == true)
			{
				render_tasks.push_back(new(std::nothrow) SetTranslucentRenderingTask());
			}
			else
			{
				render_tasks.push_back(new(std::nothrow) SetOpaqueRenderingTask());
			}
		}
		catch(const std::bad_alloc&)
		{
			throw utility::OutOfMemoryError();
		}
	}

	// See method declaration for details.
	void RenderTaskSequence::InsertSetTexturizedRenderingTask(const bool is_textured)
	{
		try
		{
			if(is_textured == true)
			{
				render_tasks.push_back(new(std::nothrow) SetTexturedRenderingTask());
			}
			else
			{
				render_tasks.push_back(new(std::nothrow) SetColoredRenderingTask());
			}
		}
		catch(const std::bad_alloc&)
		{
			throw utility::OutOfMemoryError();
		}
	}

	// See method declaration for details.
	void RenderTaskSequence::InsertSetTextureTask(IDirect3DTexture9& texture)
	{
		try
		{
			render_tasks.push_back(new(std::nothrow) SetTextureTask(texture));
		}
		catch(const std::bad_alloc&)
		{
			throw utility::OutOfMemoryError();
		}
	}

	// See method declaration for details.
	void RenderTaskSequence::InsertDrawPrimitivesTask(const DrawPrimitivesTask& original)
	{
		try
		{
			render_tasks.push_back(new(std::nothrow) DrawPrimitivesTask(original));
		}
		catch(const std::bad_alloc&)
		{
			throw utility::OutOfMemoryError();
		}
	}

	// See method declaration for details.
	void RenderTaskSequence::GenerateTransitionTasksToNewRenderState(const RenderState& current_render_state, const RenderState& new_render_state)
	{
		if(current_render_state.texture != new_render_state.texture)
		{
			InsertSetTextureTask(*new_render_state.texture);
		}
		if(current_render_state.is_textured != new_render_state.is_textured)
		{
			InsertSetVertexBufferTask(new_render_state.is_textured);
			InsertSetTexturizedRenderingTask(new_render_state.is_textured);
		}
		if(current_render_state.is_translucent != new_render_state.is_translucent)
		{
			InsertSetTranslucencyRenderingTask(new_render_state.is_translucent);
		}
	}

	// See method declaration for details.
	void RenderTaskSequence::DeleteRenderTasks()
	{
		for(auto i = render_tasks.begin(); i != render_tasks.end(); ++i)
		{
			delete *i;
		}
	}


} // d3d
} // view
} // avl
