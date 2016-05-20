//
//  ResourceManifest.cpp
//  skycastle
//
//  Created by iMac on 16/5/17.
//
//

#include "ResourceManifest.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"
#include "json/filestream.h"

#include <string>
#include <fstream>

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
using namespace cocos2d::extension;

using namespace std;

//ResourceManifest* ResourceManifest::create(const std::string& manifestUrl)
//{
//    ResourceManifest* manifest = new(std::nothrow) ResourceManifest(manifestUrl);
//    if(manifest)
//    {
//        manifest->autorelease();
//        return manifest;
//    }
//    
//    delete manifest;
//    manifest = nullptr;
//    
//    return manifest;
//}

ResourceManifest::ResourceManifest():
_localManifestUrl(""),
_domainName(""),
_remoteManifestUrl(""),
_packageUrl("")
{

}

ResourceManifest::~ResourceManifest()
{
    
}

void ResourceManifest::setLocalManifestUrl(const std::string& manifestUrl)
{
    _localManifestUrl = manifestUrl;
}

void ResourceManifest::setPackageUrl(const std::string &packageUrl)
{
    _packageUrl = packageUrl;
}

void ResourceManifest::setSavePath(const std::string &filepath, const std::string& fileName)
{
    _savePath = filepath;
    _saveFileName = fileName;
}

void ResourceManifest::prepareManifest()
{
    if (FileUtils::getInstance()->isFileExist(_localManifestUrl))
    {
        parseJsonDoc(_localManifestUrl);
        modifyManifest(_jsonDoc);
        saveManifest(_savePath + _localManifestUrl);
    }
    else
    {
        //not exist, then create it

        
    }
}

void ResourceManifest::endManifest()
{
    if (FileUtils::getInstance()->isFileExist(_localManifestUrl))
    {
        parseJsonDoc(_localManifestUrl);
        revertManifest(_jsonDoc);
        saveManifest(_savePath + _localManifestUrl);
    }
    
    FileUtils::getInstance()->renameFile(_savePath, _localManifestUrl, _saveFileName);
}

void ResourceManifest::modifyManifest(rapidjson::Document &json)
{
    if (!json.IsObject())
    {
        return;
    }
    
    // Retrieve remote manifest url
    if ( json.HasMember(KEY_MANIFEST_MANIFEST_URL) && json[KEY_MANIFEST_MANIFEST_URL].IsString() )
    {
        _remoteManifestUrl = json[KEY_MANIFEST_MANIFEST_URL].GetString();
        string remoteManifestUrl = _domainName + _remoteManifestUrl;
        json[KEY_MANIFEST_MANIFEST_URL].SetString(remoteManifestUrl.c_str(), json.GetAllocator());
    }
    
    // Retrieve local version
    if ( json.HasMember(KEY_MANIFEST_PACKAGE_URL) && json[KEY_MANIFEST_PACKAGE_URL].IsString() )
    {
        if(_packageUrl.length() > 0)
        {
            json[KEY_MANIFEST_PACKAGE_URL].SetString(_packageUrl.c_str(), json.GetAllocator());
        }
        else
        {
            _packageUrl = json[KEY_MANIFEST_PACKAGE_URL].GetString();
            string packageUrl = _domainName + _packageUrl;
            json[KEY_MANIFEST_PACKAGE_URL].SetString(packageUrl.c_str(), json.GetAllocator());
        }
    }
}

void ResourceManifest::revertManifest(rapidjson::Document &json)
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
        if(_packageUrl.length() > 0)
        {
            json[KEY_MANIFEST_PACKAGE_URL].SetString(_packageUrl.c_str(), json.GetAllocator());
        }
    }

}

void ResourceManifest::parseJsonDoc(const std::string& manifestUrl)
{
    loadJsonDoc(manifestUrl);
}

void ResourceManifest::loadJsonDoc(const std::string& url)
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
            _jsonDoc.Parse<0>(content.c_str());
            // Print error
            if (_jsonDoc.HasParseError()) {
                size_t offset = _jsonDoc.GetErrorOffset();
                if (offset > 0)
                    offset--;
                std::string errorSnippet = content.substr(offset, 10);
                CCLOG("File parse error %s at <%s>\n", _jsonDoc.GetParseError(), errorSnippet.c_str());
            }
        }
    }
}

void ResourceManifest::saveManifest(const std::string &filepath)
{
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    _jsonDoc.Accept(writer);
    
    std::ofstream output(filepath, std::ofstream::out);
    if(!output.bad())
    {
        output << buffer.GetString() << std::endl;
    }
}
