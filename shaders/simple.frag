#version 120

uniform sampler2DRect texture;
uniform float timeElapsed ;
uniform vec2 iResolution;


void main() {

	vec2 texCoord = (gl_TexCoord[0].st ) * iResolution ;
	vec4 tex = texture2DRect(texture, texCoord );


	gl_FragColor = tex;

}
