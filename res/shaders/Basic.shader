#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord; //we are sending this to the fragment shader
uniform mat4 u_MVP; // model view projection
void main()
{
   gl_Position = u_MVP * position; 
   v_TexCoord = texCoord;
};

#shader fragment
#version 330 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main()
{
   //color = vec4(1.0, 0.3, 0.8, 1.0);
   vec4 texColor = texture(u_Texture, v_TexCoord);
   color = texColor * u_Color;
};