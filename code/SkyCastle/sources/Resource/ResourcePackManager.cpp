//
//  ResourcePackManager.cpp
//  skycastle
//
//  Created by iMac on 16/5/16.
//
//

#include "ResourcePackManager.h"
#include "Constants/CommonSetting.h"
#include "GameData/ResourcePackData/ResourcePackData.h"
#include "GameData/ISGameDataManager.h"

#include <string>

#define DOWNLOAD_FAILED_REPEAT 5

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

static ResourcePackManager* _instance = nullptr;

ResourcePackManager* ResourcePackManager::getInstance()
{
    if(_instance == nullptr)
    {
        _instance = new (std::nothrow)ResourcePackManager();
    }
    
    return _instance;
}

void ResourcePackManager::end()
{
    if(_instance)
    {
        _instance->cleanup();
    }
    CC_SAFE_DELETE(_instance);
}

ResourcePackManager::ResourcePackManager():
_assetsManager(nullptr),
_assetsManagerExListener(nullptr),
_downloading(nullptr),
_failCount(0)
{
    _resourcePacks.clear();
}

ResourcePackManager::~ResourcePackManager()
{
    
}

bool ResourcePackManager::initialize()
{
    cleanup();
    
    createResourceDirectory();
    
    return true;
}

void ResourcePackManager::createResourceDirectory()
{
    std::string path = FileUtils::getInstance()->getWritablePath() + GameResourcesDownloadPath;
    FileUtils::getInstance()->createDirectory(path.c_str());
}

void ResourcePackManager::cleanup()
{
    endDownload();
}

void ResourcePackManager::start()
{
    if(!_resourcePacks.empty())
    {
        startDownload();
    }
}

void ResourcePackManager::startDownload()
{
    //return;
    if (_assetsManager != nullptr || _downloading != nullptr)
    {
        return;
    }
    
    _downloading = _resourcePacks.at(0);
    _downloading->retain();
    _resourcePacks.eraseObject(_downloading);
    _downloading->modifyManifestFile();
    
    _assetsManager = AssetsManagerEx::create(_downloading->getManifest(), _downloading->getStoragePath());
    _assetsManager->retain();
    
    _assetsManagerExListener = EventListenerAssetsManagerEx::create(_assetsManager, CC_CALLBACK_1(ResourcePackManager::receiveAssetsManagerExUpdateEvent, this));
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_assetsManagerExListener, 1);
    
    _assetsManager->update();
}

void ResourcePackManager::endDownload()
{
    CC_SAFE_RELEASE_NULL(_downloading);
    CC_SAFE_RELEASE_NULL(_assetsManager);
    if(_assetsManagerExListener)
    {
        Director::getInstance()->getEventDispatcher()->removeEventListener(_assetsManagerExListener);
    }
}

void ResourcePackManager::stopDownload()
{

}

ResourcePack* ResourcePackManager::createResourcePack(const std::string &name)
{
    ResourcePack* pack = nullptr;
    auto packDataList = ISGameDataManager::getInstance()->getResourcePackDataList();
    for(auto it : packDataList)
    {
        if (it->getName() == name)
        {
            pack = ResourcePack::create(it);
            pack->retain();
            break;
        }
    }
    
    return pack;
}

void ResourcePackManager::insertResourcePack(const std::string &name)
{
    auto pack = createResourcePack(name);
    
    insertResourcePack(pack);
}

void ResourcePackManager::insertResourcePack(ResourcePack *pack)
{
    if(addResourcePack(pack))
    {
        _resourcePacks.insert(0, pack);
    }
}

void ResourcePackManager::pushResourcePack(const std::string &name)
{
    auto pack = createResourcePack(name);
    
    pushResourcePack(pack);
}

void ResourcePackManager::pushResourcePack(ResourcePack *pack)
{
    if(addResourcePack(pack))
    {
        _resourcePacks.pushBack(pack);
    }
}

bool ResourcePackManager::addResourcePack(ResourcePack *pack)
{
    bool ret = true;
    do
    {
        if (pack == nullptr)
        {
            ret = false;
            break;
        }
        if (isResourcePackInDownloadList(pack))
        {
            ret = false;
            break;
        }
    }while(0);
    
    return ret ;
}

