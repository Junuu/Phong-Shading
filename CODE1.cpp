/*
Ctrl+k , Ctrl+c 전체 주석처리
Ctrl+k , Ctrl+u 전체 주석해제
.obj file load and Phong shading and Perspective projection and interface
*/
#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "InitShader.h"
#include <iostream>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include"vec.h"
#include"math.h"

//glew를 glfw보다 먼저 include 하지 않으면 
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))


char count[100];
char bind[100];

int vertexNum = 0;
int faceNum = 0;
int vertIndex = 0;
int faceIndex = 0;
int position_index = 0;
int normal_index = 0;

int space_count = 0; //기본값 = 멈춤

int x_flag = 1; //기본값 x축 회전
int y_flag = 0;
int z_flag = 0;

float thetaX = 0.0; //사면체의 X회전을 위한 변수
float thetaY = 0.0; //사면체의 Y회전을 위한 변수
float thetaZ = 0.0; //사면체의 Z회전을 위한 변수


float maxX = 0.0;
float maxY = 0.0;
float maxZ = 0.0;

float minX = 0.0;
float minY = 0.0;
float minZ = 0.0;

float sumX = 0.0;
float sumY = 0.0;
float sumZ = 0.0;

float avgX = 0.0;
float avgY = 0.0;
float avgZ = 0.0;

float time = 0.0f;

char file_name[20];
int indices_index = 0;
float* positions;
float* normals;
unsigned int* indices;

void gasketSetKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void gasketSetMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void idle();

