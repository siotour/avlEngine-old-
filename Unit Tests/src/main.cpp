#include<Windows.h>

void TestAssertComponent();
void TestBasicRendererComponent(HINSTANCE);
void TestD3DRendererBaseComponent(HINSTANCE);

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int show_cmd)
{
	// Introduce this component test.
	TestBasicRendererComponent(instance);
	//TestD3DRendererBaseComponent(instance);

	

	return 0;
}


/*
#include<iostream>
void TestTexturedQuadComponent();

int main()
{
	TestTexturedQuadComponent();
	 return 0;
}
*/