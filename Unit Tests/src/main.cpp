#include<Windows.h>

void TestAssertComponent();
void TestBasicRendererComponent(HINSTANCE);
void TestD3DRendererBaseComponent(HINSTANCE);
void TestVertex2DComponent();
void TestSpriteComponent();
void TestDirectInputInputDeviceComponent(HINSTANCE);
void TestTextBoxComponent(HINSTANCE);

void BugFixing(HINSTANCE);

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int show_cmd)
{
	TestBasicRendererComponent(instance);
	//TestD3DRendererBaseComponent(instance);
	//TestDirectInputInputDeviceComponent(instance);
	//TestTextBoxComponent(instance);

	

	return 0;
}



//#include<iostream>
//void TestTexturedQuadComponent();
//
//int main()
//{
//	//TestVertex2DComponent();
//	TestSpriteComponent();
//	return 0;
//}