bool ResourcePackManager::isResourcePackInDownloadList(ResourcePack *pack)
{
    bool ret = false;
    do
    {
        if (pack == nullptr)
        {
            ret = false;
            break;
        }
        
        //check is in vector
        for (auto it: _resourcePacks)
        {
            if (it->getName() == pack->getName())
            {
                ret = true;
                break;
            }
        }
        
        //check is in downloading
        if (nullptr != _downloading && pack->getName() == _downloading->getName())
        {
            ret = true;
            break;
        }
    }while(0);
    
    return ret ;
}

void ResourcePackManager::removeResourcePack(string name)
{

}

void ResourcePackManager::onError(const char *errorMsg)
{
    _downloading->onError(errorMsg);
    
    //清除当前下载状态
    endDownload();
    
    //开始下一个下载
    start();
}

void ResourcePackManager::onSuccess()
{
    _downloading->onSuccess();
    
    //清除当前下载状态
    endDownload();
    
    //开始下一个下载
    start();
}

void ResourcePackManager::onProgress(ResourcePack *pack, int percent)
{
    _downloading->onProgress(percent);
}

void ResourcePackManager::receiveAssetsManagerExUpdateEvent(EventAssetsManagerEx *event)
{
    EventAssetsManagerEx::EventCode code = event->getEventCode();
    
    switch (code)
    {
        case EventAssetsManagerEx::EventCode::ERROR_NO_LOCAL_MANIFEST:
        {
            onError("local manifest file not found.");
            break;
        }
            // 下载project.mainfest失败(version.mainfest可以不存在)
        case EventAssetsManagerEx::EventCode::ERROR_DOWNLOAD_MANIFEST:
        {
            int eCode = event->getCURLECode();
            //manifest文件不存在,则认为升级成功
            if (eCode == 22){
                //HTTP 找不到网页。找不到所请求的URL 或返回另一个HTTP 400或以上错误
                onSuccess();
            }else{
                //manifest文件下载失败
                onError("Failed to download mainfest file, download error.");
            }
            break;
        }
        case EventAssetsManagerEx::EventCode::ERROR_PARSE_MANIFEST:
        {
            onError("manifest parse failed.");
            break;
        }
        case EventAssetsManagerEx::EventCode::NEW_VERSION_FOUND:
        {
            CCLOG("ResourcePackManager : Found new version.");
            break;
        }
        case EventAssetsManagerEx::EventCode::ERROR_UPDATING:
        {
            // single file download failed
            CCLOG("[Resource Download], [error], [%s], 1 %s : %s.", _downloading->getName().c_str(), event->getAssetId().c_str(), event->getMessage().c_str());
            break;
        }
        case EventAssetsManagerEx::EventCode::ASSET_UPDATED:
        {
             //single file updated
            break;
        }
        case EventAssetsManagerEx::EventCode::ERROR_DECOMPRESS:
        {
            break;
        }
        case EventAssetsManagerEx::EventCode::UPDATE_FAILED:
        {
            _failCount ++;

            if(_failCount < DOWNLOAD_FAILED_REPEAT)
            {
                _assetsManager->downloadFailedAssets();
            }
            else
            {
                onError("update error");
            }

            break;
        }
        case EventAssetsManagerEx::EventCode::UPDATE_PROGRESSION:
        {
            std::string assetId = event->getAssetId();
            float filePercent = event->getPercentByFile();
            
            if (assetId == AssetsManagerEx::BATCH_UPDATE_ID)
            {
                onProgress(_downloading, filePercent >= 99 ? 99 : filePercent);
            }
            break;
        }
        case EventAssetsManagerEx::EventCode::ALREADY_UP_TO_DATE:
        {
            onSuccess();
            break;
        }
        case EventAssetsManagerEx::EventCode::UPDATE_FINISHED:
        {
            //download finished
            onProgress(_downloading, 100);
            onSuccess();
            break;
        }
        default:
            break;
    }
}