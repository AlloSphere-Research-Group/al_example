
uniform float time;
uniform vec2 mouse;

void main(){

  // texture coordinate scaled to (-1,1)
  vec4 uv = gl_TexCoord[0]*2.0 - 1.0;

  // set color to absolute value of texture coordinates
  gl_FragColor = abs(uv);

  // vary color with time and mouse position
  gl_FragColor.r *= abs(sin(time));
  gl_FragColor.b *= 1.0-abs(sin(time));
  gl_FragColor.g *= abs(cos(time));
  gl_FragColor.r += mouse.y;
  gl_FragColor.b += mouse.x;
}