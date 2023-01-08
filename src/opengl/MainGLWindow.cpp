//
// Created by lan on 2022/11/16.
//

#include "opengl/MainGLWindow.h"

#include <QKeyEvent>
#include <QTime>

#include "fs1_frag.h"
#include "vs1_vert.h"

// 5个一组，格式化程序的问题
float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

                    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
                    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
                    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

                    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

// world space positions of our cubes
QVector3D cube_positions[] = {QVector3D(0.0f, 0.0f, 0.0f),    QVector3D(2.0f, 5.0f, -15.0f),
                              QVector3D(-1.5f, -2.2f, -2.5f), QVector3D(-3.8f, -2.0f, -12.3f),
                              QVector3D(2.4f, -0.4f, -3.5f),  QVector3D(-1.7f, 3.0f, -7.5f),
                              QVector3D(1.3f, -2.0f, -2.5f),  QVector3D(1.5f, 2.0f, -2.5f),
                              QVector3D(1.5f, 0.2f, -1.5f),   QVector3D(-1.3f, 1.0f, -1.5f)};

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
  delete texture_;
  delete program_;
  delete vbo_;
  delete ebo_;
  delete vao_;
}

void MainGLWindow::initializeGL() {
  // camera init
  camera_ = new GLCamera({0.0f, 0.0f, 3.0f}, static_cast<float>(width()) / height());

  // the same as glad init
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

  f->glEnable(GL_DEPTH_TEST);
  // f->glEnable(GL_CULL_FACE);

  // shader link
  delete program_;
  program_ = new QOpenGLShaderProgram;
  if (!program_->addShaderFromSourceCode(QOpenGLShader::Vertex, vs1_vert)) {
    throw std::runtime_error(program_->log().toStdString());
  }
  if (!program_->addShaderFromSourceCode(QOpenGLShader::Fragment, fs1_frag)) {
    throw std::runtime_error(program_->log().toStdString());
  }
  if (!program_->link()) {
    throw std::runtime_error(program_->log().toStdString());
  }

  // 1. bind VAO
  delete vao_;
  vao_ = new QOpenGLVertexArrayObject;
  if (vao_->create()) {
    vao_->bind();
  }

  program_->bind();

  // 2. copy & set attribute
  delete vbo_;
  vbo_ = new QOpenGLBuffer;
  vbo_->create();
  vbo_->bind();
  vbo_->setUsagePattern(QOpenGLBuffer::StaticDraw);
  vbo_->allocate(vertices, sizeof(vertices));
  program_->enableAttributeArray(0);
  program_->enableAttributeArray(1);
  // f->glEnableVertexAttribArray(0);
  // f->glEnableVertexAttribArray(1);
  program_->setAttributeBuffer(0, GL_FLOAT, 0, 3, 5 * sizeof(float));
  // f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  program_->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 2, 5 * sizeof(float));
  // f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));

  // delete ebo_;
  // ebo_ = new QOpenGLBuffer;
  // ebo_->create();
  // ebo_->bind();
  // ebo_->setUsagePattern(QOpenGLBuffer::StaticDraw);
  // ebo_->allocate(indices, sizeof(indices));

  // 4. unbind
  // do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO
  // bound.
  program_->release();
  vao_->release();

  // Load img
  QImage img(":/texture/wall.jpg");
  Q_ASSERT(!img.isNull());
  delete texture_;
  texture_ = new QOpenGLTexture(img.mirrored());

  // set warp
  // f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  // f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  texture_->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
  texture_->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);

  // set filter type
  // f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  texture_->setMagnificationFilter(QOpenGLTexture::Linear);
  texture_->setMinificationFilter(QOpenGLTexture::Linear);

  // Load pic to texture
  // f->glTexImage2D(/*target*/ GL_TEXTURE_2D, /*mipmap level*/ 0, /*save format*/ GL_RGB, width, height, /*always*/ 0,
  //             /*source format*/ GL_RGB, /*data type*/ GL_UNSIGNED_BYTE, data);
  // f->glGenerateMipmap(GL_TEXTURE_2D);  // auto generate mipmap
  texture_->generateMipMaps();

  // set texture
  // f->glUseProgram(shaderProgram);
  // f->glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);
  program_->bind();
  // program_->setUniformValue("ourTexture", 0);
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
  emit UpdateInfo(time_elapsed_, (QString("frame %1 position (%2, %3, %4) pitch %5 yaw %6")
                                      .arg(num)
                                      .arg(camera_->GetPosition().x())
                                      .arg(camera_->GetPosition().y())
                                      .arg(camera_->GetPosition().z())
                                      .arg(camera_->GetAngles().x())
                                      .arg(camera_->GetAngles().y()))
                                     .toUtf8());
  time_.restart();

  QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

  // process io
  for (auto &key : keys_) {
    camera_->KeyboardCallback(key, time_elapsed_ / 1e9);
  }
  auto mouse_delta = mouse_pos_ - mouse_last_pos_;
  if (mouse_is_pressed_) {
    mouse_last_pos_ = mouse_pos_;
  } else {
    mouse_delta = {0, 0};
  }
  if (mouse_is_pressed_ || mouse_scroll_delta_ != 0) {
    camera_->MouseCallback(mouse_delta.x(), mouse_delta.y(), mouse_scroll_delta_, time_elapsed_ / 1e9);
    mouse_scroll_delta_ = 0;
  }

  // clear
  f->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // use program
  program_->bind();

  // bind textures
  // glActiveTexture(GL_TEXTURE0);  // 在绑定纹理之前先激活纹理单元
  // glBindTexture(GL_TEXTURE_2D, texture);
  texture_->bind();

  // view
  program_->setUniformValue("view", camera_->GetViewMatrix());

  // projection
  program_->setUniformValue("projection", camera_->GetProjectionMatrix());
  update();

  // draw
  vao_->bind();
  for (unsigned int i = 0; i < 10; ++i) {
    QMatrix4x4 model;
    model.setToIdentity();
    model.translate(cube_positions[i]);
    // 沿x轴旋转
    model.rotate(static_cast<float>((QDateTime::currentMSecsSinceEpoch() / 10) % 360), QVector3D(0.5f, 1.0f, 0.0f));
    program_->setUniformValue("model", model);

    f->glDrawArrays(GL_TRIANGLES, 0, 36);
  }

  vao_->release();
  program_->release();
}
void MainGLWindow::resizeGL(int w, int h) {
  // projection
  camera_->SetAspect(static_cast<float>(w) / h);
  program_->setUniformValue("projection", camera_->GetProjectionMatrix());
  update();
}
void MainGLWindow::keyPressEvent(QKeyEvent *event) {
  auto key = event->key();
  if (key == Qt::Key_W || key == Qt::Key_S || key == Qt::Key_A || key == Qt::Key_D) {
    keys_.emplace(static_cast<Qt::Key>(key));
  } else {
    event->ignore();
  }
}

void MainGLWindow::keyReleaseEvent(QKeyEvent *event) {
  auto key = event->key();
  if (key == Qt::Key_W || key == Qt::Key_S || key == Qt::Key_A || key == Qt::Key_D) {
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
