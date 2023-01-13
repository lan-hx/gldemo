//
// Created by lan on 2022/11/21.
//

#ifndef GLDEMO_SRC_CAMERA_CAMERA_H_
#define GLDEMO_SRC_CAMERA_CAMERA_H_

#include <QMatrix4x4>
#include <QObject>
#include <QVector3D>

constexpr double PI = 3.14159265358979323846;

class CameraSettings;

/**
 * 摄像机类，处理摄像机相关工作
 * @note 使用欧拉角实现
 * @note https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h
 */
class GLCamera : public QObject {
  Q_OBJECT

 public:
  friend class CameraSettings;

 private:
  // camera Attributes
  QVector3D position_;  // 摄像机位置
  QVector3D front_{};   // 摄像机朝向
  QVector3D up_{};      // 摄像机up向量
  QVector3D right_{};   // 摄像机right向量
  QVector3D world_up_;  // 世界坐标up向量
  // euler Angles
  float pitch_ = 0.0f;                       // 俯仰角
  float yaw_ = static_cast<float>(-PI / 2);  // 偏航角
  // camera options
  float movement_speed_ = 2.5f;       // 移动速度
  float mouse_sensitivity_ = 0.002f;  // 鼠标敏感度
  float zoom_ = 45.0f;                // FOV
  // projection matrix
  float &fovy_ = zoom_;
  float aspect_;
  float znear_ = 0.1f, zfar_ = 100.0f;

 public:
  /**
   * default camera
   */
  explicit GLCamera(QObject *parent = nullptr)
      : QObject(parent), position_({}), world_up_({0.0f, 1.0f, 0.0f}), aspect_(1.0f) {
    Update();
  }
  /**
   * 构造camera，参数见类的声明
   */
  GLCamera(QVector3D position, QVector3D world_up, float pitch, float yaw, float movement_speed,
           float mouse_sensitivity, float zoom, float aspect, float znear, float zfar, QObject *parent = nullptr)
      : QObject(parent),
        position_(position),
        world_up_(world_up),
        pitch_(pitch),
        yaw_(yaw),
        movement_speed_(movement_speed),
        mouse_sensitivity_(mouse_sensitivity),
        zoom_(zoom),
        aspect_(aspect),
        znear_(znear),
        zfar_(zfar) {
    Update();
  }
  /**
   * 简化版camera构造
   */
  explicit GLCamera(QVector3D position, float aspect, QVector3D world_up = {0.0f, 1.0f, 0.0f}, float pitch = 0.0f,
                    float yaw = -PI / 2, QObject *parent = nullptr)
      : QObject(parent), position_(position), world_up_(world_up), pitch_(pitch), yaw_(yaw), aspect_(aspect) {
    Update();
  }

  ~GLCamera() override = default;

  /**
   * get view matrix
   * @return view matrix
   */
  QMatrix4x4 GetViewMatrix() const;

  inline float GetFOV() const { return zoom_; }

  inline QVector2D GetAngles() const { return {pitch_, yaw_}; }

  inline QVector3D GetPosition() const { return position_; }

  inline void SetAspect(float aspect) { aspect_ = aspect; }

  QMatrix4x4 GetProjectionMatrix() const;

  /**
   * Update private parameters
   */
  void Update();

  /**
   * process keyboard event
   * @param key trigered key
   * @param time_elapsed time elapsed since last keyboard
   */
  void KeyboardCallback(Qt::Key key, float time_elapsed);
  /**
   * process mouse event
   * @param xoffset delta x
   * @param yoffset delta y
   * @param scroll_offset scroll delta
   * @param time_elapsed time elapsed since last keyboard
   * @note 看情况，可能会变接口
   */
  void MouseCallback(float xoffset, float yoffset, float scroll_offset, float time_elapsed);

 private:
  inline void SetParameters(QVector3D position, float pitch, float yaw, float zoom) {
    position_ = position;
    pitch_ = pitch;
    yaw_ = yaw;
    zoom_ = zoom;
    Update();
  }

 signals:
  void ValueChanged();
};

#endif  // GLDEMO_SRC_CAMERA_CAMERA_H_
