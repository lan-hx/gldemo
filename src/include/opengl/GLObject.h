//
// Created by lan on 2023/1/8.
//

#ifndef GLDEMO_APK_SRC_OPENGL_GLOBJECT_H_
#define GLDEMO_APK_SRC_OPENGL_GLOBJECT_H_

#include <QObject>
#include <QOpenGLShaderProgram>

#include "boundingbox/GLBoundingbox.h"
#include "opengl/GLModel.h"
#include "utils/GLTransform.h"

/**
 * OpenGL绘制的物体实例
 */
class GLObject : public QObject {
  Q_OBJECT

  friend class ObjectSettings;

 private:
  static uint64_t id_inc;
  const uint64_t id_;
  GLModel *model_;
  QOpenGLShaderProgram *shader_;

 public:
  GLTransform transform_;
  GLBoundingbox boundingbox_;

 public:
  explicit GLObject(GLModel *model, QOpenGLShaderProgram *shader, bool is_major = false, QObject *parent = nullptr)
      : id_(++id_inc),
        model_(model),
        shader_(shader),
        QObject(parent),
        boundingbox_(model->GetVertices(), is_major ? GLBoundingbox::BoundingboxType::SPHERE_BOUNDINGBOX
                                                    : GLBoundingbox::BoundingboxType::CUBE_BOUNDINGBOX) {}
  ~GLObject() override = default;

 public:
  inline uint64_t GetID() const { return id_; }
  void Draw();
  bool CollideWith(GLObject *obj);
};

#endif  // GLDEMO_APK_SRC_OPENGL_GLOBJECT_H_
