#version 120

uniform sampler2DRect texture;
uniform float timeElapsed ;
uniform float uDeltaTime;
uniform float difusionRate;
uniform vec2 iResolution;


#define APPLY 
#define D(i,j)  texture2DRect(texture,(gl_TexCoord[0].st * iResolution) + vec2(i,j)).b

void main(void)
{
	vec4 color = vec4(0.,0.,0.,1.);
	vec2 texCoord = (gl_TexCoord[0].st ) * iResolution ;
	color = texture2DRect(texture, texCoord);

	#ifdef APPLY
		float vx = 0.;
		float vy = 0.;

		float a = uDeltaTime * difusionRate * iResolution.x * iResolution.y;
		//    x[IX(i,j)] = (x0[IX(i,j)] + a * (x[IX(i-1,j)]+ x[IX(i+1,j)]+ x[IX(i,j-1)]+ x[IX(i,j+1)] ))/(1 + 4 *a);
		float density    = (D(0.,0.)    + a * (D(-1.,0.)   + D(1.,0.)    + D(0.,-1.)   + D(0.,1.)     ))/(1.+ 4.*a);
		color.b = density;
	#endif

	gl_FragColor = color;
}