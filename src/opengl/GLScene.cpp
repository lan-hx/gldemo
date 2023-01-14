//
// Created by lan on 2023/1/8.
//

#include "include/opengl/GLScene.h"

#include <QOpenGLExtraFunctions>
#include <fstream>

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
  ofstream f_compile("compile.log");
  f_compile << "compile log begin..." << endl;
  if (!shader_->addShaderFromSourceCode(QOpenGLShader::Vertex, vs_light_vert)) {
    f_compile << "Vert shader compile failed:\n" << shader_->log().toStdString() << endl;
    throw std::runtime_error(shader_->log().toStdString());
  }
  if (!shader_->addShaderFromSourceCode(QOpenGLShader::Fragment, fs_light_frag)) {
    f_compile << "Frag shader compile failed:\n" << shader_->log().toStdString() << endl;
    throw std::runtime_error(shader_->log().toStdString());
  }
  if (!shader_->link()) {
    f_compile << "link failed:\n" << shader_->log().toStdString() << endl;
    throw std::runtime_error(shader_->log().toStdString());
  }
  f_compile.close();

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
  for (const auto &dobj : DEFAULT_OBJECTS) {
    auto pmodel = GetModel(get<0>(dobj));
    auto obj = new GLObject(pmodel, shader_);
    obj->transform_.position_ = get<1>(dobj);
    obj->transform_.SetAngles(get<1>(dobj));
    obj->transform_.scale_ = get<3>(dobj);
    AddObject(obj);
  }

  lights_->CheckOffsets(shader_);
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
