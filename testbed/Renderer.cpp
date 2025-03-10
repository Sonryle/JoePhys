#include "Renderer.hpp"
#include "Colour.hpp"

// cstdio for "stderr" file path constant
#include <cstdio>

// constants for PI
const double PI = 3.14159265359;

Camera camera;
Renderer renderer;

// This function creates an openGL shader, compiles it using the source code given and then returns a handle to the shader
static GLuint createShaderFromString(const char* source, GLenum type)
{
	// create shader, link source code, and compile shader
	GLuint handle = glCreateShader(type);
	glShaderSource(handle, 1, &source, NULL);
	glCompileShader(handle);

	// check for errors during compilation and output error message if something went wrong
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

		// return -1 if there was an error during compilation
		return -1;
	}

	// return the handle to the window
	return handle;
}

// This function creates two openGL shaders using the source code provided, then creates and links them into an openGL shader program,
// and returns a handle to the shader program
static GLuint createShaderProgramFromString(const char* vertex_source, const char* fragment_source)
{
	// Create shaders using source code provided
	GLuint vertex_shader = createShaderFromString(vertex_source, GL_VERTEX_SHADER);
	GLuint fragment_shader = createShaderFromString(fragment_source, GL_FRAGMENT_SHADER);

	// If either shader did not compile correctly, return -1 and print and error message
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

	// Create shader program, attach shaders and then link the shader program into the executable shader pipeline
	GLuint handle = glCreateProgram();
	glAttachShader(handle, vertex_shader);
	glAttachShader(handle, fragment_shader);
	glBindFragDataLocation(handle, 0, "colour");
	glLinkProgram(handle);

	// Delete shaders (they have been safely copied into the shader program and so we dont need them anymore)
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// check for any errors during linking
	GLint success;
	char error_message[512];
	glGetProgramiv(handle, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(handle, 512, nullptr, error_message);
		glDeleteProgram(handle);
		fprintf(stderr, "ERROR::SHADER_PROGRAM::ERROR_WHILE_LINKING\n%s\n", error_message);
		
		// if there was an error while linking, return -1
		return -1;
	}
	
	// return the handle to the shader program
	return handle;
}

