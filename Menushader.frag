#version 330 core
out vec4 FragColor;
  
in vec2 Tex_Coord;

uniform sampler2D Texture;

void main()
{
    FragColor = texture(Texture, Tex_Coord);
    if(FragColor.a < 0.1) {discard;}
        //FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    //FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}