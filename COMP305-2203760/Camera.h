#pragma once

#include <glad/glad.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Helper.h"

// Types of camera movement
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float YAW = -90.0f; // Rotation around the vertical axis
const float PITCH = 0.0f; // Rotation around the horizontal left to right axis
const float ACCELERATION = 25.0f;
const float MAX_SPEED = 25.0f;
const float BOOST_MAX = 2.5f; // Max boost multiplier
const float BOOST_DURATION = 2.0f;
const float DRAG_DEFAULT = 0.1f;
const float DRAG_STOP = 2.5f;
const float SENSITIVITY = 0.1f; // Look sensitivity
const float FOV = 60.0f;
const float FOV_MIN = 60.0f;
const float FOV_MAX = 110.0f;
const float UP_CLAMP = 89.0f;
const float DOWN_CLAMP = -89.0f;

// Camera class processes input and calculates the Euler Angles, Vectors and Matrices
class Camera
{
public:
    // Attributes
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Euler Angles
    float yaw;
    float pitch;

    // Player settings
    float movementAcceleration;
    glm::vec3 velocity;
    glm::vec3 input;
    float distance;
    float mouseSensitivity;

    bool boosting;
    float boostMultiplier; // Multiplier applied to camera movement
    float boostTime; // Remaining duration of boost

    // Camera settings
    float fov;

    // Gameplay
    int ringsPassed;

    // Constructor with vectors
    Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f),
           float _yaw = YAW,
           float _pitch = PITCH);

    // Called once every frame
    void Update(float deltaTime);

    void PerformBoost(float duration = BOOST_DURATION);

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    // Processes input received using an ENUM
    void ProcessKeyboard(Camera_Movement direction);

    // Processes input received from mouse input
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // Processes input received from a mouse scroll-wheel event
    void ProcessMouseScroll(float yoffset);

private:
    // Calculates the forward vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};
