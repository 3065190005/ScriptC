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
			SFPtr GetLastSF();					// ��ȡ�����ջ֡������
			SFPtr PushNewSF(std::string name);	// ���һ��ջ֡ �����¼ӵĵ�����
			SFPtr PopNewSF();					// ɾ��һ��ջ֡ ���ظ�������
			SFPtr GetBaseSF();					// ��õ�һ��ջ������

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

