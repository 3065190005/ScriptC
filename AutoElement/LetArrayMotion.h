#pragma once
#ifndef _CERVER_LETARRAYMOTION__
#define _CERVER_LETARRAYMOTION__

#include "LetManager.h"
#include "LetObject.h"

#include <optional>
#include <variant>

namespace Cervice {
	namespace Obj {

		/**********
		* 数组操作类
		* 仅支持原数组变量，不识别指针
		***********/
		class LetArrayMotion
		{
		public:

		/**********************
		* 字符串对比  
		* cmp1 == cmp2 -> 0
		* cmp1 > cmp2 -> -1
		* cmp1 < cmp2 -> 1
		* error -> 2
		**********************/
		public: 
			static numberT ArrayCmp(LetObject* cmp1, LetObject* cmp2);
		private:
			static numberT cmpNum(LetObject*, LetObject*);
			static numberT cmpStr(LetObject*, LetObject*);

		/**********************
		* 返回字符串最大值
		* 成功返回最大值，否则返回null
		**********************/
		public: 
			static LetObject ArrayMax(LetObject*);

		/**********************
		* 返回字符串最小值
		* 成功返回最小值，否则返回null
		**********************/
		public:
			static LetObject ArrayMin(LetObject*);

		/**********************
		* 末尾添加元素
		* 成功返回true，否则返回false
		**********************/
		public:
			static bool ArrayAppend(LetObject*,LetObject*);

			template<typename... Ts>
			static bool ArrayAppends(LetObject* var, LetObject* va1, Ts... args);
			static bool ArrayAppends(LetObject* var) { return true; };

		/**********************
		* 检测元素出现次数
		* 成功返回次数，否则返回-1
		**********************/
		public:
			static numberT ArrayCount(LetObject*, LetObject*);

		/**********************
		* 将目标展开添加到数组末尾
		* 成功返回true，否则返回false
		**********************/
		public:
			static bool ArrayExtend(LetObject*, LetObject*);

		/**********************
		* 返回数组最后一个元素
		* 成功返回元素
		**********************/
		public:
			static std::optional<LetObject> ArrayBack(LetObject*);

		/**********************
		* 返回数组第一个元素
		* 成功返回元素
		**********************/
		public:
			static std::optional<LetObject> ArrayFront(LetObject*);


		/**********************
		* 查找指定第一个数组的为吹
		* 成功返回下标
		**********************/
		public:
			static std::optional<std::variant<numberT,std::string>> 
				ArrayIndex(LetObject*,LetObject*);

		/**********************
		* 在指定下标后面传入新元素
		* 成功返回true，否则返回false
		**********************/
		public:
			static bool ArrayInsert(LetObject*, numberT,LetObject*);
		private:
			static bool ArrayInsertNum(LetObject*, numberT, LetObject*);

		/**********************
		* 删除指定下标元素
		* 成功返回true，否则返回false
		**********************/
		public:
			static bool ArrayPop(LetObject* var, std::variant<numberT, std::string> index);

		/**********************
		* 删除指定的第一个值
		* 成功返回true，否则返回false
		**********************/
		public:
			static bool ArrayRemove(LetObject* var, LetObject* tar);

		/**********************
		* 倒序数组元素
		* 成功返回true，否则返回false
		**********************/
		public:
			static bool ArrayReverse(LetObject* var);

		/**********************
		* 倒序数组元素
		* 成功返回true，否则返回false
		**********************/
		public:
			static LetObject ArrayRange(numberT , numberT);

		/**********************
		* 将数组所有下标返回
		* 成功返回数组，否则返回null
		**********************/
		public:
			static LetObject ArrayChilds(LetObject*);

		};


		#include "LetArrayMotion.tpp"
	}
}

#endif

