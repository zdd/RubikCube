#include "D3D9.h"

D3D9::D3D9(void)
    : d3d_(NULL),
	  d3ddevice_(NULL),
	  is_fullscreen_(false)
{
	camera = new Camera();
}

D3D9::~D3D9(void)
{
	delete camera;
	camera = NULL;

	// Release Direct3D Device
	if(d3ddevice_ != NULL)
	{
		d3ddevice_->Release();
		d3ddevice_ = NULL;
	}

	// Release Direct3D object
	if(d3d_ != NULL)
	{
		d3d_->Release();
		d3d_ = NULL;
	}
}

void D3D9::InitD3D9(HWND hWnd)
{
	this->hWnd = hWnd;

	// Create the D3D object.
	if( NULL == ( d3d_ = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		MessageBox(hWnd, L"Create Direct3D9 failed!", L"error!", 0) ;

	ZeroMemory( &d3dpp_, sizeof(d3dpp_) );

	// Get max display resolution and set it as back buffer size
	D3DDISPLAYMODE displayMode ;
	d3d_->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode) ;

	// Initialize the max resolution width and height
	screen_width_  = displayMode.Width;
	screen_height_ = displayMode.Height;

	if(is_fullscreen_)
	{
		d3dpp_.Windowed = FALSE;
		d3dpp_.BackBufferWidth  = displayMode.Width;
		d3dpp_.BackBufferHeight = displayMode.Height;
	}
	else
	{
		d3dpp_.Windowed = TRUE;

		// Get current window size and set it as back buffer size
		RECT rect;
		GetClientRect(hWnd, &rect);
		d3dpp_.BackBufferWidth  = rect.right - rect.left;
		d3dpp_.BackBufferHeight = rect.bottom - rect.top;
	}
	
	// We didn't specify the back-buffer width and height, D3D will initialize it to the window width and height
	d3dpp_.hDeviceWindow = hWnd;
	d3dpp_.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dpp_.BackBufferCount           = 1;
	d3dpp_.BackBufferFormat			= D3DFMT_X8R8G8B8;
	d3dpp_.EnableAutoDepthStencil	= TRUE ;
	d3dpp_.AutoDepthStencilFormat	= D3DFMT_D16 ;
	
	// Create the D3DDevice
	HRESULT hr = d3d_->CreateDevice(D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp_, &d3ddevice_ );
	if(FAILED(hr))
	{
		MessageBox(hWnd, L"Create Direct3D9 device failed!", L"error!", 0) ;
	}

	// Setup view matrix
	D3DXVECTOR3 vecEye(0.0f, 0.0f, -10.0f);
	D3DXVECTOR3 vecAt (0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vecUp (0.0f, 1.0f, 0.0f) ;
	camera->SetViewParams(vecEye, vecAt, vecUp);
	
	// Setup projection matrix
	float aspectRatio = (float)d3dpp_.BackBufferWidth / (float)d3dpp_.BackBufferHeight ;
	camera->SetProjParams(D3DX_PI / 4, aspectRatio, 1.0f, 1000.0f) ;

	ResetDevice();
}

LPDIRECT3DTEXTURE9 D3D9::CreateTexture(int texWidth, int texHeight, D3DCOLOR color)
{
	LPDIRECT3DTEXTURE9 pTexture;

	HRESULT hr = D3DXCreateTexture(d3ddevice_, 
		texWidth, 
		texHeight, 
		0, 
		0, 
		D3DFMT_A8R8G8B8,  // 4 bytes for a pixel 
		D3DPOOL_MANAGED, 
		&pTexture);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create texture failed", L"Error", 0);
	}

	// Lock the texture and fill in color
	D3DLOCKED_RECT lockedRect;
	hr = pTexture->LockRect(0, &lockedRect, NULL, 0);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Lock texture failed!", L"Error", 0);
	}

	DWORD sideColor = 0xff000000; // the side line color

	int side_width = 10;

	// Calculate number of rows in the locked Rect
	int rowCount = (texWidth * texHeight * 4 ) / lockedRect.Pitch;

	for (int i = 0; i < texWidth; ++i)
	{
		for (int j = 0; j < texHeight; ++j)
		{
			int index = i * rowCount + j;

			int* pData = (int*)lockedRect.pBits;

			if (i <= side_width || i >= texWidth - side_width 
				|| j <= side_width || j >= texHeight - side_width)
			{
				memcpy(&pData[index], &sideColor, 4);
			}
			else
			{
				memcpy(&pData[index], &color, 4);
			}
		}
	}

	pTexture->UnlockRect(0);

	return pTexture;
}

