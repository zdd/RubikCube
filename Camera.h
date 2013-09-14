#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "ArcBall.h"

class Camera
{
public:
	Camera(void);
	~Camera(void);

public:
	void Reset() ;
	void OnFrameMove() ;
	LRESULT HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) ;
	void SetViewParams(const D3DXVECTOR3& eye_point, const D3DXVECTOR3& lookat_point, const D3DXVECTOR3& up_vector);
	void SetProjParams(float field_of_view, float aspect_ratio, float near_plane, float far_plane) ;
	void SetWindow(int window_width, int window_height, float arcball_radius = 1.0f) ;
	const D3DXMATRIX GetWorldMatrix() const ;
	const D3DXMATRIX GetViewMatrix() const ;
	const D3DXMATRIX GetProjMatrix() const ;
	const D3DXVECTOR3 GetEyePoint() const ;

private:
	bool	frame_need_update_ ;
	float	radius_;				// Distance from the camera to model 
	float	max_radius_ ;			// The Maximum distance from the camera to the model
	float	min_radius_ ;			// The Minimum distance from the camera to the model
	int		mouse_wheel_delta_;		// Amount of middle wheel scroll (+/-)
	
	D3DXVECTOR3 eye_point_ ;			// Eye position
	D3DXVECTOR3 lookat_point_ ;		// Look at position
	D3DXVECTOR3 up_vector_ ;				// Up vector

	D3DXMATRIX world_matrix_ ;			// World matrix of model
	D3DXMATRIX view_matrix_ ;			// Camera View matrix
	D3DXMATRIX proj_matrix ;			// Camera Projection matrix

	ArcBall view_arcball_ ;			// View arc ball
};

#endif // end __CAMERA_H__