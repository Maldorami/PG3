#include "Camera.h"

#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")
#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")

Camera::Camera(Renderer& renderer)
	:
	render(NULL),
	posX(0),
	posY(0),
	posZ(0),
	LAposX(0),
	LAposY(0),
	LAposZ(1.0f),
	UPposX(0),
	UPposY(1.0),
	UPposZ(0),
	_up(new D3DXVECTOR3 (0.0f, 1.0f, 0.0f)),
	_forward(new D3DXVECTOR3(0.0f, 0.0f, 1.0f)),
	_right(new D3DXVECTOR3(1.0f, 0.0f, 0.0f)),
	_lookAt(new D3DXVECTOR3(0.0f, 0.0f, 1.0f)),
	_pos(new D3DXVECTOR3(0.0f, 0.0f, -1000.0f)),
	_localView(new D3DXMATRIX()),
	frustum(new Frustum(renderer))
{
	setRender(renderer);
	update();
}
Camera::~Camera(){
	delete	_up			;
	delete	_forward	;
	delete	_right		;
	delete	_lookAt		;
	delete	_localView	;
	delete	frustum		;
	delete	_pos		;

	_up			= NULL;
	_forward	= NULL;
	_right		= NULL;
	_lookAt		= NULL;
	_localView	= NULL;
	frustum		= NULL;
	_pos		= NULL;
}
void Camera::update(){
	// Actualizo loockAt
	*_lookAt = *_pos + *_forward;

	// Calcular la nueva matrix de view
	D3DXMatrixLookAtLH(_localView, _pos, _lookAt, _up);

	// Setear la transformacion
	render->d3ddev->SetTransform(D3DTS_VIEW, _localView);
}
void Camera::roll(float angle){

	if (angle == 0.0f)
	{
		return;
	}
	
	D3DXMATRIX rotation;
	D3DXMatrixRotationAxis(&rotation, _forward, D3DXToRadian(angle));
	D3DXVec3TransformNormal(_right, _right, &rotation);
	D3DXVec3TransformNormal(_up, _up, &rotation);

	update();
}
void Camera::pitch(float angle){

	if (angle == 0.0f)
	{
		return;
	}

	D3DXMATRIX rotation;
	D3DXMatrixRotationAxis(&rotation, _right, D3DXToRadian(angle));
	
	//D3DXVec3TransformNormal(&_up, &_up, &rotation);
	D3DXVec3TransformNormal(_forward, _forward, &rotation);

	update();
}
void Camera::yaw(float angle){

	if (angle == 0.0f)
	{
		return;
	}

	D3DXMATRIX rotation;
	D3DXMatrixRotationAxis(&rotation, _up, D3DXToRadian(angle));
	D3DXVec3TransformNormal(_right, _right, &rotation);
	D3DXVec3TransformNormal(_forward, _forward, &rotation);

	update();
}
void Camera::walk(float distance){
	*_pos += *_forward * distance;

	update();
}

void Camera::strafe(float distance){
	*_pos += *_right * distance;

	update();
}

void Camera::fly(float distance){
	*_pos += *_up * distance;

	update();
} 

void Camera::setRender(Renderer& rendi){
	render = &rendi;
}

const Frustum& Camera::getFrustum() const{
	return *frustum;
}

void Camera::updateFrustum(){
	frustum->ConstructFrustum(render->screenDepth, render->projectionMatrix, _localView);
}

void Camera::setFrustum(Frustum* _frustum){
	frustum = _frustum;
}