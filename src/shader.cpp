#include "shader.h"

inline std::string read(
	const char* path
) 
{
	std::ifstream read(path);
	std::string line, contents = "";

	if (read.is_open()) {
		while (getline(read, line)) {
			contents += line + "\n";
		}
		return contents;
	}
	throw(errno);
}

void Shader::Create()
{
	std::string read_vertex_source = read(m_vert);
	std::string read_fragment_source = read(m_frag);

	const char* vertex_source = read_vertex_source.c_str();
	const char* fragment_source = read_fragment_source.c_str();

	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(
		m_vertexShader,
		1,
		&vertex_source,
		NULL
	);
	glCompileShader(m_vertexShader);

	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(
		m_fragmentShader,
		1,
		&fragment_source,
		NULL
	);
	glCompileShader(m_fragmentShader);


	GLint compileStatus;
	glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE) {
		// Retrieve the error log
		GLint logLength;
		glGetShaderiv(m_vertexShader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> log(logLength);
		glGetShaderInfoLog(m_vertexShader, logLength, nullptr, log.data());

		// Print the error log
		std::cerr << "Shader compilation failed:\n" << log.data() << std::endl;

		// Cleanup and return 0 to indicate failure
		glDeleteShader(m_vertexShader);
	}

	glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE) {
		GLint logLength;
		glGetShaderiv(m_fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> log(logLength);
		glGetShaderInfoLog(m_fragmentShader, logLength, nullptr, log.data());

		std::cerr << "Shader compilation failed:\n" << log.data() << std::endl;

		glDeleteShader(m_fragmentShader);
	}

	ID = glCreateProgram();

	glAttachShader(ID, m_vertexShader);
	glAttachShader(ID, m_fragmentShader);
	glLinkProgram(ID);

	GLint linkStatus;
	glGetProgramiv(ID, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) {
		GLint logLength;
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> log(logLength);
		glGetProgramInfoLog(ID, logLength, nullptr, log.data());
		std::cerr << "Shader program linking failed:\n" << log.data() << std::endl;
		return;
	}

	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);
}


void Shader::Activate()
{
	glUseProgram(this->ID);
}

void Shader::Deactivate()
{
	glDeleteProgram(this->ID);
}