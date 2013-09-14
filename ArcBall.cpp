#include "ArcBall.h"

ArcBall::ArcBall(void)
    : is_dragged_(false),
	  radius_(1.0f),
	  previous_point_(D3DXVECTOR3(0, 0, 0)),
	  current_point_(D3DXVECTOR3(0, 0, 0)),
	  old_point_(D3DXVECTOR3(0, 0, 0)),
	  previous_quaternion_(D3DXQUATERNION(0, 0, 0, 1)),
      current_quaternion_(D3DXQUATERNION(0, 0, 0, 1)),
      rotation_increament_(D3DXQUATERNION(0, 0, 0, 1))
{
	D3DXMatrixIdentity(&rotate_matrix_);

	RECT rc ;
	GetClientRect(GetForegroundWindow(), &rc) ;

	int window_width  = rc.right - rc.left;
	int window_height = rc.bottom - rc.top;

	SetWindow(window_width, window_height) ;
}

ArcBall::~ArcBall(void)
{
}

void ArcBall::Reset()
{
	D3DXQuaternionIdentity(&previous_quaternion_);
	D3DXQuaternionIdentity(&current_quaternion_);
	D3DXQuaternionIdentity(&rotation_increament_) ;
	D3DXMatrixIdentity(&rotate_matrix_);
	is_dragged_ = false;
	radius_ = 1.0f;
}

void ArcBall::OnBegin(int mouse_x, int mouse_y)
{
	// enter drag state only if user click the window's client area
	if(mouse_x >= 0 && mouse_x <= window_width_ 
	   && mouse_y >= 0 && mouse_y < window_height_)
	{
		is_dragged_ = true ; // begin drag state
		previous_quaternion_ = current_quaternion_ ;
		previous_point_ = ScreenToVector(mouse_x, mouse_y) ;
		old_point_ = previous_point_ ;
	}
}

void ArcBall::OnMove(int mouse_x, int mouse_y)
{
	if(is_dragged_)
	{
		current_point_ = ScreenToVector(mouse_x, mouse_y) ;
		rotation_increament_ = QuatFromBallPoints( old_point_, current_point_ ) ;
		current_quaternion_ = previous_quaternion_ * QuatFromBallPoints( previous_point_, current_point_ ) ;
		old_point_ = current_point_ ;
	}
}

void ArcBall::OnEnd()
{
	is_dragged_ = false ;
}

void ArcBall::SetWindow(int window_width, int window_height, float arcball_radius)
{
	 window_width_  = window_width; 
	 window_height_ = window_height; 
	 radius_		= arcball_radius; 
}

const D3DXMATRIX* ArcBall::GetRotationMatrix()
{
	return D3DXMatrixRotationQuaternion(&rotate_matrix_, &current_quaternion_) ;
}

D3DXQUATERNION ArcBall::GetRotationQuatIncreament()
{
	return rotation_increament_ ;
}

D3DXQUATERNION ArcBall::QuatFromBallPoints(D3DXVECTOR3& start_point, D3DXVECTOR3& end_point)
{
	// Calculate rotate angle
	float angle = D3DXVec3Dot(&start_point, &end_point);	

	// Calculate rotate axis
	D3DXVECTOR3 axis;
	D3DXVec3Cross(&axis, &start_point, &end_point);		

	// Build and Normalize the Quaternion
	D3DXQUATERNION quat(axis.x, axis.y, axis.z, angle);
	D3DXQuaternionNormalize(&quat, &quat);

	return quat;
}

D3DXVECTOR3 ArcBall::ScreenToVector(int screen_x, int screen_y)
{
	// Scale to screen
	float x = -(screen_x - window_width_ / 2) / (radius_ * window_width_ / 2);
	float y = (screen_y - window_height_ / 2) / (radius_ * window_height_ / 2);

	float z = 0.0f;
	float mag = x * x + y * y;

	if(mag > 1.0f)
	{
		float scale = 1.0f / sqrtf(mag);
		x *= scale;
		y *= scale;
	}
	else
		z = sqrtf(1.0f - mag);

	return D3DXVECTOR3(x, y, z);
}
