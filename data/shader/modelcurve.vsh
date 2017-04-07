#version 130

float gl_ClipDistance[2];

#define Vertex vec3

Vertex vertex; //FIXME: Replace this by a struct later!

uniform float offset;
uniform float startOffset;
uniform float endOffset;
uniform mat4 startMatrix;
uniform mat4 endMatrix;

void main() {
  mat4 viewProjectionMatrix = gl_ModelViewProjectionMatrix;
  vertex = gl_Vertex.xyz + vec3(offset,0.0,0.0);

  // Calculate clipping and blending info
  float t = (vertex.x - startOffset) / (endOffset - startOffset);

  Vertex startVertex;
  Vertex endVertex;
  Vertex blendedVertex;

  startVertex = (vec4(vertex, 1.0) * startMatrix).xyz;
  endVertex = (vec4(vertex, 1.0) * endMatrix).xyz;
  blendedVertex = mix(startVertex, endVertex, clamp(t, 0.0, 1.0));
  
  gl_Position = viewProjectionMatrix * vec4(blendedVertex, 1.0);
	gl_FrontColor = gl_Color;

  // Now clip remaining parts
  gl_ClipDistance[0] = t; // Clip if t is less than 0.0
  gl_ClipDistance[1] = 1.0 - t; // Clip if t is larger than 1.0
}
