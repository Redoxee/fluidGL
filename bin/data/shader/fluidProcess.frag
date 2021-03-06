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

	#ifdef APPLY
		float vx = 0.;
		float vy = 0.;

		float a = uDeltaTime * difusionRate * iResolution.x * iResolution.y;
		//    x[IX(i,j)] = (x0[IX(i,j)] + a * (x[IX(i-1,j)]+ x[IX(i+1,j)]+ x[IX(i,j-1)]+ x[IX(i,j+1)] ))/(1 + 4 *a);
		float density    = (D(0.,0.)    + a * (D(-1.,0.)   + D(1.,0.)    + D(0.,-1.)   + D(0.,1.)     ))/(1.+ 4.*a);
/*
		x = i-dt0*u[IX(i,j)]; y = j-dt0*v[IX(i,j)];
		if (x<0.5) x=0.5; if (x>N+0.5) x=N+ 0.5; i0=(int)x; i1=i0+1;
		if (y<0.5) y=0.5; if (y>N+0.5) y=N+ 0.5; j0=(int)y; j1=j0+1;
		s1 = x-i0; s0 = 1-s1; t1 = y-j0; t0 = 1-t1;
		d[IX(i,j)] = s0*(t0*d0[IX(i0,j0)]+t1*d 0[IX(i0,j1)])+
		s1*(t0*d0[IX(i1,j0)]+t1*d0[IX(i1,j1)]);*/


		color = vec4( vx , vy , density , 1.);

	#else
		vec2 texCoord = (gl_TexCoord[0].st ) * iResolution ;
		color = texture2DRect(texture, texCoord);
	#endif

	gl_FragColor = color;
}