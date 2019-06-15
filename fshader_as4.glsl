#version 330


in vec3 fN;
in vec3 fL;
in vec3 fV;
in vec4 color;

uniform vec4 uMAmb;
uniform vec4 uMDif;
uniform vec4 uMSpec;
uniform vec4 uLAmb;
uniform vec4 uLDif;
uniform int uGloss;

out vec4  fColor;

void main()
{

	vec3 NN = normalize(fN);	// vertex shader에서 넘겨 받기 전에 normalization을 했지만
	vec3 NL = normalize(fL);	// fragment shader로 오는 과정에서 interpolation의 결과로 단위 벡터가 아닌 상태가 될 수 있다.
	vec3 NV = normalize(fV);	// 그래서 한번 더 normalization 과정을 거친다.

	vec3 R = 2 * (dot(NL, NN))*NN - NL;	// 반사 벡터 계산
	
	float d = max(dot(NN, NL), 0);	// 노멀벡터와 조명벡터의 벌어진 정도
	float s = max(dot(R, NV), 0);	// 반사벡터와 카메라벡터의 벌어진 정도

	s = pow(s, uGloss);	// specular 강도 조절

	fColor = (uLAmb* uMAmb) + (uLDif* uMDif* d) + (uMSpec* uLDif * s);
			// ambient + diffuse + specular

}