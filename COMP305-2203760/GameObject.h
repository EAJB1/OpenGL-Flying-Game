#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Model.h"
#include "Shader.h"

class GameObject
{
public:
	static std::vector<GameObject*> gameObjects;

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	Model* model;
	Shader* shader;

	GameObject(Model* _model, Shader* _shader)
	{
		// Show error message
		assert(_model);
		assert(_shader);

		model = _model;
		shader = _shader;

		gameObjects.push_back(this);
	}

	virtual void Update(float deltaTime);

	void Draw();

	glm::mat4 GetMatrix();

	static void Delete(GameObject* gameObject);

	static void DrawAll();

	static void DeleteAll();

private:

};