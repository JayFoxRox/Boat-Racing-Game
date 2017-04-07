#version 130

uniform sampler2D texture;

void main() {
  vec4 t = texture2D(texture, gl_TexCoord[0].xy);
	gl_FragColor = gl_Color * t;
}
