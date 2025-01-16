#include "Camera.h"

#include <iostream>

// Constructor with vectors
Camera::Camera(glm::vec3 _position,
    glm::vec3 _up,
    float _yaw,
    float _pitch) :
    forward(glm::vec3(0.0f, 0.0f, -1.0f)),
    movementAcceleration(ACCELERATION),
    mouseSensitivity(SENSITIVITY),
    boosting(false),
    boostMultiplier(1.0f),
    fov(FOV)
{
    position = _position;
    worldUp = _up;
    yaw = _yaw;
    pitch = _pitch;

    updateCameraVectors();
}

/// <summary>
/// Called once every frame
/// </summary>
void Camera::Update(float deltaTime)
{
    // Increase then decrease boost,
    // continue until timer is zero and multiplier is one.
    if (boosting)
    {
        boostTime -= deltaTime;

        if (boostTime > 0.0f) //BOOST_DURATION / 2
            boostMultiplier += 0.1f;
        else
            boostMultiplier -= 0.1f;

        boostMultiplier = glm::clamp(boostMultiplier, 1.0f, BOOST_MAX);
        
        if (boostMultiplier == 1.0f)
            boosting = false;
    }

    // Input in worldspace
    glm::vec3 inputDir = input.x * -right + input.y * up + input.z * forward;
    input = vec3Zero;

    // Apply acceleration and boost
    if (inputDir != vec3Zero)
        velocity += movementAcceleration * boostMultiplier * deltaTime * inputDir;
    
    // Is input in velocity direction
    float dot = glm::dot(glm::normalize(inputDir), glm::normalize(velocity));

    // Apply drag (more if input is not in velocity direction)
    velocity += (dot < 0.0f || inputDir == vec3Zero ? DRAG_STOP : DRAG_DEFAULT) * deltaTime * -velocity;

    // Limit velocity if exceeding max speed with boost accounted for
    if (glm::length(velocity) > MAX_SPEED * boostMultiplier)
        velocity = MAX_SPEED * boostMultiplier * glm::normalize(velocity);

    // Apply velocity to position
    position += velocity * deltaTime;
    
    // Set FOV based on velocity (greater the velocity, greater the FOV)
    fov = FOV_MIN + ((glm::length(velocity) / 50.0f) * (FOV_MAX - FOV_MIN));
    fov = glm::clamp(fov, FOV_MIN, FOV_MAX);
}

/// <summary>
/// Boost for a duration, then decay to zero boost.
/// </summary>
void Camera::PerformBoost(float duration)
{
    boostTime = duration;
    boosting = true;
}

/// <summary>
/// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
/// </summary>
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(position, position + forward, up);
}

/// <summary>
/// Processes input received using an ENUM
/// </summary>
void Camera::ProcessKeyboard(Camera_Movement direction)
{
    if (direction == FORWARD)
        input.z++;
        //velocity += forward * movementSpeed * boostMultiplier;
    if (direction == BACKWARD)
        input.z--;
    if (direction == LEFT)
        input.x++;
    if (direction == RIGHT)
        input.x--;
    if (direction == UP)
        input.y++;
    if (direction == DOWN)
        input.y--;
}

/// <summary>
/// Processes input received from mouse input
/// </summary>
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Clamp pitch
    if (constrainPitch)
    {
        pitch = glm::clamp(pitch, DOWN_CLAMP, UP_CLAMP);
    }

    updateCameraVectors();
}

/// <summary>
/// Processes input received from a mouse scroll-wheel event
/// </summary>
void Camera::ProcessMouseScroll(float yoffset)
{
    fov -= (float)yoffset;

    fov = glm::clamp(fov, FOV_MIN, FOV_MAX);
}

/// <summary>
/// Calculates the forward vector from the Camera's (updated) Euler Angles
/// </summary>
void Camera::updateCameraVectors()
{
    glm::vec3 _forward;

    _forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    _forward.y = sin(glm::radians(pitch));
    _forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    // Normalize vectors to keep consistant speeds in all directions
    forward = glm::normalize(_forward);
    right = glm::normalize(glm::cross(forward, worldUp));
    up = glm::normalize(glm::cross(right, forward));
}
