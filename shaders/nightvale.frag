#version 120

uniform sampler2DRect texture;
uniform float timeElapsed ;
uniform vec2 iResolution;


float circle(vec2 center , float radius,float thickness,float la,float ha)
{
	float f = length(center);
	
	float a = atan(center.y,center.x);
	return(step(f,radius) * step(radius - thickness,f) * step(la,a)*step(a,ha));
}

float cable(vec2 p,float dx,float dy,float r,float thick,float la,float ha)
{
	p.x-=dx;
	p.y -= dy;
	return (circle(p,r,thick,la,ha));
}

float rectangle(vec2 p, float w, float h,float dx,float dy,float a)
{
	p.x -= dx;
	p.y -= dy;
	float x =  p.x *cos(a) - p.y* sin( a) ;
	float y = p.y *cos(a) + p.x *sin(a) ;
	return step(-w/2.,x)*step(x,w/2.)*step(-h/2.,y)*step(y,h/2.);
}

void main(void)
{

	vec2 uv = gl_TexCoord[0].xy ;
	uv.y = 1. - uv.y;
	vec2 p = -1. + 2. * uv;
	float smooth = 0.01;

	//vec3 col = vec3(step(uv.y,.5));
	//col = mix(col,vec3(.5),step(uv.x,.5)*.5);


	vec3 col = vec3(0.603,0.227,0.576);
	
	p.x+=0.5;
	p.y -= 0.15;
	
	float time = (timeElapsed)*0.25 ;
	float f2 = clamp(sin(time)*1005.-1004.5 ,-0.0,0.4);
	
	vec2 p2 = p - vec2(0.,-1.6 - f2);
	float r = sqrt(dot(p2,p2));
	r *= .5;
	float f = smoothstep(r,r+smooth,1.);
	
	p2 = p - vec2(0.,1.6+f2);
	r = sqrt(dot(p2,p2))*.5;
	//r+= time;
	f *= smoothstep(r,r+smooth,1.);
	
	float border = f;
	time = timeElapsed * .25;
	f2 = sin( time) * (1. - sin( time))*cos( time) ;
	
	p.x += f2 * 0.35;
	r = sqrt(dot(p ,p));
	f *=1.- smoothstep(r,r+smooth, 0.34);
	
	col = mix(col,vec3(0.411,0.129,0.380),f*border);
	
	p2 = p - vec2(-f2 * .001 ,0.);
	r = sqrt(dot(p2,p2));
	f = smoothstep(r,r+smooth,0.27);	
	p2 = p - vec2(0.15- f2*-0.1 ,0.);
	r = sqrt(dot(p2,p2));
	f *= 1.- smoothstep(r,r+smooth, 0.26);
	
	f2 = step(0, 78.66);
	col = mix(col,vec3(1.,f2,f2),f*border);
	
	
	p = -1. + 2. * uv;
	p.x*=2.; 
	
	f=rectangle(p,0.07,2.,0.95,-0.25,0.);
	
	f = max(f,rectangle(p,0.8,0.07,0.91,0.55,0.));
	
	f = max(f,rectangle(p,0.04,0.3,1.05,0.4,0.55));
	f = max(f,rectangle(p,0.04,0.35,0.82,0.43,-0.65));
	
	f = max(f,cable(p,1.8,1.5,1.55,.02,-2.49,0.));
	
	f = max(f,cable(p,2.,1.5,1.55,.02,-2.49,0.));
	
	f =max(f, cable(p,2.6,2.0,2.05,.02,-2.37,0.));
	
	f = max(f,cable(p,2.5,2.0,2.05,.02,-2.37,0.));
	
	f=max(f,rectangle(p,1.,1.,-.8,-1.25,1.));
	
	f=max(f,rectangle(p,1.,1.,-.5,-1.25,2.14));
	
	f=max(f,rectangle(p,0.02,0.75,-0.3,-0.87,0.));
	
	f=max(f,rectangle(p,0.5,0.02,-0.3,-0.5,0.));
	
	f=max(f,rectangle(p,0.015,0.145,-0.40,-0.5,0.3));
	f=max(f,rectangle(p,0.015,0.145,-0.45,-0.5,0.3));
	f=max(f,rectangle(p,0.015,0.145,-0.50,-0.5,0.3));
	
	f=max(f,rectangle(p,0.015,0.145,-0.20,-0.5,0.3));
	f=max(f,rectangle(p,0.015,0.145,-0.15,-0.5,0.3));
	f=max(f,rectangle(p,0.015,0.145,-0.10,-0.5,0.3));
	
	col = mix(col,vec3(0.),f);
	

	
	gl_FragColor = vec4(col,1.0);
}