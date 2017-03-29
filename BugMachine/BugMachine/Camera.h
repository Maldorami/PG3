#ifndef CAMERA_H
#define CAMERA_H

#include "Frustum.h"

#define DLLexport __declspec(dllexport)

struct D3DXVECTOR3;
typedef D3DXVECTOR3* Vector3;

class Camera{
public:
	DLLexport Camera(Renderer& render);
	DLLexport ~Camera();
	//Rotation
	DLLexport void roll(float angle);
	DLLexport void pitch(float angle);
	DLLexport void yaw(float angle);

	//Movement
	DLLexport void walk(float distance);
	DLLexport void strafe(float distance);
	DLLexport void fly(float distance);

	// frustum
	DLLexport const Frustum& getFrustum() const;
	DLLexport void setFrustum(Frustum* _frustum);
	DLLexport void updateFrustum();
	Frustum* frustum;

	DLLexport void update();

	DLLexport void setRender(Renderer& rendi);
	Renderer* render;

	//Camera Settings
	float posX;
	float posY;
	float posZ;
	float LAposX;
	float LAposY;
	float LAposZ;
	float UPposX;
	float UPposY;
	float UPposZ;

	Vector3 _up;
	Vector3 _lookAt;
	Vector3 _forward;
	Vector3 _right;
	Vector3 _pos;

	Matrix	_localView;

};
#endif