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
		* �������࣬�����ز����뼰����
		***************/
		class CommandCode
		{
		public:
			// ����������
			enum class CommandCodeType {
				Pass = 0,	// ����
				Push,		// ��ջ
				Pop,		// ��ջ
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

				UnaryAdd,	// ��
				UnarySub,	// ��

				AryIndex,	// �������±�

				Inter,		// �ӿ�����
				Func,		// ����������ͷ
				Call,		// ִ�к���
				Leave,		// �˳�����

				Yield,		// ����ջ
				Resume,		// �ָ�ջ

				Jmp,		// ��ת
				PushS,		// ������ջ
				PopS,		// �������ջ
				Lens,		// ��ȡ�����ջ�������鳤�Ȳ������µ�����
				Inc,		// �����ļ�
				Swap,		// ��������ջԪ��λ��
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
			CommandCodeType m_type;						// ����������
			std::map<std::string, auto_c> m_params;		// ���������
			CerTokClass::DebugInfo m_info;				// ������Ϣ

		};

		using CodeType = CommandCode::CommandCodeType;
		using CodeParams = std::map<std::string, auto_c>;
	}
}

#endif

