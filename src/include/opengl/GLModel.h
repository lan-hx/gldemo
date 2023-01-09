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
#include <functional>

struct GLMaterial {};

struct GLVertex{
  float position[3];
  float normal[3];
  float texcoord[2];
  float color[3];

  GLVertex():
    position{0}, normal{0}, texcoord{0}, color{0}{}

  inline bool operator == (const GLVertex &v) const {
    return position[0] == v.position[0] &&
    position[1] == v.position[1] &&
    position[2] == v.position[2] &&
    normal[0] == v.normal[0] &&
    normal[1] == v.normal[1] &&
    normal[2] == v.normal[2] &&
    texcoord[0] == v.texcoord[0] &&
    texcoord[1] == v.texcoord[1] &&
    color[0] == v.color[0] &&
    color[1] == v.color[1] &&
    color[2] == v.color[2];
  }
};

namespace std{
  template<>
  struct hash<GLVertex> {
    size_t operator()(const GLVertex &v) const {
      return (hash<float>()(v.position[0]) << 0) ^ 
      (hash<float>()(v.position[1]) << 1) ^
      (hash<float>()(v.position[2]) << 2) ^
      (hash<float>()(v.normal[0]) << 3) ^
      (hash<float>()(v.normal[1]) << 4) ^
      (hash<float>()(v.normal[2]) << 5) ^
      (hash<float>()(v.texcoord[0]) << 6) ^
      (hash<float>()(v.texcoord[1]) << 7) ^
      (hash<float>()(v.color[0]) << 8) ^
      (hash<float>()(v.color[1]) << 9) ^
      (hash<float>()(v.color[2]) << 10);
    }
  };
}

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

  std::vector<GLMaterial> _materials;
  std::vector<GLVertex> _vertices;
  std::vector<uint32_t> _indices;

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
