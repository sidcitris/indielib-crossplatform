/*****************************************************************************************
 * Desc: Tutorials a) 13 2d Camera
 *****************************************************************************************/

#include "CIndieLib_vc2008.h"
#include "IND_Surface.h"
#include "IND_Font.h"
#include "IND_Entity2d.h"
#include "IND_Camera2d.h"
#include "../../WorkingPath.h"
#include <cstring>

static const float K_ZOOMSPEED = 0.001f;

/*
==================
Main
==================
*/
int IndieLib()			
{
    //Sets the working path as the 'exe' directory. All resource paths are relative to this directory
	if (!WorkingPathSetup::setWorkingPathFromExe(NULL)) {
		std::cout<<"\nUnable to Set the working path !";
	}
	
	// ----- IndieLib intialization -----

	CIndieLib *mI = CIndieLib::instance();
	if (!mI->init()) return 0;			

	// ----- Surface loading -----

	// Loading tile for the terrain
	IND_Surface *mSurfaceBack = new IND_Surface();
	if (!mI->_surfaceManager->add(mSurfaceBack, "../../resources/twist.jpg", IND_OPAQUE, IND_32)) return 0;
	
	// Loading beetle
	IND_Surface *mSurfaceBeetle = new IND_Surface();
	if (!mI->_surfaceManager->add(mSurfaceBeetle, "../../resources/beetleship.png", IND_ALPHA, IND_32)) return 0;
	
	// Loading octopus
	IND_Surface *mSurfaceOctopus = new IND_Surface();
	if (!mI->_surfaceManager->add(mSurfaceOctopus, "../../resources/octopus.png", IND_ALPHA, IND_32)) return 0;

	// Loading bug
	IND_Surface *mSurfaceBug = new IND_Surface();
	if (!mI->_surfaceManager->add(mSurfaceBug, "../../resources/Enemy Bug.png", IND_ALPHA, IND_32)) return 0;

	// Font
	IND_Font *mFontSmall = new IND_Font();
	if (!mI->_fontManager->add(mFontSmall, "../../resources/font_small.png", "../../resources/font_small.xml", IND_ALPHA, IND_32)) return 0;

	// ----- Font creation -----

	IND_Entity2d *mTextSmallWhite = new IND_Entity2d();					
	mI->_entity2dManager->add(mTextSmallWhite);			// Entity adding
	mTextSmallWhite->setFont(mFontSmall);				// Set the font into the entity
	mTextSmallWhite->setLineSpacing(18);
	mTextSmallWhite->setCharSpacing(-8);
	mTextSmallWhite->setPosition(5, 5, 1);
	mTextSmallWhite->setAlign(IND_LEFT);

	// ----- Entities -----

	// Terrain
	IND_Entity2d *mTerrain = new IND_Entity2d();
	mI->_entity2dManager->add(mTerrain);
	mTerrain->setSurface(mSurfaceBack);

	// Beetle
	IND_Entity2d *mBeetle = new IND_Entity2d();
	mI->_entity2dManager->add(mBeetle);
	mBeetle->setSurface(mSurfaceBeetle);
	mBeetle->setHotSpot(0.5f, 0.5f);
	mBeetle->setPosition(150, 300, 2);

	// Octopus
	IND_Entity2d *mOctopus = new IND_Entity2d();
	mI->_entity2dManager->add(mOctopus);
	mOctopus->setSurface(mSurfaceOctopus);
	mOctopus->setHotSpot(0.5f, 0.5f);
	mOctopus->setPosition(450, 300, 2);

	// But
	IND_Entity2d *mBug = new IND_Entity2d();
	mI->_entity2dManager->add(mBug);
	mBug->setSurface(mSurfaceBug);
	mBug->setHotSpot(0.5f, 0.5f);
	mBug->setPosition(700, 300, 2);

	// ----- Camera -----

	float mCameraX = (float) mI->_window->getWidth () / 2;
	float mCameraY = (float) mI->_window->getHeight () / 2;
	IND_Camera2d *mCamera = new IND_Camera2d(mCameraX, mCameraY);

	// ----- Main Loop -----

	float mZoom = 1.0f;
	float mCameraAngle = 0;
	float mBugAngle = 0;
	char mText [2048];
	mText [0] = 0;
	int mSpeedMoveCamera = 200;
	int mSpeedRotation = 50;
	float mDelta;

	while (!mI->_input->onKeyPress(IND_ESCAPE) && !mI->_input->quit())
	{
		// ----- Input update ----

		mI->_input->update();

		// ----- Text -----

		strcpy (mText, "Use WASD keys for translating the camera\n");
		strcat (mText, "Use mouse wheel for zooming in/out\n");
		strcat (mText, "Use mouse buttons for rotating the camera");
		mTextSmallWhite->setText(mText);	

		// ----- Input ----

		mDelta = mI->_render->getFrameTime() / 1000.0f;

		// Camera translation
		if (mI->_input->isKeyPressed(IND_W)){
			mCameraY -= mSpeedMoveCamera * mDelta;
		}
		if (mI->_input->isKeyPressed(IND_S)){
			mCameraY += mSpeedMoveCamera * mDelta;
		}
		if (mI->_input->isKeyPressed(IND_A)){
			mCameraX -= mSpeedMoveCamera * mDelta;
		}
		if (mI->_input->isKeyPressed(IND_D)){
			mCameraX += mSpeedMoveCamera * mDelta;
		}

		// Camera Zoom in / out
		if (mI->_input->isMouseScroll()) {
			mZoom += mI->_input->getMouseScrollY() * K_ZOOMSPEED;
		}

		// Camera angle
		if (mI->_input->isMouseButtonPressed(IND_MBUTTON_LEFT)){
			mCameraAngle += mSpeedRotation * mDelta;
		}
		if (mI->_input->isMouseButtonPressed(IND_MBUTTON_RIGHT)){
			mCameraAngle -= mSpeedRotation * mDelta;
		}

		// ----- Updating entities attributes  -----

		// Rotation of the beetle and bug
		mBugAngle += mSpeedRotation * mDelta;
		mBeetle->setAngleXYZ(0, 0, mBugAngle);
		mBeetle->setAngleXYZ(0, 0, mBugAngle);
		mBug->setAngleXYZ(0, 0, -mBugAngle);
		mBug->setAngleXYZ(0, 0, -mBugAngle);

		// Tranlasting, zooming and rotating the camera
		if (mZoom < 0.1f){
			mZoom =  0.1f;
		}
		mCamera->setPosition(mCameraX, mCameraY);
		mCamera->setAngle(mCameraAngle);
		mCamera->setZoom(mZoom);

		// ----- Render  -----

		mI->_render->beginScene();
		mI->_render->clearViewPort(60, 60, 60);
		mI->_render->setViewPort2d(0, 0, mI->_window->getWidth(), mI->_window->getHeight());
		mI->_render->setCamera2d(mCamera);
		mI->_entity2dManager->renderEntities2d();
		mI->_render->endScene();
	}

	// ----- Free -----

	mI->end();

	return 0;
}
