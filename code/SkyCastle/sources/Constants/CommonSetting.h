//
//  CommonSetting.h
//  skycastle
//
//  Created by iMac on 16/5/12.
//
//

#ifndef __skycastle__CommonSetting__
#define __skycastle__CommonSetting__

// 渲染间隔
#define DISPLAY_UPDATE_INTERVAL 1.0f / 30.0

// 逻辑处理间隔
#define LOGIC_UPDATE_INTERVAL 1.0f / 60

// design resolution 宽度
#define DESIGN_RESOLUTION_WIDTH 1136
#define DESIGN_RESOLUTION_WIDTH_DIFF 0

// design resolution 高度
#define DESIGN_RESOLUTION_HEIGHT 640

// design resolution policy
#define DESIGN_RESOLUTION_POLICY	ResolutionPolicy::NO_BORDER

//download folder
#define GameResourcesDownloadPath   "GameResources/"

// Scripts 脚本加密配置
#define SCRIPTES_ENCRYPT_KEY	"Isee-1990"
#define SCRIPTES_ENCRYPT_SIGN	"Isee-sigh-1990-0829"

//version,channel,tag
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#define RESOURCE_GAME_TAG "tag1.0"
#define APP_VERSION       "1.0.0"

#undef  APP_CHANNEL
#define APP_CHANNEL                         APP_CHANNEL_SLOT_JACKPOT

#elif CC_TARGET_PLATFORM == CC_PLATFORM_MAC

#define RESOURCE_GAME_TAG "tag1.0"
#define APP_VERSION       "1.0.0"

#undef  APP_CHANNEL
#define APP_CHANNEL                         APP_CHANNEL_SLOT_JACKPOT

#endif

#endif /* defined(__skycastle__CommonSetting__) */