int main()
{
	
	std::cout << "1.dragon.obj" << "\n";
	std::cout << "2.buddha.obj" << "\n";
	std::cout << "3.cube.obj" << "\n";
	std::cout << "4.bunny.obj" << "\n";
	std::cout << "파일 이름을 입력하세요:" << "\n";
	std::cin >> file_name;
	std::cout << file_name << "을 읽는중 입니다."<<"\n";
	
	
	FILE * file = fopen(file_name, "r");
	
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}

	while (!feof(file)) {
		fscanf(file, "%s", count);
		
		if (count[0] == 'v' && count[1] == '\0')
			vertexNum += 1;
		else if (count[0] == 'f' && count[1] == '\0')
			faceNum += 1;
		memset(count, '\0', sizeof(count));
	}

	std::cout << "정점 수:"<<vertexNum << "\n";
	std::cout << "면의 수:"<<faceNum << "\n";
	

	vec4 *vertex;
	vec4 *face;
	
	vertex = (vec4 *)malloc(sizeof(vec4) * vertexNum);
	face = (vec4 *)malloc(sizeof(vec4) * faceNum);
	positions = (float *)malloc(sizeof(float) * vertexNum * 3);
	indices = (unsigned int *)malloc(sizeof(unsigned int) * faceNum * 3);
	normals = (float *)malloc(sizeof(float) * faceNum * 3 * 3);

	file = fopen(file_name, "r");

	while (!feof(file)) {

		fscanf(file, "%s", bind);

		if (bind[0] == 'v' && bind[1] == '\0') {

			fscanf(file, "%f %f %f", &vertex[vertIndex].x, &vertex[vertIndex].y, &vertex[vertIndex].z);
			
			//정규화를 위한 최대값 구하기
			if (vertex[vertIndex].x > maxX)	maxX = vertex[vertIndex].x;
			if (vertex[vertIndex].y > maxY)	maxY = vertex[vertIndex].y;
			if (vertex[vertIndex].z > maxZ)	maxZ = vertex[vertIndex].z;

				
			//정규화를 위한 최소값 구하기
			if (vertex[vertIndex].x < minX)	minX = vertex[vertIndex].x;
			if (vertex[vertIndex].y < minY)	minY = vertex[vertIndex].y;
			if (vertex[vertIndex].z < minZ)	minZ = vertex[vertIndex].z;
			
			if (maxX - minX == 0) sumX += (vertex[vertIndex].x - minX);
			else sumX += (vertex[vertIndex].x - minX) / (maxX - minX);
			if (maxY - minY == 0) sumY += (vertex[vertIndex].y - minY);
			else sumY += (vertex[vertIndex].y - minY) / (maxY - minY);
			if (maxZ - minZ == 0) sumZ += (vertex[vertIndex].z - minZ);
			else sumZ += (vertex[vertIndex].z - minZ) / (maxZ - minZ);

			//sumZ += vertex[vertIndex].z;

			
			//sumZ += vertex[vertIndex].z;
			vertIndex = vertIndex + 1;

		}

		else if (bind[0] == 'f' && bind[1] == '\0') {

			fscanf(file, "%f %f %f", &face[faceIndex].x, &face[faceIndex].y, &face[faceIndex].z);

			faceIndex = faceIndex + 1;

		}
		avgX = sumX / vertexNum;
		avgY = sumY / vertexNum;
		avgZ = sumZ / vertexNum;
	}
	
	
	
	for(int i= 0; i < vertexNum; i++)
	{
		//positions 배열에 정점에 관한 정보를 저장함
		// 저장할 때 min-max 정규화를 실행(각각의 object를 동일한 크기로 만들기 위해)
		positions[position_index] = (vertex[i].x - minX) / (maxX - minX);
		position_index = position_index + 1;

		positions[position_index] = (vertex[i].y - minY) / (maxY - minY);
		position_index = position_index + 1;

		positions[position_index] = (vertex[i].z - minZ) / (maxZ - minZ);
		position_index = position_index + 1;

	}
	
	
	for (int i = 0; i < faceNum; i++)
	{
		//indices 배열에 인덱스에 관한 정보를 저장함
		// -1을 하는 이유는 .obj파일에서 0번이아닌 1부터 시작하기때문
		indices[indices_index] = face[i].x - 1;		
		indices_index = indices_index + 1;

		indices[indices_index] = face[i].y - 1;		
		indices_index = indices_index + 1;

		indices[indices_index] = face[i].z - 1;		
		indices_index = indices_index + 1;
		
	}
	
	for (int i = 0; i < faceNum*3;)
	{
		//normal vertor 구하기	
		glm::vec3 n;		//+ normal 벡터
		glm::vec3 p, q;
		
		

		p = glm::vec3(positions[indices[i + 1] * 3] - positions[indices[i] * 3], positions[(indices[i + 1] * 3) + 1] - positions[(indices[i] * 3) + 1], positions[(indices[i + 1] * 3) + 2] - positions[(indices[i] * 3) + 2]);	//+ ba 벡터의 xyz
		q = glm::vec3(positions[indices[i + 2] * 3] - positions[indices[i] * 3], positions[(indices[i + 2] * 3) + 1] - positions[(indices[i] * 3) + 1], positions[(indices[i + 2] * 3) + 2] - positions[(indices[i] * 3) + 2]);	//+ ca 벡터의 xyz
		
		n = cross(p, q);	//+ 삼각형의 두 벡터를 cross product 하면 삼각형에 수직인 벡터를 구할 수 있다
		n = normalize(n);	//+ 삼각형에 수직인 벡터를 normalization
		for (int j = 0; j < 3; j++)
		{
			normals[normal_index++] = n.x;			
			normals[normal_index++] = n.y;
			normals[normal_index++] = n.z;
			
			
		}
		
		i = i + 3;
	}
	/*
	int normal_count = 0;
	std::cout  << "노말출력\n";
	for (int i = 0; i < faceNum*3*3; i++)
	{
		std::cout << normals[i] <<" ";
		normal_count++;
		if (normal_count % 3 == 0)
		{
			std::cout << "\n";
		}
		if (normal_count % 9 == 0)
		{
			std::cout << "\n";
		}
	}
	*/
	GLFWwindow* window; //glfw window 를 생성하기위한 포인터를 우선 생성했다.
	if (!glfwInit()) // glfw 라이브러리 초기화
		return -1;

	
	

	window = glfwCreateWindow(512, 512, "Phong Shading", NULL, NULL);	// window 생성

	
	if (!window)	// window 생성 실패 시 프로그램 종료
	{
		glfwTerminate();
		return -1;
	}
	

	glfwMakeContextCurrent(window); // window context 생성
	// 매개변수로 받은 window를 도화지로 삼겠다는 함수 만약 window1, window2로 2개의 window가 존재한다면두 개의 이상의 윈도우에 그림을 그릴 수 있습니다.
	if (glewInit() != GLEW_OK) std::cout << "Error\n";	// glew 라이브러리 초기화 및 초기화 실패 시 에러 메세지
														// glew 라이브러리는 반드시 window context 생성 후 초기화 아니면 에러

   glfwSetKeyCallback(window, gasketSetKeyCallback);
   glfwSetMouseButtonCallback(window, gasketSetMouseButtonCallback);	

	GLuint va;	// vertex array 선언, 메모리 할당, 바인드
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);

	

	GLuint vb;	// vertex buffer 선언, 메모리 할당, 바인드, data가 구성된 형식에 맞게 buffer data 준비
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
//	glBufferData(GL_ARRAY_BUFFER, vertexNum*3*sizeof(float), positions, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vertexNum * 3 * sizeof(float)+ sizeof(float) * faceNum * 3 * 3, NULL, GL_STATIC_DRAW);	// vertex의 position에 대한 정보와 color에 대한 정보를 저장할 수 있는 크기의 공간 준비
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexNum * 3 * sizeof(float), positions);	// 공간의 처음(0)부터 sizeof(positions) 크기까지는 positions 배열의 정보를 저장
	glBufferSubData(GL_ARRAY_BUFFER, vertexNum * 3 * sizeof(float), sizeof(float) * faceNum * 3 * 3, normals);	// 공간의 sizeof(positions)부터 sizeof(colors) 크기까지는 colors 배열의 정보를 저장	
	
	GLuint ib;	// index buffer, 선언, 메모리 할당, 바인드, data가 구성된 형식에 맞게 buffer data 준비
	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceNum*3*sizeof(float), indices, GL_STATIC_DRAW);
	

	GLuint program = InitShader("vshader_as4.glsl", "fshader_as4.glsl");	// shader program 가져오기
	glUseProgram(program);	// 어떤 shader program을 사용할 것인지

	GLuint location = glGetAttribLocation(program, "vPosition");	// position vertex에 대한 정보를 shader program 안의 어떤 변수와 연결시킬 것인가
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormals = glGetAttribLocation(program, "vNormals");
	glEnableVertexAttribArray(vNormals);
	glVertexAttribPointer(vNormals, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(faceNum * 3 * sizeof(float)));	// vb의 sizeof(positions) 위치에서부터 color에 대한 정보를 가져옴


	glm::mat4 myView = glm::mat4(1.0f);
	myView = glm::lookAt(glm::vec3(2.0f,2.0f, -2.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));	
	//myView = glm::lookAt(glm::vec3(2.0f*cos(time), 2.0f, -2.0f*sin(time)), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	GLuint uModelView = glGetUniformLocation(program, "uModel");
	glUniformMatrix4fv(uModelView, 1, GL_FALSE, &myView[0][0]);

	glm::mat4 myPerspective = glm::mat4(1.0f);
	myPerspective = glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 10.0f);
	//myPerspective = glm::perspective(glm::radians(30.0f*sin(time) + 30.0f), 1.0f, 0.01f, 10.0f);
	GLuint uPerspective = glGetUniformLocation(program, "uPerspective");
	glUniformMatrix4fv(uPerspective, 1, GL_FALSE, &myPerspective[0][0]);

	
	mat4 translateCenter = Translate(-avgX, -avgY, -avgZ);	
	GLuint uCenter = glGetUniformLocation(program, "uCenter");
	glUniformMatrix4fv(uCenter, 1, GL_FALSE, translateCenter);
	
	mat4 translateCenter2 = Translate(0.9f, 0.4f, -0.1f);
	GLuint uCenter2 = glGetUniformLocation(program, "uCenter2");
	glUniformMatrix4fv(uCenter2, 1, GL_FALSE, translateCenter2);

	mat4 rotation = mat4(1.0f);
	GLuint uRotate = glGetUniformLocation(program, "uRotate");
	glUniformMatrix4fv(uRotate, 1, GL_FALSE, rotation);

	glm::vec4 ma = glm::vec4(0.1, 0.1, 0.1, 1);	// ambient	
	glm::vec4 md = glm::vec4(1, 1, 3, 1);		// diffse	
	glm::vec4 ms = glm::vec4(1, 1, 0, 1);		// specular
	
	

	// for light properties
	glm::vec4 ld = glm::vec4(1, 1, 1, 1);		// diffuse
	glm::vec4 la = glm::vec4(1, 1, 1, 1);		// ambient light

	// for location
	glm::vec4 ll = glm::vec4(0, 10, 0, 1);		// light		
	glm::vec4 locLight = glm::vec4(10.0f, 1.0f, 10.0f, 1.0f);		// light
	//glm::vec4 locLight = glm::vec4(10.0f*sin(time), 1.0f, 10.0f*cos(time), 1.0f);		// light
	glm::vec4 locEye = glm::vec4(0, 0, 0, 1);		// eye
	

	int gloss = 30;
	GLuint uMAmb = glGetUniformLocation(program, "uMAmb");
	GLuint uMDif = glGetUniformLocation(program, "uMDif");
	GLuint uMSpec = glGetUniformLocation(program, "uMSpec");
	GLuint uLAmb = glGetUniformLocation(program, "uLAmb");
	GLuint uLDif = glGetUniformLocation(program, "uLDif");
	GLuint uLocLgt = glGetUniformLocation(program, "uLocLgt");
	GLuint uLocEye = glGetUniformLocation(program, "uLocEye");
	GLuint uGloss = glGetUniformLocation(program, "uGloss");
	glUniform4fv(uMAmb, 1, &ma[0]);
	glUniform4fv(uMDif, 1, &md[0]);
	glUniform4fv(uMSpec, 1, &ms[0]);
	glUniform4fv(uLAmb, 1, &la[0]);
	glUniform4fv(uLDif, 1, &ld[0]);
	glUniform4fv(uLocLgt, 1, &locLight[0]);
	glUniform4fv(uLocEye, 1, &locEye[0]);
	glUniform1i(uGloss, gloss);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

