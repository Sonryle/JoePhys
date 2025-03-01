#include "Renderer.hpp"

#include "Colours.hpp"

Camera camera;
Renderer renderer;

static GLuint createShaderFromString(const char* source, GLenum type)
{
	GLuint handle = glCreateShader(type);
	glShaderSource(handle, 1, &source, NULL);
	glCompileShader(handle);

	GLint success;
	char error_message[512];
	glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(handle, 512, nullptr, error_message);
		glDeleteShader(handle);

		if (type == GL_VERTEX_SHADER)
			fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", error_message);
		if (type == GL_FRAGMENT_SHADER)
			fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", error_message);

		return -1;
	}

	return handle;
}

static GLuint createShaderProgramFromString(const char* vertex_source, const char* fragment_source)
{
	GLuint vertex_shader = createShaderFromString(vertex_source, GL_VERTEX_SHADER);
	GLuint fragment_shader = createShaderFromString(fragment_source, GL_FRAGMENT_SHADER);
	if (vertex_shader == -1)
	{
		fprintf(stderr, "ERROR::SHADER_PROGRAM::PROGRAM COULD NOT BE USED BECAUSE OF ERRORS IN VERTEX SHADER");
		return -1;
	}
	if (fragment_shader == -1)
	{
		fprintf(stderr, "ERROR::SHADER_PROGRAM::PROGRAM COULD NOT BE USED BECAUSE OF ERRORS IN FRAGMENT SHADER");
		return -1;
	}

	GLuint handle = glCreateProgram();
	glAttachShader(handle, vertex_shader);
	glAttachShader(handle, fragment_shader);
	glBindFragDataLocation(handle, 0, "colour");
	glLinkProgram(handle);

	// Shaders have been safely copied into the shader program
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	GLint success;
	char error_message[512];
	glGetProgramiv(handle, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(handle, 512, nullptr, error_message);
		glDeleteProgram(handle);
		fprintf(stderr, "hi");

		return -1;
	}

	return handle;
}

struct GLRenderLines
{
	void Create()
	{
		
	}

	void Destroy()
	{

	}
};

struct GLRenderTriangles
{
	void Create()
	{
		const char* vs = {
			"#version 330 core\n"
			"layout(location = 0) in vec2 v_position;\n"
			"layout(location = 1) in vec4 v_colour;\n"
			"out vec4 f_colour;\n"
			"uniform mat4 projection_matrix;\n"

			"void main()\n"
			"{\n"
				"f_colour = v_colour;"
				/* "gl_Position = vec4(v_position, 0.0f, 1.0f);\n" */
				"gl_Position = projection_matrix * vec4(v_position, 0.0f, 1.0f);\n"
			"}\n"
		};
		const char* fs = {
			"#version 330 core\n"
			"in vec4 f_colour;\n"
			"out vec4 colour;\n"

			"void main()\n"
			"{\n"
				"colour = f_colour;\n"
			"}\n"
		};
		

		// Set up shaders
		program_handle = createShaderProgramFromString(vs, fs);
		projection_uniform = glGetUniformLocation(program_handle, "projection_matrix");
		position_attribute = 0;
		colour_attribute = 1;

		// VAO
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// VBOs
		glGenBuffers(2, vbos);

		// position VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(position_attribute);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

		// colour VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
		glVertexAttribPointer(colour_attribute, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(colour_attribute);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_DYNAMIC_DRAW);

		// Unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		vertice_count = 0;
	}

	void Destroy()
	{
		if (vao)
		{
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(2, vbos);
			vao = 0;
		}

		if (program_handle)
		{
			glDeleteProgram(program_handle);
			program_handle = 0;
		}
	}

	void AddVertice(const vec2 v, const colour c)
	{
		if (vertice_count == MAX_VERTICE_COUNT)
			Flush();

		vertices[vertice_count] = v;
		colours[vertice_count] = c;
		vertice_count++;
	}

