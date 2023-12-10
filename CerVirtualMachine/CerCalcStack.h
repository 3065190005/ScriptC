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
		* 计算栈，负责操作数计算
		***********************/

		class CerCalcStack
		{
		public:
			CerCalcStack();
			~CerCalcStack();

		public:
			void push_opera(auto_c);			// Push操作数
			auto_c pop_opera();					// 以值的方式返回最近的一个值并从操作栈中丢弃
			auto_c& opera_back();				// 引用获取最近的一个值
			void opera_drop();					// 丢弃最近的一个值
			void calc_opera(CodeType);			// 计算操作数


		private:
			void BinOpCalc(CodeType	type);		// 二元计算
			void UnaryCalc(CodeType type);		// 一元计算
			void IndexOpCalc(CodeType type);	// 引用计算

			std::string getBinOpStr(CodeType);
			std::string getUnaryStr(CodeType);

			std::string getAutoCTypeStr(LetObject::ObjT);

		public:
			static CerCalcStack* create();		// 创建静态函数

		private:
			std::vector<auto_c>m_stack;		// 操作数栈
		};
	}
}


#endif

