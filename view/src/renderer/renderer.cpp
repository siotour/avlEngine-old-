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
Implementation for the renderer component. See "renderer.h" for details.
@author Sheldon Bachstein
@date Dec 29, 2011
*/

#include"renderer.h"
#include"..\..\..\utility\src\vector\vector.h"
#include"..\..\..\utility\src\exceptions\exceptions.h"


namespace avl
{
namespace view
{
	// See method declaration for details.
	Renderer::Renderer(const avl::utility::Vector& screen_space)
		: screen_space_resolution(screen_space)
	{
	}

	// See method declaration for details.
	Renderer::~Renderer()
	{
	}



	// See method declaration for details.
	RendererException::RendererException(const std::string& description)
		: Exception(description)
	{
	}

	// See method declaration for details.
	RendererException::RendererException(const RendererException& original)
		: Exception(original)
	{
	}

	// See method declaration for details.
	RendererException::~RendererException()
	{
	}

	// See method declaration for details.
	RendererException::RendererException()
	{
	}




} // view
} // avl
