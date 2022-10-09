#version 330                                                             
                                                                         
in vec3 vPosition;												
in vec4 vColor;													
																
out vec4 color;													
                                                                         
void main()                                                              
{                                                                        
	gl_Position = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
	color = vColor;							                             
}