//
//  ResourcePackManager.h
//  skycastle
//
//  Created by iMac on 16/5/16.
//
//

#ifndef __skycastle__ResourcePackManager__
#define __skycastle__ResourcePackManager__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ResourcePack.h"

#include <string>

class ResourcePackManager
{
public:
    static ResourcePackManager* getInstance();
    static void end();
    
protected:
    ResourcePackManager();
    virtual ~ResourcePackManager();
    
public:
    bool initialize();
    
    void start();
    
    void stopDownload();
    
    void removeResourcePack(std::string name);
    
    void insertResourcePack(const std::string& name);
    
    void pushResourcePack(const std::string& name);
    
private:
    ResourcePack* createResourcePack(const std::string& name);
    
    void insertResourcePack(ResourcePack* pack);
    
    void pushResourcePack(ResourcePack* pack);
    
    bool addResourcePack(ResourcePack* pack);
    
    void startDownload();
    
    void endDownload();
    
    bool isResourcePackInDownloadList(ResourcePack* pack);
    
    void createResourceDirectory();
    
protected://download event
    virtual void onError(const char* errorMsg);
    
    virtual void onProgress(ResourcePack *pack, int percent);
    
    virtual void onSuccess();
    
    virtual void receiveAssetsManagerExUpdateEvent(cocos2d::extension::EventAssetsManagerEx *event);
    
    virtual void cleanup();
    
private:
    cocos2d::extension::AssetsManagerEx*                 _assetsManager;
    
    cocos2d::extension::EventListenerAssetsManagerEx*    _assetsManagerExListener;
    
    cocos2d::Vector<ResourcePack*>                       _resourcePacks;
    
    ResourcePack*                                        _downloading;
    
    int                                                  _failCount;
};

#endif /* defined(__skycastle__ResourcePackManager__) */
