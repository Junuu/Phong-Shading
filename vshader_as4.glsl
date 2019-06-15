#version 330

in vec4 vPosition;
in vec3 vNormals;

uniform mat4 uCenter;
//uniform mat4 uCenter2;
uniform mat4 uPerspective;
uniform mat4 uModel;
uniform mat4 uRotate;


uniform vec4 uLocLgt;
uniform vec4 uLocEye;


out vec3 fN;	// flat shading에서는 vertex에서 색상을 결정하고
out vec3 fL;	// 색상을 fragments shader로 넘겨 interpolation 했지만
out vec3 fV;	// phong shading에서는 색상 계산에 필요한 벡터를 계산한 후에
out vec4 color;
void main()
{
	
	gl_Position = (vPosition*uRotate*uCenter);
	//gl_Position = uPerspective *uModel*vPosition;

	vec4 pos = vPosition*uRotate*uCenter;	// 현재 vertex의 위치
	vec4 N = vec4(vNormals, 0);	// 노멀 벡터
	vec4 L = uLocLgt - pos;	// 현재 vertex에서 조명으로의 벡터
	vec4 V = uLocEye - pos;	// 현재 vertex에서 카메라로의 벡터

	fN = normalize(N.xyz);	// normalization, 단위 벡터로
	fL = normalize(L.xyz);
	fV = normalize(V.xyz);
	color = vec4(0.5f,0.5f,0.5f,1.0f);
	
	
}