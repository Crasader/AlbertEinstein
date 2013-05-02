attribute vec3 vertex;

varying vec4 color;

uniform mat4 mvp;
uniform vec3 pickColor;

void main(void)
{
	color = vec4(pickColor, 1.0);

	gl_Position = mvp * vec4(vertex, 1.0);
}
