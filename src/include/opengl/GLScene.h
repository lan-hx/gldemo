//
// Created by lan on 2023/1/8.
//

#ifndef GLDEMO_APK_SRC_OPENGL_GLSCENE_H_
#define GLDEMO_APK_SRC_OPENGL_GLSCENE_H_

#include <QObject>
#include <QOpenGLShaderProgram>
#include <string>
#include <unordered_map>

#include "opengl/GLModel.h"
#include "opengl/GLObject.h"

/**
 * OpenGL 场景
 * @note GLScene负责管理模型和着色器，维护GLObject到这些资源的链接
 */
class GLScene : public QObject {
  Q_OBJECT

 private:
  std::unordered_map<std::string, GLModel *> models_{};
  std::map<uint64_t, GLObject *> objects_{};
  QOpenGLShaderProgram *shader_{};

 public:
  explicit GLScene(QObject *parent = nullptr) : QObject(parent) {}
  // TODO
  ~GLScene() override = default;

  inline void AddModel(const std::string &name, GLModel *model) { models_.emplace(name, model); }
  inline void AddObject(GLObject *obj) { objects_.emplace(obj->GetID(), obj); }
  inline void RemoveObject(GLObject *obj) { objects_.erase(obj->GetID()); }
  inline void SetShader(QOpenGLShaderProgram *shader) { shader_ = shader; }
  inline QOpenGLShaderProgram *GetShader() { return shader_; }
  inline auto GetModels() -> decltype(models_) { return models_; }
  inline GLModel *GetModel(const std::string &name) { return models_[name]; }

  void Initialize();

 public:
};

#endif  // GLDEMO_APK_SRC_OPENGL_GLSCENE_H_
