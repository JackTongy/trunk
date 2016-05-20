//
//  ResourcePack.h
//  skycastle
//
//  Created by iMac on 16/5/16.
//
//

#ifndef __skycastle__ResourcePack__
#define __skycastle__ResourcePack__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "json/document.h"

#include <string>

class ResourcePack;
class ResourcePackData;
class ResourcePackManager;

class ResourcePackProtocal
{
public:
    //download success
    virtual void onSuccess(ResourcePack *pack) = 0;
    
    //download error
    virtual void onError(ResourcePack *pack, const char* errorMsg) = 0;
    
    //download progress
    virtual void onProgress(ResourcePack *pack, int progress) = 0;
};

class ResourcePack : public cocos2d::Ref
{
public:
    friend class ResourcePackManager;
public:
    static ResourcePack* create(const ResourcePackData* packData);
    
protected:
    ResourcePack();
    virtual ~ResourcePack();

protected:
    virtual bool init(const ResourcePackData* packData);
    
public:
    //name
    CC_SYNTHESIZE(std::string, _name, Name);
    
    //manifest
    CC_SYNTHESIZE(std::string, _manifest, Manifest);
    
    //remoteManifestUrl
    CC_SYNTHESIZE(std::string, _remoteManifestUrl, RemoteManifestUrl);
    
    //version
    CC_SYNTHESIZE(std::string, _version, Version);
    
    //download url
    CC_SYNTHESIZE(std::string, _packageUrl, PackageUrl);
    
    //storage path
    CC_SYNTHESIZE(std::string, _storagePath, StoragePath);
    
    //Delegate
    CC_SYNTHESIZE(ResourcePackProtocal*, _delegate, Delegate);
    
protected:
    virtual void onSuccess();
    
    virtual void onError(const char* errMsg);
    
    virtual void onProgress(int percent);
    
protected:
    void modifyManifestFile();
    
private:
    void parse(const std::string& manifestUrl);
    
    void loadJson(const std::string& manifestUrl);
    
    void modifyManifest(rapidjson::Document & json);
    
    void saveManifest(const std::string &filepath);
    
    void createDirectory();
    
    void initalize();
    
    void createManifestFile();
    
    std::string getPlatformString();
    
private:
    const ResourcePackData* _packData;
    
    rapidjson::Document _json;
};

#endif /* defined(__skycastle__ResourcePack__) */
