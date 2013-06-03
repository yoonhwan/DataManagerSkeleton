//
//  YHSQLiteHelper.cpp
//  CATSnDOGS
//
//  Created by yoonhwan.ko on 12. 9. 10..
//
//

#include "YHSQLiteHelper.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
#include "sqlite3.h"
#include "CCFileUtils.h"
#include <fstream>
#endif


const char *resourcePath(std::string fileName)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	CCFileUtils *util = CCFileUtils::sharedFileUtils();
	const char *path = util->fullPathFromRelativePath(fileName.c_str());
	std::string wpath = util->getWriteablePath() + std::string(fileName);
    
	FILE *sqlFile;
	if ((sqlFile = fopen(wpath.c_str(), "r")) == NULL) {
		std::ifstream f1(path, std::fstream::binary);
		std::ofstream f2(wpath.c_str(), std::fstream::trunc|std::fstream::binary);
		f2 << f1.rdbuf();
	}
	fclose(sqlFile);
	
	return wpath.c_str();
#else
    
    CCAssert(false, "");
#endif
}

CCArray *YHSQLiteHelper::excute(const char *query, std::string fileName)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
//	printf("%s\n\n", query);
	CCArray *rtn = NULL;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	const char *path = resourcePath(fileName);
	
	//connect DB
	if (sqlite3_open(path, &db) == SQLITE_OK) {
		sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);
		
		//excute query
		if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
			sqlite3_exec(db, "COMMIT", 0, 0, 0);
			
			//iuquiry data
			if (rtn == NULL) rtn = new CCArray();
            
 			while (sqlite3_step(stmt) == SQLITE_ROW) {
				
				//make a dictionary for a row
				CCDictionary *dict = new CCDictionary();
				int coulmnCount = sqlite3_column_count(stmt);
				for (int i = 0; i < coulmnCount; i++) {
					char* result = (char *)sqlite3_column_text(stmt, i);
					if(result != NULL)
					{
						CCString *value = CCString::create(result);
						dict->setObject(value, sqlite3_column_name(stmt, i));
//                        value->release();
                        
					};
				}
				rtn->addObject(dict);
                dict->release();
			}
			
			//query succeed but no data
			if (rtn->count() == 0) {
				CCDictionary *dict = new CCDictionary();
                CCString* value = new CCString("OK");
				dict->setObject(value, "query_result");
                value->release();
				rtn->addObject(dict);
                dict->release();
			}
		} else {
			fprintf(stderr ,"query failed with status %s\n", sqlite3_errmsg(db));
		}
		
		//disconnect DB
        sqlite3_reset(stmt);
        sqlite3_finalize(stmt);
        
		sqlite3_exec(db, "END TRANSACTION", 0, 0, 0);
		sqlite3_close(db);
	} else {
		fprintf(stderr ,"open failed with status %s\n path = [%s]\n", sqlite3_errmsg(db),path);
	}
	
	if (rtn != NULL && rtn->count() > 0) {
        
//        CCLOG("excute = %x",rtn);
		return rtn;
	} else {
		return NULL;
	}
#else
    
    CCAssert(false, "");
#endif
}

std::string YHSQLiteHelper::clause(CCDictionary *dict, bool isOption)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	std::string clause(" ");
	CCArray *keys = dict->allKeys();
	int count = keys->count();
	if (count > 0) clause.append(isOption ? " WHERE " : " SET ");
	for (int i = 0; i < count; i++) {
		CCString *key = CCString::create(((CCString *)keys->objectAtIndex(i))->getCString());
		clause.append(key->getCString());
		clause.append(" = ");
		clause.append("'");
		clause.append(dict->valueForKey(key->getCString())->getCString());
		clause.append("'");
		
		if (i < count - 1) {
			clause.append(isOption ? " AND " : ", ");
		}
	}
	return clause;
#else
    
    CCAssert(false,"");
#endif
}

