//
// Created by lan on 2023/1/8.
//

#include "opengl/GLModel.h"

#include <QFile>
#include <iostream>
#include <map>

using namespace std;

void GLModel::Load(const std::string &path, const std::string &texture, QOpenGLShaderProgram *shader) {
  LoadObj(path);
  auto &attrib = reader_.GetAttrib();
  auto &shapes = reader_.GetShapes();
  auto &materials = reader_.GetMaterials();
  _materials.clear();
  _materials.resize(materials.size());

  std::vector<GLVertex> vertices;
  std::vector<uint32_t> indices;
  unordered_map<GLVertex, uint32_t> uniqueVertices;

  for (const auto& material : materials){
    GLMaterial m;

    // TODO: get needed material

    _materials.emplace_back(m);
  }

  for (const auto& shape : shapes){
    for (const auto& index : shape.mesh.indices){
      GLVertex vertex;
      vertex.position[0] = attrib.vertices[3 * index.vertex_index + 0];
      vertex.position[1] = attrib.vertices[3 * index.vertex_index + 1];
      vertex.position[2] = attrib.vertices[3 * index.vertex_index + 2];
      if (index.normal_index >= 0){
        vertex.normal[0] = attrib.normals[3 * index.normal_index + 0];
        vertex.normal[1] = attrib.normals[3 * index.normal_index + 1];
        vertex.normal[2] = attrib.normals[3 * index.normal_index + 2];
      }
      if (index.texcoord_index >= 0){
        vertex.texcoord[0] = attrib.texcoords[3 * index.texcoord_index + 0];
        vertex.texcoord[1] = attrib.texcoords[3 * index.texcoord_index + 1];
      }
      if (!attrib.colors.empty()){
        vertex.color[0] = attrib.colors[3 * index.vertex_index + 0];
        vertex.color[1] = attrib.colors[3 * index.vertex_index + 1];
        vertex.color[2] = attrib.colors[3 * index.vertex_index + 2];
      }

      if (uniqueVertices.count(vertex) == 0) {
          uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
          vertices.push_back(vertex);
      }

      indices.push_back(uniqueVertices[vertex]);
    }
  }

  _vertices = vertices;
  _indices = indices;


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
  vbo_->allocate(_vertices.data(), sizeof(GLVertex) * _vertices.size());
  ebo_ = new QOpenGLBuffer;
  ebo_->create();
  ebo_->bind();
  ebo_->setUsagePattern(QOpenGLBuffer::StaticDraw);
  ebo_->allocate(_indices.data(), sizeof(uint32_t) * _indices.size());

  shader->enableAttributeArray(0);
  shader->setAttributeBuffer(0, GL_FLOAT, offsetof(GLVertex, position), 3, sizeof(GLVertex));
  shader->enableAttributeArray(1);
  shader->setAttributeBuffer(1, GL_FLOAT, offsetof(GLVertex, normal), 3, sizeof(GLVertex));
  shader->enableAttributeArray(2);
  shader->setAttributeBuffer(2, GL_FLOAT, offsetof(GLVertex, texcoord), 2, sizeof(GLVertex));
  shader->enableAttributeArray(3);
  shader->setAttributeBuffer(3, GL_FLOAT, offsetof(GLVertex, color), 3, sizeof(GLVertex));
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
