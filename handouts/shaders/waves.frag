
  uniform float time;
  uniform vec2 mouse;

  void main() {

    float amp = mouse.y;
    float thick = mouse.x;
    float frequency = 1.0;

    vec2 uv = 2.0 * gl_TexCoord[0].xy - 1.0;
    
    float c = 0.0;
    float thickness = thick / 10.0;
    float freq = 2.0 + frequency;

    for( float i = 0.0; i < 8.0; i++ ){
      uv.x += sin( uv.y + time * freq ) * amp;
      // uv.x = abs( 1./uv.x ) * amp;    
      c += abs( thickness / uv.x );    
    }

    gl_FragColor = vec4( vec3(c,amp,amp),1 );
  }
