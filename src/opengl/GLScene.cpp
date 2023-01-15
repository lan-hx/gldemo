//
// Created by lan on 2023/1/8.
//

#include "include/opengl/GLScene.h"

#include <QOpenGLExtraFunctions>

#include "fs_light_frag.h"
#include "vs_light_vert.h"

using namespace std;

GLScene::GLScene(QRect viewport, QObject *parent)
    : viewport_(viewport),
      camera_(new GLCamera({0.0f, 0.0f, 3.0f}, static_cast<float>(viewport.width()) / viewport.height())),
      lights_(new GLLights),
      QObject(parent) {}

void GLScene::Initialize(const std::vector<std::pair<std::string, std::string>> &models) {
  // initialize shader program
  delete shader_;
  shader_ = new QOpenGLShaderProgram;
  if (!shader_->addShaderFromSourceCode(QOpenGLShader::Vertex, vs_light_vert)) {
    throw std::runtime_error(shader_->log().toStdString());
  }
  qDebug() << "[INFO] GLScene::Initialize: build vertex shader log: " << shader_->log();
  if (!shader_->addShaderFromSourceCode(QOpenGLShader::Fragment, fs_light_frag)) {
    throw std::runtime_error(shader_->log().toStdString());
  }
  qDebug() << "[INFO] GLScene::Initialize: build frag shader log: " << shader_->log();
  if (!shader_->link()) {
    throw std::runtime_error(shader_->log().toStdString());
  }
  qDebug() << "[INFO] GLScene::Initialize: link shader log: " << shader_->log();

  // load models
  if (models.empty()) {
    for (const auto &m : DEFAULT_MODELS) {
      AddModel(get<0>(m), new GLModel(get<0>(m), get<1>(m), shader_));
    }
  } else {
    for (const auto &m : models) {
      AddModel(m.first, new GLModel(m.first, m.second, shader_));
    }
  }

  // load objects
  bool major_loaded = false;
  for (const auto &dobj : DEFAULT_OBJECTS) {
    auto pmodel = GetModel(get<0>(dobj));
    auto obj = new GLObject(pmodel, shader_, !major_loaded);
    if (!major_loaded) {
      major_loaded = true;
      major_object_ = obj;
    }
    obj->transform_.position_ = get<1>(dobj);
    obj->transform_.SetAngles(get<1>(dobj));
    obj->transform_.scale_ = get<3>(dobj);
    AddObject(obj);
  }

<<<<<<< HEAD
  camera_->bind_object(major_object_);

  // major_object_ = objects_[0];

=======
  lights_->CheckOffsets(shader_);
>>>>>>> origin/fancy
  lights_->SetupShader(shader_, "Lights");
  AddLight(GLLights::AddAmbientLight({1.0f, 1.0f, 1.0f}, 0.2f));
  AddLight(GLLights::AddSpotLight({0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, 1.0f, 1.047198f, 1.0f,
                                  0.0f, 1.0f));
}
void GLScene::Draw() {
  QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
  // clear
  f->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // set uniform for all active shaders
  shader_->bind();
  shader_->setUniformValue("view", camera_->GetViewMatrix());
  shader_->setUniformValue("projection", camera_->GetProjectionMatrix());
  shader_->setUniformValue("viewPos", camera_->GetPosition());
  shader_->release();

  lights_->Use();

  // draw
  for (auto &obj : objects_) {
    obj.second->Draw();
  }

  lights_->Release();
}

// TODO
GLScene::~GLScene() {
  delete camera_;
  for (auto &obj : objects_) {
    delete obj.second;
  }
  for (auto &m : models_) {
    delete m.second;
  }
  delete shader_;
}

void GLScene::KeyboardCallback(Qt::Key key, float time_elapsed) {
  QVector3D dir;
  switch (key) {
    case Qt::Key_W:
      dir = major_object_->transform_.GetRight().normalized();
      break;
    case Qt::Key_S:
      dir = -major_object_->transform_.GetRight().normalized();
      break;
    case Qt::Key_A:
      dir = major_object_->transform_.GetFront().normalized();
      break;
    case Qt::Key_D:
      dir = -major_object_->transform_.GetFront().normalized();
      break;
    case Qt::Key_Space:
      dir = major_object_->transform_.GetUp().normalized();
      break;
    case Qt::Key_Shift:
      dir = -major_object_->transform_.GetUp().normalized();
      break;
    default:;
  }
  // camera_->move(dir, time_elapsed);
  bool collision = true;
  while (collision) {
    if (dir.length() < 0.05f) break;
    camera_->move(dir, time_elapsed);
    collision = false;
    for (auto &iter : objects_) {
      auto obj = iter.second;
      if (obj->GetID() != major_object_->GetID()) {
        if (major_object_->CollideWith(obj)) {
          collision = true;
          break;
        }
      }
    }
    if (collision) {
      camera_->move(-dir, time_elapsed);
    }
    dir = 0.5f * dir;
  }
  if (collision) {
    camera_->move(-dir * 2, time_elapsed);
  }
  // emit ValueChanged();
}

void GLScene::MouseCallBack(float xoffset, float yoffset, float scroll_offset, float time_elapsed) {
  camera_->MouseCallback(xoffset, yoffset, scroll_offset, time_elapsed);
}