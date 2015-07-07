#include <iostream>
#include <cmath>
#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "canvas.h"
#include "geometry.h"
#include "display.h"
#include "camera.h"
#include "cow.h"
#include "mathFunctions.h"


#define M_PI 3.14159265

using namespace std;

string::size_type sz;

typedef unsigned char Rgb[3];

Vec3f c2 = {1, 0, 0};
Vec3f c1 = {0, 1, 0};
Vec3f c0 = {0, 0, 1};

class Triangle {
public:
    Vec3f v0, v1, v2;
    Triangle(Vec3f &v0, Vec3f &v1, Vec3f &v2) {
        this->v0 = v0;
        this->v1 = v1;
        this->v2 = v2;
    }
};

class BoundingBox {
public:
    float xmin, xmax, ymin, ymax;
    BoundingBox(Triangle &triangle) {
        xmin = min3(triangle.v0.x, triangle.v1.x, triangle.v2.x);
        xmax = max3(triangle.v0.x, triangle.v1.x, triangle.v2.x);
        ymin = min3(triangle.v0.y, triangle.v1.y, triangle.v2.y);
        ymax = max3(triangle.v0.y, triangle.v1.y, triangle.v2.y);
    }

    bool isOutOfScreen(Display &display) {
        if (xmin > display.width - 1 || xmax < 0 || ymin > display.height - 1 || ymax < 0)
            return true;
        return false;
    }
};

void loadOBJFile(vector<Vec3f> &vertices, vector<int> &faces) {
    string line;
	ifstream fileStream("test.obj");

	int faceCount = 0;

	while (std::getline(fileStream, line))
	{
		istringstream stringStream(line);
		string word;
		int wordCount = 0;
		bool isFace = false;

		vector<string> values;

		while (stringStream >> word) {
			values.push_back(word);
		}

		if (values[0] == "v") {
			Vec3f vertex;
			vertex.x = std::stof(values[1], &sz);
			vertex.y = std::stof(values[2], &sz);
			vertex.z = std::stof(values[3], &sz);
			vertices.push_back(vertex);
		}

		if (values[0] == "f") {
			faceCount++;
			faces.push_back(std::stoi(values[1], &sz));
			faces.push_back(std::stoi(values[2], &sz));
			faces.push_back(std::stoi(values[3], &sz));
		}
	}
}

Vec3f computePixelCoordinates(Vec3f &pointInWorldSpace, Camera &camera, Display &display, bool &visible) {
	Vec3f pointInCameraSpace;
	camera.getPointInCameraSpace(pointInWorldSpace, pointInCameraSpace);

	Vec2f pointInScreenSpace;
	pointInScreenSpace.x = pointInCameraSpace.x / -pointInCameraSpace.z * camera.nearClippingPlane;
	pointInScreenSpace.y = pointInCameraSpace.y / -pointInCameraSpace.z * camera.nearClippingPlane;

	camera.isVisibleToCamera(pointInScreenSpace, visible);

	Vec2f pointInNDC;
	pointInNDC.x = (2 * pointInScreenSpace.x / (camera.canvas.right -  camera.canvas.left)) - ((camera.canvas.right + camera.canvas.left) / (camera.canvas.right - camera.canvas.left));
	pointInNDC.y = (2 * pointInScreenSpace.y / (camera.canvas.top -  camera.canvas.bottom)) - ((camera.canvas.top + camera.canvas.bottom) / (camera.canvas.top - camera.canvas.bottom));

	Vec3f pointInRasterSpace;
	pointInRasterSpace.x = (int)((pointInNDC.x + 1) / 2 * display.width);
	pointInRasterSpace.y = (int)((1-pointInNDC.y) / 2 * display.height);
	pointInRasterSpace.z = -pointInCameraSpace.z;

	return pointInRasterSpace;
}

