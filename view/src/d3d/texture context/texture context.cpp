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
Implementation for the texture context component. See "texture context.h" for details.
@author Sheldon Bachstein
@date Aug 01, 2012
*/

#include"texture context.h"
#include"..\..\renderer\renderer.h"
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

	// See function declaration for details.
	TextureContext& GetTextureContextFromHandle(TexHandleToTexContext& textures, const utility::TexturedQuad::TextureHandle handle)
	{
		auto i = textures.find(handle);
		if(i == textures.end())
		{
			throw RendererException("avl::view::d3d::GetTextureContextFromHandle() -- Invalid texture handle used.");
		}
		return i->second;
	}

	// See method declaration for details.
	TextureContext::TextureContext(IDirect3DTexture9& initial_texture, const bool translucent)
		: texture(initial_texture), is_translucent(translucent)
	{
	}

	// See method declaration for details.
	TextureContext::TextureContext(const TextureContext& original)
		: texture(original.texture), is_translucent(original.is_translucent)
	{
	}

	// See method declaration for details.
	TextureContext::~TextureContext()
	{
	}



} // d3d
} // view
} // avl
