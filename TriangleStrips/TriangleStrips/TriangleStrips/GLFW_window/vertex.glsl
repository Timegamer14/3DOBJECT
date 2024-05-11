#version 400
in vec3 position;
in vec2 texUV;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
out vec2 textureUV;

void main(void)
{
   vec4 a = vec4(position, 1.0);

   //transform the vertices by the modelMatrix
   a =  projectionMatrix * viewMatrix * modelMatrix * a;
   textureUV = texUV;
   gl_Position = a;
} 