attribute vec3 vertex;
attribute vec3 normal;

varying vec4 color;

uniform mat4 mv;
uniform mat4 mvp;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 lightColor;

void main(void)
{
	vec4 lightPosition = vec4(0.0, 1000.0, 3000.0, 1.0);

	vec4 vertexPosition = mv * vec4(vertex, 1.0);
	vec4 mvNormal = mv * vec4(normal, 0.0);

	vec4 lightDirection = normalize(lightPosition - vertexPosition);

	float factor = max(dot(mvNormal, lightDirection), 0.0);

	vec3 bColor = ambientColor * lightColor + diffuseColor * lightColor * factor;
	color = vec4(bColor, 1.0);

	gl_Position = mvp * vec4(vertex, 1.0);
}      