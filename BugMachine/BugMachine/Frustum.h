#ifndef FRUSTUM_H
#define FRUSTUM_H

#define DLLexport __declspec(dllexport)

#include "Renderer.h"

class Frustum{

public:
	DLLexport Frustum(Renderer& rkRenderer);
	DLLexport ~Frustum();
	DLLexport void ConstructFrustum(float screenDepth, Matrix& projectionMatrix, Matrix& viewMatrix);

	DLLexport CollisionResult CheckCollision(const AABB& aabb);
	Renderer rendi;
	Plane* m_planes;
	Matrix matrix;
private:
	const int planes = 6;

};
#endif