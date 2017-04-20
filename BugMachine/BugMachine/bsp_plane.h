#ifndef BSP_PLANE_H
#define BSP_PLANE_H

#define DLLexport __declspec(dllexport)

#include "Renderer.h"
#include "Camera.h"
#include "Entity3D.h"

class Entity3D;

class bsp_plane {

public:
	DLLexport bsp_plane();
	DLLexport ~bsp_plane();
	DLLexport void bsp_ConstructPlane();
	DLLexport void bsp_DoBSP(Entity3D& aabb, Camera* cam);

	Plane m_plane;

};
#endif