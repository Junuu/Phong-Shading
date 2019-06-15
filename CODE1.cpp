/*
Ctrl+k , Ctrl+c ��ü �ּ�ó��
Ctrl+k , Ctrl+u ��ü �ּ�����
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

//glew�� glfw���� ���� include ���� ������ 
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))


char count[100];
char bind[100];

int vertexNum = 0;
int faceNum = 0;
int vertIndex = 0;
int faceIndex = 0;
int position_index = 0;
int normal_index = 0;

int space_count = 0; //�⺻�� = ����

int x_flag = 1; //�⺻�� x�� ȸ��
int y_flag = 0;
int z_flag = 0;

float thetaX = 0.0; //���ü�� Xȸ���� ���� ����
float thetaY = 0.0; //���ü�� Yȸ���� ���� ����
float thetaZ = 0.0; //���ü�� Zȸ���� ���� ����


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
	std::cout << "���� �̸��� �Է��ϼ���:" << "\n";
	std::cin >> file_name;
	std::cout << file_name << "�� �д��� �Դϴ�."<<"\n";
	
	
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

	std::cout << "���� ��:"<<vertexNum << "\n";
	std::cout << "���� ��:"<<faceNum << "\n";
	

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
			
			//����ȭ�� ���� �ִ밪 ���ϱ�
			if (vertex[vertIndex].x > maxX)	maxX = vertex[vertIndex].x;
			if (vertex[vertIndex].y > maxY)	maxY = vertex[vertIndex].y;
			if (vertex[vertIndex].z > maxZ)	maxZ = vertex[vertIndex].z;

				
			//����ȭ�� ���� �ּҰ� ���ϱ�
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
		//positions �迭�� ������ ���� ������ ������
		// ������ �� min-max ����ȭ�� ����(������ object�� ������ ũ��� ����� ����)
		positions[position_index] = (vertex[i].x - minX) / (maxX - minX);
		position_index = position_index + 1;

		positions[position_index] = (vertex[i].y - minY) / (maxY - minY);
		position_index = position_index + 1;

		positions[position_index] = (vertex[i].z - minZ) / (maxZ - minZ);
		position_index = position_index + 1;

	}
	
	
	for (int i = 0; i < faceNum; i++)
	{
		//indices �迭�� �ε����� ���� ������ ������
		// -1�� �ϴ� ������ .obj���Ͽ��� 0���̾ƴ� 1���� �����ϱ⶧��
		indices[indices_index] = face[i].x - 1;		
		indices_index = indices_index + 1;

		indices[indices_index] = face[i].y - 1;		
		indices_index = indices_index + 1;

		indices[indices_index] = face[i].z - 1;		
		indices_index = indices_index + 1;
		
	}
	
	for (int i = 0; i < faceNum*3;)
	{
		//normal vertor ���ϱ�	
		glm::vec3 n;		//+ normal ����
		glm::vec3 p, q;
		
		

		p = glm::vec3(positions[indices[i + 1] * 3] - positions[indices[i] * 3], positions[(indices[i + 1] * 3) + 1] - positions[(indices[i] * 3) + 1], positions[(indices[i + 1] * 3) + 2] - positions[(indices[i] * 3) + 2]);	//+ ba ������ xyz
		q = glm::vec3(positions[indices[i + 2] * 3] - positions[indices[i] * 3], positions[(indices[i + 2] * 3) + 1] - positions[(indices[i] * 3) + 1], positions[(indices[i + 2] * 3) + 2] - positions[(indices[i] * 3) + 2]);	//+ ca ������ xyz
		
		n = cross(p, q);	//+ �ﰢ���� �� ���͸� cross product �ϸ� �ﰢ���� ������ ���͸� ���� �� �ִ�
		n = normalize(n);	//+ �ﰢ���� ������ ���͸� normalization
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
	std::cout  << "�븻���\n";
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
	GLFWwindow* window; //glfw window �� �����ϱ����� �����͸� �켱 �����ߴ�.
	if (!glfwInit()) // glfw ���̺귯�� �ʱ�ȭ
		return -1;

	
	

	window = glfwCreateWindow(512, 512, "Phong Shading", NULL, NULL);	// window ����

	
	if (!window)	// window ���� ���� �� ���α׷� ����
	{
		glfwTerminate();
		return -1;
	}
	

	glfwMakeContextCurrent(window); // window context ����
	// �Ű������� ���� window�� ��ȭ���� ��ڴٴ� �Լ� ���� window1, window2�� 2���� window�� �����Ѵٸ�� ���� �̻��� �����쿡 �׸��� �׸� �� �ֽ��ϴ�.
	if (glewInit() != GLEW_OK) std::cout << "Error\n";	// glew ���̺귯�� �ʱ�ȭ �� �ʱ�ȭ ���� �� ���� �޼���
														// glew ���̺귯���� �ݵ�� window context ���� �� �ʱ�ȭ �ƴϸ� ����

   glfwSetKeyCallback(window, gasketSetKeyCallback);
   glfwSetMouseButtonCallback(window, gasketSetMouseButtonCallback);	

	GLuint va;	// vertex array ����, �޸� �Ҵ�, ���ε�
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);

	

	GLuint vb;	// vertex buffer ����, �޸� �Ҵ�, ���ε�, data�� ������ ���Ŀ� �°� buffer data �غ�
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
//	glBufferData(GL_ARRAY_BUFFER, vertexNum*3*sizeof(float), positions, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vertexNum * 3 * sizeof(float)+ sizeof(float) * faceNum * 3 * 3, NULL, GL_STATIC_DRAW);	// vertex�� position�� ���� ������ color�� ���� ������ ������ �� �ִ� ũ���� ���� �غ�
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexNum * 3 * sizeof(float), positions);	// ������ ó��(0)���� sizeof(positions) ũ������� positions �迭�� ������ ����
	glBufferSubData(GL_ARRAY_BUFFER, vertexNum * 3 * sizeof(float), sizeof(float) * faceNum * 3 * 3, normals);	// ������ sizeof(positions)���� sizeof(colors) ũ������� colors �迭�� ������ ����	
	
	GLuint ib;	// index buffer, ����, �޸� �Ҵ�, ���ε�, data�� ������ ���Ŀ� �°� buffer data �غ�
	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceNum*3*sizeof(float), indices, GL_STATIC_DRAW);
	

	GLuint program = InitShader("vshader_as4.glsl", "fshader_as4.glsl");	// shader program ��������
	glUseProgram(program);	// � shader program�� ����� ������

	GLuint location = glGetAttribLocation(program, "vPosition");	// position vertex�� ���� ������ shader program ���� � ������ �����ų ���ΰ�
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormals = glGetAttribLocation(program, "vNormals");
	glEnableVertexAttribArray(vNormals);
	glVertexAttribPointer(vNormals, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(faceNum * 3 * sizeof(float)));	// vb�� sizeof(positions) ��ġ�������� color�� ���� ������ ������


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

// window â�� ����� ������ ���ѷ��� 
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		if (space_count == 1)
		{			
			rotation = RotateX(thetaX)*RotateY(thetaY)*RotateZ(thetaZ); //math.h �ȿ� �Լ��� ��� �����Ѵ�.
			GLuint uRotate = glGetUniformLocation(program, "uRotate");
			glUniformMatrix4fv(uRotate, 1, GL_FALSE, rotation);
			//v�� cpu���� �����ַ��� �ϴ� ���� 1�� shader������ matrix 1���� ���ٴ� �ǹ�?, GL_FLASE�� ����� tranpose ����			
			idle();
		}
		time += 0.002f;
		glDrawElements(GL_TRIANGLES, faceNum * 3, GL_UNSIGNED_INT, NULL);
		glfwSwapBuffers(window);	// front buffer�� back buffer ��ü
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
		if (space_count == 0) space_count = 1; //1�̸� ȸ��
		else space_count = 0; //0�̸� ����
	}
}

void gasketSetMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		std::cout << "LEFT BUTTON is Pressed\n";
		
		if (x_flag == 1)
		{
			std::cout << "ȸ���� ���� \n";
			x_flag = 0;
			y_flag = 1;
		}
		else if (y_flag == 1)
		{
			std::cout << "ȸ���� ���� \n";
			y_flag = 0;
			z_flag = 1;
		}
		else if (z_flag == 1)
		{
			std::cout << "ȸ���� ���� \n";
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