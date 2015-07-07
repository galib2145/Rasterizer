#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>

#include "canvas.h"
#include "geometry.h"

#define M_PI 3.14159265

const float inchToMm = 25.4;

class Camera {

public:
    float focalLength;
    float filmApertureHeight, filmApertureWidth;
    float nearClippingPlane, farClippingPlane;

    Canvas canvas;

    Matrix44f cameraMatrix, inverseCameraMatrix;

    void calculateCanvasCoordinates();

    Camera(float focalLength, float filmApertureWidth,
           float filmApertureHeight, float nearClippingPlane,
           float farClippingPlane);

    void setCameraPosition(Matrix44f cameraMatrix);

    void getPointInCameraSpace(const Vec3f &pointInWorldSpace, Vec3f &pointInCameraSpace);

    void isVisibleToCamera(const Vec2f &pointInScreenSpace, bool &visible);
};


#endif
