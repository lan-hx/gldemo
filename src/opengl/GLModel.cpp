//
// Created by lan on 2023/1/8.
//

#include "opengl/GLModel.h"

#include <QFile>
#include <iostream>

using namespace std;

void GLModel::Load(const std::string &path, const std::string &texture, QOpenGLShaderProgram *shader) {
  LoadObj(path);
  auto &attrib = reader_.GetAttrib();
  auto &shapes = reader_.GetShapes();
  auto &materials = reader_.GetMaterials();

  SetupVao(shader);

  LoadTexture(texture);
}

void GLModel::LoadObj(const string &path) {
  path_ = path;

  // get obj string
  QFile obj_file(path.c_str());
  obj_file.open(QIODevice::ReadOnly);

  // parse obj
  tinyobj::ObjReaderConfig reader_config;

  if (!reader_.ParseFromString(obj_file.readAll().toStdString(), "")) {
    if (!reader_.Error().empty()) {
      cerr << "[ERROR] TinyObjReader: " << reader_.Error();
    }
    throw runtime_error(reader_.Error());
  }
  if (!reader_.Warning().empty()) {
    std::cout << "[WARNING] TinyObjReader: " << reader_.Warning();
  }
  obj_file.close();
}

void GLModel::SetupVao(QOpenGLShaderProgram *shader) {
  auto &attrib = reader_.GetAttrib();
  auto &shapes = reader_.GetShapes();
  auto &materials = reader_.GetMaterials();

  // TODO: setup vao, vbo, ebo
  delete vao_;
  delete vbo_;
  delete ebo_;
  vao_ = new QOpenGLVertexArrayObject;
  if (vao_->create()) {
    vao_->bind();
  }
  shader->bind();
  vbo_ = new QOpenGLBuffer;
  vbo_->create();
  vbo_->bind();
  vbo_->setUsagePattern(QOpenGLBuffer::StaticDraw);
  // vbo_->allocate(vertices, sizeof(vertices));
  // shader->enableAttributeArray(0);
  // shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, 5 * sizeof(float));

  // ebo_ = new QOpenGLBuffer;
  // ebo_->create();
  // ebo_->bind();
  // ebo_->setUsagePattern(QOpenGLBuffer::StaticDraw);
  // ebo_->allocate(indices, sizeof(indices));

  shader->release();
  vao_->release();
}

void GLModel::LoadTexture(const std::string &texture, const std::function<void(QOpenGLTexture *)> &setting) {
  QImage img(texture.c_str());
  Q_ASSERT(!img.isNull());
  delete texture_;
  texture_ = new QOpenGLTexture(img.mirrored());

  setting(texture_);
}
