#version 330 core
layout (location = 0) in vec2 aPos;

out vec4 Position;
out vec2 screenPosition;

void main()
{
	//gl_InstanceID
	screenPosition = aPos;
	Position = vec4((aPos - vec2(600, 400)) / vec2(600, -400), 0.0f, 1.0f);
	gl_Position = Position;
}