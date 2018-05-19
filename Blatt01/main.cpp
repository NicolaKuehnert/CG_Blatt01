#include "stdafx.h"
#include <iostream>
#include <vector>


#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "GLSLProgram.h"
#include "GLTools.h"
#include "GraphicObject.h"

// Standard window width
const int WINDOW_WIDTH  = 1200;
// Standard window height
const int WINDOW_HEIGHT = 600;
// GLUT window id/handle
int glutID = 0;
float X_VIEW=-5.0f, Y_VIEW=3.0f, Z_VIEW=4.0f;

cg::GLSLProgram program;

glm::mat4x4 view;
glm::mat4x4 projection;

float zNear = 0.1f;
float zFar  = 100.0f;


GraphicObject quad;


void renderQuad()
{
	// Create mvp.
	glm::mat4x4 mvp = projection * view * quad.model;

	// Bind the shader program and set uniform(s).
	program.use();
	program.setUniform("mvp", mvp);
	
	// Bind vertex array object so we can render the 2 triangles.
	//2 triangles have 6 vertices
	quad.renderObject(12*3);
}

void initQuad()
{
	// Construct triangles. These vectors can go out of scope after we have send all data to the graphics card.
	// Every side has two triangles á 3 vertices
	const std::vector<glm::vec3> vertices = {
		{1.0f, 1.0f, 1.0f}, //front
		{0.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 1.0f},

		{1.0f, 1.0f, 1.0f}, 
		{1.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f}, // end front

		{0.0f, 0.0f, 0.0f}, // bottom
		{1.0f, 0.0f, 1.0f}, 
		{0.0f, 0.0f, 1.0f}, 

		{0.0f, 0.0f, 0.0f}, 
		{1.0f, 0.0f, 1.0f}, 
		{1.0f, 0.0f, 0.0f}, // end bottom

		{0.0f, 0.0f, 0.0f}, // left
		{0.0f, 1.0f, 0.0f}, 
		{0.0f, 1.0f, 1.0f},

		{0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 1.0f}, 
		{0.0f, 0.0f, 1.0f}, // end left

		{1.0f, 1.0f, 0.0f}, // right
		{1.0f, 0.0f, 1.0f}, 
		{1.0f, 1.0f, 1.0f}, 

		{1.0f, 1.0f, 0.0f}, 
		{1.0f, 0.0f, 0.0f}, 
		{1.0f, 0.0f, 1.0f}, // end right

		{1.0f, 0.0f, 1.0f}, // back
		{0.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 1.0f},

		{1.0f, 0.0f, 1.0f},
		{1.0f, 1.0f, 1.0f}, 
		{0.0f, 1.0f, 1.0f}, // end back

		{0.0f, 1.0f, 0.0f}, // top
		{0.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},

		{0.0f, 1.0f, 0.0f},
		{1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 0.0f}  // end top

	};
	
	const std::vector<glm::vec3> colors = { 
		{ 0.5f, 0.0f, 0.0f }, // front
		{ 0.5f, 0.0f, 0.0f }, 
		{ 0.5f, 0.0f, 0.0f }, 

		{ 0.5f, 0.0f, 0.0f }, 
		{ 0.5f, 0.0f, 0.0f }, 
		{ 0.5f, 0.0f, 0.0f }, // end front

		{ 1.0f, 0.0f, 0.0f }, // bottom
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f }, 

		{ 1.0f, 0.0f, 0.0f }, 
		{ 1.0f, 0.0f, 0.0f }, 
		{ 1.0f, 0.0f, 0.0f }, // end bottom

		{ 0.0f, 0.5f, 0.0f }, // left
		{ 0.0f, 0.5f, 0.0f }, 
		{ 0.0f, 0.5f, 0.0f },

		{ 0.0f, 0.5f, 0.0f }, 
		{ 0.0f, 0.5f, 0.0f }, 
		{ 0.0f, 0.5f, 0.0f }, // end left

		{ 0.0f, 1.0f, 0.0f }, // right
		{ 0.0f, 1.0f, 0.0f }, 
		{ 0.0f, 1.0f, 0.0f }, 

		{ 0.0f, 1.0f, 0.0f }, 
		{ 0.0f, 1.0f, 0.0f }, 
		{ 0.0f, 1.0f, 0.0f }, // end right

		{ 0.0f, 0.0f, 1.0f }, // back
		{ 0.0f, 0.0f, 1.0f }, 
		{ 0.0f, 0.0f, 1.0f },

		{ 0.0f, 0.0f, 1.0f }, 
		{ 0.0f, 0.0f, 1.0f }, 
		{ 0.0f, 0.0f, 1.0f }, // end back

		{ 1.0f, 1.0f, 1.0f }, // top
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f }  // end top

	};

	// The indices of the sides, beginning at the front
	const std::vector<GLushort> indices = { 
		6, 7, 8, // bottom
		9, 10, 11,
		24, 25, 26, // back
		27, 28, 29,
		12, 13, 14, // left
		15, 16, 17,
		18, 19, 20, // right
		21, 22, 23,
		30, 31, 32, // top
		33, 34, 35,
		0, 1, 2, // front
		3, 4, 5
		
		};

	GLuint programId = program.getHandle();
	quad.initObject(vertices, colors, indices, programId);
}

