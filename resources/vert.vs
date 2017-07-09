 #version 330 core

 layout (location = 0) in vec3 vPos;
 layout (location = 1) in vec3 vColor;
 
 uniform mat4 model;
 uniform mat4 view;
 uniform mat4 proj;
 
 out vec3 color;
 
 void main() {
    gl_Position = proj * view * vec4(vPos.xyz, 1);
    color = vColor;
 }