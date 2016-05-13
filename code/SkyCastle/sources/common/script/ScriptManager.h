//
//  ScriptManager.h
//  skycastle
//
//  Created by iMac on 16/5/12.
//
//

#ifndef __skycastle__ScriptManager__
#define __skycastle__ScriptManager__

#include "cocos2d.h"

USING_NS_CC;

class ScriptManager
{
public:
    static ScriptManager* getInstance();
    static void end();
private:
    ScriptManager();
    ~ScriptManager();
    
public:
    void initialize();
    void executeScriptFile(const char* scriptFile);
    
private:
    void registerQuickModule();
    void registerBindingModule();
};

#endif /* defined(__skycastle__ScriptManager__) */
