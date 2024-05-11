#version 400
in vec2 textureUV;
uniform sampler2D mytexture;

void main (void)  
{     
   gl_FragColor = texture(mytexture, textureUV);  
}