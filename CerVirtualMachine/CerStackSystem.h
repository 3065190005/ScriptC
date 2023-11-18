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
			SFPtr GetLastSF();						// ��ȡ�����ջ֡������
			SFPtr GetLastFuncSf();					// ��ȡ����ĺ���ջ֡������
			SFPtr PushNewSF(std::string name);		// ���һ��ջ֡ �����¼ӵĵ�����
			SFPtr PopNewSF();						// ɾ��һ��ջ֡ ���ظ�������
			SFPtr GetBaseSF();						// ��õ�һ��ջ������
			bool SaveLastSF(numberT id,numberT eip);// ���������ջ
			numberT LoadSfOnce(numberT id);			// �ָ�һ��id��һ��ջ

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

