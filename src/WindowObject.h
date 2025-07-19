#pragma once

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Mesh.h"
#include "Shader.h"
#include "BoundingBox.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glfw/glfw3.h>

//Following enums used to aid in simplifying transformations

enum Transformation 
{
	Rotate,
	Scale,
	Translate,
	NoTrans,
};

enum Axis
{
	X,
	Y,
	Z,
	NoAxis,
};

enum Sign
{
	Negative,
	Positive
};

// Class used to encapsulate all possible operations on objects viewable in the scene.
class WindowObject
{
public:
	WindowObject();

	virtual void draw(Shader& shader) = 0;
	virtual void drawBoundingBox(Shader& shader);

	virtual void rotateX(float degrees);
	virtual void rotateY(float degrees);
	virtual void rotateZ(float degrees);

	virtual void scale(float scaleX, float scaleY, float scaleZ);
	virtual void scale(glm::vec3 scale);
	virtual void scale(float scale);

	virtual void translate(float x, float y, float z);
	virtual void translate(glm::vec3 pos);

	virtual void setVisible(bool val);
	virtual void setSelected(bool val);
	virtual void setTransformation(Transformation trans);
	virtual void setAxis(Axis axis);
	virtual void handleTransformation(Sign sign);

	virtual glm::vec3 getPosition();
	virtual void getRayEntryExitDistancesBoundingBox(float* maxEntry, float* minExit, glm::vec3& rayOrigin, glm::vec3& rayDirection);
	virtual glm::mat4 getModel();

	virtual void remove();

protected:
	VAO vao;
	VBO vbo;
	EBO ebo;

	glm::mat4 scaleMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 translationMatrix;

	BoundingBox boundingBox;
	bool visible;
	bool selected;

	Transformation currentTransformation;
	Axis currentAxis;

	virtual void initialiseBounds() = 0;
};

