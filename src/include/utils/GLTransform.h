//
// Created by lan on 2023/1/8.
//

#ifndef GLDEMO_APK_SRC_UTILS_GLTRANSFORM_H_
#define GLDEMO_APK_SRC_UTILS_GLTRANSFORM_H_

#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>

/**
 * 物体的位置变换
 * @note 对外接口使用欧拉角，内部使用四元数
 */

struct GLTransform {
  QVector3D position_{0.0f, 0.0f, 0.0f};
  float pitch_{0.0f}, yaw_{0.0f}, roll_{0.0f};
  QQuaternion rotation_{0.0f, 0.0f, 0.0f, 0.0f};
  QVector3D scale_{1.0f, 1.0f, 1.0f};

  inline void SetAngles(float pitch, float yaw, float roll) {
    pitch_ = pitch;
    yaw_ = yaw;
    roll_ = roll;
    rotation_ = QQuaternion::fromEulerAngles(pitch, yaw, roll);
  }
  inline void SetAngles(QVector3D angle) {
    pitch_ = angle.x();
    yaw_ = angle.y();
    roll_ = angle.z();
    rotation_ = QQuaternion::fromEulerAngles(angle);
  }

  inline QVector3D GetFront() const { return rotation_ * QVector3D{0.0f, 0.0f, -1.0f}; }
  inline QVector3D GetUp() const { return rotation_ * QVector3D{0.0f, 1.0f, 0.0f}; }
  inline QVector3D GetRight() const { return rotation_ * QVector3D{1.0f, 0.0f, 0.0f}; }
  inline QMatrix4x4 GetModelMatrix() const {
    QMatrix4x4 m;
    m.setToIdentity();
    m.translate(position_);
    m.rotate(rotation_);
    m.scale(scale_);
    return m;
  }
};

#endif  // GLDEMO_APK_SRC_UTILS_GLTRANSFORM_H_
