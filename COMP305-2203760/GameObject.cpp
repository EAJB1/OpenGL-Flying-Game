#include "GameObject.h"

std::vector<GameObject*> GameObject::gameObjects;

void GameObject::Update(float deltaTime)
{
	return;
}

void GameObject::Draw()
{
	glm::mat4 matrix = GetMatrix();

	shader->setMat4("model", matrix);

	model->Draw(*shader);
}

/// <summary>
/// Returns matrix that converts from local to world space.
/// </summary>
glm::mat4 GameObject::GetMatrix()
{
	glm::mat4 matrix(1.0f);

	matrix = glm::translate(matrix, position);

	// Extrinsic rotation
	matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	matrix = glm::scale(matrix, scale);

	return matrix;
}

/// <summary>
/// Delete a single game object
/// </summary>
void GameObject::Delete(GameObject* gameObject)
{
	GameObject::gameObjects.erase(std::remove(GameObject::gameObjects.begin(), GameObject::gameObjects.end(), gameObject), GameObject::gameObjects.end());

	delete gameObject;
	gameObject = nullptr;
}

/// <summary>
/// Draw every game object.
/// </summary>
void GameObject::DrawAll()
{
	for (GameObject* g : GameObject::gameObjects)
	{
		g->Draw();
	}
}

/// <summary>
/// Delete all game objects.
/// </summary>
void GameObject::DeleteAll()
{
	// Delete object at pointer
	for (GameObject* g : gameObjects)
	{
		delete g;
		g = nullptr;
	}

	// Clear list
	GameObject::gameObjects.clear();
}