	void Flush()
	{
		if (vertice_count == 0)
			return;

		glUseProgram(program_handle);

		// Get Projection Matrix from Camera
		GLfloat proj_mat[16];
		camera.GenerateProjectionMatrix(proj_mat);
		glUniformMatrix4fv(projection_uniform, 1, 0, proj_mat);

		// Bind VAO
		glBindVertexArray(vao);

		// Bind VBOs and attach our vertex position and colour data to them
		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertice_count * sizeof(vec2), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertice_count * sizeof(colour), colours);

		// Render Scene
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDrawArrays(GL_TRIANGLES, 0, vertice_count);
		glDisable(GL_BLEND);

		// Unbind our VAO, VBOs & Shader Program
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		// reset vertice count to 0
		// we don't have to delete the old vertice data in 'vertices'
		// because glDrawArrays() will only draw up to 'vertice_count'
		// number of vertices. So the old vertices won't be used at all.
		vertice_count = 0;

	}

	// shaders & shader program
	GLuint program_handle;
	GLuint projection_uniform;
	GLuint position_attribute;
	GLuint colour_attribute;

	// VAO & VBOs (vbo[0] for position & vbo[1] for colour)
	GLuint vao;
	GLuint vbos[2];

	// Vertex data
	static const int MAX_VERTICE_COUNT = 2048;
	vec2 vertices[MAX_VERTICE_COUNT];
	colour colours[MAX_VERTICE_COUNT];
	int vertice_count;
};

void Renderer::Create()
{
	triangles = new GLRenderTriangles;
	triangles->Create();
	
	return;
}

void Renderer::Destroy()
{
	if (lines != nullptr)
	{
		lines->Destroy();
		delete lines;
		lines = nullptr;
	}

	if (triangles != nullptr)
	{
		triangles->Destroy();
		delete triangles;
		triangles = nullptr;
	}
}

void Renderer::RenderDefaultTriangle()
{
	colour col(1.0f, 0.0f, 1.0f, 1.0f);
	vec2 p1(-0.5f, -0.5f);
	vec2 p2( 0.5f, -0.5f);
	vec2 p3( 0.0f,  0.5f);

	triangles->AddVertice(p1, col);
	triangles->AddVertice(p2, col);
	triangles->AddVertice(p3, col);
}

void Renderer::Flush()
{
	triangles->Flush();
}

void Camera::GenerateProjectionMatrix(float p[16])
{
	// bottom and left will be -top and -right
	const real top = 500;
	const real right = 500;

	const real near = 1;
	const real far = 1;

	/* // Row 1 */
	/* p[0] = 1.0f / right; */
	/* p[1] = 0.0f; */
	/* p[2] = 0.0f; */
	/* p[3] = 0.0f; */

	/* // Row 3 */
	/* p[4] = 0.0f; */
	/* p[5] = 1.0f / top; */
	/* p[6] = 0.0f; */
	/* p[7] = 0.0f; */

	/* // Row 2 */
	/* p[8] = 0.0f; */
	/* p[9] = 0.0f; */
	/* p[10] = -2.0f / (far - near); */
	/* p[11] = 0.0f; */

	/* // Row 4 */
	/* p[12] = 0.0f; */
	/* p[13] = 0.0f; */
	/* p[14] = -( (far + near) / ((far - near)) ); */
	/* p[15] = 1.0f; */

	// DOES NOTHING
	// Row 1
	p[0] = 1.0f;
	p[1] = 0.0f;
	p[2] = 0.0f;
	p[3] = 0.0f;

	// Row 3
	p[4] = 0.0f;
	p[5] = 1.0f;
	p[6] = 0.0f;
	p[7] = 0.0f;

	// Row 2
	p[8] = 0.0f;
	p[9] = 0.0f;
	p[10] = 1.0f;
	p[11] = 0.0f;

	// Row 4
	p[12] = 0.0f;
	p[13] = 0.0f;
	p[14] = 0.0f;
	p[15] = 1.0f;


}
