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
		filter[i]=1.;
	}
	filter[12] = 1.;
	vec4 color = vec4(0.,0.,0.,1.);


	if(APPLY == 1){
		for(int i = 0; i<25;++i)
		{
			vec2 texCoord = (gl_TexCoord[0].st + vec2(i/5,mod(i,5) )/100. ) * iResolution ;
			color += filter[i] * texture2DRect(texture, texCoord);
		}
		if(color.r+color.g+color.b > .0)
		{
			color = vec4(1.);
		}

	}
	else
	{
		vec2 texCoord = (gl_TexCoord[0].st ) * iResolution ;
		color= texture2DRect(texture, texCoord);
	}
	gl_FragColor = color;
}