
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}    

	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	
	float vertices[] = {
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

	};  

	unsigned int indices[] = {
		0,1,2,
		1,2,3,
		0,2,4,
		2,4,6,
		2,3,6,
		3,7,6,
		1,3,5,
		3,5,7,
		1,5,0,
		5,4,0,
		7,5,4,
		7,6,4

	};
	
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	Shader shader = Shader("vertexShader.glsl", "fragmentShader.glsl");

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//float time = glfwGetTime();
	//float red = (sin(time) / 2.0f) + 0.5;
	//int ourColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	unsigned int tranformLoc = glGetUniformLocation(shader.ID, "transform");


	while (!glfwWindowShouldClose(window)) 
	{
		processInput(window);

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		glm::mat4 projectionTranform = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);

		glm::mat4 viewTranform = glm::mat4(1.0f);
		viewTranform = glm::translate(viewTranform, glm::vec3(0.0f, 0.0f, -3.0f));

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::mat4 modelTransform = glm::mat4(1.0f);
		modelTransform = glm::rotate(modelTransform, (float)glfwGetTime(), glm::vec3(0.0, 1.0, 0.0));
		modelTransform = glm::scale(modelTransform, glm::vec3(0.5, 0.5, 0.5));

		shader.use();
		shader.setMat4("model", modelTransform);
		shader.setMat4("view", viewTranform);
		shader.setMat4("projection", projectionTranform);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}