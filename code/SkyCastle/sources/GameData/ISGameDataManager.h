//
//  ISGameDataManager.h
//  skycastle
//
//  Created by iMac on 16/5/18.
//
//

#ifndef __skycastle__ISGameDataManager__
#define __skycastle__ISGameDataManager__

#include "cocos2d.h"

#include <string>

class ResourcePackData;
class ISGameDataManager
{
public:
    enum class ServerType {
        None,
        Development, //开发服务器
        Testing, //测试服务器
        Production //产品服务器
    };
    
public:
    static ISGameDataManager* getInstance();
    
private:
    ISGameDataManager();
    ~ISGameDataManager();
    
public:
    void clearData();
    
    void addResourcePackData(ResourcePackData* packData);
    
    const cocos2d::Vector<ResourcePackData*> getResourcePackDataList() const;
    
public:
    CC_SYNTHESIZE(ServerType, _serverType, ServerType);
    
private:
    cocos2d::Vector<ResourcePackData*>      _resourcePackDataList;
};

#endif /* defined(__skycastle__ISGameDataManager__) */
