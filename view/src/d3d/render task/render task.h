#pragma once
#ifndef AVL_VIEW_RENDER_TASK__
#define AVL_VIEW_RENDER_TASK__
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
Defines the RenderTask class, which is the base class for all render tasks
used to render graphics to the screen.
@author Sheldon Bachstein
@date Aug 01, 2012
*/

#include<vector>
#include<memory>

namespace avl
{
namespace view
{
namespace d3d
{
	// Forward declaration.
	struct RenderContext;

	/**
	Represents a task to be executed in the process of rendering
	graphics to the screen.
	*/
	class RenderTask
	{
	public:
		RenderTask();
		RenderTask(const RenderTask& original);
		virtual ~RenderTask();

		/** Executes the task on \a render_context.
		@param render_context The context in which to execute the task.
		*/
		virtual void Execute(RenderContext& render_context) = 0;

	private:
		/// NOT IMPLEMENTED.
		const RenderTask& operator=(const RenderTask&);
	};


} // d3d
} // view
} // avl
#endif // AVL_VIEW_RENDER_TASK__
