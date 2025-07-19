#include "WindowObject.h"

WindowObject::WindowObject()
	:	scaleMatrix(glm::mat4(1.0f)), rotationMatrix(glm::mat4(1.0f)), translationMatrix(glm::mat4(1.0f)),
		visible(true), selected(false), 
		currentTransformation(NoTrans), currentAxis(NoAxis)
{
	boundingBox.setMatrices(&scaleMatrix, &rotationMatrix, &translationMatrix);
}

void WindowObject::drawBoundingBox(Shader& shader)
{
	boundingBox.draw(shader);
}

void WindowObject::rotateX(float degrees)
{
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(degrees), glm::vec3(1.0f, 0.0f, 0.0f));
}

void WindowObject::rotateY(float degrees)
{
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(degrees), glm::vec3(0.0f, 1.0f, 0.0f));
}

void WindowObject::rotateZ(float degrees)
{
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(degrees), glm::vec3(0.0f, 0.0f, 1.0f));
}

void WindowObject::scale(float scaleX, float scaleY, float scaleZ)
{
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(scaleX, scaleY, scaleZ));
}

void WindowObject::scale(glm::vec3 scale)
{
	scaleMatrix = glm::scale(scaleMatrix, scale);
}

void WindowObject::scale(float scale)
{
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(scale, scale, scale));
}

void WindowObject::translate(float x, float y, float z)
{
	translationMatrix = glm::translate(translationMatrix, glm::vec3(x, y, z));
}

void WindowObject::translate(glm::vec3 position)
{
	translationMatrix = glm::translate(translationMatrix, position);
}

void WindowObject::setVisible(bool val)
{
	visible = val;
}

void WindowObject::setSelected(bool val)
{
	selected = val;
	boundingBox.setSelected(val);
}

glm::vec3 WindowObject::getPosition()
{
	return translationMatrix[3];
}

glm::mat4 WindowObject::getModel()
{
	// Separated to avoid potential issues with rotation orders
	return glm::mat4(1.0f) * translationMatrix * rotationMatrix * scaleMatrix;
}

void WindowObject::remove()
{
	vao.remove();
	vbo.remove();
	ebo.remove();
}

void alignToAxis(glm::vec3* rayOriginLocal, glm::vec3* dirfrac, const glm::mat4& model, const glm::vec3& rayOrigin, const glm::vec3& rayDirection)
{
	// Get local ray coordinates
	// Simplifies collision to be on an axis aligned bounding box
	glm::mat4 modelInverse = glm::inverse(model);
	*rayOriginLocal = glm::vec3(modelInverse * glm::vec4(rayOrigin, 1.0f));
	glm::vec3 rayDirectionLocal = glm::vec3(modelInverse * glm::vec4(rayDirection, 0.0f));
	// Avoid dividing by 0
	if (rayDirectionLocal.x == 0.0f)
		rayDirectionLocal.x += 0.001f;
	if (rayDirectionLocal.y == 0.0f)
		rayDirectionLocal.y += 0.001f;
	if (rayDirectionLocal.z == 0.0f)
		rayDirectionLocal.z += 0.001f;
	rayDirectionLocal = glm::normalize(rayDirectionLocal);
	*dirfrac = glm::vec3(1.0f) / rayDirectionLocal;
}

void getMinMax(float* maxEntry, float* minExit, const glm::vec3& minExtent, const glm::vec3 maxExtent, const glm::vec3& rayOriginLocal, const glm::vec3& dirfrac)
{
	// Calculate the intersection distances
	// Essentially calculating the distance that the ray is inside of the box
	glm::vec3 t1 = (minExtent - rayOriginLocal) * dirfrac;
	glm::vec3 t2 = (maxExtent - rayOriginLocal) * dirfrac;

	if (t1.length() > t2.length())
	{
		glm::vec3 temp = t1;
		t1 = t2;
		t2 = temp;
	}

	// Find the entry and exit distances
	glm::vec3 tmin = glm::min(t1, t2);
	glm::vec3 tmax = glm::max(t1, t2);

	// Find the maximum entry distance among the axes
	*maxEntry = glm::max(tmin.x, glm::max(tmin.y, tmin.z));
	// Find the minimum exit distance among the axes
	*minExit  = glm::min(tmax.x, glm::min(tmax.y, tmax.z));
}

// Using basic slabs algorithm described in Ray Tracing Gems 2 Book
void WindowObject::getRayEntryExitDistancesBoundingBox(float* maxEntry, float* minExit, glm::vec3& rayOrigin, glm::vec3& rayDirection)
{
	glm::vec3 rayOriginLocal, dirfrac;

	alignToAxis(&rayOriginLocal, &dirfrac, getModel(), rayOrigin, rayDirection);

	glm::vec3 minExtent = boundingBox.getMinimumVertex();
	glm::vec3 maxExtent = boundingBox.getMaximumVertex();

	getMinMax(maxEntry, minExit, minExtent, maxExtent, rayOriginLocal, dirfrac);
}


void WindowObject::setTransformation(Transformation trans)
{
	currentTransformation = trans;
}

void WindowObject::setAxis(Axis axis)
{
	currentAxis = axis;
}

void WindowObject::handleTransformation(Sign sign)
{
	// Handle scale, rotation or transformation of the object
	float scaleConst = 1.01f;
	float rotateConst = 2.0f;
	float transConst = 0.02f;

	if (sign == Negative) 
	{
		scaleConst = 1.0f - abs(1.0f - scaleConst);
		rotateConst = -rotateConst;
		transConst = -transConst;
	}

	switch (currentTransformation) {
	case Scale:
		if (currentAxis == X)
			scale(scaleConst, 1.0f, 1.0f);
		else if (currentAxis == Y)
			scale(1.0f, scaleConst, 1.0f);
		else if (currentAxis == Z)
			scale(1.0f, 1.0f, scaleConst);
		break;

	case Rotate:
		if (currentAxis == X)
			rotateX(rotateConst);
		else if (currentAxis == Y)
			rotateY(rotateConst);
		else if (currentAxis == Z)
			rotateZ(rotateConst);
		break;

	case Translate:
		if (currentAxis == X)
			translate(transConst, 0, 0);
		else if (currentAxis == Y)
			translate(0, transConst, 0);
		else if (currentAxis == Z)
			translate(0, 0, transConst);
		break;
	}	
}