// window 창이 종료될 때까지 무한루프 
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		if (space_count == 1)
		{			
			rotation = RotateX(thetaX)*RotateY(thetaY)*RotateZ(thetaZ); //math.h 안에 함수가 모두 존재한다.
			GLuint uRotate = glGetUniformLocation(program, "uRotate");
			glUniformMatrix4fv(uRotate, 1, GL_FALSE, rotation);
			//v는 cpu에서 보내주려고 하는 상태 1은 shader에서의 matrix 1개가 들어간다는 의미?, GL_FLASE는 행렬의 tranpose 여부			
			idle();
		}
		time += 0.002f;
		glDrawElements(GL_TRIANGLES, faceNum * 3, GL_UNSIGNED_INT, NULL);
		glfwSwapBuffers(window);	// front buffer와 back buffer 교체
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}


void gasketSetKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		std::cout << "SPACE BAR is Pressed\n";
		if (space_count == 0) space_count = 1; //1이면 회전
		else space_count = 0; //0이면 멈춤
	}
}

void gasketSetMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		std::cout << "LEFT BUTTON is Pressed\n";
		
		if (x_flag == 1)
		{
			std::cout << "회전축 변경 \n";
			x_flag = 0;
			y_flag = 1;
		}
		else if (y_flag == 1)
		{
			std::cout << "회전축 변경 \n";
			y_flag = 0;
			z_flag = 1;
		}
		else if (z_flag == 1)
		{
			std::cout << "회전축 변경 \n";
			z_flag = 0;
			x_flag = 1;			
		}
	}
}
void idle()
{
	time += 0.002f;

	if (x_flag == 1)thetaX += 0.1f;

	if (y_flag == 1)thetaY += 0.1f;

	if (z_flag == 1)thetaZ += 0.1f;
}