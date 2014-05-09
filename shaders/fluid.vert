#version 120

in vec2 textureCoord;

attribut vec2 TexCoord;
void main(void)
{
	vec4 pos = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	gl_Position = pos;
	TexCoord = textureCoord;
}