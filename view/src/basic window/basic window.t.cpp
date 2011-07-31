/**********
 * Author: Sheldon Bachstein
 * Date: Feb 03, 2010
 * Description: Unit test for the basic window component. See basic basic window.h for details.
 * Expected output:
					A movable, bordered window without any buttons and a fully white client area.
					It should have a square client area of size 500x500 pixels and say "Bleh1"
					in the title bar and taskbar icon. The taskbar icon should also have an
					error-esque icon.
 **********/

#include"basic window.h"


void TestBasicWindowComponent(HINSTANCE hInstance)
{
	avl::view::BasicWindow basic_window(hInstance, "Bleh1", 500, 500);

	while(basic_window.Update() == true)
	{
	}
}