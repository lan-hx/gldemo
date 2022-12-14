//
// Created by lan on 2023/1/8.
//

#include "opengl/GLModel.h"

#include <QFile>
#include <iostream>
#include <map>

using namespace std;

bool GLModel::Load(const std::string &path, const std::string &texture, QOpenGLShaderProgram *shader) {
  if (!LoadObj(path)) {
    return false;
  }

  SetupVao(shader);

  return LoadTexture(texture);
}

bool GLModel::LoadObj(const string &path) {
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
    return false;
  }
  if (!reader_.Warning().empty()) {
    std::cout << "[WARNING] TinyObjReader: " << reader_.Warning();
  }
  obj_file.close();

  auto &attrib = reader_.GetAttrib();
  auto &shapes = reader_.GetShapes();
  auto &materials = reader_.GetMaterials();

  materials_.clear();
  // materials_.resize(materials.size());

  std::vector<GLVertex> vertices;
  std::vector<uint32_t> indices;
  unordered_map<GLVertex, uint32_t> unique_vertices;

  // for (const auto &material : materials) {
  //   GLMaterial m;
  //
  //   // get needed material
  //
  //   materials_.emplace_back(m);
  // }

  auto &default_material = materials_.emplace_back(GLMaterial());
  default_material.ka_ = QVector3D(1.0f, 1.0f, 1.0f);
  default_material.kd_ = QVector3D(1.0f, 1.0f, 1.0f);
  default_material.ks_ = QVector3D(1.0f, 1.0f, 1.0f);
  default_material.ns_ = 10.0f;

  for (const auto &shape : shapes) {
    for (const auto &index : shape.mesh.indices) {
      GLVertex vertex;
      vertex.position_[0] = attrib.vertices[3 * index.vertex_index + 0];
      vertex.position_[1] = attrib.vertices[3 * index.vertex_index + 1];
      vertex.position_[2] = attrib.vertices[3 * index.vertex_index + 2];
      if (index.normal_index >= 0) {
        vertex.normal_[0] = attrib.normals[3 * index.normal_index + 0];
        vertex.normal_[1] = attrib.normals[3 * index.normal_index + 1];
        vertex.normal_[2] = attrib.normals[3 * index.normal_index + 2];
      }
      if (index.texcoord_index >= 0) {
        vertex.texcoord_[0] = attrib.texcoords[2 * index.texcoord_index + 0];
        vertex.texcoord_[1] = attrib.texcoords[2 * index.texcoord_index + 1];
      }
      if (!attrib.colors.empty()) {
        vertex.color_[0] = attrib.colors[3 * index.vertex_index + 0];
        vertex.color_[1] = attrib.colors[3 * index.vertex_index + 1];
        vertex.color_[2] = attrib.colors[3 * index.vertex_index + 2];
      }

      if (!unique_vertices.contains(vertex)) {
        unique_vertices[vertex] = static_cast<uint32_t>(vertices.size());
        vertices.push_back(vertex);
      }

      indices.push_back(unique_vertices[vertex]);
    }
  }

  vertices_ = vertices;
  indices_ = indices;

  return true;
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
  cerr << path_ << ": " << vao_->objectId() << endl;
  shader->bind();
  vbo_ = new QOpenGLBuffer;
  vbo_->create();
  vbo_->bind();
  vbo_->setUsagePattern(QOpenGLBuffer::StaticDraw);
  vbo_->allocate(vertices_.data(), sizeof(GLVertex) * vertices_.size());
  ebo_ = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
  ebo_->create();
  ebo_->bind();
  ebo_->setUsagePattern(QOpenGLBuffer::StaticDraw);
  ebo_->allocate(indices_.data(), sizeof(uint32_t) * indices_.size());

  shader->enableAttributeArray(0);
  shader->setAttributeBuffer(0, GL_FLOAT, offsetof(GLVertex, position_), 3, sizeof(GLVertex));
  shader->enableAttributeArray(1);
  shader->setAttributeBuffer(1, GL_FLOAT, offsetof(GLVertex, normal_), 3, sizeof(GLVertex));
  shader->enableAttributeArray(2);
  shader->setAttributeBuffer(2, GL_FLOAT, offsetof(GLVertex, texcoord_), 2, sizeof(GLVertex));
  shader->enableAttributeArray(3);
  shader->setAttributeBuffer(3, GL_FLOAT, offsetof(GLVertex, color_), 3, sizeof(GLVertex));

  shader->release();
  vao_->release();
}

bool GLModel::LoadTexture(const std::string &texture, const std::function<void(QOpenGLTexture *)> &setting) {
  if (texture.empty()) {
    QImage img(":/texture/white.png");
    Q_ASSERT(!img.isNull());
    texture_path_ = texture;
    delete texture_;
    texture_ = new QOpenGLTexture(img.mirrored());
  } else {
    QImage img(texture.c_str());
    if (img.isNull()) {
      cerr << "[ERROR] image invalid, texture load failed: " << texture << endl;
      return false;
    }
    texture_path_ = texture;
    delete texture_;
    texture_ = new QOpenGLTexture(img.mirrored());

    setting(texture_);
  }
  return true;
}
void GLModel::SetMaterial(QOpenGLShaderProgram *shader) {
  shader->bind();
  shader->setUniformValue("material.ka", materials_[0].ka_);
  shader->setUniformValue("material.kd", materials_[0].kd_);
  shader->setUniformValue("material.ks", materials_[0].ks_);
  shader->setUniformValue("material.ns", materials_[0].ns_);
  shader->release();
}
