//
// Created by zyc on 2022/1/15
//

#ifndef GLDEMO_SRC_BOUNDINGBOX_BOUNDINGBOX_H_
#define GLDEMO_SRC_BOUNDINGBOX_BOUNDINGBOX_H_

#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>
#include <set>
#include <vector>

#include "opengl/GLModel.h"
#include "types/GLTypes.h"

class GLBoundingbox {
 private:
  QVector3D a, b;

 public:
  typedef enum { CUBE_BOUNDINGBOX, SPHERE_BOUNDINGBOX } BoundingboxType;
  BoundingboxType type_;
  GLBoundingbox(const std::vector<GLVertex> &vertices, const BoundingboxType type = CUBE_BOUNDINGBOX) {
    type_ = type;
    if (type_ == CUBE_BOUNDINGBOX) {
      for (auto vertex = vertices.begin(); vertex != vertices.end(); ++vertex) {
        if (vertex->position_[0] < a.x()) {
          a.setX(vertex->position_[0]);
        }
        if (vertex->position_[1] < a.y()) {
          a.setY(vertex->position_[1]);
        }
        if (vertex->position_[2] < a.z()) {
          a.setZ(vertex->position_[2]);
        }
        if (vertex->position_[0] > b.x()) {
          b.setX(vertex->position_[0]);
        }
        if (vertex->position_[1] > b.y()) {
          b.setY(vertex->position_[1]);
        }
        if (vertex->position_[2] > b.z()) {
          b.setZ(vertex->position_[2]);
        }
      }
    }
    if (type_ == SPHERE_BOUNDINGBOX) {
      a.setX(0.0f), a.setY(0.0f), a.setZ(0.0f);
      for (auto vertex = vertices.begin(); vertex != vertices.end(); ++vertex) {
        a += QVector3D(vertex->position_[0] / vertices.size(), vertex->position_[1] / vertices.size(),
                       vertex->position_[2] / vertices.size());
      }

      for (auto vertex = vertices.begin(); vertex != vertices.end(); ++vertex) {
        QVector3D p(vertex->position_[0], vertex->position_[1], vertex->position_[2]);
        if ((a - p).length() > b.x()) {
          b.setX((a - p).length());
        }
      }
      b.setY(0.0f), b.setZ(0.0f);
    }
  }

  inline QVector3D GetMinPoint() const { return a; }
  inline QVector3D GetMaxPoint() const { return b; }
  inline QVector3D GetCenter() const { return a; }
  inline float GetRadius() const { return b.length(); }
};

#endif