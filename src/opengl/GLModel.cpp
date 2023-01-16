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
      cerr << "[ERROR] load \"" << path << "\" TinyObjReader: " << reader_.Error() << flush;
    }
    return false;
  }
  if (!reader_.Warning().empty()) {
    std::cout << "[WARNING] load \"" << path << "\" TinyObjReader: " << reader_.Warning() << flush;
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
bool GLModel::SaveObj(const string &path) {
  auto &attributes = reader_.GetAttrib();
  auto &shapes = reader_.GetShapes();

  FILE *fp = fopen(path.c_str(), "w");
  if (fp == nullptr) {
    fprintf(stderr, "[ERROR] GLModel::SaveObj: Failed to open file \"%s\" for write.\n", path.c_str());
    return false;
  }

  // facevarying vtx
  for (size_t k = 0; k < attributes.vertices.size(); k += 3) {
    fprintf(fp, "v %f %f %f\n", attributes.vertices[k + 0], attributes.vertices[k + 1], attributes.vertices[k + 2]);
  }

  fprintf(fp, "\n");

  // facevarying normal
  for (size_t k = 0; k < attributes.normals.size(); k += 3) {
    fprintf(fp, "vn %f %f %f\n", attributes.normals[k + 0], attributes.normals[k + 1], attributes.normals[k + 2]);
  }

  fprintf(fp, "\n");

  // facevarying texcoord
  for (size_t k = 0; k < attributes.texcoords.size(); k += 2) {
    fprintf(fp, "vt %f %f\n", attributes.texcoords[k + 0], attributes.texcoords[k + 1]);
  }

  for (size_t i = 0; i < shapes.size(); i++) {
    fprintf(fp, "\n");

    if (shapes[i].name.empty()) {
      fprintf(fp, "g Unknown\n");
    } else {
      fprintf(fp, "g %s\n", shapes[i].name.c_str());
    }

    bool has_vn = false;
    bool has_vt = false;
    // Assumes normals and textures are set shape-wise.
    if (shapes[i].mesh.indices.size() > 0) {
      has_vn = shapes[i].mesh.indices[0].normal_index != -1;
      has_vt = shapes[i].mesh.indices[0].texcoord_index != -1;
    }

    // face
    int face_index = 0;
    for (size_t k = 0; k < shapes[i].mesh.indices.size(); k += shapes[i].mesh.num_face_vertices[face_index++]) {
      unsigned char v_per_f = shapes[i].mesh.num_face_vertices[face_index];
      // Imperformant, but if you want to have variable vertices per face, you need some kind of a dynamic loop.
      fprintf(fp, "f");
      for (int l = 0; l < v_per_f; l++) {
        const tinyobj::index_t &ref = shapes[i].mesh.indices[k + l];
        if (has_vn && has_vt) {
          // v0/t0/vn0
          fprintf(fp, " %d/%d/%d", ref.vertex_index + 1, ref.texcoord_index + 1, ref.normal_index + 1);
          continue;
        }
        if (has_vn && !has_vt) {
          // v0//vn0
          fprintf(fp, " %d//%d", ref.vertex_index + 1, ref.normal_index + 1);
          continue;
        }
        if (!has_vn && has_vt) {
          // v0/vt0
          fprintf(fp, " %d/%d", ref.vertex_index + 1, ref.texcoord_index + 1);
          continue;
        }
        if (!has_vn && !has_vt) {
          // v0 v1 v2
          fprintf(fp, " %d", ref.vertex_index + 1);
          continue;
        }
      }
      fprintf(fp, "\n");
    }
  }

  fclose(fp);

  return true;
}
