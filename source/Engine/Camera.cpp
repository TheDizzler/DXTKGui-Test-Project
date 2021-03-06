#include "../pch.h"
#include "Camera.h"

Camera::Camera() {

	zoom = 1.0f;
	position = Vector2::Zero;
}

Camera::~Camera() {
}

void Camera::setViewport(D3D11_VIEWPORT cameraViewport) {
	viewportWidth = cameraViewport.Width;
	viewportHeight = cameraViewport.Height;
	viewportCenter = Vector3(viewportWidth * .5, viewportHeight * .5, 0);
}

void Camera::setViewport(int vwprtWdth, int vwprtHght) {
	viewportWidth = vwprtWdth;
	viewportHeight = vwprtHght;
	viewportCenter = Vector3(viewportWidth * .5, viewportHeight * .5, 0);
}

void Camera::updateViewport(const Vector2& viewport, const Vector2& viewportPos, bool zoomToFit) {

	viewportWidth = viewport.x - viewportPos.x;
	viewportHeight = viewport.y - viewportPos.y;
	viewportPosition = viewportPos;
	viewportCenter = Vector3((viewportWidth) * .5 + viewportPosition.x,
		(viewportHeight) * .5 + viewportPosition.y, 0);

}

bool Camera::viewContains(const Vector2& point) {
	RECT* rect = viewportWorldBoundary();

	bool contains = rect->left < point.x && point.x < rect->right
		&& rect->top < point.y && point.y < rect->bottom;
	delete rect;
	return contains;
}

float Camera::getZoom() {
	return zoom;
}

void Camera::setZoomToResolution(int width, int height) {
	float xZoom = Globals::WINDOW_WIDTH / width;
	float yZoom = Globals::WINDOW_HEIGHT / height;

	if (xZoom < yZoom)
		zoom = xZoom;
	else
		zoom = yZoom;
}

void Camera::setZoom(float zoomTo) {
	zoom = zoomTo;
	if (zoom < 0.2f)
		zoom = 0.2f;
	else if (zoom > 2.5f)
		zoom = 2.5;
}

void Camera::adjustZoom(float amount) {

	zoom += amount;
	if (zoom < 0.2f)
		zoom = 0.2f;
	else if (zoom > 2.5f)
		zoom = 2.5;

}


void Camera::moveCamera(Vector2 cameraMovement, bool clampToArea) {
	position = position + cameraMovement;
}

const Vector2 Camera::getPosition() const {
	return position;
}


RECT* Camera::viewportWorldBoundary() {

	Vector2* viewportCorner = screenToWorldPtr(Vector2::Zero);
	Vector2* viewportBottomCorner =
		screenToWorldPtr(Vector2(viewportWidth, viewportHeight));

	RECT* rect = new RECT{
		(int) viewportCorner->x, (int) viewportCorner->y,
		(int) (viewportBottomCorner->x - viewportCorner->x),
		(int) (viewportBottomCorner->y - viewportCorner->y)};

	delete viewportCorner;
	delete viewportBottomCorner;

	return rect;
}


void Camera::centerOn(Vector2 pos, bool clampToArea) {

	position = pos;
}

Vector2& Camera::worldToScreen(Vector2 worldPosition) {
	return Vector2::Transform(worldPosition, translationMatrix());
}

Vector2* Camera::screenToWorldPtr(Vector2 onScreenPosition) {

	Vector2* vec = new Vector2();
	Vector2::Transform(onScreenPosition, translationMatrix().Invert(), *vec);

	return vec;
}

Vector2 Camera::screenToWorld(Vector2 onScreenPosition) {
	return Vector2::Transform(onScreenPosition, translationMatrix().Invert());
}



Matrix Camera::translationMatrix() {
	// casting to int prevents filtering artifacts??
	return Matrix::CreateTranslation(-(int) position.x, -(int) position.y, 0)
		* Matrix::CreateRotationZ(rotation)
		* Matrix::CreateScale(zoom, zoom, 1)
		* Matrix::CreateTranslation(viewportCenter);
}
