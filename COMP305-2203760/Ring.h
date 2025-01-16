#pragma once

#include <tuple>

#include "Helper.h"
#include "GameObject.h"

const std::tuple<float, float> SPAWN_RADIUS(80.0f, 110.0f);

class Ring : public GameObject
{
	using GameObject::GameObject;

public:
	static std::vector<Ring*> rings;
	static Ring* currentRing;

	float radius = 1.0f;

	glm::vec3 GetNormal();

	bool Collision(glm::vec3 lastPoint, glm::vec3 currentPoint);

	static void SpawnRing(Model* _model, Shader* _shader);

	void RandomPositionInForward(glm::vec3 origin, glm::vec3 lastRotation);

	void RandomPositionRotation(glm::vec3 origin);
};