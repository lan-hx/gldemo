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
#include <functional>
#include <vector>

struct GLMaterial {
  QVector3D ka_;
  QVector3D kd_;
  QVector3D ks_;
  float ns_;
};

struct GLVertex {
  float position_[3];
  float normal_[3];
  float texcoord_[2];
  float color_[3];

  GLVertex() : position_{0}, normal_{0}, texcoord_{0}, color_{0} {}

  inline bool operator==(const GLVertex &v) const {
    return position_[0] == v.position_[0] && position_[1] == v.position_[1] && position_[2] == v.position_[2] &&
           normal_[0] == v.normal_[0] && normal_[1] == v.normal_[1] && normal_[2] == v.normal_[2] &&
           texcoord_[0] == v.texcoord_[0] && texcoord_[1] == v.texcoord_[1] && color_[0] == v.color_[0] &&
           color_[1] == v.color_[1] && color_[2] == v.color_[2];
  }
};

namespace std {
template <>
struct hash<GLVertex> {
  size_t operator()(const GLVertex &v) const {
    return (hash<float>()(v.position_[0]) << 0) ^ (hash<float>()(v.position_[1]) << 1) ^
           (hash<float>()(v.position_[2]) << 2) ^ (hash<float>()(v.normal_[0]) << 3) ^
           (hash<float>()(v.normal_[1]) << 4) ^ (hash<float>()(v.normal_[2]) << 5) ^
           (hash<float>()(v.texcoord_[0]) << 6) ^ (hash<float>()(v.texcoord_[1]) << 7) ^
           (hash<float>()(v.color_[0]) << 8) ^ (hash<float>()(v.color_[1]) << 9) ^ (hash<float>()(v.color_[2]) << 10);
  }
};
}  // namespace std

class GLModel : public QObject {
  Q_OBJECT

  friend class GLObject;
  friend class ModelSettings;

 private:
  std::string path_;
  tinyobj::ObjReader reader_;
  std::string texture_path_;
  QOpenGLTexture *texture_ = nullptr;

 private:
  QOpenGLVertexArrayObject *vao_ = nullptr;
  QOpenGLBuffer *vbo_ = nullptr;
  QOpenGLBuffer *ebo_ = nullptr;

  std::vector<GLMaterial> materials_{};
  std::vector<GLVertex> vertices_{};
  std::vector<uint32_t> indices_{};

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

  bool Load(const std::string &path, const std::string &texture, QOpenGLShaderProgram *shader);
  bool LoadObj(const std::string &path);
  bool SaveObj(const std::string &path);
  void SetupVao(QOpenGLShaderProgram *shader);
  /**
   * 加载材质
   * @param texture 材质文件位置
   * @param setting 自定义材质设置
   */
  bool LoadTexture(
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
  void SetMaterial(QOpenGLShaderProgram *shader);
  inline const std::string &GetPath() { return path_; }
};

#endif  // GLDEMO_APK_SRC_OPENGL_GLMODEL_H_
