#pragma once

#include "Math.h"
#include "Sphere.h"
#include "Rock.h"

const std::tuple<float, float> SPAWN_DISTANCE(750.0f, 1500.0f);
const std::tuple<float, float> ASTEROID_SCALE(1.0f, 50.0f);
const std::tuple<int, int> ASTEROID_COUNT(1000, 1500);
const float ASTEROID_SPEED(30.0f);

class Planet : public Sphere
{
	using Sphere::Sphere;

public:
	std::vector<Rock*> asteroids;

	void Update(float deltaTime) override;

	void GenerateAsteroidBelt(std::vector<Model*> _models, Shader* _shader);
};