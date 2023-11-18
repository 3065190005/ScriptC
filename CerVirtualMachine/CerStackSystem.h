#pragma once
#ifndef _SCRIPTC_CERSTACKSYSTEM_
#define _SCRIPTC_CERSTACKSYSTEM_

#include <vector>
#include <random>
#include "CerRunTime.h"

namespace ScriptC {
	namespace Obj {

		using namespace AutoMem::Obj;

		class CerRunTime;
		class CerStackFrame;

		using SFPtr = std::list<CerStackFrame>::iterator;
		using StackFrameList = std::list<CerStackFrame>;

		class CerStackSystem
		{
		public:
			SFPtr GetLastSF();						// 获取最近的栈帧迭代器
			SFPtr GetLastFuncSf();					// 获取最近的函数栈帧迭代器
			SFPtr PushNewSF(std::string name);		// 添加一个栈帧 返回新加的迭代器
			SFPtr PopNewSF();						// 删除一个栈帧 返回父迭代器
			SFPtr GetBaseSF();						// 获得第一个栈迭代器
			bool SaveLastSF(numberT id,numberT eip);// 保存最近的栈
			numberT LoadSfOnce(numberT id);			// 恢复一个id的一个栈

			void SetBkStackThisRef(numberT id, auto_c ref);	
			void ClearBkStackThisRef(numberT id);

			numberT GetOnlyId();
			void ClearOnlyId(numberT);
			bool HasOnlyId(numberT);

			void setVarMapValue(std::string name, auto_c element,bool recursion);
			auto_c getVarMapValue(std::string name , bool recursion);
			autoPtr getVarMapValuePtr(std::string name, bool recursion);
			struct CerRunTime::InterProperties getInterMapValue(std::string name, bool recursion);
			struct CerRunTime::FuncDescription getFuncMapAddress(std::string name);

		private:
			std::list<CerStackFrame> m_stacks;
			std::map<numberT, StackFrameList> m_bakcup_stacks;
			std::map<numberT, auto_c> m_bk_stack_this_ref;
			std::map<numberT, std::list<numberT>> m_bk_stack_eip;
			std::vector<numberT> m_stack_id;
		};
	}
}

#endif

