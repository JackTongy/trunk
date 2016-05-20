//
//  ISGameConfigManager.cpp
//  skycastle
//
//  Created by iMac on 16/5/20.
//
//

#include "ISGameConfigManager.h"
#include "GameData/ISGameDataManager.h"

using namespace cocos2d;
using namespace std;

#define PROJECT_GAME_CONFIG                 "Config/project_game_setting_config.json"

#define PROJECT_SERVER_URL                  "ServerUrl"
#define PROJECT_SERVER_DEBUG_URL            "ServerDebugUrl"
#define PROJECT_RESOURCE_PACK_URL           "ResourcePackUrl"
#define PROJECT_RESOURCE_PACK_DEBUG_URL     "ResourcePackDebugUrl"

ISGameConfigManager* ISGameConfigManager::getInstance()
{
    static ISGameConfigManager instance;
    return &instance;
}

ISGameConfigManager::ISGameConfigManager()
{
    
}

ISGameConfigManager::~ISGameConfigManager()
{
    
}

void ISGameConfigManager::initialize()
{
    parse();
}

void ISGameConfigManager::parse()
{
    if(FileUtils::getInstance()->isFileExist(PROJECT_GAME_CONFIG))
    {
        loadJsonConfig();
        
        parseServerConfig();
    }
}

void ISGameConfigManager::loadJsonConfig()
{
    string content = FileUtils::getInstance()->getStringFromFile(PROJECT_GAME_CONFIG);
    if (content.size() == 0)
    {
        CCLOG("Fail to retrieve local file content: %s\n", PROJECT_GAME_CONFIG);
    }
    else
    {
        // Parse file with rapid json
        _json.Parse<0>(content.c_str());
        // Print error
        if (_json.HasParseError()) {
            size_t offset = _json.GetErrorOffset();
            if (offset > 0)
                offset--;
            std::string errorSnippet = content.substr(offset, 10);
            CCLOG("File parse error %s at <%s>\n", _json.GetParseError(), errorSnippet.c_str());
        }
    }
}

void ISGameConfigManager::parseServerConfig()
{
    if(!_json.IsObject())
    {
        return;
    }
    
    if(_json.HasMember(PROJECT_SERVER_URL) && _json[PROJECT_SERVER_URL].IsString())
    {
        _serverUrl = _json[PROJECT_SERVER_URL].GetString();
    }
    
    if(_json.HasMember(PROJECT_SERVER_DEBUG_URL) && _json[PROJECT_SERVER_DEBUG_URL].IsString())
    {
        _serverDebugUrl = _json[PROJECT_SERVER_DEBUG_URL].GetString();
    }
    
    if(_json.HasMember(PROJECT_RESOURCE_PACK_URL) && _json[PROJECT_RESOURCE_PACK_URL].IsString())
    {
        _resourcePackUrl = _json[PROJECT_RESOURCE_PACK_URL].GetString();
    }
    
    if(_json.HasMember(PROJECT_RESOURCE_PACK_DEBUG_URL) && _json[PROJECT_RESOURCE_PACK_DEBUG_URL].IsString())
    {
        _resourcePackDebugUrl = _json[PROJECT_RESOURCE_PACK_DEBUG_URL].GetString();
    }
}

const std::string ISGameConfigManager::getServerUrl() const
{
    string serverUrl = "";
    ISGameDataManager::ServerType serverType = ISGameDataManager::getInstance()->getServerType();
    switch (serverType)
    {
        case ISGameDataManager::ServerType::None:
        case ISGameDataManager::ServerType::Testing:
        case ISGameDataManager::ServerType::Development:
            serverUrl = _serverUrl;
            break;
        case ISGameDataManager::ServerType::Production:
            serverUrl = _serverDebugUrl;
            break;
        default:
            break;
    }
    return serverUrl;
}

const std::string ISGameConfigManager::getResourcePackUrl() const
{
    string resourcePackUrl = "";
    ISGameDataManager::ServerType serverType = ISGameDataManager::getInstance()->getServerType();
    switch (serverType)
    {
        case ISGameDataManager::ServerType::None:
        case ISGameDataManager::ServerType::Testing:
        case ISGameDataManager::ServerType::Development:
            resourcePackUrl = _resourcePackUrl;
            break;
        case ISGameDataManager::ServerType::Production:
            resourcePackUrl = _resourcePackUrl;
            break;
        default:
            break;
    }
    return resourcePackUrl;
}