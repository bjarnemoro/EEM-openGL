#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aPosition;
layout (location = 2) in int anchor;

out vec4 Position;
out vec2 screenPosition;

uniform float time;
uniform int selected;
out float my_time;
flat out int is_selected;
flat out int a_anchor;

void main()
{
	//gl_InstanceID
	if (selected == gl_InstanceID)
		is_selected = 0;
	else
		is_selected = 1;
	a_anchor = anchor;
	my_time = time;
	screenPosition = aPosition;
	Position = vec4(aPos + vec3(aPosition - vec2(600, 400), 0) / vec3(600, -400, 1), 1.0f);
	gl_Position = Position;
	
}