LPDIRECT3DTEXTURE9 D3D9::CreateInnerTexture(int texWidth, int texHeight, D3DCOLOR color)
{
	LPDIRECT3DTEXTURE9 pTexture;

	HRESULT hr = D3DXCreateTexture(d3ddevice_, 
		texWidth, 
		texHeight, 
		0, 
		0, 
		D3DFMT_A8R8G8B8,  // 4 bytes for a pixel 
		D3DPOOL_MANAGED, 
		&pTexture);

	if(FAILED(hr))
	{
		MessageBox(NULL, L"Create texture failed", L"Error", 0);
	}

	// Lock the texture and fill in color
	D3DLOCKED_RECT lockedRect;
	hr = pTexture->LockRect(0, &lockedRect, NULL, 0);
	if(FAILED(hr))
	{
		MessageBox(NULL, L"Lock texture failed!", L"Error", 0);
	}

	DWORD sideColor = 0xff121212; // the side line color

	// Calculate number of rows in the locked Rect
	int rowCount = (texWidth * texHeight * 4 ) / lockedRect.Pitch;

	for (int i = 0; i < texWidth; ++i)
	{
		for (int j = 0; j < texHeight; ++j)
		{
			int index = i * rowCount + j;

			int* pData = (int*)lockedRect.pBits;
		
			memcpy(&pData[index], &color, 4);
		}
	}

	pTexture->UnlockRect(0);

	return pTexture;
}

void D3D9::SetupMatrix()
{
	// View matrix
	D3DXMATRIX matView = camera->GetViewMatrix() ;
	d3ddevice_->SetTransform(D3DTS_VIEW, &matView) ;

	// Projection matrix
	D3DXMATRIX matProj = camera->GetProjMatrix() ;
	d3ddevice_->SetTransform(D3DTS_PROJECTION, &matProj) ;
}

void D3D9::SetupLight()
{
	// Create light
	D3DLIGHT9 pointLight ;

	// Light color
	D3DXCOLOR color = D3DCOLOR_XRGB(255, 255, 255) ;

	// The light position is always same as the camera eye point
	// so no matter how you rotate the camera, the cube will keep the same brightness
	 D3DXVECTOR3 position = camera->GetEyePoint() ;

	// Light type, we use point light here
	pointLight.Type			= D3DLIGHT_POINT ;

	// Light attributes
	pointLight.Ambient		= color * 0.6f;
	pointLight.Diffuse		= color;
	pointLight.Specular		= color * 0.6f;
	pointLight.Position		= position;
	pointLight.Range		= 320.0f;
	pointLight.Falloff		= 1.0f;
	pointLight.Attenuation0	= 1.0f;
	pointLight.Attenuation1	= 0.0f;
	pointLight.Attenuation2	= 0.0f;

	// Set material
	D3DMATERIAL9 material ;
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0) ;
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0);
	material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0);
	material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	material.Power = 2.0f ;
	d3ddevice_->SetMaterial(&material) ;

	// Enable light
	d3ddevice_->SetLight(0, &pointLight) ;		
	d3ddevice_->LightEnable(0, true) ;		
}

void D3D9::ResizeD3DScene(int width, int height)
{
	if (height == 0)				// Prevent A Divide By Zero By
		height = 1;					// Making Height Equal One

	// Compute aspect ratio
	float fAspectRatio = width / (FLOAT)height;

	// Setup Projection matrix
	camera->SetProjParams(D3DX_PI / 4, fAspectRatio, 1.0f, 1000.0f);

	camera->SetWindow(width, height);
}

// We can remove this parameter here, and use device to get the 
// back buffer size dynamically.
HRESULT D3D9::ResetDevice()
{
	// Check device state
	HRESULT hr = d3ddevice_->TestCooperativeLevel() ;

	// Device can be reset now
	if (SUCCEEDED(hr) || hr == D3DERR_DEVICENOTRESET)
	{
		// Reset device
		HRESULT hr = d3ddevice_->Reset(&d3dpp_);
		if (SUCCEEDED(hr))
		{
			ResizeD3DScene(d3dpp_.BackBufferWidth, d3dpp_.BackBufferHeight) ;
		}
		else // Reset device failed, show error box
		{
			const WCHAR* errorString = DXGetErrorString(hr) ;
			DXTRACE_ERR_MSGBOX(errorString, hr) ;
		}
	}
	// Device is still in lost state, wait
	else if (hr == D3DERR_DEVICELOST)
	{
		Sleep(25) ;
	}
	else // Other error, Show error box
	{
		const WCHAR* errorString = DXGetErrorString(hr) ;
		DXTRACE_ERR_MSGBOX(errorString, hr) ;
	}

	return hr ;
}

