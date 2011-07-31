/**********
 * Author: Sheldon Bachstein
 * Date: Dec 25, 2010
 * Description: Unit test for the d3d renderer base component. See d3d renderer base.h for details.
 * Expected output: 
 **********/

#include"d3d renderer base.h"
#include"..\d3d display profile\d3d display profile.h"
#include"..\basic window\basic window.h"
#include<Windows.h>
#include<vector>
#include<iostream>

void TestD3DRendererBaseComponent(HINSTANCE instance)
{
	const std::vector<avl::view::D3DDisplayProfile> profiles = avl::view::EnumerateDisplayProfiles();

	int length = profiles.size();

	return;
	//avl::view::BasicWindow window(instance, "D3DRendererBase Unit Test", 600, 400);
	//avl::view::D3DRendererBase renderer(window.GetWindowHandle(), 600, 400, false);

	//// Sloppy, just a temp test. Remove. D3DDevice::Present() is never called, so the viewport
	//// clearing is never presented to the window.
	//while(window.Update() == true)
	//{
	//	renderer.ClearViewport();
	//}
}