//
//  ISGameConfigManager.h
//  skycastle
//
//  Created by iMac on 16/5/20.
//
//

#ifndef __skycastle__ISGameConfigManager__
#define __skycastle__ISGameConfigManager__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "json/document.h"

#include <string>

class ISGameConfigManager
{
public:
    static ISGameConfigManager* getInstance();
    
private:
    ISGameConfigManager();
    ~ISGameConfigManager();
    
public:
    void initialize();
    
    const std::string getServerUrl() const;
    
    const std::string getResourcePackUrl() const;
    
private:
    void parse();
    
    void loadJsonConfig();
    
    void parseServerConfig();
    
private:
    std::string         _serverUrl;
    
    std::string         _serverDebugUrl;
    
    std::string         _resourcePackUrl;
    
    std::string         _resourcePackDebugUrl;
    
private:
    rapidjson::Document _json;
};

#endif /* defined(__skycastle__ISGameConfigManager__) */
