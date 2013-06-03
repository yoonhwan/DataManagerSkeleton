//
//  YHGameDataManager.h
//  CATSnDOGS
//
//  Created by yoonhwan on 12. 8. 27..
//
//

#pragma once
#include "cocos2d.h"
USING_NS_CC;

#include "json.h"
#include "YHMacro.h"

class YHGameDataManager : public CCObject
{
public:
    SHARED_SINGLETON(YHGameDataManager);
    
    virtual ~YHGameDataManager();
    bool init();
    
    static std::string JsonToCharBuf(Json::Value json);
    static Json::Value CharBufToJson(std::string buffer);
    
    static CCObject* JsonToDict(Json::Value subJson);
    static CCObject* JsonToDictThreadSafe(Json::Value subJson);
    void ThreadSafeDictRelease(CCObject* obj);
    static Json::Value DictToJson(CCObject* dict);
    static void PrintDict(CCObject* dict, int depth = 0, bool isArray=false);
    static void PrintJSON(Json::Value json);
    
    static void log(const char * pszFormat, ...);
    
};

