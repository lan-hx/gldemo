#ifndef GLDEMO_SRC_TYPES_GLTYPES_H
#define GLDEMO_SRC_TYPES_GLTYPES_H

#include <cmath>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>

inline float dot(const QVector3D &v1, const QVector3D& v2){
    return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

struct Plane {
    QVector3D normal_;
    float signed_distance_;
public:
	Plane() : normal_{ 0.0f, 1.0f, 0.0f }, signed_distance_{ 0.0f } { }
	Plane(const QVector3D& normal, float distance)
		: normal_{ normal.normalized() }, signed_distance_{ distance } { }
	Plane(const QVector3D point, const QVector3D& normal)
		: normal_{ normal.normalized() }, 
		  signed_distance_{ -dot(point, this->normal_) } { }

	float getSignedDistanceToPoint(QVector3D point) const {
		return dot(normal_, point) + signed_distance_;
	}
};

// bool isOnOrForwardPlane(const QVector3D &extents, const QVector3D &center, const Plane &plane) {
// 	return dot(extents, plane.normal_) <= plane.getSignedDistanceToPoint(center);
// }

#endif