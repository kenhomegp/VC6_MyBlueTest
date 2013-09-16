#ifndef _COMMON_H
#define _COMMON_H

//-----------------------------------------------------------------------------
//  ©w¸q¥¨¶°
#ifndef MEM_NEW
#define MEM_NEW(pData,type,nLen)	        \
	{								        \
		if (pData != NULL)			        \
		{							        \
			delete pData;			        \
			pData	=	NULL;               \
		}							        \
        pData   =   new type[nLen];         \
        memset(pData,0,sizeof(type)*nLen);  \
	}
#endif //MEM_NEW

#ifndef MEM_DEL
#define MEM_DEL(pData)                      \
	{								        \
		if (pData != NULL)			        \
		{							        \
			delete pData;			        \
			pData	=	NULL;               \
		}							        \
	}
#endif //MEM_NEW

#ifndef _DIM
#define dim(x)  (sizeof(x) / sizeof(x[0]))
#endif

//-----------------------------------------------------------------------------

#endif