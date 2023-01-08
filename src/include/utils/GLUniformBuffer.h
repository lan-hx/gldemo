//
// Created by lan on 2023/1/8.
//

#ifndef GLDEMO_APK_SRC_UTILS_GLUNIFORMBUFFER_H_
#define GLDEMO_APK_SRC_UTILS_GLUNIFORMBUFFER_H_

#include <QObject>
#include <QOpenGLShaderProgram>

class GLUniformBuffer : public QObject {
  Q_OBJECT

 private:
  unsigned int ubo_;

 public:
  explicit GLUniformBuffer(size_t bufferSize, QObject *parent = nullptr);
  GLUniformBuffer(GLUniformBuffer &&rhs) = delete;
  GLUniformBuffer(GLUniformBuffer &rhs) = delete;
  ~GLUniformBuffer() override;

  static void SetShaderBindingPoint(QOpenGLShaderProgram *shader, const char *name, uint32_t index);
  void SetBindingPoint(uint32_t index);
  void Bind();
  void Release();
  void Write(size_t offset, size_t size, void *data);
};

#endif  // GLDEMO_APK_SRC_UTILS_GLUNIFORMBUFFER_H_
