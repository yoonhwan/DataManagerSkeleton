//
//  YHMacro.h
//  DataManagerSkeleton
//
//  Created by yoonhwan on 13. 6. 3..
//
//

#ifndef DataManagerSkeleton_YHMacro_h
#define DataManagerSkeleton_YHMacro_h

#define SHARED_SINGLETON_INIT(name)         \
name* name::pRet = NULL;

#define SHARED_SINGLETON(name) \
static name* pRet;                          \
static name* shared##name()                 \
{                                           \
\
if(pRet == NULL)                        \
{                                       \
pRet = new name();                  \
if(pRet != NULL && pRet->init())    \
{                                   \
return pRet;                    \
}else                               \
{                                   \
delete pRet;                    \
pRet = NULL;                    \
return NULL;                    \
}                                   \
}else                                   \
return pRet;                        \
}                                           \
static void destroy##name()                 \
{                                           \
\
if(pRet != NULL)                        \
{                                       \
delete pRet;                        \
pRet = NULL;                        \
}                                       \
}

#endif
