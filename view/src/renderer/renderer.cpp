/**
@file
Implementation for the renderer component. See "renderer.h" for details.
@author Sheldon Bachstein
@date Dec 29, 2011
*/

#include"renderer.h"
#include"..\..\..\utility\src\exceptions\exceptions.h"


namespace avl
{
namespace view
{

	Renderer::Renderer()
	{
	}


	Renderer::~Renderer()
	{
	}





	RendererException::RendererException(const std::string& description)
		: Exception(description)
	{
	}


	RendererException::RendererException(const RendererException& original)
		: Exception(original)
	{
	}


	RendererException::~RendererException()
	{
	}


	RendererException::RendererException()
	{
	}




} // view
} // avl
