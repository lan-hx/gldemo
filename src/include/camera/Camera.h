//
// Created by lan on 2022/11/21.
//

#ifndef GLDEMO_SRC_CAMERA_CAMERA_H_
#define GLDEMO_SRC_CAMERA_CAMERA_H_

#include <QMatrix4x4>
#include <QObject>
#include <QVector3D>

/**
 * 摄像机类，处理摄像机相关工作
 * @note 使用欧拉角实现
 * @note https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h
 */
class Camera : public QObject {
  Q_OBJECT

 private:
  // camera Attributes
  QVector3D position_;  // TODO(lan): to be commented
  QVector3D front_;     // TODO(lan): to be commented
  QVector3D up_;        // TODO(lan): to be commented
  QVector3D right_;     // TODO(lan): to be commented
  QVector3D world_up_;  // TODO(lan): to be commented
  // euler Angles
  float yaw_ = -90.0f;  // TODO(lan): to be commented
  float pitch_ = 0.0f;  // TODO(lan): to be commented
  // camera options
  float movement_speed_ = 2.5f;     // TODO(lan): to be commented
  float mouse_sensitivity_ = 0.1f;  // TODO(lan): to be commented
  float zoom_ = 45.0f;              // TODO(lan): to be commented

 public:
  /**
   * default camera
   */
  Camera() = default;
  /**
   * TODO(lan): to be commented
   * @param ... TODO(lan): to be commented
   */
  // Camera(float ...) {}

  /**
   * get view matrix
   * @return view matrix
   */
  QMatrix4x4 GetViewMatrix();

  /**
   * Update private parameters
   */
  void Update();

  /**
   * process keyboard event
   * @param key trigered key
   * @param is_pressed if the key is pressed or released
   */
  void KeyboardCallback(Qt::Key key, bool is_pressed);
  /**
   * process mouse event
   * @param xoffset delta x
   * @param yoffset delta y
   * @param is_pressed if the mouse left key is pressed or released
   * @note 看情况，可能会变接口
   */
  void MouseCallback(float xoffset, float yoffset, bool is_pressed);
};

#endif  // GLDEMO_SRC_CAMERA_CAMERA_H_
