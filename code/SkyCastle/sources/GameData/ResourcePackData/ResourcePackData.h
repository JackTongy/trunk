//
//  ResourcePackData.h
//  skycastle
//
//  Created by iMac on 16/5/20.
//
//

#ifndef __skycastle__ResourcePackData__
#define __skycastle__ResourcePackData__

#include "cocos2d.h"

#include <string>

class ResourcePackData : public cocos2d::Ref
{
public:
    ResourcePackData();
    ~ResourcePackData();
    
public:
    //name
    CC_SYNTHESIZE(std::string, _name, Name);
    
    //save path
    CC_SYNTHESIZE(std::string, _directory, Directory);
    
    //package url
    CC_SYNTHESIZE(std::string, _packageUrl, PackageUrl);
    
    //remote manifest url
    CC_SYNTHESIZE(std::string, _remoteManifestUrl, RemoteManifestUrl);
};

#endif /* defined(__skycastle__ResourcePackData__) */
