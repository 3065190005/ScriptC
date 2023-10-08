#pragma once
#ifndef _SCRIPTC_CERSTACKSYSTEM_
#define _SCRIPTC_CERSTACKSYSTEM_

#include <vector>
#include "CerRunTime.h"

namespace ScriptC {
	namespace Obj {

		using namespace AutoMem::Obj;

		class CerRunTime;
		class CerStackFrame;

		using SFPtr = std::list<CerStackFrame>::iterator;

		class CerStackSystem
		{
		public:
			SFPtr GetLastSF();					// 获取最近的栈帧迭代器
			SFPtr PushNewSF(std::string name);	// 添加一个栈帧 返回新加的迭代器
			SFPtr PopNewSF();					// 删除一个栈帧 返回父迭代器
			SFPtr GetBaseSF();					// 获得第一个栈迭代器

			void setVarMapValue(std::string name, auto_c element,bool recursion);
			auto_c getVarMapValue(std::string name , bool recursion);

			autoPtr getVarMapValuePtr(std::string name, bool recursion);

			struct CerRunTime::InterProperties getInterMapValue(std::string name, bool recursion);

			struct CerRunTime::FuncDescription getFuncMapAddress(std::string name);

		private:
			std::list<CerStackFrame> m_stacks;
		};
	}
}

#endif

