#pragma once
#ifndef AVL_VIEW_RENDER_STATE__
#define AVL_VIEW_RENDER_STATE__
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
Defines the RenderState data structure and functions for generating
a RenderState and inverse RenderState for a specific
DrawPrimitivesTask object.
@author Sheldon Bachstein
@date Aug 07, 2012
*/

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
	struct RenderState;
	class DrawPrimitivesTask;

	/** Generates the RenderState which is necessary to render
	\a draw_primitive.
	@param draw_primitive The primitive to generate a RenderState for.
	*/
	const RenderState GetRenderState(DrawPrimitivesTask& draw_primitive);
	/** Generates the RenderState inverse to that which is necessary
	to render \a draw_primitive.
	@param draw_primitive The primitive to generate an inverse
	RenderState for.
	*/
	const RenderState GetInverseRenderState(const DrawPrimitivesTask& draw_primitive);

	/**
	Represents the state which a RenderContext must be in in order
	to correctly render a specific DrawPrimitivesTask.
	*/
	struct RenderState
	{
	public:
		bool is_translucent;
		bool is_textured;
		/// The texture which should be used while rendering, if
		/// \ref is_textured is true.
		IDirect3DTexture9* texture;
	};



} // d3d
} // view
} // avl
#endif // AVL_VIEW_RENDER_STATE__
