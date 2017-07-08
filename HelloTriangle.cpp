
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

#pragma comment(lib, "glfw3.lib")

GLfloat vertexs[] = {
	0.0f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f, 
	0.5f, -0.5f, 0.0f
};

GLuint shaderProgram, VAO;

void Initialize();

int main()
{
	// 这个变量在之后会用到
	GLFWwindow* window;
	glfwInit();
	
	// 这三行代码表示我们是在写OpenGL3.3内核模式的代码

	// 指定OpenGL主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// 以及次版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// 指定使用内核模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "Learn OpenGL", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to glfw CreateWindow" << std::endl;
		glfwTerminate();
		getchar();
		exit(1);
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		getchar();
		exit(1);
	}

	Initialize();

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		//glUseProgram(0);
		//glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void Initialize()
{
	GLuint VBO, vertexShader, fragmentShader;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

	{
		char buf[256];
		const char* temp;
		std::ostringstream ss;
		std::ifstream is("vertexShader.glsl", std::ios::in);
		std::string str;

		if (!is.is_open())
		{
			std::cout << "Failed to Open File!" << std::endl;
			std::cin.get();
			glfwTerminate();
			exit(1);
		}

		while (!is.eof())
		{
			is.getline(buf, 256);
			ss << buf << '\n';
		}
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		str = ss.str();
		temp = str.c_str();
		glShaderSource(vertexShader, 1, &temp, nullptr);
		glCompileShader(vertexShader);

		int success;
		char infolog[512];
		
		// 检测是否编译错误
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		
		if (!success)
		{
			// 获取错误信息
			glGetShaderInfoLog(vertexShader, 512, nullptr, infolog);
			
			// 并显示在屏幕上或输出到调试信息
			std::cerr << "Vertex Shader Compile Fail, Error Info Log: \n" << infolog << std::endl;

			glfwTerminate();
			std::cin.get();
			exit(1);
		}
	}

	{
		char buf[256];
		const char* temp;
		std::ostringstream ss;
		std::ifstream is("fragmentShader.glsl", std::ios::in);
		std::string str;

		if (!is.is_open())
		{
			std::cout << "Failed to Open File!" << std::endl;
			std::cin.get();
			glfwTerminate();
			exit(1);
		}

		while (!is.eof())
		{
			is.getline(buf, 256);
			ss << buf << '\n';
		}
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		str = ss.str();
		temp = str.c_str();
		glShaderSource(fragmentShader, 1, &temp, nullptr);
		glCompileShader(fragmentShader);

		int success;
		char infolog[512];

		// 检测是否编译错误
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			// 获取错误信息
			glGetShaderInfoLog(fragmentShader, 512, nullptr, infolog);

			// 并显示在屏幕上或输出到调试信息
			std::cerr << "Fragment Shader Compile Fail, Error Info Log: \n" << infolog << std::endl;

			glfwTerminate();
			std::cin.get();
			exit(1);
		}
	}

	shaderProgram = glCreateProgram();
	
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	{
		int success;
		char infolog[512];

		// 注意这里是glGetProgramiv和GL_LINK_STATUS
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

		if (!success)
		{
			// 获取错误信息，注意是glGetProgramInfoLog
			glGetProgramInfoLog(shaderProgram, 512, nullptr, infolog);

			// 并显示在屏幕上或输出到调试信息
			std::cerr << "Shader Program Link Fail, Error Info Log: \n" << infolog << std::endl;

			glfwTerminate();
			std::cin.get();
			exit(1);
		}
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
