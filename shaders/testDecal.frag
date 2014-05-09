#version 120

uniform sampler2DRect texture;
uniform float timeElapsed ;
uniform vec2 iResolution;

#define APPLY 1

void main(void)
{
	vec4 color = vec4(0.,0.,0.,1.);
	float filter[9];
	for(int index = 0; index < 9; ++ index)
	{
		filter[index] = .25;
	}
	filter[0] = 0;
	filter[2] = 0;
	filter[4] = 0;
	filter[6] = 0;
	filter[8] = 0;

	if(APPLY == 1){

		float f = 0.;
		for(int i = 0; i < 9 ; ++i)
		{
			vec2 texCoord = ( gl_TexCoord[0].st ) * iResolution  + vec2( ( i / 3 - 1 ) , mod( i , 3 ) - 1 );
			color += filter[i] * texture2DRect(texture, texCoord);
			f += 1.;
		}
		color.q = 1.;

	}
	else
	{
		vec2 texCoord = (gl_TexCoord[0].st ) * iResolution ;
		color = texture2DRect(texture, texCoord);
	}
	gl_FragColor = color;
}