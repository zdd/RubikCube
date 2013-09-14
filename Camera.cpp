#include "Camera.h"

Camera::Camera(void)
    : radius_(100),
	  min_radius_(50),
	  max_radius_(300),
	  mouse_wheel_delta_(0),
	  frame_need_update_(false)
{
	D3DXMatrixIdentity(&world_matrix_);
	D3DXMatrixIdentity(&view_matrix_);
}

Camera::~Camera(void)
{
}

void Camera::Reset()
{
	frame_need_update_ = false ;
	D3DXMatrixIdentity(&world_matrix_) ;
	view_arcball_.Reset();
}

// Update the scene for every frame
void Camera::OnFrameMove()
{
	// No need to handle if no drag since last frame move
	if(!frame_need_update_)
		return ;
	frame_need_update_ = false ;

	if(mouse_wheel_delta_)
	{
		radius_ -= mouse_wheel_delta_ * radius_ * 0.1f / 360.0f;

		// Make the radius in range of [min_radius_, max_radius_]
		// This can Prevent the cube became too big or too small
		radius_ = max(radius_, min_radius_) ;
		radius_ = min(radius_, max_radius_) ;
	}

	// The mouse delta is retrieved IN every WM_MOUSE message and do not accumulate, so clear it after one frame
	mouse_wheel_delta_ = 0 ;

	// Get the inverse of the view Arcball's rotation matrix
	D3DXMATRIX rotate_matrix ;
	D3DXMatrixInverse(&rotate_matrix, NULL, view_arcball_.GetRotationMatrix());

	// Transform vectors based on camera's rotation matrix
	D3DXVECTOR3 world_up_vector;
	D3DXVECTOR3 loacal_up_vector = D3DXVECTOR3(0, 1, 0);
	D3DXVec3TransformCoord(&world_up_vector, &loacal_up_vector, &rotate_matrix);

	D3DXVECTOR3 world_ahead_vector;
	D3DXVECTOR3 local_ahead_vector = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformCoord(&world_ahead_vector, &local_ahead_vector, &rotate_matrix);

	// Update the eye point based on a radius away from the lookAt position
	eye_point_ = lookat_point_ - world_ahead_vector * radius_;

	// Update the view matrix
	D3DXMatrixLookAtLH(&view_matrix_, &eye_point_, &lookat_point_, &world_up_vector);
}

// This function is used to handling the mouse message for the view arc ball
// include the right button message, the left button message is handled in the game window message process function
LRESULT Camera::HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// update view arc ball
	if(uMsg == WM_RBUTTONDOWN)
	{
		SetCapture(hWnd) ;

		frame_need_update_ = true ;
		int mouse_x = (short)LOWORD(lParam) ;
		int mouse_y = (short)HIWORD(lParam) ;
		view_arcball_.OnBegin(mouse_x, mouse_y) ;
	}

	// mouse move
	if(uMsg == WM_MOUSEMOVE)
	{
		frame_need_update_ = true ;
		int mouse_x = (short)LOWORD(lParam);
		int mouse_y = (short)HIWORD(lParam);
		view_arcball_.OnMove(mouse_x, mouse_y) ;
	}

	// right button up, terminate view arc ball rotation
	if(uMsg == WM_RBUTTONUP)
	{
		frame_need_update_ = true ;
		view_arcball_.OnEnd();
		ReleaseCapture() ;
	}

	// Mouse wheel, zoom in/out
	if(uMsg == WM_MOUSEWHEEL) 
	{
		frame_need_update_ = true ;
		mouse_wheel_delta_ += (short)HIWORD(wParam);
	}

	return TRUE ;
}

void Camera::SetViewParams(const D3DXVECTOR3& eye_point, const D3DXVECTOR3& lookat_point, const D3DXVECTOR3& up_vector)
{
	eye_point_	  = eye_point ;
	lookat_point_ = lookat_point ;
	up_vector_	  = up_vector ;

	D3DXMatrixLookAtLH(&view_matrix_, &eye_point, &lookat_point, &up_vector) ;
	frame_need_update_ = true ;
}

void Camera::SetProjParams(float field_of_view, float aspect_ratio, float near_plane, float far_plane)
{
	D3DXMatrixPerspectiveFovLH(&proj_matrix, field_of_view, aspect_ratio, near_plane, far_plane) ;
	frame_need_update_ = true ;
}

void Camera::SetWindow(int window_width, int window_height, float arcball_radius)
{
	view_arcball_.SetWindow(window_width, window_height, arcball_radius) ;
}

const D3DXMATRIX Camera::GetWorldMatrix() const
{
	return world_matrix_ ;
}

const D3DXMATRIX Camera::GetViewMatrix() const
{
	return view_matrix_ ;
}

const D3DXMATRIX Camera::GetProjMatrix() const
{
	return proj_matrix ;
}

const D3DXVECTOR3 Camera::GetEyePoint() const
{
	return eye_point_ ;
}