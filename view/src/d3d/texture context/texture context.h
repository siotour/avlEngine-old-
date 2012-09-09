#pragma once
#ifndef AVL_VIEW_TEXTURE_CONTEXT__
#define AVL_VIEW_TEXTURE_CONTEXT__
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
Defines the TexHandleToTexContext type and the TextureContext
structure.
@author Sheldon Bachstein
@date Aug 01, 2012
*/

#include"..\..\..\..\utility\src\textured quad\textured quad.h"
#include<map>
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
	struct TextureContext;

	/// Maps texture handles to texture contexts.
	typedef std::map<const utility::TexturedQuad::TextureHandle, TextureContext> TexHandleToTexContext;

	/** Obtains the texture context associated with \a handle
	inside of \a textures.
	@param textures The map of texture handles and their
	associated texture contexts.
	@param handle The handle to get the texture context for.
	@throws RendererException If \a handle is not mapped to any texture.
	*/
	TextureContext& GetTextureContextFromHandle(TexHandleToTexContext& textures, const utility::TexturedQuad::TextureHandle handle);

	/**
	Groups together a texture and a flag specifying whether
	or not the texture is translucent.
	*/
	struct TextureContext
	{
	public:
		TextureContext(IDirect3DTexture9& initial_texture, const bool translucent);
		TextureContext(const TextureContext& original);
		~TextureContext();

		const bool is_translucent;
		IDirect3DTexture9& texture;

	private:
		/// NOT IMPLEMENTED.
		TextureContext();
		/// NOT IMPLEMENTED.
		const TextureContext& operator=(const TextureContext&);
	};


} // d3d
} // view
} // avl
#endif // AVL_VIEW_TEXTURE_CONTEXT__
