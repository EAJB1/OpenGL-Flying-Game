#include "Planet.h"

void Planet::Update(float deltaTime)
{
	bool flip = false;

	for (Rock* asteroid : asteroids)
	{
		glm::vec3 offset = asteroid->position - position;

		offset = glm::angleAxis(glm::radians(deltaTime * ASTEROID_SPEED * (flip ? -1.0f : 1.0f)) / asteroid->scale.x, worldUp) * offset;

		asteroid->position = position + offset;
		
		flip = !flip;
	}
}

void Planet::GenerateAsteroidBelt(std::vector<Model*> _models, Shader* _shader)
{
	int count = RandomInt(ASTEROID_COUNT);

	for (int i = 0; i < count; i++)
	{
		glm::vec3 direction = glm::normalize(RandomVector());

		float offset = scale.x * radius + RandomFloat(SPAWN_DISTANCE);

		Rock* r = new Rock(_models[i % _models.size()], _shader);

		r->position = position + (offset * direction);
		r->rotation = 360.0f * RandomVector();
		r->scale = RandomFloat(ASTEROID_SCALE) * vec3One;

		asteroids.push_back(r);
	}
}
