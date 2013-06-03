//
//  YHGameDataManager.cpp
//  CATSnDOGS
//
//  Created by yoonhwan on 12. 8. 27..
//
//

#include "YHGameDataManager.h"
#include "json.h"

SHARED_SINGLETON_INIT(YHGameDataManager);

YHGameDataManager::~YHGameDataManager()
{
}

bool YHGameDataManager::init()
{
    return true;
    
}

std::string YHGameDataManager::JsonToCharBuf(Json::Value json)
{
    Json::FastWriter writer;
    // Make a new JSON document for the configuration. Preserve original comments.
    std::string outputConfig;
    outputConfig.assign(writer.write(json));
    
    outputConfig = outputConfig.substr(0,outputConfig.length()-1);
    return outputConfig;
}

Json::Value YHGameDataManager::CharBufToJson(std::string buffer)
{
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( buffer, root );
    if ( !parsingSuccessful )
    {
        // report to the user the failure and their locations in the document.
        CCLOG("Failed to parse configuration %s",reader.getFormatedErrorMessages().c_str());
        return NULL;
    }
    return root;
}

void YHGameDataManager::PrintDict(CCObject* obj, int depth, bool isArray)
{
#if USE_DEBUG_CONSOLE
    if(dynamic_cast<CCArray*>(obj))
    {
        printf("[\n");
        depth+=1;
        CCArray* array = dynamic_cast<CCArray*>(obj);
        for (int i = 0; i < array->count(); i++) {
            
            CCObject* subObj = array->objectAtIndex(i);
            PrintDict(subObj, depth+1);
        }
        depth-=1;
        for(int i = 0 ; i < depth; i++)
            printf("\t");
        printf("]\n");
    }else if(dynamic_cast<CCDictionary*>(obj))
    {
        for(int i = 0 ; i < depth; i++)
            printf("\t");
        printf("{\n");
        depth+=1;
        
        CCDictionary* dict = dynamic_cast<CCDictionary*>(obj);
        CCDictElement *pElement, *tmp;
        HASH_ITER(hh, dict->m_pElements, pElement, tmp)
        {
            for(int i = 0 ; i < depth; i++)
                printf("\t");
            printf("\"%s\" : ",pElement->getStrKey());
            if(dynamic_cast<CCArray*>(pElement->getObject()))
            {
                CCArray* array = dynamic_cast<CCArray*>(pElement->getObject());
                PrintDict(array, depth+1);
            }else if(dynamic_cast<CCDictionary*>(pElement->getObject()))
            {
                CCDictionary* subDict = dynamic_cast<CCDictionary*>(pElement->getObject());
                PrintDict(subDict, depth+1);
            }else if(dynamic_cast<CCString*>(pElement->getObject()))
            {
                CCString* str = dynamic_cast<CCString*>(pElement->getObject());
                PrintDict(str);
            }else
            {
                CCAssert(false, "");
            }
        }
        depth-=1;
        for(int i = 0 ; i < depth; i++)
            printf("\t");
        printf("}\n");
        
    }else if(dynamic_cast<CCString*>(obj))
    {
        CCString* str = dynamic_cast<CCString*>(obj);
        
        for(int i = 0 ; i < depth; i++)
            printf("\t");
        printf("\"%s\",\n", str->getCString());
    }else
    {
        CCAssert(false, "");
    }
#endif
}

CCObject* YHGameDataManager::JsonToDictThreadSafe(Json::Value subJson)
{
    if(subJson.isArray())
    {
        CCArray* array = new CCArray;
        for (Json::Value::iterator iter = subJson.begin(); iter != subJson.end(); iter++)
        {
            const Json::Value subDoc = *iter;
            CCObject* subDict = JsonToDictThreadSafe(subDoc);
            array->addObject(subDict);
            CC_SAFE_RELEASE_NULL(subDict);
        }
        return array;
    }else if(subJson.isObject())
    {
        CCDictionary* dict = new CCDictionary;
        for( Json::ValueIterator itr = subJson.begin() ; itr != subJson.end() ; itr++ ) {
            
            Json::Value obj = subJson[itr.key().asString()];
            if(obj.isArray())
            {
                CCObject* subDict = JsonToDictThreadSafe(obj);
                dict->setObject(subDict, itr.key().asString().c_str());
                CC_SAFE_RELEASE_NULL(subDict);
                
            }else if(obj.isObject())
            {
                CCObject* subDict = JsonToDictThreadSafe(obj);
                dict->setObject(subDict, itr.key().asString().c_str());
                CC_SAFE_RELEASE_NULL(subDict);
                
            }else
            {
                CCObject* subDict = JsonToDictThreadSafe(obj);
                dict->setObject(subDict, itr.key().asString().c_str());
                CC_SAFE_RELEASE_NULL(subDict);
                
            }
        }
        return dict;
        
    }else if(subJson.isString())
    {
        return new CCString(subJson.asString());
    }else if(subJson.isInt())
    {
        CCString* value = new CCString();
        value->initWithFormat("%d",subJson.asInt());
        return value;
    }else if(subJson.isBool())
    {
        CCString* value = new CCString("true");
        return value;
    }else if(subJson.isUInt())
    {
        CCString* value = new CCString();
        value->initWithFormat("%u",subJson.asUInt());
        return value;
    }else if(subJson.isDouble())
    {
        CCString* value = new CCString();
        value->initWithFormat("%f",subJson.asDouble());
        return value;
    }else
    {
        CCAssert(false, "");
    }
    
    return NULL;
}

