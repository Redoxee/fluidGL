#version 120

uniform float timeElapsed ;

void main() {

	vec2 texCoord = gl_TexCoord[0].st;
	vec4 tex = texture2DRect(texture, texCoord );

	gl_FragColor = tex;

}
