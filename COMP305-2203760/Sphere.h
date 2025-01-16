#pragma once

#include "GameObject.h"

class Sphere : public GameObject
{
	using GameObject::GameObject;

public:
	float radius = 1.0f;

};