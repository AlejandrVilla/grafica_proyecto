#version 330 core
out vec4 FragColor;
uniform vec4 ourColor2;
//in vec4 ourColor;

void main()
{
	//FragColor = ourColor + ourColor2;
	FragColor = ourColor2;
}