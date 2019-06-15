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


out vec3 fN;	// flat shading������ vertex���� ������ �����ϰ�
out vec3 fL;	// ������ fragments shader�� �Ѱ� interpolation ������
out vec3 fV;	// phong shading������ ���� ��꿡 �ʿ��� ���͸� ����� �Ŀ�
out vec4 color;
void main()
{
	
	gl_Position = (vPosition*uRotate*uCenter);
	//gl_Position = uPerspective *uModel*vPosition;

	vec4 pos = vPosition*uRotate*uCenter;	// ���� vertex�� ��ġ
	vec4 N = vec4(vNormals, 0);	// ��� ����
	vec4 L = uLocLgt - pos;	// ���� vertex���� ���������� ����
	vec4 V = uLocEye - pos;	// ���� vertex���� ī�޶���� ����

	fN = normalize(N.xyz);	// normalization, ���� ���ͷ�
	fL = normalize(L.xyz);
	fV = normalize(V.xyz);
	color = vec4(0.5f,0.5f,0.5f,1.0f);
	
	
}