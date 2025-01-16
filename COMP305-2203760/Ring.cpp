#include "Ring.h"

std::vector<Ring*> Ring::rings;
Ring* Ring::currentRing;

glm::vec3 Ring::GetNormal()
{
	return glm::normalize(GetMatrix() * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
}

bool Ring::Collision(glm::vec3 lastPoint, glm::vec3 currentPoint)
{
	if (currentPoint == lastPoint) 
		return false;

	glm::vec3 delta = currentPoint - lastPoint;
	glm::vec3 direction = glm::normalize(delta);
	glm::vec3 normal = GetNormal();

	float denom = glm::dot(normal, direction);

	// Ray is parallel to plane
	if (fabs(denom) <= 0.0001f) 
		return false;
	
	// Distance along ray to plane
	float t = glm::dot((position - lastPoint), normal) / denom;

	if (fabs(t) > glm::length(delta)) 
		return false;

	// ray plane intersection point
	glm::vec3 intersection = lastPoint + t * glm::normalize(delta);

	if (glm::length(intersection - position) >= scale.x * radius)
		return false;

	return true;
}

void Ring::SpawnRing(Model* _model, Shader* _shader)
{
	Ring* newRing = new Ring(_model, _shader);
	newRing->scale = 5.0f * vec3One;

	newRing->RandomPositionInForward(currentRing->position, currentRing->rotation);

	Ring::rings.push_back(newRing);

	if (Ring::rings.size() > 3)
	{
		Ring* r = Ring::rings[0];

		Ring::rings.erase(std::remove(Ring::rings.begin(), Ring::rings.end(), r), Ring::rings.end());

		GameObject::Delete(r);
	}

	currentRing = newRing;
}

/// <summary>
/// Randomise position within radius and rotate to face the camera.
/// </summary>
void Ring::RandomPositionInForward(glm::vec3 origin, glm::vec3 lastRotation)
{
	// Random rotation within viewing area
	rotation.y = lastRotation.y + RandomFloat(-25.0f, 25.0f);
	rotation.x = lastRotation.x + RandomFloat(-15.0f, 15.0f);

	// Move by random distance in the local forward direction of the ring
	position = origin + (RandomFloat(SPAWN_RADIUS) * -GetNormal());
}

/// <summary>
/// Randomise position and rotation of ring, within radius.
/// </summary>
void Ring::RandomPositionRotation(glm::vec3 origin)
{
	// Set random rotation
	rotation.y = RandomFloat(0.0f, 360.0f);
	rotation.x = RandomFloat(-90.0f, 90.0f);

	// Move by random distance in the local forward direction of the ring
	position = origin + RandomFloat(SPAWN_RADIUS) * GetNormal();
}