CCObject* YHGameDataManager::JsonToDict(Json::Value subJson)
{
    if(subJson.isArray())
    {
        CCArray* array = CCArray::create();
        for (Json::Value::iterator iter = subJson.begin(); iter != subJson.end(); iter++)
        {
            const Json::Value subDoc = *iter;
            CCObject* subDict = JsonToDict(subDoc);
            array->addObject(subDict);
        }
        return array;
    }else if(subJson.isObject())
    {
        CCDictionary* dict = CCDictionary::create();
        for( Json::ValueIterator itr = subJson.begin() ; itr != subJson.end() ; itr++ ) {

            Json::Value obj = subJson[itr.key().asString()];
            if(obj.isArray())
            {
                CCObject* subDict = JsonToDict(obj);
                dict->setObject(subDict, itr.key().asString().c_str());
                
            }else if(obj.isObject())
            {
                CCObject* subDict = JsonToDict(obj);
                dict->setObject(subDict, itr.key().asString().c_str());
                
            }else
            {
                dict->setObject(JsonToDict(obj), itr.key().asString().c_str());
            }
        }
        return dict;
        
    }else if(subJson.isString())
    {
        return CCString::create(subJson.asString());
    }else if(subJson.isInt())
    {
        return CCString::createWithFormat("%d",subJson.asInt());
    }else if(subJson.isBool())
    {
        CCString* value = CCString::createWithFormat("true");
        return value;
    }else if(subJson.isUInt())
    {
        return CCString::createWithFormat("%u",subJson.asUInt());
    }else if(subJson.isDouble())
    {
        return CCString::createWithFormat("%f",subJson.asDouble());
    }else
    {
        CCAssert(false, "");
    }
    
    return NULL;
    
}

Json::Value YHGameDataManager::DictToJson(CCObject* obj)
{
    Json::Value json;
    
    if(dynamic_cast<CCArray*>(obj))
    {
        
        CCArray* array = dynamic_cast<CCArray*>(obj);
        for (int i = 0; i < array->count(); i++) {
            
            CCObject* subObj = array->objectAtIndex(i);
            json.append(DictToJson(subObj));
        }
        return json;
    }else if(dynamic_cast<CCDictionary*>(obj))
    {
        
        CCDictionary* dict = dynamic_cast<CCDictionary*>(obj);
        CCDictElement *pElement, *tmp;
        HASH_ITER(hh, dict->m_pElements, pElement, tmp)
        {
            if(dynamic_cast<CCArray*>(pElement->getObject()))
            {
                CCArray* array = dynamic_cast<CCArray*>(pElement->getObject());
                json[pElement->getStrKey()] = DictToJson(array);
            }else if(dynamic_cast<CCDictionary*>(pElement->getObject()))
            {
                CCDictionary* subDict = dynamic_cast<CCDictionary*>(pElement->getObject());
                json[pElement->getStrKey()] = DictToJson(subDict);
            }else if(dynamic_cast<CCString*>(pElement->getObject()))
            {
                CCString* str = dynamic_cast<CCString*>(pElement->getObject());
                json[pElement->getStrKey()] = str->getCString();
                
            }else
            {
                CCAssert(false, "");
            }
        }
        return json;
    }else if(dynamic_cast<CCString*>(obj))
    {
        CCString* str = dynamic_cast<CCString*>(obj);
        json = str->getCString();
        return json;
        
    }else
    {
        CCAssert(false, "");
    }
    
}

void YHGameDataManager::PrintJSON(Json::Value json)
{
#if USE_DEBUG_CONSOLE
    Json::StyledWriter writer;
    std::string outputConfig = writer.write( json );
    YHGameDataManager::log("%s",outputConfig.c_str());
#endif
}

void YHGameDataManager::log(const char * pszFormat, ...)
{
    printf("YHGameDataManager Log: ");
    char* szBuf = new char[kMaxLogLen*100];
    
    va_list ap;
    va_start(ap, pszFormat);
    vsprintf(szBuf, pszFormat, ap);
    va_end(ap);
    printf("%s", szBuf);
    printf("\n");
    
    delete[] szBuf;
}
