#pragma once
#ifndef AVL_VIEW_GRAPHIC_BATCH__
#define AVL_VIEW_GRAPHIC_BATCH__
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
Defines the GraphicBatch class.
@author Sheldon Bachstein
@date Aug 01, 2012
*/

#include"..\texture context\texture context.h"
#include"..\render task\render task.h"
#include"..\wrapper functions\wrapper functions.h"
#include"..\draw primitives task\draw primitives task.h"
#include"..\..\..\..\utility\src\graphic\graphic.h"
#include"..\..\..\..\utility\src\textured quad\textured quad.h"
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

	// Forward declarations.
	class RenderTaskSequence;
	struct RenderContext;
	
	/**
	@todo Either replace this class with a function or else properly
	document it.
	*/
	class GraphicBatch
	{
	public:
		GraphicBatch(const utility::GraphicList& graphics, TexHandleToTexContext& textures);
		~GraphicBatch();

		void Render(RenderContext& render_context);


	private:

		static const bool MustSetDeviceState(IDirect3DDevice9& device);
		static void SetDeviceState(IDirect3DDevice9& device);

		VertexQueue textured_vertices;
		VertexQueue colored_vertices;
		IndexQueue indices;
		std::auto_ptr<RenderTaskSequence> render_tasks;


		/// NOT IMPLEMENTED.
		GraphicBatch(const GraphicBatch&);
		/// NOT IMPLEMENTED.
		const GraphicBatch& operator=(const GraphicBatch&);
	};


} // d3d
} // view
} // avl
#endif // AVL_VIEW_GRAPHIC_BATCH__
