#version 120

uniform sampler2DRect texture;
uniform float timeElapsed ;
uniform float uDeltaTime;
uniform float difusionRate;
uniform vec2 iResolution;


#define APPLY 
#define D(i,j)  texture2DRect(texture,(vec2(i,j))).b
#define U(i,j)  texture2DRect(texture,(vec2(i,j))).r
#define V(i,j)  texture2DRect(texture,(vec2(i,j))).g

void main(void)
{
	vec4 color = vec4(0.,0.,0.,1.);
	vec2 texCoord = (gl_TexCoord[0].st ) * iResolution ;
	color = texture2DRect(texture, texCoord);

	#ifdef APPLY

		float i, j, i0, j0, i1, j1;
		float x, y, s0, t0, s1, t1, dt0;

		vec2 ij = (gl_TexCoord[0].st * iResolution);
		i = ij.x;
		j =ij.y; 

		dt0 = uDeltaTime * iResolution.x;

		x  = i - dt0 * U(i,j); 
		y  = j - dt0 * V(i,j);
		x  = max( x , .5);
		x  = min( x , iResolution.x + .5);
		i0 = x; 
		i1 = i0 + 1.;
		y  = max(y,.5);
		y  = min(y, iResolution.y + .5);
		j0 = y;
		j1 = j0 + 1.;
		s1 = x - i0; 
		s0 = 1. - s1; 
		t1 = y - j0; 
		t0 = 1. - t1;

		float density = s0 * ( t0 * D(i0,j0) + t1 * D(i0,j1)) + s1 * ( t0 * D(i1,j0)+ t1 * (i1,j1));

		color.b = density;
	#endif

	gl_FragColor = color;
}
