//
// Created by lan on 2022/11/21.
//

#include "camera/Camera.h"

QMatrix4x4 Camera::GetViewMatrix() { return {}; }

void Camera::Update() {}

void Camera::KeyboardCallback(Qt::Key key, bool is_pressed) {}

void Camera::MouseCallback(float xoffset, float yoffset, bool is_pressed) {}
