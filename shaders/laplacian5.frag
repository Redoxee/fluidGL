#version 120

uniform sampler2DRect texture;
uniform float timeElapsed ;
uniform vec2 iResolution;

#define APPLY 1

void main(void)
{
	
	 float filter[25];
	for(int i = 0; i < 25; ++i)
	{
		filter[i]=-1.;
	}
	filter[12] = 24.;
	vec4 color = vec4(0.,0.,0.,1.);


	if(APPLY == 1){
		float delta = sin((25. + sin(gl_TexCoord[0].t * 70. + timeElapsed * 6.) ) * gl_TexCoord[0].t + timeElapsed + cos(gl_TexCoord[0].s * 40. + timeElapsed )) * 15.;
		for(int i = 0; i<25;++i)
		{
			vec2 texCoord = (gl_TexCoord[0].st + vec2(i/5,mod(i,5) )/100. ) * iResolution ;
			texCoord.s += delta;
			color += filter[i] * texture2DRect(texture, texCoord);
		}
		if((color.r+color.g+color.b)/3. < .8)
		{
			color = vec4(0.,0.,0.,1.);
		}

	}
	else
	{
		vec2 texCoord = (gl_TexCoord[0].st ) * iResolution ;
		color= texture2DRect(texture, texCoord);
	}
	gl_FragColor = color;
}