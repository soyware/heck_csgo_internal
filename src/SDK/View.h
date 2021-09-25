#pragma once

enum MotionBlurMode_t
{
	MOTION_BLUR_DISABLE = 1,
	MOTION_BLUR_GAME = 2,			// Game uses real-time inter-frame data
	MOTION_BLUR_SFM = 3				// Use SFM data passed in CViewSetup structure
};

class CViewSetup
{
public:
// shared by 2D & 3D views

	// left side of view window
	int			x;
	int			m_nUnscaledX;
	// top side of view window
	int			y;
	int			m_nUnscaledY;
	// width of view window
	int			width;
	int			m_nUnscaledWidth;
	// height of view window
	int			height;
	int			m_nUnscaledHeight;

// the rest are only used by 3D views

	// Orthographic projection?
	bool		m_bOrtho;
	// View-space rectangle for ortho projection.
	float		m_OrthoLeft;
	float		m_OrthoTop;
	float		m_OrthoRight;
	float		m_OrthoBottom;

	// Note: This really should be called "m_bCustomCameraMatrix", because m_matCustomViewMatrix is interpreted as world->camera, not world->view.
	bool		m_bCustomViewMatrix;
	matrix3x4_t	m_matCustomViewMatrix;

	bool		m_bCustomProjMatrix;
	VMatrix		m_matCustomProjMatrix;

	const /*CVolumeCuller**/void* m_pCSMVolumeCuller;

	// horizontal FOV in degrees
	float		fov;
	// horizontal FOV in degrees for in-view model
	float		fovViewmodel;

	// 3D origin of camera
	Vector		origin;

	// heading of camera (pitch, yaw, roll)
	QAngle		angles;
	// local Z coordinate of near plane of camera
	float		zNear;
	// local Z coordinate of far plane of camera
	float		zFar;

	// local Z coordinate of near plane of camera ( when rendering view model )
	float		zNearViewmodel;
	// local Z coordinate of far plane of camera ( when rendering view model )
	float		zFarViewmodel;

	// The aspect ratio to use for computing the perspective projection matrix
	// (0.0f means use the viewport)
	float		m_flAspectRatio;

	// Camera settings to control depth of field
	float		m_flNearBlurDepth;
	float		m_flNearFocusDepth;
	float		m_flFarFocusDepth;
	float		m_flFarBlurDepth;
	float		m_flNearBlurRadius;
	float		m_flFarBlurRadius;
	int			m_nDoFQuality;

	// Camera settings to control motion blur
	MotionBlurMode_t	m_nMotionBlurMode;
	float	m_flShutterTime;				// In seconds
	Vector	m_vShutterOpenPosition;			// Start of frame or "shutter open"
	QAngle	m_shutterOpenAngles;			//
	Vector	m_vShutterClosePosition;		// End of frame or "shutter close"
	QAngle	m_shutterCloseAngles;			// 

	// Controls for off-center projection (needed for poster rendering)
	float		m_flOffCenterTop;
	float		m_flOffCenterBottom;
	float		m_flOffCenterLeft;
	float		m_flOffCenterRight;
	bool		m_bOffCenter : 1;

	// set to true if this is to draw into a subrect of the larger screen
	// this really is a hack, but no more than the rest of the way this class is used
	bool		m_bRenderToSubrectOfLargerScreen : 1;

	// Controls that the SFM needs to tell the engine when to do certain post-processing steps
	bool		m_bDoBloomAndToneMapping : 1;
	bool		m_bDoDepthOfField : 1;
	bool		m_bHDRTarget : 1;
	bool		m_bDrawWorldNormal : 1;
	bool		m_bCullFrontFaces : 1;

	// Cached mode for certain full-scene per-frame varying state such as sun entity coverage
	bool		m_bCacheFullSceneState : 1;

	// True if this is a CSM depth view. The CSM world->view matrix doesn't have an XY translation (that's moved into the CSM ortho view->projection 
	// matrix to address continuity issues), so the usual assumptions made about camera/view space do not necessarily apply.
	bool		m_bCSMView : 1;
};
