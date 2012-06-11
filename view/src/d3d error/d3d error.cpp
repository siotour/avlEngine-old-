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
Implementation for the d3d error component. See "d3d error.h" for details.
@author Sheldon Bachstein
@date Apr 01, 2012
*/

#include"d3d error.h"
#include<sstream>


namespace avl
{
namespace view
{
namespace d3d
{

	// See function declaration for details.
	D3DError::D3DError(const std::string& d3d_function, const std::string& explanation, const HRESULT& error_code)
		: error_code(error_code)
	{
		description = "The Direct3D function ";
		description += d3d_function;
		description += " failed with error code ";
		std::ostringstream temp;
		temp << error_code;
		description += temp.str();
		description += ": ";
		description += explanation;
	}




	// See function declaration for details.
	D3DError::~D3DError()
	{
	}




	// See function declaration for details.
	D3DError::D3DError(const D3DError& original)
	: RendererException(original), error_code(original.GetErrorCode())
	{
	}




	// See function declaration for details.
	const HRESULT& D3DError::GetErrorCode() const
	{
		return error_code;
	}



} // d3d
} // view
} // avl
