#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <vector>
#pragma once
class GraphicObject
{
	/* IDs for several buffers. */
	GLuint vao;

	GLuint positionBuffer;
	GLuint colorBuffer;

	GLuint indexBuffer;

public:
	GraphicObject();
	~GraphicObject();
	void initObject(std::vector<glm::vec3> vertices, std::vector<glm::vec3> colors, std::vector<GLushort> indices, GLuint programId);
	void renderObject(GLsizei vertices);
	void deleteObject();

	/* Model matrix */
	glm::mat4x4 model;
};

