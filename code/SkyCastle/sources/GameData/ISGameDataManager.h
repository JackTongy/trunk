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

class ISGameDataManager
{
public:
    static ISGameDataManager* getInstance();
    static void end();
    
private:
    ISGameDataManager();
    ~ISGameDataManager();
    
public:
    std::string getDownloadUrl();
};

#endif /* defined(__skycastle__ISGameDataManager__) */