int  main() {
    //Matrix44f cameraToWorld(-0.95424, 0, 0.299041, 0, 0.061242, 0.55763, 0.274823, 0, -0.28637, 0.488002, -0.913809, 0, -3.734612, 7.610426, -14.152769, 1);
    Matrix44f cameraToWorld(-0.95424, 0, 0.299041, 0, 0.0861242, 0.95763, 0.274823, 0, -0.28637, 0.288002, -0.913809, 0, -3.734612, 7.610426, -14.152769, 1);

    Camera camera(17, 0.825, 0.825, 0.1, 1000);
    camera.setCameraPosition(cameraToWorld);

    vector<Triangle> triangles;

    int imageHeight = 512;
    int imageWidth = 512;

    Display display(imageHeight, imageWidth);

    Vec3<unsigned char> *frameBuffer = new Vec3<unsigned char>[imageWidth * imageHeight];
    for (uint32_t i = 0; i < imageWidth * imageHeight; ++i)
        frameBuffer[i] = Vec3<unsigned char>(255);

    std::ofstream ofs;
    ofs.open("./pinhole.svg");
    ofs << "<svg version=\"1.1\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" width=\"" << display.width << "\" height=\"" << display.height << "\">" << std::endl;

	int numberOfFaces = 9468 / 3;

	for (int i = 0; i < numberOfFaces; i++) {
		Vec3f vertOne = vertices[nvertices[i * 3]];
		Vec3f vertTwo = vertices[nvertices[i * 3 + 1]];
		Vec3f vertThree = vertices[nvertices[i * 3 + 2]];

		Vec3f v0, v1, v2;
		bool visible;

		if(i==2556) {
            cout << "Peray porsi" << endl;
        }

		v0 = computePixelCoordinates(vertOne, camera, display, visible);
		v1 = computePixelCoordinates(vertTwo, camera, display, visible);
		v2 = computePixelCoordinates(vertThree, camera, display, visible);


		int val = visible ? 0 : 255;
        ofs << "<line x1=\"" << v0.x << "\" y1=\"" << v0.y << "\" x2=\"" << v1.x << "\" y2=\"" << v1.y << "\" style=\"stroke:rgb(" << val << ",0,0);stroke-width:1\" />\n";
        ofs << "<line x1=\"" << v1.x << "\" y1=\"" << v1.y << "\" x2=\"" << v2.x << "\" y2=\"" << v2.y << "\" style=\"stroke:rgb(" << val << ",0,0);stroke-width:1\" />\n";
        ofs << "<line x1=\"" << v2.x << "\" y1=\"" << v2.y << "\" x2=\"" << v0.x << "\" y2=\"" << v0.y << "\" style=\"stroke:rgb(" << val << ",0,0);stroke-width:1\" />\n";

		Triangle triangle(v0, v1, v2);
		triangles.push_back(triangle);
	}

	ofs << "</svg>\n";
    ofs.close();

    cout << "Numbers of triangles : " << triangles.size() << endl;

	for (int i = 0; i < 2600; i++) {
        cout << i << endl;

        if(i==2556) {
            cout << "Peray porsi" << endl;
        }

        BoundingBox boundingBox(triangles[i]);

        if(boundingBox.isOutOfScreen(display)) continue;

        Vec3f v0 = triangles[i].v0, v1 = triangles[i].v1, v2 = triangles[i].v2;
        float area = edgeFunction(v0, v1, v2);

        for(int x = boundingBox.xmin; x <=  boundingBox.xmax; x++) {
            for(int y = boundingBox.ymin; y <= boundingBox.ymax; y++) {
                Vec3f pixelSample(x + 0.5, y + 0.5, 0);

                float bA0 = edgeFunction(v1, v2, pixelSample);
                float bA1 = edgeFunction(v2, v0, pixelSample);
                float bA2 = edgeFunction(v0, v1, pixelSample);

                if (bA0 >= 0 && bA1 >= 0 && bA2 >= 0) {
                    float bC0 = bA0 / area;
                    float bC1 = bA1 / area;
                    float bC2 = bA2 / area;

                    float oneByZ = bC0 / v0.z + bC1 / v1.z + bC2 / v2.z;
                    float z = 1 / oneByZ;

                    float r = bC0 * c0[0] + bC1 * c1[0] + bC2 * c2[0];
                    float g = bC0 * c0[1] + bC1 * c1[1] + bC2 * c2[1];
                    float b = bC0 * c0[2] + bC1 * c1[2] + bC2 * c2[2];

                    r *= z, g *= z, b *= z;

                    frameBuffer[y * imageWidth + x].x = r * 255;
                    frameBuffer[y * imageWidth + x].y = g * 255;
                    frameBuffer[y * imageWidth + x].z = b * 255;
                }
            }
        }
	}

	std::ofstream outfs;
    outfs.open("./raster2d.ppm");
    outfs << "P6\n" << imageWidth << " " << imageHeight << "\n255\n";
    outfs.write((char*)frameBuffer, imageWidth * imageHeight * 3);
    outfs.close();

    delete [] frameBuffer;

    std::cout << "Finished!!" << std::endl;
	cin.get();
	return 0;
}

