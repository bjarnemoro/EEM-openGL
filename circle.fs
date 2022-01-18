#version 330 core
out vec4 FragColor;

in vec4 gl_FragCoord;
in vec4 Position;
in vec2 screenPosition;

in float my_time;
flat in int is_selected;
flat in int a_anchor;

void main()
{
    vec3 color;
    vec2 updatedPos = vec2(screenPosition.x, 800 - screenPosition.y);
    float mid_dist = length(gl_FragCoord.xy - updatedPos);
    float grad = 0.1*sin(my_time*20);

    if (mid_dist < 6)
        if (is_selected == 1)
            color = vec3(0.3f + grad, 0.6f + grad, 0.2f - grad);
        else
            color = vec3((sin((Position.x-my_time)*20)+1)/2, (cos((Position.y+my_time)*36)+1)/2, (sin((Position.x+my_time)*56)+1)/2);
    else if (mid_dist < 8)
        color = vec3(a_anchor, a_anchor, a_anchor);
    else
        discard;

    FragColor = vec4(color, 1.0f);
}