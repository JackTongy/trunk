//
//  StringUtils.h
//  skycastle
//
//  Created by iMac on 16/5/16.
//
//

#ifndef __skycastle__StringUtils__
#define __skycastle__StringUtils__

#include "cocos2d.h"
#include <string>

class StringUtil
{
public:
    std::string format(const char *fmt, ...);
};

#endif /* defined(__skycastle__StringUtils__) */
