//
//  ScriptManager.cpp
//  skycastle
//
//  Created by iMac on 16/5/12.
//
//

#include "ScriptManager.h"
#include "CCLuaEngine.h"
#include "Constants/CommonSetting.h"

#include "ConfigParser.h"
#include "lua_module_register.h"

// extra lua module
#include "cocos2dx_extra.h"
#include "lua_extensions/lua_extensions_more.h"
#include "luabinding/lua_cocos2dx_extension_filter_auto.hpp"
#include "luabinding/lua_cocos2dx_extension_nanovg_auto.hpp"
#include "luabinding/lua_cocos2dx_extension_nanovg_manual.hpp"
#include "luabinding/cocos2dx_extra_luabinding.h"
#include "luabinding/HelperFunc_luabinding.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "luabinding/cocos2dx_extra_ios_iap_luabinding.h"
#endif
#if ANYSDK_DEFINE > 0
#include "anysdkbindings.h"
#include "anysdk_manual_bindings.h"
#endif

static void quick_module_register(lua_State *L)
{
    luaopen_lua_extensions_more(L);
    
    lua_getglobal(L, "_G");
    if (lua_istable(L, -1))//stack:...,_G,
    {
        register_all_quick_manual(L);
        // extra
        luaopen_cocos2dx_extra_luabinding(L);
        register_all_cocos2dx_extension_filter(L);
        register_all_cocos2dx_extension_nanovg(L);
        register_all_cocos2dx_extension_nanovg_manual(L);
        luaopen_HelperFunc_luabinding(L);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        luaopen_cocos2dx_extra_ios_iap_luabinding(L);
#endif
    }
    lua_pop(L, 1);
}

//----------------------------ScriptManager-------------------------//
static ScriptManager* _instance = nullptr;

ScriptManager* ScriptManager::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new ScriptManager();
    }
    
    return _instance;
}

void ScriptManager::end()
{
    CC_SAFE_DELETE(_instance);
}

ScriptManager::ScriptManager()
{
    
}

ScriptManager::~ScriptManager()
{
    
}

void ScriptManager::initialize()
{
    registerQuickModule();
    registerBindingModule();
}

void ScriptManager::registerQuickModule()
{
    auto engine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);
    lua_State* L = engine->getLuaStack()->getLuaState();
    lua_module_register(L);
    
    // use Quick-Cocos2d-X
    quick_module_register(L);
    
    LuaStack* stack = engine->getLuaStack();
#if ANYSDK_DEFINE > 0
    lua_getglobal(stack->getLuaState(), "_G");
    tolua_anysdk_open(stack->getLuaState());
    tolua_anysdk_manual_open(stack->getLuaState());
    lua_pop(stack->getLuaState(), 1);
#endif
    
    stack->setXXTEAKeyAndSign(SCRIPTES_ENCRYPT_KEY, strlen(SCRIPTES_ENCRYPT_KEY), SCRIPTES_ENCRYPT_SIGN, strlen(SCRIPTES_ENCRYPT_SIGN));
}

void ScriptManager::registerBindingModule()
{
    
}

void ScriptManager::executeScriptFile(const char* scriptFile)
{
    if (NULL != scriptFile)
    {
        LuaEngine *pEngine = LuaEngine::getInstance();
        LuaStack *pStack = pEngine->getLuaStack();
        
        std::string path = FileUtils::getInstance()->fullPathForFilename(scriptFile);
        
        size_t pos;
        while ((pos = path.find_first_of("\\")) != std::string::npos) {
            path.replace(pos, 1, "/");
        }
        size_t p = path.find_last_of("/\\");
        if (p != path.npos) {
            const string dir = path.substr(0, p);
            pStack->addSearchPath(dir.c_str());
            
            p = dir.find_last_of("/\\");
            if (p != dir.npos) {
                std::string __path = dir.substr(0, p);
                pStack->addSearchPath(__path.c_str());
            }
        }
        // set script path
        string env = "__LUA_STARTUP_FILE__=\"";
        env.append(path);
        env.append("\"");
        pEngine->executeString(env.c_str());
        pEngine->executeScriptFile(path.c_str());
    }
}

