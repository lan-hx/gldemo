//
// Created by lan on 2022/11/16.
//

#ifndef GLDEMO_SRC_INCLUDE_OPENGL_MAINGLWINDOW_H_
#define GLDEMO_SRC_INCLUDE_OPENGL_MAINGLWINDOW_H_

#include <QElapsedTimer>
#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>
#include <QtOpenGL/QOpenGLBuffer>
#include <QtOpenGL/QOpenGLShaderProgram>
#include <QtOpenGL/QOpenGLVertexArrayObject>
#include <QtOpenGLWidgets/QOpenGLWidget>

class MainGLWindow : public QOpenGLWidget, public QOpenGLExtraFunctions {
  Q_OBJECT

 public:
  explicit MainGLWindow(QWidget *parent = nullptr);
  ~MainGLWindow() override;

 protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;

 private:
  QOpenGLVertexArrayObject *vao_ =
      nullptr;                    // Vertex Array Object: array of vertex attrib and bounded VBO and bounded EBO
  QOpenGLBuffer *vbo_ = nullptr;  // Vertex Buffer Objects
  QOpenGLBuffer *ebo_ = nullptr;  // Element Buffer Object / Index Buffer Object

  QOpenGLShaderProgram *program_ = nullptr;

  // QMatrix4x4 model_;
  QMatrix4x4 view_;
  QMatrix4x4 projection_;

  QOpenGLTexture *texture_ = nullptr;

  QElapsedTimer time_;

 signals:
  void UpdateInfo(int64_t nsec, const char *debug_info);  // NOLINT
};

#endif  // GLDEMO_SRC_INCLUDE_OPENGL_MAINGLWINDOW_H_
