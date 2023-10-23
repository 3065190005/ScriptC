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
		* 虚拟机相关实现，负责处理command code
		*************************************/
		
		class CerVm
		{
		public:
			using VectorStr = std::vector<std::string>;

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

		public:
			void CodeCallFunc(std::string _funcName, std::vector<auto_c> _params, std::string _thisName);
			void CodePopNewSf();

		public:
			void printRunCode();		// 打印当前代码 debug
			bool isRunOver();			// 代码是否执行完毕
			bool CmpCodeType(CommandCode::CommandCodeType type1 , CommandCode::CommandCodeType type2);	// 两个字节码类型是否匹配
			bool CmpCurrentType(CommandCode::CommandCodeType type);				// 匹配当前该执行的字节码是否为 type
			VectorStr isCallGc();		// 返回当前栈的所有 接口变量名 (不包含this)
			VectorStr isCallGcR();		// 返回当前栈所涵盖的所有 接口变量名 (不包含this)
			bool GcCallBack();			// 是否调用接口的析构函数
			std::string findClassFunc(std::string className, std::string funcName);// 递归获取类的函数名

		public:
			bool takeEat(CommandCode::CommandCodeType type);
			void advance();

		public:
			static CerVm* create(std::vector<CommandCode> codes);
			static std::map<std::string, size_t> m_CodeBaseAddress;

			void setBaseAddress(std::string str);
			size_t getBaseAddress();

		private: 
			bool m_isinit;								// 是否初始化
			std::vector<CommandCode> m_command_codes;	// 所有code
			CommandCode m_current_cmd_code;				// 当前code
			size_t m_command_codes_index;				// 下一个code下标
			CerStackSystem m_stacks;					// 栈系统
			std::string m_BaseAddress;
		};
	}
}

#endif