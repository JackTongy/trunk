//
//  ISGameDataManager.cpp
//  skycastle
//
//  Created by iMac on 16/5/18.
//
//

#include "ISGameDataManager.h"
#include "ResourcePackData/ResourcePackData.h"

ISGameDataManager* ISGameDataManager::getInstance()
{
    static ISGameDataManager instance;
    return &instance;
}

ISGameDataManager::ISGameDataManager()
{
    clearData();
}

ISGameDataManager::~ISGameDataManager()
{
    clearData();
}

void ISGameDataManager::clearData()
{
    _serverType = ServerType::None;
    
    _resourcePackDataList.clear();
}

void ISGameDataManager::addResourcePackData(ResourcePackData* packData)
{
    for(auto it : _resourcePackDataList)
    {
        if (it->getName() == packData->getName())
        {
            _resourcePackDataList.pushBack(packData);
            break;
        }
    }
}

const cocos2d::Vector<ResourcePackData*> ISGameDataManager::getResourcePackDataList() const
{
    return _resourcePackDataList;
}

