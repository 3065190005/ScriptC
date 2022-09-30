#pragma once
#ifndef _CERVER_CERCALCSTACK_
#define _CERVER_CERCALCSTACK_
#include <vector>

namespace Cervice {
	namespace Obj {

		class CerTokClass;
		class CommandCode;
		
		/***********************
		* class CerCalcStack
		* ����ջ���������������
		***********************/

		class CerCalcStack
		{
		public:
			CerCalcStack();
			~CerCalcStack();

		public:
			void push_opera(auto_c);			// Push������
			auto_c pop_opera();				// Pop������
			void calc_opera(CodeType);			// ���������


		private:
			void BinOpCalc(CodeType	type);		// ��Ԫ����
			void UnaryCalc(CodeType type);		// һԪ����

		public:
			static CerCalcStack* create();		// ������̬����

		private:
			std::vector<auto_c>m_stack;		// ������ջ
		};
	}
}


#endif

