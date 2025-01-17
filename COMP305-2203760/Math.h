#pragma once

#include "glm/glm.hpp"

#include <cstdlib>
#include <tuple>
#include <ctime>

static bool first = true;

// Vectors
const glm::vec3 vec3Zero = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 vec3One = glm::vec3(1.0f, 1.0f, 1.0f);

// World Directions
const glm::vec3 worldForward = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 worldRight = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

// A seed is created from system time in seconds.
static void Seed()
{
	if (first)
		srand(static_cast <unsigned> (time(0)));

	first = false;
}

static int RandomInt(int min = 0, int max = 1)
{
	Seed();

	return min + (rand() % (max - min + 1));
}

static int RandomInt(std::tuple<int, int> minMax)
{
	return RandomInt(std::get<0>(minMax), std::get<1>(minMax));
}

static float RandomFloat(float min = 0.0f, float max = 1.0f)
{
	Seed();

	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

static float RandomFloat(std::tuple<float, float> minMax)
{
	return RandomFloat(std::get<0>(minMax), std::get<1>(minMax));
}

static glm::vec3 RandomVector(glm::vec3 min = glm::vec3(-1.0f), glm::vec3 max = glm::vec3(1.0f))
{
	return glm::vec3(RandomFloat(min.x, max.x), RandomFloat(min.y, max.y), RandomFloat(min.z, max.z));
}

static glm::vec3 RandomVector(std::tuple<glm::vec3, glm::vec3> minMax)
{
	return RandomVector(std::get<0>(minMax), std::get<1>(minMax));
}