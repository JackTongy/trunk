//
//  ResourcePack.cpp
//  skycastle
//
//  Created by iMac on 16/5/16.
//
//

#include "ResourcePack.h"
#include "Utils/StringUtils.h"
#include "Constants/CommonSetting.h"
#include "GameConfig/ISGameConfigManager.h"
#include "GameData/ResourcePackData/ResourcePackData.h"

#include "json/stringbuffer.h"
#include "json/prettywriter.h"
#include "json/filestream.h"

#include <fstream>

#define MANIFEST_FILE_NAME       "project.manifest"
#define MANIFEST_TEMP_FILE      "project_temp_local.manifest"
#define MANIFEST_FORMAT         "project_%s.manifest"
#define MANIFEST_LOCAL_FORMAT   "project_%s_local.manifest"
#define DOWNLOAD_URL_FORMAT     "http://staging.ae-mobile.com:91/"

#define MANIFEST_URL_FORMAT     "GameResource/SlotsTheme/%s/%s/%s/project_%s.manifest"
#define MANIFEST_PACKAGEURL_FORMAT "GameResource/SlotsTheme/%s/%s/%/"

#define KEY_MANIFEST_VERSION             "version"
#define KEY_MANIFEST_PACKAGE_URL         "packageUrl"
#define KEY_MANIFEST_MANIFEST_URL        "remoteManifestUrl"
#define KEY_MANIFEST_VERSION_URL         "remoteVersionUrl"
#define KEY_MANIFEST_GROUP_VERSIONS      "groupVersions"
#define KEY_MANIFEST_ENGINE_VERSION      "engineVersion"
#define KEY_MANIFEST_ASSETS              "assets"
#define KEY_MANIFEST_COMPRESSED_FILES    "compressedFiles"
#define KEY_MANIFEST_SEARCH_PATHS        "searchPaths"

#define KEY_MANIFEST_PATH                "path"
#define KEY_MANIFEST_MD5                 "md5"
#define KEY_MANIFEST_GROUP               "group"
#define KEY_MANIFEST_COMPRESSED          "compressed"
#define KEY_MANIFEST_COMPRESSED_FILE     "compressedFile"
#define KEY_MANIFEST_DOWNLOAD_STATE      "downloadState"

using namespace cocos2d;
using namespace std;

ResourcePack* ResourcePack::create(const ResourcePackData* packData)
{
    ResourcePack* pack = new (std::nothrow) ResourcePack();
    if(pack && pack->init(packData))
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
_storagePath("")
{

}

ResourcePack::~ResourcePack()
{

}

bool ResourcePack::init(const ResourcePackData* packData)
{
    _packData = packData;
    if(_packData == nullptr)
    {
        return false;
    }
    
    initalize();
    
    return true;
}

void ResourcePack::initalize()
{
    // save directory
    _storagePath = FileUtils::getInstance()->getWritablePath() + GameResourcesDownloadPath + _packData->getDirectory();
    
    // name
    _name = _packData->getName();
    
    //file name; local manifest url
    _manifest = StringUtils::format(MANIFEST_FORMAT, _name.c_str());
    
    //modified package url
    _packageUrl = ISGameConfigManager::getInstance()->getResourcePackUrl() + _packData->getPackageUrl();
    
    //modified remote manifest url
    _remoteManifestUrl = ISGameConfigManager::getInstance()->getResourcePackUrl() + _packData->getRemoteManifestUrl();
}

void ResourcePack::createDirectory()
{
    FileUtils::getInstance()->createDirectory(_storagePath.c_str());
    
    FileUtils::getInstance()->addSearchPath(_storagePath.c_str());
}

void ResourcePack::createManifestFile()
{
    if(_name.length() <= 0 || _packageUrl.length() <= 0){
        return;
    }
    
    string manifest = StringUtils::format(MANIFEST_LOCAL_FORMAT, _name.c_str());
    if(!FileUtils::getInstance()->isFileExist(manifest))
    {
        manifest = MANIFEST_TEMP_FILE;
    }
    
    if(manifest.length() > 0)
    {
        parse(manifest);
        modifyManifest(_json);
        saveManifest(_storagePath + _manifest);
    }
}

void ResourcePack::modifyManifestFile()
{
    createDirectory();
    
    if(!FileUtils::getInstance()->isFileExist(_manifest))
    {
        createManifestFile();
    }
}

void ResourcePack::parse(const std::string& manifestUrl)
{
    loadJson(manifestUrl);
}

void ResourcePack::loadJson(const std::string& url)
{
    std::string content;
    if (FileUtils::getInstance()->isFileExist(url))
    {
        // Load file content
        content = FileUtils::getInstance()->getStringFromFile(url);
        
        if (content.size() == 0)
        {
            CCLOG("Fail to retrieve local file content: %s\n", url.c_str());
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
}

void ResourcePack::modifyManifest(rapidjson::Document &json)
{
    if (!json.IsObject())
    {
        return;
    }
    
    // Retrieve remote manifest url
    if ( json.HasMember(KEY_MANIFEST_MANIFEST_URL) && json[KEY_MANIFEST_MANIFEST_URL].IsString() )
    {
        json[KEY_MANIFEST_MANIFEST_URL].SetString(_remoteManifestUrl.c_str(), json.GetAllocator());
    }
    
    // Retrieve local version
    if ( json.HasMember(KEY_MANIFEST_PACKAGE_URL) && json[KEY_MANIFEST_PACKAGE_URL].IsString() )
    {
        json[KEY_MANIFEST_PACKAGE_URL].SetString(_packageUrl.c_str(), json.GetAllocator());
    }
}

void ResourcePack::saveManifest(const std::string &filepath)
{
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    _json.Accept(writer);
    
    std::ofstream output(filepath, std::ofstream::out);
    if(!output.bad())
    {
        output << buffer.GetString() << std::endl;
    }
}

void ResourcePack::onSuccess()
{
    if(_delegate)
    {
        _delegate->onSuccess(this);
    }
    FileUtils::getInstance()->renameFile(_storagePath, MANIFEST_FILE_NAME, _manifest);
}

void ResourcePack::onError(const char* errMsg)
{
    if(_delegate)
    {
        _delegate->onError(this, errMsg);
    }
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
