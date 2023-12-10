#pragma once
#ifndef _SCRIPTC_CERCALCSTACK_
#define _SCRIPTC_CERCALCSTACK_
#include <vector>

namespace ScriptC {
	namespace Obj {

		using namespace AutoMem::Obj;

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
			auto_c pop_opera();					// ��ֵ�ķ�ʽ���������һ��ֵ���Ӳ���ջ�ж���
			auto_c& opera_back();				// ���û�ȡ�����һ��ֵ
			void opera_drop();					// ���������һ��ֵ
			void calc_opera(CodeType);			// ���������


		private:
			void BinOpCalc(CodeType	type);		// ��Ԫ����
			void UnaryCalc(CodeType type);		// һԪ����
			void IndexOpCalc(CodeType type);	// ���ü���

			std::string getBinOpStr(CodeType);
			std::string getUnaryStr(CodeType);

			std::string getAutoCTypeStr(LetObject::ObjT);

		public:
			static CerCalcStack* create();		// ������̬����

		private:
			std::vector<auto_c>m_stack;		// ������ջ
		};
	}
}


#endif

