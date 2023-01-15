//
// Created by lan on 2023/1/8.
//

#include "opengl/GLObject.h"

#include <QOpenGLFunctions>
#include <algorithm>
#include <cmath>

uint64_t GLObject::id_inc = 0;

void GLObject::Draw() {
  auto f = QOpenGLContext::currentContext()->functions();
  model_->SetMaterial(shader_);
  model_->GetTexture()->bind();
  shader_->bind();
  model_->GetVao()->bind();
  shader_->setUniformValue("model", transform_.GetModelMatrix());
  f->glDrawElements(GL_TRIANGLES, model_->indices_.size(), GL_UNSIGNED_INT, nullptr);
  model_->GetVao()->release();
  shader_->release();
}

bool GLObject::CollideWith(GLObject *obj) {
  if (boundingbox_.type_ == GLBoundingbox::BoundingboxType::SPHERE_BOUNDINGBOX) {
    if (obj->boundingbox_.type_ == GLBoundingbox::BoundingboxType::CUBE_BOUNDINGBOX) {
      QVector3D center = boundingbox_.GetCenter();
      float radius = boundingbox_.GetRadius();
      QVector3D min_p = obj->boundingbox_.GetMinPoint();
      QVector3D max_p = obj->boundingbox_.GetMaxPoint();
      QMatrix4x4 trans1 = transform_.GetModelMatrix();
      QMatrix4x4 trans2 = obj->transform_.GetModelMatrix();
      center = (trans1 * QVector4D(center, 1.0f)).toVector3D();
      radius = transform_.scale_.x() * radius;
      min_p = (trans2 * QVector4D(min_p, 1.0f)).toVector3D();
      max_p = (trans2 * QVector4D(max_p, 1.0f)).toVector3D();
      QVector3D x_dir(1.0f, 0.0f, 0.0f);
      QVector3D y_dir(0.0f, 1.0f, 0.0f);
      QVector3D z_dir(0.0f, 0.0f, 1.0f);
      x_dir = (trans2 * QVector4D(x_dir, 0.0f)).toVector3D();
      y_dir = (trans2 * QVector4D(y_dir, 0.0f)).toVector3D();
      z_dir = (trans2 * QVector4D(z_dir, 0.0f)).toVector3D();
      Plane p1(min_p, QVector3D::crossProduct(x_dir, y_dir).normalized());
      Plane p2(min_p, QVector3D::crossProduct(y_dir, z_dir).normalized());
      Plane p3(min_p, QVector3D::crossProduct(z_dir, x_dir).normalized());
      Plane p4(max_p, QVector3D::crossProduct(y_dir, x_dir).normalized());
      Plane p5(max_p, QVector3D::crossProduct(z_dir, y_dir).normalized());
      Plane p6(max_p, QVector3D::crossProduct(x_dir, z_dir).normalized());
      // float dis1sq = pow(std::max(p1.getSignedDistanceToPoint(center), 0.0f), 2);
      // float dis2sq = pow(std::max(p2.getSignedDistanceToPoint(center), 0.0f), 2);
      // float dis3sq = pow(std::max(p3.getSignedDistanceToPoint(center), 0.0f), 2);
      // float dis4sq = pow(std::max(p4.getSignedDistanceToPoint(center), 0.0f), 2);
      // float dis5sq = pow(std::max(p5.getSignedDistanceToPoint(center), 0.0f), 2);
      // float dis6sq = pow(std::max(p6.getSignedDistanceToPoint(center), 0.0f), 2);

      // qDebug<< obj->GetID()<< " "<< dis2sq<< " "<< dis5sq<< "\n";
      float dis = sqrt(pow(std::max(p1.getSignedDistanceToPoint(center), 0.0f), 2) +
                       pow(std::max(p2.getSignedDistanceToPoint(center), 0.0f), 2) +
                       pow(std::max(p3.getSignedDistanceToPoint(center), 0.0f), 2) +
                       pow(std::max(p4.getSignedDistanceToPoint(center), 0.0f), 2) +
                       pow(std::max(p5.getSignedDistanceToPoint(center), 0.0f), 2) +
                       pow(std::max(p6.getSignedDistanceToPoint(center), 0.0f), 2));
      return dis < radius;
    }
    if (obj->boundingbox_.type_ == GLBoundingbox::BoundingboxType::SPHERE_BOUNDINGBOX) {
      QVector3D center1 = boundingbox_.GetCenter();
      QVector3D center2 = obj->boundingbox_.GetCenter();
      float radius1 = boundingbox_.GetRadius();
      float radius2 = obj->boundingbox_.GetRadius();
      QMatrix4x4 trans1 = transform_.GetModelMatrix();
      QMatrix4x4 trans2 = obj->transform_.GetModelMatrix();
      center1 = (trans1 * QVector4D(center1, 1.0f)).toVector3D();
      center2 = (trans2 * QVector4D(center2, 1.0f)).toVector3D();
      radius1 *= transform_.scale_.x();
      radius2 *= obj->transform_.scale_.x();
      return (center1 - center2).length() < radius1 + radius2;
    }
  }
  if (boundingbox_.type_ == GLBoundingbox::BoundingboxType::CUBE_BOUNDINGBOX) {
    if (obj->boundingbox_.type_ == GLBoundingbox::BoundingboxType::SPHERE_BOUNDINGBOX) {
      return obj->CollideWith(this);
    }
  }
  return false;
}