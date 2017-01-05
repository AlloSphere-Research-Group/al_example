//2 Tweets Challenge by nimitz (twitter: @stormoid)
// adapted from: https://www.shadertoy.com/view/4tl3W8

uniform float time;
uniform vec2 mouse;

void main(){
  vec4 uv = gl_TexCoord[0]*2.0 - 1.0;
  vec4 p = vec4(uv.x,uv.y,0.0,1.0)/1.0 - vec4(.9,.5,0,0), c=p-p;
  float t=time+mouse.x,r=length(p.xy+=sin(t+sin(t*.8))*.4),a=atan(p.y,p.x);
  for (float i = 0.;i<60.;i++)
    c = c*.98 + (sin(i+vec4(5,3,2,1))*.5+.5)*smoothstep(.99, 1., sin(log(r+i*.05)-t-i+sin(a +=t*.01)));
  gl_FragColor = c*r;
}