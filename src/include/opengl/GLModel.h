//
// Created by lan on 2023/1/8.
//

#ifndef GLDEMO_APK_SRC_OPENGL_GLMODEL_H_
#define GLDEMO_APK_SRC_OPENGL_GLMODEL_H_

#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <vector>

struct GLMaterial {};

struct GLVertex {
  size_t v_;
  size_t vt_;
  size_t vn_;
};

struct GLFace {
  GLVertex v_[3];
};

struct GLMesh {
  std::vector<QVector3D> vertex_;
  std::vector<QVector3D> texture_coord_;
  std::vector<QVector3D> normal_;
  std::vector<GLFace> faces_;
};

class GLModel : public QObject {
  Q_OBJECT

 private:
  std::string path_;
  GLMesh mesh_;
  GLMaterial material_;
  QOpenGLTexture *texture_;

 private:
  QOpenGLVertexArrayObject *vao_;
  QOpenGLBuffer *vbo_;
  QOpenGLBuffer *ebo_;

 public:
  // TODO
  explicit GLModel(const std::string &path, QObject *parent = nullptr) : path_(path), QObject(parent) {
    if (!path.empty()) {
      Load(path);
    }
  }
  // TODO
  ~GLModel() override = default;

  void Load(const std::string &path);
};

#endif  // GLDEMO_APK_SRC_OPENGL_GLMODEL_H_
