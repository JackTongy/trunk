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
#include <string>

class ResourcePack;
class ResourceManifest;
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
    friend class ResourceManifest;
public:
    static ResourcePack* create(std::string name);
    
protected:
    ResourcePack();
    virtual ~ResourcePack();

public:
    virtual bool init(std::string name);
    
public:
    //name
    CC_SYNTHESIZE(std::string, _name, Name);
    
    //manifest
    CC_SYNTHESIZE(std::string, _manifest, Manifest);
    
    //version
    CC_SYNTHESIZE(std::string, _version, Version);
    
    //download url
    CC_SYNTHESIZE(std::string, _packageUrl, PackageUrl);
    
    //storage path
    CC_SYNTHESIZE(std::string, _storagePath, StoragePath);
    
    //Delegate
    CC_SYNTHESIZE(ResourcePackProtocal*, _delegate, Delegate);
    
    //modify manifest
    CC_SYNTHESIZE_READONLY(ResourceManifest*, _resourceManifest, ResourceManifest);
    
    //project manifest
    CC_SYNTHESIZE_READONLY(std::string, _projectManifest, ProjectManifest);
    
public:
    virtual void onSuccess();
    
    virtual void onError(const char* errMsg);
    
    virtual void onProgress(int percent);
protected:
    bool isManifestExist();
    
    void createManifestFile();
    
    void modifyManifestFile();
    
    std::string getPlatformString();
    
private:
    std::string  _localManifest;
};

#endif /* defined(__skycastle__ResourcePack__) */