/*
 Initialization. Should return true if everything is ok and false if something went wrong.
 */
bool init()
{
	// OpenGL: Set "background" color and enable depth testing.
	glClearColor(0.4f, 0.5f, 0.2f, 1.0f);

	// Construct view matrix.
	glm::vec3 eye(X_VIEW, Y_VIEW, Z_VIEW);
	glm::vec3 center(0.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);

	view = glm::lookAt(eye, center, up);

	// Create a shader program and set light direction.
	if (!program.compileShaderFromFile("shader/simple.vert", cg::GLSLShader::VERTEX))
	{
		std::cerr << program.log();
		return false;
	}

	if (!program.compileShaderFromFile("shader/simple.frag", cg::GLSLShader::FRAGMENT))
	{
		std::cerr << program.log();
		return false;
	}
	
	if (!program.link())
	{
		std::cerr << program.log();
		return false;
	}

	// Create objects.
	initQuad();

	return true;
}


/*
 Release resources on termination.
 */
void release()
{
	// Shader program will be released upon program termination.
	quad.deleteObject();
}

/*
 Rendering.
 */
void render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	renderQuad();
}

void glutDisplay ()
{
   GLCODE(render());
   glutSwapBuffers();
}

/*
 Resize callback.
 */
void glutResize (int width, int height)
{
	// Division by zero is bad...
	height = height < 1 ? 1 : height;
	glViewport(0, 0, width, height);

	// Construct projection matrix.
	projection = glm::perspective(45.0f, (float) width / height, zNear, zFar);
}

/*
 Callback for char input.
 */
void glutKeyboard (unsigned char keycode, int x, int y)
{
	switch (keycode)
	{
	case 27: // ESC
	  glutDestroyWindow ( glutID );
	  return;
	  
	case '+':
		Y_VIEW+=1.0f;
		init();
		renderQuad();
		break;
	case '-':
		Y_VIEW-=1.0f;
		init();
		renderQuad();
		break;
	case 'x':
		// do something
		break;
	case 'y':
		// do something
		break;
	}
	render();
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	// GLUT: Initialize freeglut library (window toolkit).
        glutInitWindowSize    (WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(40,40);
	glutInit(&argc, argv);

	// GLUT: Create a window and opengl context (version 4.3 core profile).
	glutInitContextVersion(4, 3);
	glutInitContextFlags  (GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
	glutInitDisplayMode   (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	glutCreateWindow("Computergrafik - Praktikum");
	glutID = glutGetWindow();
	  
	// GLEW: Load opengl extensions
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();

	if (result != GLEW_OK) {
	   return -1;
	}

	// GLUT: Set callbacks for events.
	glutReshapeFunc(glutResize);
	glutDisplayFunc(glutDisplay);
	//glutIdleFunc   (glutDisplay); // redisplay when idle

	glutKeyboardFunc(glutKeyboard);

	// Init VAO.
	{
		GLCODE(bool result = init());
		if (!result) {
			release();
			return -2;
		}
	}

	// GLUT: Loop until the user closes the window
	// rendering & event handling
	glutMainLoop ();

	// Clean up everything on termination.
	release();

	return 0;
}
