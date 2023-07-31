#pragma once
#ifndef __AUTOMEM_LETMARCRO__
#define __AUTOMEM_LETMARCRO__

// manager
#define MANAGER_BLOCKS_COUNT_MAX 4096	 // 最大内存块数量
#define MANAGER_STATIC_BYTE_SIZE 8		 // 静态块常量字节数
#define MANAGER_DYNAMIC_BYTE_SIZE 32	 // 动态块最小字节数
#define MANAGER_MEMERYS_SPACE_STEP 1024	 // 每次扩充步长

namespace AutoMem {
	namespace Obj {
		enum class NatureType {
			none = 0 << 0,
			ptr = 1 << 0,
			cls = 1 << 1,
		};
	}
}


// object
#define OBJ_IF_CALL(expr,func) if(expr)func
#define OBJ_IF_BREAK(expr) if(expr)break
#define OBJ_IF_RETURN(expr,var) if(expr)return var
#define OBJ_IFB_CALL(expr,func) if(expr){func;break;}
#endif