bool YHSQLiteHelper::insertQuery(std::string dbName, const char *tableName, CCDictionary *values)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	CCAssert(values != NULL, "values dictionary should not null.");
	std::string query("INSERT INTO ");
	query.append(tableName);
	query.append(" (");
	std::string valueStr(" VALUES (");
	
	CCArray *keys = values->allKeys();
	int count = keys->count();
	for (int i = 0; i < count; i++) {
		CCString *key = CCString::create(((CCString *)keys->objectAtIndex(i))->getCString());
		query.append(key->getCString());
		valueStr.append("'");
		valueStr.append(values->valueForKey(key->getCString())->getCString());
		valueStr.append("'");
		if (i < count - 1) {
			query.append(", ");
			valueStr.append(", ");
		} else {
			query.append(")");
			valueStr.append(")");
		}
        
	}
	query.append(valueStr);
	
	//쿼리 실행
	CCArray *result = YHSQLiteHelper::excute(query.c_str(), dbName);
	query.clear();

	//결과 리턴
	if (result != NULL && result->count() > 0) {
        CC_SAFE_RELEASE_NULL(result);
		return true;
	} else {
        CC_SAFE_RELEASE_NULL(result);
		return false;
	}
#else
    
    CCAssert(false, "");
#endif
}

bool YHSQLiteHelper::insertQuery(std::string dbName, const char *tableName, CCArray *values)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	CCAssert(values != NULL, "values dictionary should not null.");
	std::string query("");
	
	for (int j = 0; j < values->count(); j++) {
		CCDictionary *dict = (CCDictionary *)values->objectAtIndex(j);
		query.append("INSERT INTO ");
		query.append(tableName);
		query.append(" (");
		std::string valueStr(" VALUES (");
		CCArray *keys = dict->allKeys();
		int count = keys->count();
		for (int i = 0; i < count; i++) {
			CCString *key = CCString::create(((CCString *)keys->objectAtIndex(i))->getCString());
			query.append(key->getCString());
			valueStr.append("'");
			valueStr.append(dict->valueForKey(key->getCString())->getCString());
			valueStr.append("'");
			if (i < count - 1) {
				query.append(", ");
				valueStr.append(", ");
			} else {
				query.append(")");
				valueStr.append(")");
			}
			
		}
		query.append(valueStr);
		query.append(";\n");
	}
	
	//쿼리 실행
	CCLOG("%s", query.c_str());
	CCArray *result = YHSQLiteHelper::excute(query.c_str(), dbName);
	query.clear();
	
	//결과 리턴
	if (result != NULL && result->count() > 0) {
        CC_SAFE_RELEASE_NULL(result);
		return true;
	} else {
        CC_SAFE_RELEASE_NULL(result);
		return false;
	}
#else
    CCAssert(false, "");
#endif
}

CCArray *YHSQLiteHelper::selectQuery(std::string dbName, const char *tableName, CCDictionary *option)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	std::string query("SELECT * FROM ");
	query.append(tableName);
	if (option != NULL) query.append(YHSQLiteHelper::clause(option, true));
    
	//쿼리 실행
	CCArray *result = YHSQLiteHelper::excute(query.c_str(), dbName);
	query.clear();
	
	//결과 리턴
	if (result != NULL && result->count() > 0) {
		return result;
	} else {
        CC_SAFE_RELEASE_NULL(result);
		return false;
	}
#else
    CCAssert(false, "");
#endif
}

CCArray *YHSQLiteHelper::selectQuery(std::string dbName, const char *tableName, CCDictionary *option, std::string orderKey, bool asc)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	std::string query("SELECT * FROM ");
	query.append(tableName);
	if (option != NULL) query.append(YHSQLiteHelper::clause(option, true));
    
    
    if (orderKey.length() > 0) {
        query.append(" ORDER BY ");
        query.append(orderKey);
        if(asc)
            query.append(" ASC");
        else
            query.append(" DESC");
    }
    
	//쿼리 실행
	CCArray *result = YHSQLiteHelper::excute(query.c_str(), dbName);
	query.clear();
	
	//결과 리턴
	if (result != NULL && result->count() > 0) {
		return result;
	} else {
        CC_SAFE_RELEASE_NULL(result);
		return false;
	}
