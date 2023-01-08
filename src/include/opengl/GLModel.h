//
// Created by lan on 2023/1/8.
//

#ifndef GLDEMO_APK_SRC_OPENGL_GLMODEL_H_
#define GLDEMO_APK_SRC_OPENGL_GLMODEL_H_

#include <tiny_obj_loader.h>

#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <vector>

struct GLMaterial {};

class GLModel : public QObject {
  Q_OBJECT

 private:
  std::string path_;
  tinyobj::ObjReader reader_;
  QOpenGLTexture *texture_;

 private:
  QOpenGLVertexArrayObject *vao_;
  QOpenGLBuffer *vbo_;
  QOpenGLBuffer *ebo_;

 public:
  explicit GLModel(QObject *parent = nullptr) : QObject(parent) {}
  explicit GLModel(const std::string &path, const std::string &texture, QOpenGLShaderProgram *shader,
                   QObject *parent = nullptr)
      : QObject(parent) {
    Load(path, texture, shader);
  }
  ~GLModel() override {
    delete texture_;
    delete ebo_;
    delete vbo_;
    delete vao_;
  }

  void Load(const std::string &path, const std::string &texture, QOpenGLShaderProgram *shader);
  void LoadObj(const std::string &path);
  void SetupVao(QOpenGLShaderProgram *shader);
  /**
   * 加载材质
   * @param texture 材质文件位置
   * @param setting 自定义材质设置
   */
  void LoadTexture(
      const std::string &texture, const std::function<void(QOpenGLTexture *)> &setting = [](QOpenGLTexture *texture) {
        // set warp
        texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
        texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);

        // set filter type
        texture->setMagnificationFilter(QOpenGLTexture::Linear);
        texture->setMinificationFilter(QOpenGLTexture::Linear);

        texture->generateMipMaps();
      });

  inline QOpenGLVertexArrayObject *GetVao() { return vao_; }
  inline QOpenGLTexture *GetTexture() { return texture_; }
};

#endif  // GLDEMO_APK_SRC_OPENGL_GLMODEL_H_
