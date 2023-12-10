#pragma once
#ifndef _SCRIPTC_COMMANDCODE_
#define _SCRIPTC_COMMANDCODE_

#include <iostream> 
#include <map>
#include "..\AutoElement\LetObject.h"
#include "..\CerLexical\CerLexical.h"

namespace ScriptC {
	namespace Obj {

		using namespace AutoMem::Obj;

		/**************
		* class CommandCode
		* 操作码类，存放相关操作码及参数
		***************/
		class CommandCode
		{
		public:
			// 操作码类型
			enum class CommandCodeType {
				Pass = 0,	// 跳过
				Push,		// 入栈
				Pop,		// 出栈
				Add,		// +
				Sub,		// -
				Mul,		// *
				Div,		// /
				Mod,		// %

				Not,		// !
				Mat,		// ~

				LeftO,		// <<
				RightO,		// >>

				Gtr,		// >
				Geq,		// >=
				Lss,		// <
				Leq,		// <=

				Equ,		// ==
				Neq,		// !=

				And,		// &
				Xor,		// ^
				Or,			// |
				DAnd,		// &&

				DOr,		// ||

				UnaryAdd,	// 正
				UnarySub,	// 负

				AryIndex,	// 中括号下标

				Inter,		// 接口声明
				Func,		// 函数声明开头
				Call,		// 执行函数
				Leave,		// 退出函数

				Yield,		// 挂起栈
				Resume,		// 恢复栈

				Jmp,		// 跳转
				PushS,		// 推送新栈
				PopS,		// 销毁最近栈
				Lens,		// 获取最近的栈计算数组长度并返回新的数组
				Inc,		// 导入文件
				Swap,		// 交换两个栈元素位置
			};

		public:
			CommandCode(CommandCodeType, std::map<std::string, auto_c> = {});
			~CommandCode();

			CommandCodeType getCodeType();

			void insertCodeParams(std::string, auto_c);
			std::map<std::string, auto_c>* getCodeParams();
			void setDebugInfo(CerTokClass::DebugInfo info);
			CerTokClass::DebugInfo getDebugInfo();

		public:
			std::string getCodeTypeStr();

		private:
			CommandCodeType m_type;						// 操作码类型
			std::map<std::string, auto_c> m_params;		// 操作码参数
			CerTokClass::DebugInfo m_info;				// 报错信息

		};

		using CodeType = CommandCode::CommandCodeType;
		using CodeParams = std::map<std::string, auto_c>;
	}
}

#endif