// A struct which will make the process of rendering lines far easier. All you need to do is call "Create()" and
// after that, you can add any vertices to the vertices array by using "AddVertice()". (openGL loops through the
// vertices and draws lines between each pair of vertices which it finds) In order to render the lines in the
// vertices array, all you need to do is call "Flush()" and the vertices array will be cleared and all the lines
// will be drawn to the screen.
struct GLRenderLines
{
	// Create shaders, shader program and set up VAO and VBOs
	void Create()
	{
		// Source code for vertex & fragment shader
		const char* vs = {
			"#version 330 core\n"
			"layout(location = 0) in vec2 v_position;\n"
			"layout(location = 1) in vec4 v_colour;\n"
			"out vec4 f_colour;\n"
			"uniform mat4 projection_matrix;\n"

			"void main()\n"
			"{\n"
				"f_colour = v_colour;"
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
		// Set the position and colour attribute locations to be 0 and 1
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

	// deletes all resources
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

	// add a vertice to the 'vertices' array
	void AddVertice(const vec2 v, const colour c)
	{
		// if vertices array is full, render scene, clear vertices
		// array and add vertices to the empty vertices array
		if (vertice_count >= MAX_VERTICE_COUNT)
			Flush();

		vertices[vertice_count] = v;
		colours[vertice_count] = c;
		vertice_count++;
	}

	void Flush()
	{
		// If there is nothing to render, return
		if (vertice_count == 0)
			return;

		// use our shader program
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
		glDrawArrays(GL_LINES, 0, vertice_count);
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

// The exact same thing as GLRenderLines, but for triangles. The only difference is the shader source code (vs and fs, defined in Create())
struct GLRenderTriangles
{
	void Create()
	{
		// Set up shader source code (this is the only difference between GLRenderTriangles and GLRenderLines)
		const char* vs = {
			"#version 330 core\n"
			"layout(location = 0) in vec2 v_position;\n"
			"layout(location = 1) in vec4 v_colour;\n"
			"out vec4 f_colour;\n"
			"uniform mat4 projection_matrix;\n"

			"void main()\n"
			"{\n"
				"f_colour = v_colour;"
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
		if (vertice_count >= MAX_VERTICE_COUNT)
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

// Constructor
Renderer::Renderer()
{
	lines = nullptr;
	triangles = nullptr;
}

// Destructor
Renderer::~Renderer()
{
	if (lines != nullptr || triangles != nullptr)
		Destroy();
}

// Creates new GLRenderLines and GLRenderTriangles structs
void Renderer::Create()
{
	lines = new GLRenderLines;
	lines->Create();
	triangles = new GLRenderTriangles;
	triangles->Create();
	
	return;
}

// Calls "Destroy()" on GLRenderLines and GLRenderTriangles
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

// This function can be used to add a triangle to the "vertices" array in GLDrawTriangles.
void Renderer::AddTriangle(const vec2 p1, const vec2 p2, const vec2 p3, const colour col)
{
	// if there is not enough space for a new triangle in the "vertices" array in GLDrawTriangles, Flush.
	if (triangles->vertice_count + 3 >= triangles->MAX_VERTICE_COUNT)
		Flush();
	
	// Add triangle
	triangles->AddVertice(p1, col);
	triangles->AddVertice(p2, col);
	triangles->AddVertice(p3, col);
}

// This function can be used to add a line to the "vertices" array in GLDrawLines.
void Renderer::AddLine(const vec2 p1, const vec2 p2, const colour col)
{
	// If tehre is not enough space for a new line in the "vertices" array in GLDrawLines, Flush.
	if (lines->vertice_count + 2 >= lines->MAX_VERTICE_COUNT)
		Flush();

	// Add line
	lines->AddVertice(p1, col);
	lines->AddVertice(p2, col);
}

// This will draw a circle out of lines. It will not be filled in.
void Renderer::AddCircle(const vec2 position, const float radius, const int segments, const colour col)
{
	// rotation increment between points in circle
	const real rotation_increment = 2.0f * (float)PI / segments;

	// sin & cos of rotation increment
	const real sin_increment = sinf(rotation_increment);
	const real cos_increment = cosf(rotation_increment);

	// vector which will store the current rotation
	vec2 rotation(1.0f, 0.0f);
	// vector which stores the position of first point
	vec2 point_pos(position + radius * rotation);

	// loop for the amount of segments that there are
	for (int i = 0; i < segments; i++)
	{
		// this will be our new rotation
		vec2 new_rotation;
		
		// find the new rotation
		new_rotation.x = cos_increment * rotation.x - sin_increment * rotation.y;
		new_rotation.y = sin_increment * rotation.x + cos_increment * rotation.y;

		// get the position of the new point
		vec2 new_point_pos = position + radius * new_rotation;

		// draw a line connecting the two points
		AddLine(point_pos, new_point_pos, col);

		// set the old point & rotation to the most recent
		rotation = new_rotation;
		point_pos = new_point_pos;
	}
}

// This will draw a circle out of triangles & outline it with lines
void Renderer::AddSolidCircle(const vec2 position, const float radius, const int segments, const colour fill_col, const colour outline_col)
{
	// rotation increment between points in circle
	const real rotation_increment = 2.0f * (float)PI / segments;

	// sin & cos of rotation increment
	const real sin_increment = sinf(rotation_increment);
	const real cos_increment = cosf(rotation_increment);

	// vector which will store the current rotation
	vec2 rotation(1.0f, 0.0f);
	// vector which stores the position of first point
	vec2 point_pos(position + radius * rotation);

	// loop for the amount of segments that there are
	for (int i = 0; i < segments; i++)
	{
		// this will be our new rotation
		vec2 new_rotation;
		
		// find the new rotation
		new_rotation.x = cos_increment * rotation.x - sin_increment * rotation.y;
		new_rotation.y = sin_increment * rotation.x + cos_increment * rotation.y;

		// get the position of the new point
		vec2 new_point_pos = position + radius * new_rotation;

		// draw a line connecting the two points
		AddLine(point_pos, new_point_pos, outline_col);

		// draw a triangle connecting the center to the two points;
		AddTriangle(point_pos, new_point_pos, position, fill_col);

		// set the old point & rotation to the most recent
		rotation = new_rotation;
		point_pos = new_point_pos;
	}
}

// Creates a 8x2 grid of triangles each with the corresponding colour of the colour palette
void Renderer::AddColourTest()
{
	// TOP ROW OF TRIANGLES
	// --------------------
	
	// tri 1
	vec2 tt1p1( -390,  0 );
	vec2 tt1p2( -310,  0 );
	vec2 tt1p3( -350, 65 );
	colour tt1col(palette.colours[Palette::JP_DARK_GRAY]);
	// tri 2
	vec2 tt2p1( -290,  0 );
	vec2 tt2p2( -210,  0 );
	vec2 tt2p3( -250, 65 );
	colour tt2col(palette.colours[Palette::JP_DARK_RED]);
	// tri 3
	vec2 tt3p1( -190,   0 );
	vec2 tt3p2( -110,   0 );
	vec2 tt3p3( -150,  65 );
	colour tt3col(palette.colours[Palette::JP_DARK_GREEN]);
	// tri 4
	vec2 tt4p1( -90,   0 );
	vec2 tt4p2( -10,   0 );
	vec2 tt4p3( -50,  65 );
	colour tt4col(palette.colours[Palette::JP_DARK_YELLOW]);
	// tri 5
	vec2 tt5p1(  10,   0 );
	vec2 tt5p2(  90,   0 );
	vec2 tt5p3(  50,   65 );
	colour tt5col(palette.colours[Palette::JP_DARK_BLUE]);
	// tri 6
	vec2 tt6p1( 110,   0 );
	vec2 tt6p2( 190,   0 );
	vec2 tt6p3( 150,  65 );
	colour tt6col(palette.colours[Palette::JP_DARK_PURPLE]);
	// tri 7
	vec2 tt7p1( 210,   0 );
	vec2 tt7p2( 290,   0 );
	vec2 tt7p3( 250,  65 );
	colour tt7col(palette.colours[Palette::JP_DARK_AQUA]);
	// tri 8
	vec2 tt8p1( 310,   0 );
	vec2 tt8p2( 390,   0 );
	vec2 tt8p3( 350,  65 );
	colour tt8col(palette.colours[Palette::JP_DARK_WHITE]);

	// BOTTOM ROW OF TRIANGLES
	// --------------------
	
	// tri 1
	vec2 bt1p1( -390, 0 );
	vec2 bt1p2( -310, 0 );
	vec2 bt1p3( -350,  -35 );
	colour bt1col(palette.colours[Palette::JP_GRAY]);
	// tri 2
	vec2 bt2p1( -290, 0 );
	vec2 bt2p2( -210, 0 );
	vec2 bt2p3( -250,  -35 );
	colour bt2col(palette.colours[Palette::JP_RED]);
	// tri 3
	vec2 bt3p1( -190, 0 );
	vec2 bt3p2( -110, 0 );
	vec2 bt3p3( -150, -35 );
	colour bt3col(palette.colours[Palette::JP_GREEN]);
	// tri 4
	vec2 bt4p1(  -90, 0 );
	vec2 bt4p2(  -10, 0 );
	vec2 bt4p3(  -50, -35 );
	colour bt4col(palette.colours[Palette::JP_YELLOW]);
	// tri 5
	vec2 bt5p1(  10, 0 );
	vec2 bt5p2(  90, 0 );
	vec2 bt5p3(  50, -35 );
	colour bt5col(palette.colours[Palette::JP_BLUE]);
	// tri 6
	vec2 bt6p1( 110, 0 );
	vec2 bt6p2( 190, 0 );
	vec2 bt6p3( 150, -35 );
	colour bt6col(palette.colours[Palette::JP_PURPLE]);
	// tri 7
	vec2 bt7p1( 210, 0 );
	vec2 bt7p2( 290, 0 );
	vec2 bt7p3( 250, -35 );
	colour bt7col(palette.colours[Palette::JP_AQUA]);
	// tri 8
	vec2 bt8p1( 310, 0 );
	vec2 bt8p2( 390, 0 );
	vec2 bt8p3( 350, -35 );
	colour bt8col(palette.colours[Palette::JP_WHITE]);

	// RENDER TOP TRIANGLES
	AddTriangle(tt1p1, tt1p2, tt1p3, tt1col);
	AddTriangle(tt2p1, tt2p2, tt2p3, tt2col);
	AddTriangle(tt3p1, tt3p2, tt3p3, tt3col);
	AddTriangle(tt4p1, tt4p2, tt4p3, tt4col);
	AddTriangle(tt5p1, tt5p2, tt5p3, tt5col);
	AddTriangle(tt6p1, tt6p2, tt6p3, tt6col);
	AddTriangle(tt7p1, tt7p2, tt7p3, tt7col);
	AddTriangle(tt8p1, tt8p2, tt8p3, tt8col);
	// RENDER BOTTOM TRIANGLES
	AddTriangle(bt1p1, bt1p2, bt1p3, bt1col);
	AddTriangle(bt2p1, bt2p2, bt2p3, bt2col);
	AddTriangle(bt3p1, bt3p2, bt3p3, bt3col);
	AddTriangle(bt4p1, bt4p2, bt4p3, bt4col);
	AddTriangle(bt5p1, bt5p2, bt5p3, bt5col);
	AddTriangle(bt6p1, bt6p2, bt6p3, bt6col);
	AddTriangle(bt7p1, bt7p2, bt7p3, bt7col);
	AddTriangle(bt8p1, bt8p2, bt8p3, bt8col);
}

// Calls Flush on GLDrawTriangles struct & GLDrawLines struct.
void Renderer::Flush()
{
	triangles->Flush();
	lines->Flush();
}

// Generates projection matrix which will be applied to all our points in the vertex shaders before rendering
void Camera::GenerateProjectionMatrix(float p[16])
{
	// set right, left, top, bottom, far and near so that 0,0 is in the center of the screen
	real right = (float)window_width / 2;
	real left = (float)window_width / -2;
	real top = (float)window_height / 2;
	real bottom = (float)window_height / -2;
	real far = 1;
	real near = -1;

	// Multiply corners of the screen by camera zoom
	right *= zoom;
	left *= zoom;
	top *= zoom;
	bottom *= zoom;

	// Offset corners of the screen by the camera position (the camera's center)
	right += center.x;
	left += center.x;
	top += center.y;
	bottom += center.y;

	// Generate Projection Matrix

	// Row 1
	p[0] = 2.0f / (right - left);
	p[1] = 0.0f;
	p[2] = 0.0f;
	p[3] = 0.0f;

	// Row 3
	p[4] = 0.0f;
	p[5] = 2.0f / (top - bottom);
	p[6] = 0.0f;
	p[7] = 0.0f;

	// Row 2
	p[8] = 0.0f;
	p[9] = 0.0f;
	p[10] = -2.0f / (far - near);
	p[11] = 0.0f;

	// Row 4
	p[12] = -( (right + left) / (right - left) );
	p[13] = -( (top + bottom) / (top - bottom) );
	p[14] = -( (far + near) / (far - near) );
	p[15] = 1.0f;
}
