#pragma once
#ifndef AVL_VIEW_DRAW_PRIMITIVES_TASK_LIST__
#define AVL_VIEW_DRAW_PRIMITIVES_TASK_LIST__
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
Defines the DrawPrimitivesTaskList class.
@author Sheldon Bachstein
@date Aug 06, 2012
*/

#include"..\draw primitives task\draw primitives task.h"
#include"..\texture context\texture context.h"
#include"..\wrapper functions\wrapper functions.h"
#include"..\..\..\..\utility\src\graphic\graphic.h"
#include<list>

namespace avl
{
namespace view
{
namespace d3d
{

	/**
	Provides a convenient means of containing and manipulating
	DrawPrimitivesTask objects. Given a list of Graphic objects to
	be rendered, provides a means of constructing a list of
	sorted and batch-optimized DrawPrimitivesTask objects.
	*/
	class DrawPrimitivesTaskList
	{
	public:
		/** Derives DrawPrimitiveTask objects directly from \a graphics.
		@param graphics A list of unsorted graphics to be rendered.
		@param textures The texture map defining the textures used by any
		textured RenderPrimitive objects.
		@throws avl::utility::OutOfMemoryError If we run out of memory.
		@throws RendererException If we find an unsupported
		RenderPrimitive type.
		*/
		DrawPrimitivesTaskList(const utility::GraphicList& graphics, TexHandleToTexContext& textures);
		~DrawPrimitivesTaskList();
		
		/** Standardizes this list of DrawPrimitivesTask objects by first
		sorting them and then batching together any compatible tasks.
		*/
		void Standardize();
		/** Extracts the raw vertex data for the list. Note that the
		list generally should be standardized by calling Standardize()
		before extracting any render data.
		@param textured_vertex_queue [OUT] The vertex data for any textured vertices
		will be inserted here.
		@param colored_vertex_queue [OUT] The vertex data for any colored vertices
		will be inserted here.
		@throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		void ExtractVertexData(VertexQueue& textured_vertex_queue, VertexQueue& colored_vertex_queue);
		/** Extracts the raw index data for the list. Note that the
		list generally should be standardized by calling Standardize()
		before extracting any render data.
		@param index_queue [OUT] The index data will be inserted here.
		@throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		void ExtractIndexData(IndexQueue& index_queue);
		
		/** Get an iterator to the beginning of the list.
		@return An iterator pointing to the beginning of the list.
		*/
		DrawPrimitivesTasks::iterator Begin();
		/** Get an iterator to the element past the last one in
		the list.
		@return An iterator pointing to the element past the last one
		in the list.
		*/
		DrawPrimitivesTasks::iterator End();

	private:
		/** Extracts all of the RenderPrimitive objects from a list
		of Graphic objects.
		@param graphics RenderPrimitives are extracted from these.
		@param render_primitives [OUT] The extracted RenderPrimitive
		objects will be inserted here.
		@throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		static void ExtractRenderPrimitives(const utility::GraphicList& graphics, utility::RenderPrimitiveList& render_primitives);
		
		/** Given a list of RenderPrimitive objects, generates a
		corresponding list of DrawPrimitivesTask objects. The generated
		DrawPrimitivesTask objects are inserted into
		\ref draw_primitives_tasks.
		@param render_primitives The DrawPrimitivesTask
		objects will be generated from these.
		@param textures The texture map defining the textures used by any
		textured RenderPrimitive objects.
		@throws avl::utility::OutOfMemoryError If we run out of memory.
		@throws RendererException If we find an unsupported
		RenderPrimitive type.
		*/
		void GenerateDrawPrimitivesTasks(const utility::RenderPrimitiveList& render_primitives, TexHandleToTexContext& textures);
		/** Given a TexturedQuad object, generates a corresponding
		DrawPrimitivesTask object. The generated DrawPrimitivesTask
		object is inserted into \ref draw_primitives_tasks.
		@param quad The DrawPrimitivesTask object is based on this.
		@param textures The texture map defining the texture used by
		\a quad.
		@throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		void GenerateDrawPrimitivesTask(const utility::TexturedQuad& quad, TexHandleToTexContext& textures);
		
		/** Sorts \ref draw_primitives_tasks using the less-than
		operator defined in the DrawPrimitivesTask class.
		*/
		void SortDrawPrimitivesTasks();
		/** Combines any consecutive, compatible DrawPrimitivesTask
		objects in \ref draw_primitives_tasks, effectively batching
		them together.
		*/
		void CombineDrawPrimitivesTasks();

		/// Holds the DrawPrimitivesTask objects.
		DrawPrimitivesTasks draw_primitives_tasks;

		/// NOT IMPLEMENTED.
		DrawPrimitivesTaskList(const DrawPrimitivesTaskList&);
		/// NOT IMPLEMENTED.
		const DrawPrimitivesTaskList& operator=(const DrawPrimitivesTaskList&);
	};




} // d3d
} // view
} // avl
#endif // AVL_VIEW_DRAW_PRIMITIVES_TASK_LIST__
