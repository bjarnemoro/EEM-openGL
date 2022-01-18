#include "camera.h"
#define PI 3.14159265

// constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 mid_position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    MidPosition = mid_position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}
// constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset*5;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 200.0f)
        Zoom = 200.0f;

    updateCameraVectors();
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));

    yaw_rad = (Yaw / 360) * (2 * PI);
    pitch_rad = (Pitch / 360) * (2 * PI);
    float r = 15 * log(2 + Zoom * 0.3);
    //convert carthesian coordinates to a polar coordinate system

    float x = MidPosition.x + r * -cos(pitch_rad) * cos(yaw_rad);
    float y = MidPosition.y + r * -sin(pitch_rad);
    float z = MidPosition.z + r * -cos(pitch_rad) * sin(yaw_rad);
    Position = glm::vec3(x, y, z);
}

glm::vec3 Camera::GetRay(double xpos, double ypos)
{
    //calculate the angle the mouse is positioned at with middle of screen 0deg and sides -22.5deg and 22.5deg
    // * (SCR_WIDTH / SCR_HEIGHT)

    float x_theta = -(0.5 * 90) * ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT) * ((xpos - 0.5 * SCREEN_WIDTH) / (0.5 * SCREEN_WIDTH));
    float y_theta = -(0.5 * 90) * ((ypos - 0.5 * SCREEN_HEIGHT) / (0.5 * SCREEN_HEIGHT));

    glm::vec3 ray_x_rotated = glm::rotate(Front, glm::radians(x_theta), Up);
    glm::vec3 ray_xy_rotated = glm::rotate(ray_x_rotated, glm::radians(y_theta), Right);

    return ray_xy_rotated;
}