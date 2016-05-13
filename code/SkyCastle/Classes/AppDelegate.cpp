#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
#include "CodeIDESupport.h"
#include "Runtime.h"
#endif
#include "ConfigParser.h"

#include "Common/Script/ScriptManager.h"
#include "Constants/CommonSetting.h"


using namespace CocosDenshion;

USING_NS_CC;
using namespace std;

//
AppDelegate::AppDelegate()
:_launchMode(0)
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	endRuntime();
#elif (COCOS2D_DEBUG > 0 && CC_CODE_IDE_DEBUG_SUPPORT > 0)
	// NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
	if (_launchMode)
	{
		endRuntime();
	}
#endif

	ConfigParser::purge();
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	initRuntime();
#elif (COCOS2D_DEBUG > 0 && CC_CODE_IDE_DEBUG_SUPPORT > 0)
	// NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
	if (_launchMode)
	{
		initRuntime();
	}
#endif
    
    setSearchPath();
    
    initDirector();
    
    setDesignResolutionPolicy();
    
    ScriptManager::getInstance()->initialize();
    
    ScriptManager::getInstance()->executeScriptFile("Game/main.lua");
    return true;
}

void AppDelegate::initDirector()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        Size viewSize = ConfigParser::getInstance()->getInitViewSize();
        string title = ConfigParser::getInstance()->getInitViewName();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        extern void createSimulator(const char* viewName, float width, float height, bool isLandscape = true, float frameZoomFactor = 1.0f);
        bool isLanscape = ConfigParser::getInstance()->isLanscape();
        createSimulator(title.c_str(),viewSize.width,viewSize.height, isLanscape);
#else
        glview = cocos2d::GLViewImpl::createWithRect(title.c_str(), Rect(0, 0, viewSize.width, viewSize.height));
        director->setOpenGLView(glview);
#endif
        director->startAnimation();
    }
}

void AppDelegate::setDesignResolutionPolicy()
{
    auto director = Director::getInstance();
    auto glView = director->getOpenGLView();
    float scaleX = glView->getFrameSize().width / DESIGN_RESOLUTION_WIDTH;
    float scaleY = glView->getFrameSize().height / DESIGN_RESOLUTION_HEIGHT;
    float designWidth = DESIGN_RESOLUTION_WIDTH;
    float designHeight = DESIGN_RESOLUTION_HEIGHT;
    
    if (scaleX < scaleY) {
        designHeight = glView->getFrameSize().height / scaleX;
    } else {
        designWidth = glView->getFrameSize().width / scaleY;
    }
    glView->setDesignResolutionSize(designWidth, designHeight, DESIGN_RESOLUTION_POLICY);
}

void AppDelegate::setSearchPath()
{
    string writablePath = FileUtils::getInstance()->getWritablePath();
    string downloadPath = writablePath + GameResourcesDownloadPath;
    
    // 下载目录，这里的优先级别，需要比下面的高
    FileUtils::getInstance()->addSearchPath(writablePath.c_str());
    
    FileUtils::getInstance()->addSearchPath((downloadPath + "res/").c_str());
    
    FileUtils::getInstance()->addSearchPath((downloadPath + "scripts/").c_str());
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    std::string path = COCOS_PROJECT_ROOT;
    std::string writePath = path + "../../design/cache/";
    if(!FileUtils::getInstance()->isDirectoryExist(writePath)) {
        FileUtils::getInstance()->createDirectory(writePath);
    }
    FileUtils::getInstance()->setWritablePath(writePath.c_str());
    
    FileUtils::getInstance()->addSearchPath(path);
    FileUtils::getInstance()->addSearchPath(path + "res");
    FileUtils::getInstance()->addSearchPath(path + "scripts");
    FileUtils::getInstance()->addSearchPath(path + "res/config");
    
    FileUtils::getInstance()->addSearchPath(path + "../../design/");
    FileUtils::getInstance()->addSearchPath(path + "../../design/cache/");
#elif CC_TARGET_PLATFORM == CC_PLATFROM_ANDROID
    FileUtils::getInstance()->addSearchPath("res/");
    FileUtils::getInstance()->addSearchPath("res/config/");
    FileUtils::getInstance()->addSearchPath("scripts/");
#else
    FileUtils::getInstance()->addSearchPath("res/");
    FileUtils::getInstance()->addSearchPath("res/config/");
    FileUtils::getInstance()->addSearchPath("scripts/");
#endif
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
    Director::getInstance()->pause();

    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();

    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("APP_ENTER_BACKGROUND_EVENT");
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->resume();
    Director::getInstance()->startAnimation();

    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();

    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("APP_ENTER_FOREGROUND_EVENT");
}

void AppDelegate::setLaunchMode(int mode)
{
    _launchMode = mode;
}
