//
// Created by lan on 2022/11/16.
//

#include "opengl/MainGLWindow.h"

#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QTime>

/**
 *
 * 兼容OpenGL和OpenGL ES
 * @param src 原shader，不包含version
 * @return 包含version的shader
 */
// QByteArray VersionedShaderCode(const char *src) {
//   QByteArray versioned_src;
//
//   if (QOpenGLContext::currentContext()->isOpenGLES()) {
//     versioned_src.append(QByteArrayLiteral("#version 300 es\n"));
//   } else {
//     versioned_src.append(QByteArrayLiteral("#version 330\n"));
//   }
//
//   versioned_src.append(src);
//   return versioned_src;
// }

MainGLWindow::MainGLWindow(QWidget *parent) : QOpenGLWidget(parent) { setFocusPolicy(Qt::StrongFocus); }

MainGLWindow::~MainGLWindow() {
  makeCurrent();
  delete scene_;
}

void MainGLWindow::initializeGL() {
  scene_ = new GLScene(geometry(), this);

  // the same as glad init
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

  f->glEnable(GL_DEPTH_TEST);
  // f->glEnable(GL_CULL_FACE);

  scene_->Initialize({});
}

// QVector3D camera_pos = QVector3D(0.0f, 0.0f, 3.0f);
// QVector3D camera_front = QVector3D(0.0f, 0.0f, -1.0f);
// QVector3D camera_up = QVector3D(0.0f, 1.0f, 0.0f);
//
void MainGLWindow::paintGL() {
  static int num = 0;
  ++num;
  // qDebug() << num << ' ' << QDateTime::currentMSecsSinceEpoch();
  time_elapsed_ = time_.nsecsElapsed();
  auto camera = scene_->GetCamera();
  emit UpdateInfo(time_elapsed_, (QString("frame %1 position (%2, %3, %4) pitch %5 yaw %6")
                                      .arg(num)
                                      .arg(camera->GetPosition().x())
                                      .arg(camera->GetPosition().y())
                                      .arg(camera->GetPosition().z())
                                      .arg(camera->GetAngles().x())
                                      .arg(camera->GetAngles().y()))
                                     .toUtf8());
  time_.restart();

  // QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

  // process io
  for (auto &key : keys_) {
    camera->KeyboardCallback(key, time_elapsed_ / 1e9);
  }
  auto mouse_delta = mouse_pos_ - mouse_last_pos_;
  if (mouse_is_pressed_) {
    mouse_last_pos_ = mouse_pos_;
  } else {
    mouse_delta = {0, 0};
  }
  if (mouse_is_pressed_ || mouse_scroll_delta_ != 0) {
    camera->MouseCallback(mouse_delta.x(), mouse_delta.y(), mouse_scroll_delta_, time_elapsed_ / 1e9);
    mouse_scroll_delta_ = 0;
  }

  scene_->Draw();
}
void MainGLWindow::resizeGL(int w, int h) {
  scene_->SetViewPort(QRect(0, 0, w, h));
  update();
}
void MainGLWindow::keyPressEvent(QKeyEvent *event) {
  auto key = event->key();
  if (key == Qt::Key_W || key == Qt::Key_S || key == Qt::Key_A || key == Qt::Key_D || key == Qt::Key_Space ||
      key == Qt::Key_Shift) {
    keys_.emplace(static_cast<Qt::Key>(key));
  } else if (key == Qt::Key_F12) {
    TakeScreenShot();
  } else {
    event->ignore();
  }
}

void MainGLWindow::keyReleaseEvent(QKeyEvent *event) {
  auto key = event->key();
  if (key == Qt::Key_W || key == Qt::Key_S || key == Qt::Key_A || key == Qt::Key_D || key == Qt::Key_Space ||
      key == Qt::Key_Shift) {
    keys_.erase(static_cast<Qt::Key>(key));
  } else {
    event->ignore();
  }
}

void MainGLWindow::mouseMoveEvent(QMouseEvent *event) { mouse_pos_ = event->pos(); }

void MainGLWindow::wheelEvent(QWheelEvent *event) { mouse_scroll_delta_ += event->angleDelta().y(); }
void MainGLWindow::mousePressEvent(QMouseEvent *event) {
  if ((event->buttons() & Qt::LeftButton) != 0) {
    mouse_is_pressed_ = true;
    mouse_last_pos_ = event->pos();
    mouse_pos_ = event->pos();
  }
}
void MainGLWindow::mouseReleaseEvent(QMouseEvent *event) {
  if ((event->buttons() & Qt::LeftButton) == 0) {
    mouse_is_pressed_ = false;
  }
}
void MainGLWindow::TakeScreenShot() {
  // save screenshot
  auto pixmap = grab();
  auto file_name = QFileDialog::getSaveFileName(this, tr("将截图保存到..."), tr(""), tr("Images(*.png *.jpg)"));
  if (!pixmap.save(file_name)) {
    QMessageBox::warning(this, tr("保存失败"), tr("好像什么出错了"));
  }
}
