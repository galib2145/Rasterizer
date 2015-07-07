#include "camera.h"

Camera::Camera(float focalLength, float filmApertureWidth,
               float filmApertureHeight, float nearClippingPlane,
               float farClippingPlane) {
    this->focalLength = focalLength;
    this->filmApertureWidth = filmApertureWidth;
    this->filmApertureHeight = filmApertureHeight;
    this->nearClippingPlane = nearClippingPlane;
    this->farClippingPlane = farClippingPlane;
    calculateCanvasCoordinates();
}

void Camera::calculateCanvasCoordinates() {
    float perpendicular = (filmApertureWidth / 2) * inchToMm;
	float base = focalLength;
	float tanValue = perpendicular / base;
	float angleOfViewHorizontal = (atan(tanValue) * 180) / M_PI;

	perpendicular = (filmApertureHeight / 2) * inchToMm;
	base = focalLength;
	tanValue = perpendicular / base;
	float angleOfViewVertical = (atan(tanValue) * 180) / M_PI;

	canvas.right = tan((angleOfViewHorizontal * M_PI) / 180 ) * nearClippingPlane;
	canvas.left = -canvas.right;

	canvas.top = tan((angleOfViewVertical * M_PI) / 180) * nearClippingPlane;
	canvas.bottom = -canvas.top;
}

void Camera::setCameraPosition(Matrix44f cameraMatrix) {
    this->cameraMatrix = cameraMatrix;
    this->inverseCameraMatrix = cameraMatrix.inverse();
}

void Camera::getPointInCameraSpace(const Vec3f &pointInWorldSpace, Vec3f &pointInCameraSpace) {
    inverseCameraMatrix.multVecMatrix(pointInWorldSpace, pointInCameraSpace);
}

void Camera::isVisibleToCamera(const Vec2f &pointInScreenSpace, bool &visible) {
    if (pointInScreenSpace.x >= canvas.left && pointInScreenSpace.x <= canvas.right
		&& pointInScreenSpace.y <= canvas.top && pointInScreenSpace.y <= canvas.bottom) {
		visible = false;
	} else {
		visible = true;
	}
}


