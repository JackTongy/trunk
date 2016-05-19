//
//  ResourcePack.cpp
//  skycastle
//
//  Created by iMac on 16/5/16.
//
//

#include "ResourcePack.h"
#include "Constants/CommonSetting.h"
#include "Utils/StringUtils.h"
#include "ResourceManifest.h"

#define MANIFEST_FORMAT "project_%s.manifest"
#define MANIFEST_LOCAL_FORMAT "project_%s_local.manifest"
#define DOWNLOAD_URL_FORMAT "http://staging.ae-mobile.com:91/"

#define MANIFEST_URL_FORMAT "GameResource/SlotsTheme/%s/%s/%s/project_%s.manifest"
#define MANIFEST_PACKAGEURL_FORMAT "GameResource/SlotsTheme/%s/%s/%/"

using namespace cocos2d;
using namespace std;

ResourcePack* ResourcePack::create(string name)
{
    ResourcePack* pack = new (std::nothrow) ResourcePack();
    if(pack && pack->init(name))
    {
        pack->autorelease();
        return pack;
    }
    
    delete pack;
    pack = nullptr;
    return pack;
}

ResourcePack::ResourcePack():
_name(""),
_version(""),
_packageUrl(""),
_manifest(""),
_delegate(nullptr),
_resourceManifest(nullptr),
_projectManifest(""),
_storagePath("")
{
    _resourceManifest = new (std::nothrow) ResourceManifest();
}

ResourcePack::~ResourcePack()
{
    CC_SAFE_DELETE(_resourceManifest);
}

bool ResourcePack::init(std::string name)
{
    _name = name;
    if(_name.length() <= 0)
    {
        return false;
    }
    
    // 设置存储路径
    _storagePath = FileUtils::getInstance()->getWritablePath() + GameResourcesDownloadPath;
    
    // 保存的文件名
    _manifest = StringUtils::format(MANIFEST_FORMAT, _name.c_str());
    
    _localManifest = StringUtils::format(MANIFEST_LOCAL_FORMAT, _name.c_str());
    
    modifyManifestFile();
    
    return true;
}

bool ResourcePack::isManifestExist()
{
    if (_manifest.length() <= 0) {
        return false;
    }
    
    if (!FileUtils::getInstance()->isFileExist(_manifest)) {
        return false;
    }
    
    return true;
}

void ResourcePack::createManifestFile()
{
    if(_name.length() <= 0 || _packageUrl.length() <= 0){
        return;
    }
}

void ResourcePack::modifyManifestFile()
{
    if (FileUtils::getInstance()->isFileExist(_manifest.c_str()))
    {
        _resourceManifest->setLocalManifestUrl(_manifest.c_str());
        _projectManifest = _storagePath + _manifest;
    }
    else
    {
        _resourceManifest->setLocalManifestUrl(_localManifest.c_str());
        _projectManifest = _storagePath + _localManifest;
    }
    //modify manifest
    _resourceManifest->setRemoteDomain(DOWNLOAD_URL_FORMAT);
    
    _resourceManifest->setPackageUrl("");
    
    _resourceManifest->setSavePath(_storagePath, _manifest);
    
    _resourceManifest->prepareManifest();
    
    //test
    //_resourceManifest->endManifest();
}

void ResourcePack::onSuccess()
{
    if(_delegate)
    {
        _delegate->onSuccess(this);
    }
    
    _resourceManifest->endManifest();
}

void ResourcePack::onError(const char* errMsg)
{
    if(_delegate)
    {
        _delegate->onError(this, errMsg);
    }
    
    _resourceManifest->endManifest();
}

void ResourcePack::onProgress(int percent)
{
    if(_delegate)
    {
        _delegate->onProgress(this, percent);
    }
}

string ResourcePack::getPlatformString()
{
    string platform = "android";
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    platform = "android";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    platform = "win32";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    platform = "mac";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    platform = "ios";
#endif
    
    return platform;
}
