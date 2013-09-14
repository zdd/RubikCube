#ifndef __ARCBALL_H__
#define __ARCBALL_H__

#include "d3dx9.h"

class ArcBall
{
public:
	ArcBall(void);
	~ArcBall(void);

public:
	void Reset() ;
	void OnBegin(int mouse_x, int mouse_y) ;
	void OnMove(int mouse_x, int mouse_y) ;
	void OnEnd() ;

	D3DXQUATERNION QuatFromBallPoints(D3DXVECTOR3& start_point, D3DXVECTOR3& end_point);
	const D3DXMATRIX* GetRotationMatrix() ;
	D3DXQUATERNION GetRotationQuatIncreament() ;
	void SetWindow(int window_width, int window_height, float arcball_radius = 1.0f) ;

private:
	int		window_width_ ;	// arc ball's window width
	int		window_height_ ; // arc ball's window height
	float	radius_ ;	// arc ball's radius in screen coordinates
	bool	is_dragged_ ;	// whether the arc ball is dragged

	D3DXQUATERNION	previous_quaternion_ ;	// quaternion before mouse down
	D3DXQUATERNION	current_quaternion_ ;	// current quaternion
	D3DXQUATERNION	rotation_increament_ ;	// rotation increment 
	D3DXVECTOR3		previous_point_ ;		// starting point of arc ball rotate
	D3DXVECTOR3		current_point_ ;		// current point of arc ball rotate
	D3DXVECTOR3		old_point_ ;			// old point 
	D3DXMATRIXA16	rotate_matrix_;			// rotation matrix

	// Convert scree point to arcball point(vector)
	D3DXVECTOR3	ScreenToVector(int screen_x, int screen_y) ;

};

#endif // end __ARCBALL_H__