#else
    CCAssert(false, "");
#endif
}

bool YHSQLiteHelper::updateQuery(std::string dbName, const char *tableName, CCDictionary *values, CCDictionary *option)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	CCAssert(option != NULL, "option dictionary should not null.");
	std::string query("UPDATE ");
	query.append(tableName);
	query.append(YHSQLiteHelper::clause(values, false));
	query.append(YHSQLiteHelper::clause(option, true));
	
	//쿼리 실행
	CCArray *result = YHSQLiteHelper::excute(query.c_str(), dbName);
	query.clear();

	//결과 리턴
	if (result != NULL && result->count() > 0) {
        CC_SAFE_RELEASE_NULL(result);
		return true;
	} else {
        CC_SAFE_RELEASE_NULL(result);
		return false;
	}
#else
    CCAssert(false, "");
#endif
}

bool YHSQLiteHelper::deleteQuery(std::string dbName, const char *tableName, CCDictionary *option)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
	CCAssert(option != NULL, "option dictionary should not null.");
	std::string query("DELETE FROM ");
	query.append(tableName);
	query.append(YHSQLiteHelper::clause(option, true));
	
	//쿼리 실행
	CCArray *result = YHSQLiteHelper::excute(query.c_str(), dbName);
	query.clear();

	//결과 리턴
	if (result != NULL && result->count() > 0) {
        CC_SAFE_RELEASE_NULL(result);
		return true;
	} else {
        CC_SAFE_RELEASE_NULL(result);
		return false;
	}
#else
    CCAssert(false, "");
#endif
}

bool YHSQLiteHelper::test()
{
    return true;
	
//    //C
//	CCDictionary *values = CCDictionary::create();
//	values->setObject(CCString::create("test"), "Code");
//	values->setObject(CCString::create("test"), "CodeName");
//	values->setObject(CCString::create("test"), "GroupCode");
//	values->setObject(CCString::create("test"), "CodeDesc");
//	if (YHSQLiteHelper::insertQuery("data.sqlite", "M3CodeTB", values)) {
//		printf("sqlite insert ok\n");
//	}
//	
//	//R
//	CCArray *result = YHSQLiteHelper::selectQuery("data.sqlite", "M3CodeTB", NULL);
//	if (result != NULL && result->count() > 0) {
//		CCDictionary *dict = (CCDictionary *)result->objectAtIndex(0);
//		printf(" %s \n", dict->valueForKey("CodeDesc")->getCString());
//	}
//    
//	//U
//	values->setObject(CCString::create("good"), "CodeDesc");
//	CCDictionary *option = CCDictionary::create();
//	option->setObject(CCString::create("test"), "Code");
//	if (YHSQLiteHelper::updateQuery("data.sqlite", "M3CodeTB", values, option)) {
//		printf("sqlite update ok\n");
//	}
//	
//	//confirm
//	result = YHSQLiteHelper::selectQuery("data.sqlite", "M3CodeTB", NULL);
//	if (result != NULL && result->count() > 0) {
//		CCDictionary *dict = (CCDictionary *)result->objectAtIndex(0);
//		printf(" %s \n", dict->valueForKey("CodeDesc")->getCString());
//	}
//    
//	//D
//	if (YHSQLiteHelper::deleteQuery("data.sqlite", "M3CodeTB", option)) {
//		printf("sqlite delete ok\n");
//	}
//	
//	
//	result = YHSQLiteHelper::selectQuery("data.sqlite", "M3CardTB", NULL);
//	for (int i = 0; i < result->count(); i++) {
//		printf("record#%d", i);
//		CNDGameDataManager::PrintDict((CCDictionary *)result->objectAtIndex(i));
//	}
//    
//	return false;
}
