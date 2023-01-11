//
// Created by lan on 2022/11/21.
//

#include "camera/GLCamera.h"

#include <cmath>

QMatrix4x4 GLCamera::GetViewMatrix() const {
  QMatrix4x4 view;
  view.lookAt(position_, position_ + front_, up_);
  return view;
}

void GLCamera::Update() {
  front_ = {cosf(yaw_) * cosf(pitch_), sinf(pitch_), cosf(pitch_) * sinf(yaw_)};
  front_.normalize();
  right_ = QVector3D::crossProduct(front_, world_up_).normalized();
  up_ = QVector3D::crossProduct(right_, front_).normalized();
}

void GLCamera::KeyboardCallback(Qt::Key key, float time_elapsed) {
  float v = movement_speed_ * time_elapsed;
  switch (key) {
    case Qt::Key_W:
      position_ += front_ * v;
      break;
    case Qt::Key_S:
      position_ -= front_ * v;
      break;
    case Qt::Key_A:
      position_ -= right_ * v;
      break;
    case Qt::Key_D:
      position_ += right_ * v;
      break;
    case Qt::Key_Space:
      position_ += up_ * v;
      break;
    case Qt::Key_Shift:
      position_ -= up_ * v;
      break;
    default:;
  }
  emit ValueChanged();
}

void GLCamera::MouseCallback(float xoffset, float yoffset, float scroll_offset, float time_elapsed) {
  xoffset *= mouse_sensitivity_;
  yoffset *= mouse_sensitivity_;
  yaw_ -= xoffset;
  pitch_ += yoffset;
  if (pitch_ > PI * 89 / 180) {
    pitch_ = static_cast<float>(PI * 89 / 180);
  }
  if (pitch_ < -PI * 89 / 180) {
    pitch_ = static_cast<float>(-PI * 89 / 180);
  }
  zoom_ += scroll_offset * mouse_sensitivity_;
  if (zoom_ < 1.0f) {
    zoom_ = 1.0f;
  }
  if (zoom_ > 60.0f) {
    zoom_ = 60.0f;
  }

  Update();
  emit ValueChanged();
}

QMatrix4x4 GLCamera::GetProjectionMatrix() const {
  QMatrix4x4 projection;
  projection.setToIdentity();
  projection.perspective(fovy_, aspect_, znear_, zfar_);
  return projection;
}
