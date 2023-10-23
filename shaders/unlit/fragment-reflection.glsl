#version 330 core

in vec3 o_positionWorld;
in vec3 o_normalWorld;
out vec4 FragColor;
uniform samplerCube skybox;
uniform vec3 cameraPos;

void main()
{
	vec3 E = normalize(o_positionWorld - cameraPos);
	vec3 R = reflect(E, normalize(o_normalWorld));
	FragColor = texture(skybox, R);
}