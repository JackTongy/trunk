//
//  ResourceManifest.h
//  skycastle
//
//  Created by iMac on 16/5/17.
//
//

#ifndef __skycastle__ResourceManifest__
#define __skycastle__ResourceManifest__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "json/document.h"

#include "ResourcePack.h"

#include <string>

class ResourceManifest : public cocos2d::Ref
{
public:
    friend class ResourcePack;
    
//public:
//    static ResourceManifest* create(const std::string& manifestUrl = "");
    
protected:
    ResourceManifest();
    ~ResourceManifest();
    
protected:
    void setLocalManifestUrl(const std::string& manifestUrl);
    
    void setPackageUrl(const std::string& packageUrl);
    
    void prepareManifest();
    
    void setSavePath(const std::string &filepath, const std::string& fileName);
    
    void endManifest();
    
private:
    void parseJsonDoc(const std::string& manifestUrl);
    
    void loadJsonDoc(const std::string& manifestUrl);
    
    void saveManifest(const std::string &filepath);
    
    void modifyManifest(rapidjson::Document & json);
    
    void revertManifest(rapidjson::Document & json);
    
private:
    std::string         _localManifestUrl;
    
    std::string         _remoteManifestUrl;
    
    std::string         _domainName;
    
    std::string         _packageUrl;
    
    std::string         _savePath;
    
    std::string         _saveFileName;
    
    rapidjson::Document _jsonDoc;
};

#endif /* defined(__skycastle__ResourceManifest__) */