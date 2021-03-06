#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

/** Based on
https://roguesharp.wordpress.com/2014/07/13/tutorial-5-creating-a-2d-camera-with-pan-and-zoom-in-monogame/ */
class Camera {
public:
	Camera();
	/*Camera(int viewportWidth, int viewportHeight);
	Camera(const Vector2& viewport);*/
	virtual ~Camera();

	void setViewport(D3D11_VIEWPORT cameraViewport);
	void setViewport(int viewportWidth, int viewportHeight);
	void updateViewport(const Vector2& viewportArea,
		const Vector2& viewportPosition, bool zoomToFit = false);


	float rotation = 0.0f;

	Vector2 viewportPosition;
	int viewportWidth;
	int viewportHeight;
	Vector3 viewportCenter;

	bool viewContains(const Vector2& point);
	
	float getZoom();
	void setZoomToResolution(int width, int height);
		
	void setZoom(float zoomAmount);
	void adjustZoom(float amount);
	/** Move the camera in an X and Y amount based on the cameraMovement param.
	*	If clampToMap is true the camera will try not to pan outside of the
	*	bounds of the map. */
	void moveCamera(Vector2 cameraMovement, bool clampToArea = false);
	const Vector2 getPosition() const;

	RECT* viewportWorldBoundary();

	void centerOn(Vector2 pos, bool clampToArea = false);

	Matrix translationMatrix();
	Vector2& worldToScreen(Vector2 worldPosition);
	Vector2* screenToWorldPtr(Vector2 onScreenPosition);
	Vector2 screenToWorld(Vector2 onScreenPosition);
private:

	Vector2 position;

	float zoom;


};