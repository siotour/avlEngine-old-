#pragma once
#ifndef AVL_VIEW_D3D_D3D_ERROR__
#define AVL_VIEW_D3D_D3D_ERROR__
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
Implements the \ref avl::view::d3d::D3DError class, which represents an error
or failure due to a Direct3D function call.
@author Sheldon Bachstein
@date Apr 01, 2012
*/

#include"../renderer/renderer.h"
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

	/** This exception type is thrown when one of the Direct3D functions fail. In addition
	to a description of the problem, it records the error code returned from the function.
	*/
	class D3DError: public RendererException
	{
	public:
		/** Full-spec constructor.
		@param d3d_function The name of the Direct3D function which failed.
		@param explanation A description of the problem which has occurred.
		@param error_code The actual error code returned from calling \a d3d_function.
		function.
		*/
		D3DError(const std::string& d3d_function, const std::string& explanation, const HRESULT& error_code);
		/** Basic destructor.*/
		virtual ~D3DError();
		/** Copy constructor.
		@param original The object being copied.
		*/
		D3DError(const D3DError& original);

		/** Returns the failed error code.
		@return The error code returned from the problematic Direct3D function.
		*/
		const HRESULT& GetErrorCode() const;

	private:
		/// The error code.
		const HRESULT error_code;

		/// NOT IMPLEMENTED.
		const D3DError& operator=(const D3DError&);
	};



} // d3d
} // view
} // avl
#endif // AVL_VIEW_D3D_D3D_ERROR__
