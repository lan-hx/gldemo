//
// Created by lan on 2023/1/8.
//

#ifndef GLDEMO_APK_SRC_UTILS_GLLIGHT_H_
#define GLDEMO_APK_SRC_UTILS_GLLIGHT_H_

#include <utils/GLUniformBuffer.h>

#include <QObject>
#include <QVector3D>

enum GLLightType { AmbientLight = 0, DirectionalLight = 1, SpotLight = 2 };

struct GLLightStd140 {
  QVector3D position_{0.0f, 0.0f, 0.0f};
  uint8_t padding1_;
  QVector3D direction_{0.0f, 0.0f, 0.0f};
  uint8_t padding2_;
  QVector3D color_{1.0f, 1.0f, 1.0f};
  float intensity_;
  float angle_;
  float kc_, kl_, kq_;
  GLLightType type_;
  uint8_t padding3_;
  uint64_t padding4_;
};

static_assert(offsetof(GLLightStd140, position_) == 0);
static_assert(offsetof(GLLightStd140, direction_) == 16);
static_assert(offsetof(GLLightStd140, color_) == 32);
static_assert(offsetof(GLLightStd140, intensity_) == 44);
static_assert(offsetof(GLLightStd140, angle_) == 48);
static_assert(offsetof(GLLightStd140, kc_) == 52);
static_assert(offsetof(GLLightStd140, kl_) == 56);
static_assert(offsetof(GLLightStd140, kq_) == 60);
static_assert(offsetof(GLLightStd140, type_) == 64);
static_assert(sizeof(GLLightStd140) == 80);

using GLLight = GLLightStd140;

class GLLights : public QObject {
  Q_OBJECT

  friend class LightSettings;

 private:
  uint64_t id_inc_ = 0;
  uint64_t uniform_index_;
  std::map<uint64_t, GLLight> lights_{};
  GLUniformBuffer ub_;
  size_t max_count_;

 public:
  explicit GLLights(uint64_t uniform_index = 0, size_t max_count = 20, QObject *parent = nullptr)
      : uniform_index_(uniform_index),
        ub_(max_count * sizeof(GLLightStd140) + 16, this),
        max_count_(max_count),
        QObject(parent) {
    ub_.SetBindingPoint(uniform_index_);
  }

  void SetupShader(QOpenGLShaderProgram *shader, const char *name) {
    GLUniformBuffer::SetShaderBindingPoint(shader, name, uniform_index_);
    Update();
  }

  void Update() {
    auto it = lights_.begin();
    uint64_t i = 0;
    for (; i < 20 && it != lights_.end(); ++i, ++it) {
      ub_.Write(i * sizeof(GLLight), sizeof(GLLight), &it->second);
    }
    ub_.Write(max_count_ * sizeof(GLLight), sizeof(unsigned int), &i);
  }

  void Use() { ub_.Bind(); }
  void Release() { ub_.Release(); }

  const std::map<uint64_t, GLLight> &GetLights() { return lights_; }

  uint64_t AddLight(GLLight light) {
    assert(lights_.size() < max_count_);
    auto id = lights_.emplace(id_inc_++, light).first->first;
    Update();
    return id;
  }
  void RemoveLight(uint64_t id) {
    lights_.erase(id);
    Update();
  }

  static GLLight AddAmbientLight(QVector3D color, float intensity) {
    auto light = GLLightStd140{};
    light.type_ = AmbientLight;
    light.color_ = color;
    light.intensity_ = intensity;
    return light;
  }
  static GLLight AddDirectionalLight(QVector3D direction, QVector3D color, float intensity) {
    auto light = GLLightStd140{};
    light.type_ = DirectionalLight;
    light.direction_ = direction;
    light.color_ = color;
    light.intensity_ = intensity;
    return light;
  }
  static GLLight AddSpotLight(QVector3D position, QVector3D direction, QVector3D color, float intensity, float angle,
                              float kc, float kl, float kq) {
    auto light = GLLightStd140{};
    light.type_ = SpotLight;
    light.position_ = position;
    light.direction_ = direction;
    light.color_ = color;
    light.intensity_ = intensity;
    light.angle_ = angle;
    light.kc_ = kc;
    light.kl_ = kl;
    light.kq_ = kq;
    return light;
  }
};

#endif  // GLDEMO_APK_SRC_UTILS_GLLIGHT_H_
