#pragma once
#ifndef AVL_VIEW_RENDER_TASK_SEQUENCE__
#define AVL_VIEW_RENDER_TASK_SEQUENCE__
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
Defines the RenderTaskSequence class.
@author Sheldon Bachstein
@date Aug 07, 2012
*/

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

	// Forward declaration.
	class RenderTask;
	struct RenderContext;
	class DrawPrimitivesTask;
	class DrawPrimitivesTaskList;
	struct RenderState;

	/**
	Encapsulates a complete, executable sequence of RenderTask objects
	appropriate for rendering  a series of DrawPrimitivesTask objects. Given
	a list of RenderPrimitivesTask objects, generates the necessary
	transitional RenderTask objects in order to correctly render the
	DrawPrimitivesTask objects.
	*/
	class RenderTaskSequence
	{
	public:
		/** Given the DrawPrimitivesTask objects in
		\a draw_primitives_tasks, generates the necessary transitional
		RenderTask objects such that the sequence as a whole will
		correctly render \a draw_primitives_tasks when executed.
		@param draw_primitives_tasks The tasks around which to generate
		a complete sequence of renderable RenderTask objects.
		@throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		RenderTaskSequence(DrawPrimitivesTaskList& draw_primitives_tasks);
		~RenderTaskSequence();

		/** Executes the sequence of RenderTask objects in the context of
		\a render_context.
		@param render_context The context with which to execute.
		@throws D3DError If a Direct3D function call fails.
		*/
		void Execute(RenderContext& render_context);


	private:
		/** Generates the RenderTask objects necessary to transition from
		\a previous_render_state to \a new_render_state.
		@param previous_render_state The current state of a RenderContext.
		@param new_render_state The desired state of a RenderContext.
		@throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		void GenerateTransitionTasksToNewRenderState(const RenderState& previous_render_state, const RenderState& new_render_state);

		/** Inserts a SetVertexBufferTask object into \ref render_tasks.
		@param is_textured Do we set the textured or the colored vertex
		buffer?
		@throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		void InsertSetVertexBufferTask(const bool is_textured);
		/** Inserts a SetIndexBufferTask object into \ref render_tasks.
		@throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		void InsertSetIndexBufferTask();
		/** Inserts either a SetTranslucentRenderingTask or a
		SetOpaqueRenderingTask into \ref render_tasks, depending on the
		value of \a is_translucent.
		@param is_translucent If true, a SetTranslucentRenderingTask
		is inserted. If false, a SetOpaqueRenderingTask is inserted.
		@throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		void InsertSetTranslucencyRenderingTask(const bool is_translucent);
		/** Inserts either a SetTexturedRenderingTask or a
		SetColoredRenderingTask into \ref render_tasks, depending on the
		value of \a is_textured.
		@param is_textured If true, a SetTexturedRenderingTask
		is inserted. If false, a SetTexturizedRenderingTask is inserted.
		@throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		void InsertSetTexturizedRenderingTask(const bool is_textured);
		/** Inserts a SetTextureTask into \ref render_tasks, supplying
		\a texture as the texture to be set.
		@param texture The texture to be set.
		@throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		void InsertSetTextureTask(IDirect3DTexture9& texture);
		/** Inserts a copy of \a original into \ref render_tasks.
		@param original The DrawPrimitivesTask to be inserted.
		@throws avl::utility::OutOfMemoryError If we run out of memory.
		*/
		void InsertDrawPrimitivesTask(const DrawPrimitivesTask& original);

		/** Deletes the RenderTask pointers in
		\ref RenderTaskSequence::render_tasks.
		*/
		void DeleteRenderTasks();


		std::vector<RenderTask* const> render_tasks;

		/// NOT IMPLEMENTED.
		RenderTaskSequence();
		/// NOT IMPLEMENTED.
		RenderTaskSequence(const RenderTaskSequence&);
		/// NOT IMPLEMENTED.
		const RenderTaskSequence& operator=(const RenderTaskSequence&);
	};


} // d3d
} // view
} // avl
#endif // AVL_VIEW_RENDER_TASK_SEQUENCE__
