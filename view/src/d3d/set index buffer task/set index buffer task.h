#pragma once
#ifndef AVL_VIEW_SET_INDEX_BUFFER_TASK__
#define AVL_VIEW_SET_INDEX_BUFFER_TASK__
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
Defines the SetIndexBufferTask class.
@author Sheldon Bachstein
@date Aug 01, 2012
*/

#include"..\render task\render task.h"

namespace avl
{
namespace view
{
namespace d3d
{
	// Forward declarations.
	struct RenderContext;

	/**
	Sets the index buffer of a RenderContext.
	*/
	class SetIndexBufferTask: public RenderTask
	{
	public:
		SetIndexBufferTask();
		SetIndexBufferTask(const SetIndexBufferTask& original);
		~SetIndexBufferTask();

		/** Sets the index buffer of \a render_context to prepare
		for indexed rendering.
		@param render_context The context for which to set the index buffer.
		*/
		void Execute(RenderContext& render_context);

	private:
		// NOT IMPLEMENTED.
		const SetIndexBufferTask& operator=(const SetIndexBufferTask&);
	};


} // d3d
} // view
} // avl
#endif // AVL_VIEW_SET_INDEX_BUFFER_TASK__
