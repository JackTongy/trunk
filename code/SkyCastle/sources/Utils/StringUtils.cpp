//
//  StringUtils.cpp
//  skycastle
//
//  Created by iMac on 16/5/16.
//
//

#include "StringUtils.h"

using namespace cocos2d;
using namespace std;

string StringUtil::format(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    
    char buffer[10240] = { 0 };
    vsprintf(buffer, format, ap);
    
    va_end(ap);
    return string(buffer);
}