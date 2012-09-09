#pragma once
#ifndef AVL_VIEW_RENDERER__
#define AVL_VIEW_RENDERER__
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
Defines the \ref avl::view::Renderer class and the \ref avl::view::RendererException exception class.
@author Sheldon Bachstein
@date Dec 29, 2011
*/

#include"..\image\image.h"
#include"..\..\..\utility\src\exceptions\exceptions.h"
#include"..\..\..\utility\src\graphic\graphic.h"
#include"..\..\..\utility\src\textured quad\textured quad.h"
#include"..\..\..\utility\src\vector\vector.h"
#include<string>


namespace avl
{
namespace view
{
	/**
	Defines an interface for rendering \ref avl::utility::RenderPrimitive objects to the screen.
	*/
	class Renderer
	{
	public:
		/** Basic constructor.
		@param screen_space The adjusted screen resolution for the renderer. 
		The center of the screen will be at (0, 0). The x component will specify
		the distance from the center to either side, and the y component will
		specify the distance from the center to the top or bottom.
		*/
		Renderer(const avl::utility::Vector& screen_space);
		/** Basic destructor.*/
		virtual ~Renderer();

		/** Makes it possible to render \a image using the returned texture handle.
		@post It will be possible to render \a image by using the returned texture handle.
		@param image The image to be saved internally and accessed with the returned texture
		handle.
		@return A texture handle used to access \a image as a texture. This texture handle may be
		used to render \a image.
		*/
		virtual const utility::TexturedQuad::TextureHandle AddTexture(const Image& image) = 0;

		/** Removes the texture associated with \a handle so that it will be freed from memory
		and will no longer be able to be rendered.
		@post \a handle will no longer be associated with a texture.
		@param handle The texture handle to delete.
		*/
		virtual void DeleteTexture(const utility::TexturedQuad::TextureHandle& handle) = 0;

		/** Deletes all textures and renders the handles associated with them invalid.
		@post All previously issued texture handles will be rendered invalid, but they
		may become associated with new textures in the future.
		*/
		virtual void ClearTextures() = 0;

		/** Renders \a graphics to the screen. Details may vary depending on the implementation.
		@param graphics The primitives to be rendered.
		@throws RendererException If one of the objects in \a primitives contains a texture
		handle which isn't associated with a texture or if an error makes it impossible to
		perform the rendering.
		*/
		virtual void RenderGraphics(const utility::GraphicList& graphics) = 0;

	protected:
		/// The adjusted screen resolution for the renderer. The center of
		/// the screen will be at (0, 0). The x component will specify the
		/// distance from the center to either side, and the y component will
		/// specify the distance from the center to the top or bottom.
		const avl::utility::Vector screen_space_resolution;

	private:

		/// Not implemented.
		Renderer(const Renderer&);
		/// Not implemented.
		Renderer& operator=(const Renderer&);

	};



	/**
	Represents a failure within a renderer.
	*/
	class RendererException: public utility::Exception
	{
	public:
		/** Basic constructor.
		@param description A description of why this exception was thrown.
		*/
		RendererException(const std::string& description);
		/** Copy constructor.
		@param original The object being copied.
		*/
		RendererException(const RendererException& original);
		/** Basic destructor.*/
		virtual ~RendererException();

	protected:
		/** Default constructor.*/
		RendererException();

	private:
		/// Not implemented.
		RendererException& operator=(const RendererException&);
	};





} // view
} // avl
#endif // AVL_VIEW_RENDERER__
