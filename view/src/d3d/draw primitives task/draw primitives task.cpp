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
Implementation for the draw primitives task component. See "draw primitives task.h" for details.
@author Sheldon Bachstein
@date Aug 01, 2012
*/

#include"draw primitives task.h"
#include"..\render context\render context.h"
#include"..\wrapper functions\wrapper functions.h"
#include"..\d3d error\d3d error.h"
#include"..\..\renderer\renderer.h"
#include"..\..\..\..\utility\src\textured quad\textured quad.h"
#include"..\..\..\..\utility\src\exceptions\exceptions.h"
#include<vector>
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
	DrawPrimitivesTask::DrawPrimitivesTask(const utility::TexturedQuad& quad, IDirect3DTexture9* const initial_texture, const bool translucent)
		: is_textured(true),
		  is_translucent(translucent),
		  primitive(D3DPT_TRIANGLELIST), 
		  number_of_vertices(4), 
		  number_of_primitives(2), 
		  z_depth(quad.GetZ()), 
		  texture(initial_texture),
		  color(0),
		  base_vertex_is_set(false),
		  base_index_is_set(false)
	{
		InsertQuadVerticesIntoQueue(quad.GetPosition(), position_vertices);
		InsertQuadVerticesIntoQueue(quad.GetTexturePosition(), texture_vertices);
	}

	// See method declaration for details.
	DrawPrimitivesTask::DrawPrimitivesTask(const DrawPrimitivesTask& original)
		: is_textured(original.is_textured),
		  is_translucent(original.is_translucent),
		  primitive(original.primitive),
		  number_of_vertices(original.number_of_vertices),
		  number_of_primitives(original.number_of_primitives),
		  z_depth(original.z_depth),
		  texture(original.texture),
		  color(original.color),
		  base_vertex(original.base_vertex),
		  base_index(original.base_index),
		  position_vertices(original.position_vertices),
		  texture_vertices(original.texture_vertices),
		  base_vertex_is_set(original.base_vertex_is_set),
		  base_index_is_set(original.base_index_is_set)
	{
	}

	// See method declaration for details.
	DrawPrimitivesTask::~DrawPrimitivesTask()
	{
	}

	// See method declaration for details.
	const bool DrawPrimitivesTask::IsTextured() const
	{
		return is_textured;
	}

	// See method declaration for details.
	const bool DrawPrimitivesTask::IsTranslucent() const
	{
		return is_translucent;
	}

	// See method declaration for details.
	IDirect3DTexture9* const DrawPrimitivesTask::GetTexture()
	{
		return texture;
	}

	
	void DrawPrimitivesTask::SetBaseVertex(const UINT vertex)
	{
		base_vertex_is_set = true;
		base_vertex = vertex;
	}


	void DrawPrimitivesTask::SetBaseIndex(const UINT index)
	{
		base_index_is_set = true;
		base_index = index;
	}

	// See method declaration for details.
	void DrawPrimitivesTask::GetVertexData(VertexQueue& queue)
	{
		if(is_textured == true)
		{
			InsertTexturedVerticesIntoQueue(queue);
		}
		else
		{
			InsertColoredVerticesIntoQueue(queue);
		}
	}

	// See method declaration for details.
	void DrawPrimitivesTask::InsertTexturedVerticesIntoQueue(VertexQueue& queue)
	{
		for(std::size_t i = 0; i < position_vertices.size(); ++i)
		{
			InsertTexturedVertexIntoQueue(*position_vertices[i], z_depth, *texture_vertices[i], queue);
		}
		position_vertices.clear();
		texture_vertices.clear();
	}

	// See method declaration for details.
	void DrawPrimitivesTask::InsertColoredVerticesIntoQueue(VertexQueue& queue)
	{
		for(std::size_t i = 0; i < position_vertices.size(); ++i)
		{
			InsertColoredVertexIntoQueue(*position_vertices[i], z_depth, color, queue);
		}
		position_vertices.clear();
	}

	// See method declaration for details.
	void DrawPrimitivesTask::InsertTexturedVertexIntoQueue(const utility::Vector& position, const float z, const utility::Vector& texture_position, VertexQueue& queue)
	{
		try
		{
			queue.push_back(position.GetX());
			queue.push_back(position.GetY());
			queue.push_back(z);
			queue.push_back(texture_position.GetX());
			queue.push_back(texture_position.GetY());
		}
		catch(const std::bad_alloc&)
		{
			throw utility::OutOfMemoryError();
		}
	}
	// See method declaration for details.
	void DrawPrimitivesTask::InsertColoredVertexIntoQueue(const utility::Vector& position, const float z, const D3DCOLOR color, VertexQueue& queue)
	{
		try
		{
			queue.push_back(position.GetX());
			queue.push_back(position.GetY());
			queue.push_back(z);
			queue.push_back(static_cast<const FLOAT>(color));
		}
		catch(const std::bad_alloc&)
		{
			throw utility::OutOfMemoryError();
		}
	}

	// See method declaration for details.
	void DrawPrimitivesTask::GetIndexData(IndexQueue& queue)
	{
		if(base_vertex_is_set == false)
		{
			throw utility::InvalidCallException("avl::view::d3d::DrawPrimitivesTask::GetIndexData()", "The base vertex must first be set. See SetBaseVertex().");
		}
		switch(primitive)
		{
		case D3DPT_TRIANGLELIST:
			// Each quad consists of two triangles.
			InsertQuadIndicesIntoQueue(base_vertex, number_of_primitives / 2, queue);
			break;
		default:
			throw RendererException("avl::view::d3d::DrawPrimitivesTask::ExtractIndices() -- Unknown D3D primitive type.");
		}
	}

	// See method declaration for details.
	void DrawPrimitivesTask::Execute(RenderContext& render_context)
	{
		if(base_index_is_set == false)
		{
			throw utility::InvalidCallException("avl::view::d3d::DrawPrimitivesTask::Execute()", "The base index must first be set. See SetBaseIndex().");
		}
		HRESULT result = render_context.device.DrawIndexedPrimitive(primitive, 0, 0, number_of_vertices, base_index, number_of_primitives);
		if(FAILED(result))
		{
			throw D3DError("avl::view::d3d::DrawPrimitivesTask::Execute()", "Unable to draw indexed primitives.", result);
		}
	}

	// See method declaration for details.
	const bool DrawPrimitivesTask::CanCombineWith(const DrawPrimitivesTask& other) const
	{
		if(primitive == other.primitive)
		{
			if(is_translucent == other.is_translucent)
			{
				if(texture == other.texture)
				{
					if(z_depth == other.z_depth)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	// See method declaration for details.
	void DrawPrimitivesTask::CombineWith(const DrawPrimitivesTask& other)
	{
		number_of_vertices += other.number_of_vertices;
		number_of_primitives += other.number_of_primitives;
		try
		{
			position_vertices.insert(position_vertices.end(), other.position_vertices.begin(), other.position_vertices.end());
			texture_vertices.insert(texture_vertices.end(), other.texture_vertices.begin(), other.texture_vertices.end());
		}
		catch(const std::bad_alloc&)
		{
			throw utility::OutOfMemoryError();
		}
	}

	// See method declaration for details.
	const bool DrawPrimitivesTask::operator<(const DrawPrimitivesTask& rhs) const
	{
		if(is_translucent == true)
		{
			if(rhs.is_translucent == true)
			{
				return CompareTranslucentToTranslucent(*this, rhs);
			}
			else
			{
				return CompareTranslucentToOpaque(*this, rhs);
			}
		}
		else
		{
			if(rhs.is_translucent == true)
			{
				return CompareOpaqueToTranslucent(*this, rhs);
			}
			else
			{
				return CompareOpaqueToOpaque(*this, rhs);
			}
		}
	}

	// See method declaration for details.
	const bool DrawPrimitivesTask::CompareTranslucentToTranslucent(const DrawPrimitivesTask& lhs, const DrawPrimitivesTask& rhs)
	{
		if(lhs.z_depth > rhs.z_depth)
		{
			return true;
		}
		else if(lhs.z_depth < rhs.z_depth)
		{
			return false;
		}
		else
		{
			return &lhs.texture < &rhs.texture;
		}
	}

	// See method declaration for details.
	const bool DrawPrimitivesTask::CompareTranslucentToOpaque(const DrawPrimitivesTask& lhs, const DrawPrimitivesTask& rhs)
	{
		return false;
	}

	// See method declaration for details.
	const bool DrawPrimitivesTask::CompareOpaqueToTranslucent(const DrawPrimitivesTask& lhs, const DrawPrimitivesTask& rhs)
	{
		return true;
	}

	// See method declaration for details.
	const bool DrawPrimitivesTask::CompareOpaqueToOpaque(const DrawPrimitivesTask& lhs, const DrawPrimitivesTask& rhs)
	{
		if(lhs.z_depth > rhs.z_depth)
		{
			return false;
		}
		else if(lhs.z_depth < rhs.z_depth)
		{
			return true;
		}
		else
		{
			return &lhs.texture < &rhs.texture;
		}
	}

	// See method declaration for details.
	void DrawPrimitivesTask::InsertQuadVerticesIntoQueue(const utility::Quad& quad, std::vector<const utility::Vector* const>& queue)
	{
		try
		{
			queue.push_back(&quad.GetP1());
			queue.push_back(&quad.GetP2());
			queue.push_back(&quad.GetP3());
			queue.push_back(&quad.GetP4());
		}
		catch(const std::bad_alloc&)
		{
			throw utility::OutOfMemoryError();
		}
	}

	// See method declaration for details.
	void DrawPrimitivesTask::InsertQuadIndicesIntoQueue(const UINT base_vertex, const UINT number_of_quads, IndexQueue& queue)
	{
		try
		{
			// Triangle 1.
			queue.push_back(base_vertex);
			queue.push_back(base_vertex + 1);
			queue.push_back(base_vertex + 2);
			// Triangle 2.
			queue.push_back(base_vertex + 2);
			queue.push_back(base_vertex + 3);
			queue.push_back(base_vertex);
		}
		catch(const std::bad_alloc&)
		{
			throw utility::OutOfMemoryError();
		}
		// Recurse for each quad.
		if(number_of_quads > 1)
		{
			InsertQuadIndicesIntoQueue(base_vertex + 4, number_of_quads - 1, queue);
		}
	}

}
} // view
} // avl
