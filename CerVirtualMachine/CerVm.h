#pragma once
#ifndef _SCRIPTC_CERVM_
#define _SCRIPTC_CERVM_

namespace ScriptC {
	namespace Obj {

		using namespace AutoMem::Obj;

		class CommandCode;
		class CerStackSystem;

		/************************************
		* class CerVm
		* ��������ʵ�֣�������command code
		*************************************/
		
		class CerVm
		{
		public:
			using VectorStr = std::vector<std::string>;
			using ByteCodes = std::map<std::string, std::vector<CommandCode>>;
		public:
			CerVm();
			~CerVm();

		public:
			bool initVm();
			void Command();
			void runTime();
		
		private:
			void VmInter();
			void VmFunc();
			void VmCall();
			void VmLeave();
			void VmPush();
			void VmJmp();
			void VmPushS();
			void VmPopS();
			void VmLens();
			void VmInc();
			void VmYield();
			void VmResume();
			auto_c VmPop();
			auto_c VmAdd();
			auto_c VmSub();
			auto_c VmMul();
			auto_c VmDiv();
			auto_c VmMod();
			auto_c VmNot();
			auto_c VmMat();
			auto_c VmLeftO();
			auto_c VmRightO();
			auto_c VmGtr();
			auto_c VmGeq();
			auto_c VmLss();
			auto_c VmLeq();
			auto_c VmEqu();
			auto_c VmNeq();
			auto_c VmAnd();
			auto_c VmXor();
			auto_c VmOr();
			auto_c VmDAnd();
			auto_c VmDOr();
			auto_c VmUnaryAdd();
			auto_c VmUnarySub();
			auto_c VmPass();

		private:
			void VmSwap();

		private:
			bool BuiltInCond(numberT,CommandCode);

		public:
			void CodeCallFunc(std::string _funcName, std::vector<auto_c> _params, std::string _thisName);		// ͨ�� ����������this
			void CodeCallFunc(std::string _funcName, std::vector<auto_c> _params, auto_c _thisName);			// ͨ����������this
			void CodePopNewSf();

		public:
			void printRunCode();		// ��ӡ��ǰ���� debug
			bool isRunOver();			// �����Ƿ�ִ�����
			bool isLeaveStackPop();		// ���Leave�Ƿ���������ջ
			bool CmpCodeType(CommandCode::CommandCodeType type1 , CommandCode::CommandCodeType type2);	// �����ֽ��������Ƿ�ƥ��
			bool CmpCurrentType(CommandCode::CommandCodeType type);				// ƥ�䵱ǰ��ִ�е��ֽ����Ƿ�Ϊ type

		private:
			VectorStr isCallGc();		// ���ص�ǰջ������ �ӿڱ����� (������this)
			VectorStr isCallGc(SFPtr);	// ���ص�ǰջ������ �ӿڱ����� (������this)
			VectorStr isCallGcR();		// ���ص�ǰջ�����ǵ����� �ӿڱ����� (������this)

			bool getCallGcVar(SFPtr);	// ���ص�ǰջ��������Ҫ����ı���
			bool getCallGcVarR();		// ���ص�ǰջ������Ҫ����ı����ݹ��ȡ

			bool GcCallBack();			// �Ƿ���ýӿڵ���������
			bool GcCallBackDataSf();	// ��ջ�����ͷ�
			std::string findClassFunc(std::string className, std::string funcName);// �ݹ��ȡ��ĺ�����

		public:
			bool takeEat(CommandCode::CommandCodeType type);
			void advance();

		public:
			static CerVm* create(std::vector<CommandCode> codes);
			static CerVm* create(std::map<std::string, std::vector<CommandCode>> codes);

			void setCodeFile(std::string str);
			std::string getCodeFile();
			CommandCode commandAt(size_t pos);
			size_t commandSize();
			void commandNullptrThrow();

		private: 
			bool m_isinit;								// �Ƿ��ʼ��
			ByteCodes m_codes;							// ����codes
			std::vector<CommandCode> *m_command_codes;	// ��ǰ�������е�code�б�
			CommandCode m_current_cmd_code;				// ��ǰcode
			size_t m_command_codes_index;				// ��һ��code�±�
			CerStackSystem m_stacks;					// ջϵͳ
			std::string m_code_file;					// ��ǰִ�е�code�����ļ�
			std::list<auto_c> m_destory_var;			// ���ͷŵı���ref�ϼ�
			std::list<auto_c> m_destory_data;			// ���ͷŵı�������ϼ�
		};
	}
}

#endif