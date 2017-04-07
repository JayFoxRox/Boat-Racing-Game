#version 130

uniform sampler2D texture;

void main() {
  vec4 t = texture2D(texture, gl_TexCoord[0].xy);
  float alpha = t.r > 0.0 ? 1.0 : 0.0;
	gl_FragColor = (gl_Color.rgb, alpha);
}
