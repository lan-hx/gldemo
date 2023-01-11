//
// Created by lan on 2023/1/8.
//

#ifndef GLDEMO_APK_SRC_OPENGL_GLSCENE_H_
#define GLDEMO_APK_SRC_OPENGL_GLSCENE_H_

#include <QObject>
#include <QOpenGLShaderProgram>
#include <string>
#include <unordered_map>

#include "camera/GLCamera.h"
#include "opengl/GLModel.h"
#include "opengl/GLObject.h"
#include "utils/GLLight.h"

/**
 * OpenGL 场景
 * @note GLScene负责管理模型和着色器，维护GLObject到这些资源的链接
 * @note uniform存在shader program里，attribute存在VAO里
 */
class GLScene : public QObject {
  Q_OBJECT

 private:
  std::unordered_map<std::string, GLModel *> models_{};
  std::map<uint64_t, GLObject *> objects_{};
  QOpenGLShaderProgram *shader_{};
  QRect viewport_;
  GLCamera *camera_;
  GLLights *lights_;

 public:
  explicit GLScene(QRect viewport, QObject *parent = nullptr);
  GLScene(const GLScene &r) = delete;
  GLScene(const GLScene &&r) = delete;
  ~GLScene() override;

  inline void AddModel(const std::string &name, GLModel *model) { models_.emplace(name, model); }
  inline auto GetModels() -> decltype(models_) & { return models_; }
  inline GLModel *GetModel(const std::string &name) { return models_[name]; }
  inline uint64_t AddObject(GLObject *obj) { return objects_.emplace(obj->GetID(), obj).first->first; }
  inline void RemoveObject(uint64_t id) { objects_.erase(id); }
  inline void RemoveObject(GLObject *obj) { objects_.erase(obj->GetID()); }
  inline auto GetObjects() -> decltype(objects_) & { return objects_; }
  inline void SetShader(QOpenGLShaderProgram *shader) { shader_ = shader; }
  inline QOpenGLShaderProgram *GetShader() { return shader_; }
  inline GLCamera *GetCamera() { return camera_; }
  inline void SetViewPort(QRect viewport) {
    viewport_ = viewport;
    camera_->SetAspect(static_cast<float>(viewport.width()) / viewport.height());
  }
  inline uint64_t AddLight(GLLight light) { return lights_->AddLight(light); }
  inline GLLights *GetLights() { return lights_; }

  /**
   * load resource and default objects
   * @param models if empty then load DEFAULT_MODELS
   */
  void Initialize(const std::vector<std::pair<std::string, std::string>> &models);

  void Draw();
  void Update() { Draw(); }

 public:
  static constexpr auto DEFAULT_MODELS = {
      // obj path | texture path | aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
      std::make_tuple(":/model/cube.obj", ":/texture/wall.jpg"),
      std::make_tuple(":/model/sphere.obj", ""),
      std::make_tuple(":/model/bunny.obj", ""),
      std::make_tuple(":/model/rose.obj", ""),
  };
  static constexpr auto DEFAULT_OBJECTS = {
      // model | position | rotation | scale
      std::make_tuple(":/model/cube.obj", QVector3D{0.0f, 0.0f, 0.0f}, QVector3D{0.0f, 0.0f, 0.0f},
                      QVector3D{1.0f, 1.0f, 1.0f}),
      std::make_tuple(":/model/sphere.obj", QVector3D{3.0f, 0.0f, 0.0f}, QVector3D{0.0f, 0.0f, 0.0f},
                      QVector3D{1.0f, 1.0f, 1.0f}),
  };
};

#endif  // GLDEMO_APK_SRC_OPENGL_GLSCENE_H_