void D3D9::FrameMove()
{
	camera->OnFrameMove() ;
}

// Calculate the picking ray and transform it to model space 
// x and y are the screen coordinates when left button down
Ray D3D9::CalculatePickingRay(int x, int y)
{
	float px = 0.0f;
	float py = 0.0f;

	// Get viewport
	D3DVIEWPORT9 vp;
	d3ddevice_->GetViewport(&vp);

	// Get Projection matrix
	D3DXMATRIX proj;
	d3ddevice_->GetTransform(D3DTS_PROJECTION, &proj);

	px = ((( 2.0f*x) / vp.Width)  - 1.0f) / proj(0, 0);
	py = (((-2.0f*y) / vp.Height) + 1.0f) / proj(1, 1);

	Ray ray;
	ray.origin    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ray.direction = D3DXVECTOR3(px, py, 1.0f); 

	// Get view matrix
	D3DXMATRIX view;
	d3ddevice_->GetTransform(D3DTS_VIEW, &view);

	// Get world matrix
	D3DXMATRIX world;
	d3ddevice_->GetTransform(D3DTS_WORLD, &world);

	// Concatinate them in to single matrix
	D3DXMATRIX WorldView = world * view;

	// inverse it
	D3DXMATRIX worldviewInverse;
	D3DXMatrixInverse(&worldviewInverse, 0, &WorldView);


	// Transform the ray to model space
	D3DXVec3TransformCoord(&ray.origin, &ray.origin, &worldviewInverse) ;
	D3DXVec3TransformNormal(&ray.direction, &ray.direction, &worldviewInverse) ;

	// Normalize the direction
	D3DXVec3Normalize(&ray.direction, &ray.direction) ;

	return ray;
}

// Transform the screen point to vector in model space
D3DXVECTOR3 D3D9::ScreenToVector3(int x, int y)
{
	D3DXVECTOR3 vector3 ;

	// Get viewport
	D3DVIEWPORT9 vp;
	d3ddevice_->GetViewport(&vp);

	// Get Projection matrix
	D3DXMATRIX proj;
	d3ddevice_->GetTransform(D3DTS_PROJECTION, &proj);

	vector3.x = ((( 2.0f*x) / vp.Width)  - 1.0f) / proj(0, 0);
	vector3.y = (((-2.0f*y) / vp.Height) + 1.0f) / proj(1, 1);
	vector3.z = 1.0f ;

	// Get view matrix
	D3DXMATRIX view;
	d3ddevice_->GetTransform(D3DTS_VIEW, &view);

	// Get world matrix
	D3DXMATRIX world;
	d3ddevice_->GetTransform(D3DTS_WORLD, &world);

	// Concatinate them in to single matrix
	D3DXMATRIX WorldView = world * view;

	// inverse it
	D3DXMATRIX worldviewInverse;
	D3DXMatrixInverse(&worldviewInverse, 0, &WorldView);

	D3DXVec3TransformCoord(&vector3, &vector3, &worldviewInverse) ;

	D3DXVec3Normalize(&vector3, &vector3) ;

	return vector3 ;
}

LRESULT D3D9::HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return camera->HandleMessages(hWnd, uMsg, wParam, lParam);
}

LPDIRECT3D9 D3D9::GetD3D9() const
{
	return d3d_;
}

LPDIRECT3DDEVICE9 D3D9::GetD3DDevice() const
{
	return d3ddevice_;
}

D3DPRESENT_PARAMETERS D3D9::GetD3Dpp() const
{
	return d3dpp_;
}

void D3D9::SetBackBufferWidth(int width)
{
	d3dpp_.BackBufferWidth = width;
}

void D3D9::SetBackBufferHeight(int height)
{
	d3dpp_.BackBufferHeight = height;
}

void D3D9::SetIsFullScreen(bool is_fullscreen)
{
	is_fullscreen_ = is_fullscreen;
	d3dpp_.Windowed = !is_fullscreen;
}

bool D3D9::GetIsFullScreen() const
{
	return is_fullscreen_;
}

int	D3D9::GetLastWindowWidth() const
{
	return last_window_width_;
}

void D3D9::SetLastWindowWidth(int windowWidth)
{
	last_window_width_ = windowWidth;
}

int	D3D9::GetLastWindowHeight() const
{
	return last_window_height_;
}

void D3D9::SetLastWindowHeight(int windowHeight)
{
	last_window_height_ = windowHeight;
}

int	D3D9::GetScreenWidth() const
{
	return screen_width_;
}

int D3D9::GetScreenHeight() const
{
	return screen_height_;
}
