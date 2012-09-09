#pragma once
#ifndef AVL_VIEW_DRAW_PRIMITIVES_TASK__
#define AVL_VIEW_DRAW_PRIMITIVES_TASK__
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
Defines the DrawPrimitivesTask class.
@author Sheldon Bachstein
@date Aug 01, 2012
*/

#include"..\render task\render task.h"
#include"..\wrapper functions\wrapper functions.h"
#include"..\..\..\..\utility\src\textured quad\textured quad.h"
#include<vector>
#include<list>
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
	class DrawPrimitivesTask;
	struct RenderContext;

	/** Not to be confused with \ref avl::view::d3d::DrawPrimitivesTaskList.
	*/
	typedef std::list<DrawPrimitivesTask> DrawPrimitivesTasks;

	/**
	Draws a batch of D3D primitives to a RenderContext.
	@todo Finish documenting me.
	*/
	class DrawPrimitivesTask: public RenderTask
	{
	public:
		/** Used to draw a \ref avl::utility::TexturedQuad.
		@param quad The quad to draw.
		@param initial_texture The texture to use while drawing.
		@param translucent True if the textured quad is translucent, and false
		if it's opaque.
		*/
		DrawPrimitivesTask(const utility::TexturedQuad& quad, IDirect3DTexture9* const initial_texture, const bool translucent);
		DrawPrimitivesTask(const DrawPrimitivesTask& original);
		~DrawPrimitivesTask();

		/** Specify the index of the first vertex of this primitives
		batch. This is used as the basis for the index data.
		@param vertex The index of the first vertex of this primitives
		batch.
		*/
		void SetBaseVertex(const UINT vertex);
		/** Specify the index of the first index of this primitives
		batch. This is used to specify the beginning of the index data
		during the call to DrawIndexPrimitive().
		@param index The index of the first index of this primitives
		batch.
		*/
		void SetBaseIndex(const UINT index);

		/** Retrieves the vertex data for this primitive batch.
		@post The vertex data will be insert into \a queue.
		@param queue [OUT] The vertex data will be inserted here.
		*/
		void GetVertexData(VertexQueue& queue);
		/** Retrieves the index data for this primitives batch.
		@pre The base vertex must be set. See SetBaseVertex().
		@post The index data will be insert into \a queue.
		@param queue [OUT] The index data will be inserted here.
		*/
		void GetIndexData(IndexQueue& queue);

		/** Draws this primitive batch to \a render_context.
		@pre The base index must be set. See SetBaseIndex().
		@param render_context The contex which this primitive batch
		will be drawn.
		*/
		void Execute(RenderContext& render_context);

		/** Is this primitives batch textured?
		@return True if this primitives batch is textured, and false if not.
		*/
		const bool IsTextured() const;
		/** Is this primitives batch translucent or colored?
		@return True if this primitives batch is translucent, and false it's
		colored.
		*/
		const bool IsTranslucent() const;
		/** Retrieves the texture used to draw this primitive batch, or nullptr
		if this primitives batch isn't textured.
		@return The texture used to draw this primitives batch.
		*/
		IDirect3DTexture9* const GetTexture();

		/** Checks to see if this primitves batch can be combined with \a other.
		@param other The primitives batch to check if it can be combined with
		this one.
		@return True if this object can be combined with \a other, and false if
		not.
		*/
		const bool CanCombineWith(const DrawPrimitivesTask& other) const;
		/** Combines this primitives batch with \a other.
		@pre You must first make sure that this primitives batch may correctly
		be combined with \a other by calling CanCombineWith().
		@param other The primitives batch to combine with.
		*/
		void CombineWith(const DrawPrimitivesTask& other);

		/** Imposes an ordering between two DrawPrimitivesTasks so that they
		may be sorted.
		@param rhs The object which is being compared to this one.
		@return True if this object is less than \a rhs.
		*/
		const bool operator<(const DrawPrimitivesTask& rhs) const;

	private:
		/** 
		*/
		void InsertTexturedVerticesIntoQueue(VertexQueue& queue);
		/** 
		*/
		void InsertColoredVerticesIntoQueue(VertexQueue& queue);
		/** 
		*/
		static void InsertTexturedVertexIntoQueue(const utility::Vector& position, const float z, const utility::Vector& texture_position, VertexQueue& queue);
		/** 
		*/
		static void InsertColoredVertexIntoQueue(const utility::Vector& position, const float z, const D3DCOLOR color, VertexQueue& queue);

		/** 
		*/
		static const bool CompareTranslucentToTranslucent(const DrawPrimitivesTask& lhs, const DrawPrimitivesTask& rhs);
		/** 
		*/
		static const bool CompareTranslucentToOpaque(const DrawPrimitivesTask& lhs, const DrawPrimitivesTask& rhs);
		/** 
		*/
		static const bool CompareOpaqueToTranslucent(const DrawPrimitivesTask& lhs, const DrawPrimitivesTask& rhs);
		/** 
		*/
		static const bool CompareOpaqueToOpaque(const DrawPrimitivesTask& lhs, const DrawPrimitivesTask& rhs);

		/** 
		*/
		static void InsertQuadVerticesIntoQueue(const utility::Quad& quad, std::vector<const utility::Vector* const>& queue);
		/** 
		*/
		static void InsertQuadIndicesIntoQueue(const UINT base_vertex, const UINT number_of_quads, IndexQueue& queue);

		/// Has the base vertex been set?
		bool base_vertex_is_set;
		/// Has the base index been set?
		bool base_index_is_set;
		/// The base vertex of the primitives batch.
		UINT base_vertex;
		/// The base index of the primitives batch.
		UINT base_index;
		/// The number of vertices in the batch.
		UINT number_of_vertices;
		/// The number of primitives Direct3D primitives in the batch.
		UINT number_of_primitives;
		/// The type of Direct3D primitive to be drawn.
		D3DPRIMITIVETYPE primitive;
		/// Is this primitives batch translucent?
		const bool is_translucent;
		/// Is this primitives batch textured? If not, it's colored.
		const bool is_textured;
		/// The z depth of the vertices in this primitives batch.
		const float z_depth;
		/// The texture used to draw this batch, if any.
		IDirect3DTexture9* const texture;
		/// The color used in this batch, if any.
		const D3DCOLOR color;

		/// The position vertices used in this batch.
		std::vector<const utility::Vector* const> position_vertices;
		/// The texture vertices used in this batch.
		std::vector<const utility::Vector* const> texture_vertices;

		/// NOT IMPLEMENTED.
		const DrawPrimitivesTask& operator=(const DrawPrimitivesTask&);
	};



} // d3d
} // view
} // avl
#endif // AVL_VIEW_DRAW_PRIMITIVES_TASK__
