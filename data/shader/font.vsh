#version 130

void main() {
	gl_Position    = gl_ModelViewMatrix * gl_Vertex;
	gl_FrontColor  = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
