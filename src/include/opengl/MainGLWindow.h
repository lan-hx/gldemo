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
#include <set>

#include "camera/GLCamera.h"

class MainWindow;

class MainGLWindow : public QOpenGLWidget, public QOpenGLExtraFunctions {
  Q_OBJECT

  friend class MainWindow;

 public:
  explicit MainGLWindow(QWidget *parent = nullptr);
  ~MainGLWindow() override;

  void TakeScreenShot();

 protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;

 protected:
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

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

  int64_t time_elapsed_;  // in ns

  // camera
  GLCamera *camera_ = nullptr;

  // io
  std::set<Qt::Key> keys_{};
  bool mouse_is_pressed_ = false;
  QPoint mouse_last_pos_{0, 0}, mouse_pos_{0, 0};
  float mouse_scroll_delta_ = 0;

 signals:
  void UpdateInfo(int64_t nsec, const char *debug_info);             // NOLINT
  void SendKey(Qt::Key key, float time_elapsed);                     // NOLINT
  void SendMouse(float xoffset, float yoffset, float time_elapsed);  // NOLINT
};

#endif  // GLDEMO_SRC_INCLUDE_OPENGL_MAINGLWINDOW_H_
