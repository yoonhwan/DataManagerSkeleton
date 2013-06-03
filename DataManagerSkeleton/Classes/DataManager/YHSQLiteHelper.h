//
//  YHSQLiteHelper.h
//  CATSnDOGS
//
//  Created by yoonhwan.ko on 12. 9. 10..
//
//

#ifndef __CATSnDOGS__YHSQLiteHelper__
#define __CATSnDOGS__YHSQLiteHelper__
#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class YHSQLiteHelper {
	
public:
	static CCArray *excute(const char *query, std::string fileName);
	static std::string clause(CCDictionary *dict, bool isOption);
	static bool insertQuery(std::string dbName, const char *tableName, CCDictionary *values);
	static bool insertQuery(std::string dbName, const char *tableName, CCArray *values);
	static CCArray *selectQuery(std::string dbName, const char *tableName, CCDictionary *option);
    static CCArray* selectQuery(std::string dbName, const char *tableName, CCDictionary *option, std::string orderKey, bool asc);
	static bool updateQuery(std::string dbName, const char *tableName, CCDictionary *values, CCDictionary *option);
	static bool deleteQuery(std::string dbName, const char *tableName, CCDictionary *option);
	static bool test();
};

#endif /* defined(__CATSnDOGS__YHSQLiteHelper__) */
