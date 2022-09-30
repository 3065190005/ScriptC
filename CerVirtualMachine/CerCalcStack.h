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
		* 计算栈，负责操作数计算
		***********************/

		class CerCalcStack
		{
		public:
			CerCalcStack();
			~CerCalcStack();

		public:
			void push_opera(auto_c);			// Push操作数
			auto_c pop_opera();				// Pop操作数
			void calc_opera(CodeType);			// 计算操作数


		private:
			void BinOpCalc(CodeType	type);		// 二元计算
			void UnaryCalc(CodeType type);		// 一元计算

		public:
			static CerCalcStack* create();		// 创建静态函数

		private:
			std::vector<auto_c>m_stack;		// 操作数栈
		};
	}
}


#endif

