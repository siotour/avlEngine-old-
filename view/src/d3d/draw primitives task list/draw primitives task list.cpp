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
Implementation for the draw primitives task list component. See "draw primitives task list.h" for details.
@author Sheldon Bachstein
@date Aug 06, 2012
*/

#include"draw primitives task list.h"
#include"..\render task\render task.h"
#include"..\draw primitives task\draw primitives task.h"
#include"..\texture context\texture context.h"
#include"..\wrapper functions\wrapper functions.h"
#include"..\d3d error\d3d error.h"
#include"..\..\renderer\renderer.h"
#include"..\..\..\..\utility\src\render primitive\render primitive.h"
#include"..\..\..\..\utility\src\graphic\graphic.h"
#include"..\..\..\..\utility\src\exceptions\exceptions.h"
#include"..\..\..\..\utility\src\assert\assert.h"


namespace avl
{
namespace view
{
namespace d3d
{

	// See method declaration for details.
	DrawPrimitivesTaskList::DrawPrimitivesTaskList(const utility::GraphicList& graphics, TexHandleToTexContext& textures)
	{
		utility::RenderPrimitiveList render_primitives;
		ExtractRenderPrimitives(graphics, render_primitives);
		GenerateDrawPrimitivesTasks(render_primitives, textures);
	}

	// See method declaration for details.
	DrawPrimitivesTaskList::~DrawPrimitivesTaskList()
	{
	}

	// See method declaration for details.
	void DrawPrimitivesTaskList::Standardize()
	{
		SortDrawPrimitivesTasks();
		CombineDrawPrimitivesTasks();
	}

	// See method declaration for details.
	DrawPrimitivesTasks::iterator DrawPrimitivesTaskList::Begin()
	{
		return draw_primitives_tasks.begin();
	}

	// See method declaration for details.
	DrawPrimitivesTasks::iterator DrawPrimitivesTaskList::End()
	{
		return draw_primitives_tasks.end();
	}

	// See method declaration for details.
	void DrawPrimitivesTaskList::ExtractVertexData(VertexQueue& textured_vertex_queue, VertexQueue& colored_vertex_queue)
	{
		for(auto i = draw_primitives_tasks.begin(); i != draw_primitives_tasks.end(); ++i)
		{
			if(i->IsTextured() == true)
			{
				ASSERT(textured_vertex_queue.size() % 5 == 0);
				i->SetBaseVertex(textured_vertex_queue.size() / 5);
				i->GetVertexData(textured_vertex_queue);
			}
			else
			{
				ASSERT(colored_vertex_queue.size() % 4 == 0);
				i->SetBaseVertex(textured_vertex_queue.size() / 4);
				i->GetVertexData(colored_vertex_queue);
			}
		}
	}

	// See method declaration for details.
	void DrawPrimitivesTaskList::ExtractIndexData(IndexQueue& index_queue)
	{
		for(auto i = draw_primitives_tasks.begin(); i != draw_primitives_tasks.end(); ++i)
		{
			i->SetBaseIndex(index_queue.size());
			i->GetIndexData(index_queue);
		}
	}

	// See method declaration for details.
	void DrawPrimitivesTaskList::ExtractRenderPrimitives(const utility::GraphicList& graphics, utility::RenderPrimitiveList& render_primitives)
	{
		try
		{
			const utility::RenderPrimitiveList* current_graphic_primitives;
			for(auto i = graphics.cbegin(); i != graphics.cend(); ++i)
			{
				current_graphic_primitives = &(*i)->GetRenderPrimitives();
				render_primitives.insert(render_primitives.cend(), current_graphic_primitives->cbegin(), current_graphic_primitives->cend());
			}
		}
		catch(const std::bad_alloc&)
		{
			throw utility::OutOfMemoryError();
		}
	}

	// See method declaration for details.
	void DrawPrimitivesTaskList::GenerateDrawPrimitivesTasks(const utility::RenderPrimitiveList& render_primitives, TexHandleToTexContext& textures)
	{
		for(auto i = render_primitives.cbegin(); i != render_primitives.cend(); ++i)
		{
			switch((*i)->GetType())
			{
			case utility::RenderPrimitive::TEXTURED_QUAD:
				GenerateDrawPrimitivesTask(*static_cast<const utility::TexturedQuad* const>(*i), textures);
				break;
			default:
				throw RendererException("avl::view::d3d::DrawPrimitivesTaskList::GenerateDrawPrimitivesTasks() -- Unable to render unsupported RenderPrimitive type.");
			}
		}
	}

	// See method declaration for details.
	void DrawPrimitivesTaskList::GenerateDrawPrimitivesTask(const utility::TexturedQuad& quad, TexHandleToTexContext& textures)
	{
		TextureContext& texture_context = GetTextureContextFromHandle(textures, quad.GetTextureHandle());
		DrawPrimitivesTask task(quad, &texture_context.texture, texture_context.is_translucent);
		try
		{
			draw_primitives_tasks.push_back(task);
		}
		catch(const std::bad_alloc&)
		{
			throw utility::OutOfMemoryError();
		}
	}

	

	// See method declaration for details.
	void DrawPrimitivesTaskList::SortDrawPrimitivesTasks()
	{
		draw_primitives_tasks.sort();
	}

	// See method declaration for details.
	void DrawPrimitivesTaskList::CombineDrawPrimitivesTasks()
	{
		if(draw_primitives_tasks.size() < 2)
		{
			return;
		}

		auto current_task = draw_primitives_tasks.begin();
		auto next_task = current_task;
		for(++next_task; next_task != draw_primitives_tasks.end(); ++next_task)
		{
			if(current_task->CanCombineWith(*next_task) == true)
			{
				current_task->CombineWith(*next_task);
				draw_primitives_tasks.erase(next_task);
				next_task = current_task;
			}
			else
			{
				++current_task;
			}
		}
	}


} // d3d
} // view
} // avl
