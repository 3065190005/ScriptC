#pragma once
#ifndef _CERVER_LETMANAGER__
#define _CERVER_LETMANAGER__

#include "letMacro.h"
#include <list>

namespace Cervice {
	namespace Obj {

		class LetManager
		{
		public:
			enum class bType
			{
				None = 0,
				Static = 1,
				Dynamic = 2
			};

			typedef struct _block	// 内存块
			{
				char* ptr = nullptr;				// 块
				size_t ref = 0;						// 计数
				size_t lens = 0;					// 长度
				bool isArray = false;				// 是否数组
				std::list<_block*>::iterator iter;	// 指针索引
			}Block;

			static std::list<Block*>::iterator void_ptr; // 类nullptr

		public: // 构造函数
			LetManager();
			~LetManager();

		public: // 静态函数
			static LetManager* getInstance();	// 获取单例

		public: // 共用函数
			Block* malloc_L(bool type);		// 获取块
			void free_L(Block**);			// 回收块
			void realloc_L(Block**, size_t);	// 重新分配

		private: // 内部函数
			bool initialize();		// 初始化
			void destory();			// 销毁

			bool joinMemery();		// 内存增加

		protected:
		class Gc { public:~Gc(); };		// 内存回收GC

		private: // 变量
			bool m_init_success;				// 成功初始化
			static LetManager* m_instance;		// 静态

			size_t m_free_count;				// 空余块量
			size_t m_malloc_count;				// 已用块量
			std::list<Block*> m_Free_Blocks;	// 空余块
			std::list<Block*> m_Malloc_Blocks;	// 已使块

			size_t m_free_count_d;					// 空余块量 Dymanic
			size_t m_malloc_count_d;				// 已用块量 Dymanic
			std::list<Block*> m_Free_Blocks_D;		// 空余块 Dymanic
			std::list<Block*> m_Malloc_Blocks_D;	// 已使块 Dymanic
		};

		using Block = struct LetManager::Block;
		using Ptr = std::list<Block*>::iterator;
		using rPtr = std::list<Block*>::reverse_iterator;

	}
}

#endif