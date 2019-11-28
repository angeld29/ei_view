#pragma warning(disable : 4251)
#pragma warning(disable : 4275)
#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreCameraMan.h"

class MyTestApp : public OgreBites::ApplicationContext, public OgreBites::InputListener
{
public:
    MyTestApp();
    void setup();
	virtual void 	shutdown()
	{
		OgreBites::ApplicationContext::shutdown();

		delete mCameraMan;
	}

	virtual void frameRendered(const Ogre::FrameEvent& evt)
	{
		mCameraMan->frameRendered(evt);   // if dialog isn't up, then update the camera
	}
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt)
	{
		if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
		{
			getRoot()->queueEndRendering();
		}
		mCameraMan->keyPressed(evt);
		return true;
	}

	virtual bool keyReleased(const OgreBites::KeyboardEvent& evt)
	{
		mCameraMan->keyReleased(evt);

		return true;
	}

	virtual bool mouseMoved(const OgreBites::MouseMotionEvent& evt)
	{
		mCameraMan->mouseMoved(evt);
		return true;
	}
	virtual bool mousePressed(const OgreBites::MouseButtonEvent& evt)
	{
		mCameraMan->mousePressed(evt);
		return true;
	}

	virtual bool mouseReleased(const OgreBites::MouseButtonEvent& evt)
	{
		mCameraMan->mouseReleased(evt);
		return true;
	}
	virtual bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt) {
		mCameraMan->mouseWheelRolled(evt);
		return true;
	}
protected:
	OgreBites::CameraMan* mCameraMan;           // basic camera controller
};

//! [constructor]
MyTestApp::MyTestApp() : OgreBites::ApplicationContext("OgreTutorialApp")
{
	mCameraMan = 0;
}
//! [constructor]


//! [setup]
void MyTestApp::setup(void)
{
    // do not forget to call the base first
    OgreBites::ApplicationContext::setup();
    
    // register for input events
    addInputListener(this);

    // get a pointer to the already created root
    Ogre::Root* root = getRoot();
    Ogre::SceneManager* scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // without light we would just get a black screen    
    Ogre::Light* light = scnMgr->createLight("MainLight");
    Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 10, 15);
    lightNode->attachObject(light);

    // also need to tell where we are
    Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(0, 0, 15);
    camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

    // create the camera
    Ogre::Camera* cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);

    // and tell it to render into the main window
    getRenderWindow()->addViewport(cam);

    // finally something to render
    Ogre::Entity* ent = scnMgr->createEntity("Sinbad.mesh");
    Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(ent);
	mCameraMan = new OgreBites::CameraMan(camNode);   // create a default camera controller
	mCameraMan->setStyle(OgreBites::CS_FREELOOK);
}
//! [setup]

//! [main]
int main(int argc, char *argv[])
{
    MyTestApp app;
    app.initApp();
    app.getRoot()->startRendering();
    app.closeApp();
    return 0;
}
//! [main]
