#include "QuickLib/quicklib.h"
#include "..\AutoElement\LetNatureDefine.h"
#include "..\AutoElement\LetManager.h"
#include "..\CerLexical\CerLexical.h"
#include "..\CerInterpreter\CommandCode.h"
#include "..\DllReader\DllFuncReader.h"
#include "..\ErrorHandling\ErrorHandling.h"
#include "CerCalcStack.h"
#include "CerRunTime.h"
#include "CerStackFrame.h"
#include "CerStackSystem.h"
#include "CerVmMacro.h"
#include "CerVm.h"

#include <sstream>
#include <algorithm>
using namespace ScriptC::Obj;


ScriptC::Obj::CerVm::CerVm() :
	m_isinit(false),
	m_command_codes_index(0),
	m_current_cmd_code(CodeType::Pass, {})
{
}

ScriptC::Obj::CerVm::~CerVm()
{
}

bool ScriptC::Obj::CerVm::initVm()
{
	if (m_isinit)
		return true;

	m_stacks.PushNewSF("!console");
	m_isinit = true;
	return true;
}

void ScriptC::Obj::CerVm::Command()
{
	// gc callback
	if (GcCallBack())
		return;

	// gc dataSf callback
	if (GcCallBackDataSf())
		return;

	// debug print
	printRunCode();

	switch (m_current_cmd_code.getCodeType())
	{
	case CommandCode::CommandCodeType::Pass:
		VmPass();
		break;
	case CommandCode::CommandCodeType::Pop:
		VmPop();
		break;
	case CommandCode::CommandCodeType::Push:
		VmPush();
		break;
	case CommandCode::CommandCodeType::Add:
		VmAdd();
		break;
	case CommandCode::CommandCodeType::Sub:
		VmSub();
		break;
	case CommandCode::CommandCodeType::Mul:
		VmMul();
		break;
	case CommandCode::CommandCodeType::Div:
		VmDiv();
		break;
	case CommandCode::CommandCodeType::Mod:
		VmMod();
		break;
	case CommandCode::CommandCodeType::Not:
		VmNot();
		break;
	case CommandCode::CommandCodeType::Mat:
		VmMat();
		break;
	case CommandCode::CommandCodeType::LeftO:
		VmLeftO();
		break;
	case CommandCode::CommandCodeType::RightO:
		VmRightO();
		break;
	case CommandCode::CommandCodeType::Gtr:
		VmGtr();
		break;
	case CommandCode::CommandCodeType::Geq:
		VmGeq();
		break;
	case CommandCode::CommandCodeType::Lss:
		VmLss();
		break;
	case CommandCode::CommandCodeType::Leq:
		VmLeq();
		break;
	case CommandCode::CommandCodeType::Equ:
		VmEqu();
		break;
	case CommandCode::CommandCodeType::Neq:
		VmNeq();
		break;
	case CommandCode::CommandCodeType::And:
		VmAnd();
		break;
	case CommandCode::CommandCodeType::Xor:
		VmXor();
		break;
	case CommandCode::CommandCodeType::Or:
		VmOr();
		break;
	case CommandCode::CommandCodeType::DAnd:
		VmDAnd();
		break;
	case CommandCode::CommandCodeType::DOr:
		VmDOr();
		break;
	case CommandCode::CommandCodeType::UnaryAdd:
		VmUnaryAdd();
		break;
	case CommandCode::CommandCodeType::UnarySub:
		VmUnarySub();
		break;
	case CommandCode::CommandCodeType::AryIndex:
		VmAryIndex();
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Inter:
		VmInter();
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Func:
		VmFunc();
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Call:
		VmCall();
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Leave:
		VmLeave();
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Jmp:
		VmJmp();
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::PushS:
		VmPushS();
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::PopS:
		VmPopS();
		break;
	case ScriptC::Obj::CommandCode::CommandCodeType::Lens:
		VmLens();
		break;
	case CommandCode::CommandCodeType::Inc:
		VmInc();
		break;
	case CommandCode::CommandCodeType::Swap:
		VmSwap();
		break;
	case CommandCode::CommandCodeType::Yield:
		VmYield();
		break;
	case CommandCode::CommandCodeType::Resume:
		VmResume();
		break;
	default:
	{
		auto errHis = ErrorHandling::getInstance();
		errHis->throwErr(EType::Vm, "Unknown bytecode \n Please Check C++ Code");
	}
	}
}

void ScriptC::Obj::CerVm::runTime()
{
	vmlog("\n\nVmRunTime: >>>\n");

	extern std::string G_mainFile;
	auto_c fileName;
	fileName << G_mainFile;
	m_stacks.setVarMapValue(SYSTEM_MAIN_NAME, std::move(fileName), false);
	m_stacks.setVarMapValue(SYSTEM_FILE_NAME, std::move(fileName), false);

	m_stacks.PushNewSF("!program");

	if (commandSize() == 0 || !m_isinit) {
		return;
	}

	m_command_codes_index = 0;
	m_current_cmd_code = commandAt(m_command_codes_index);
	m_command_codes_index = 1;

	do
	{
		Command();
	} while (!isRunOver());

	m_stacks.PopNewSF();
}

void ScriptC::Obj::CerVm::VmInter()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();

	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Inter);

	auto_c InterName = (*cmd.getCodeParams())["param1"];
	auto_c InterParent = (*cmd.getCodeParams())["param2"];

	std::string name, parent;
	InterName >> name;
	InterParent >> parent;

	runT->setInterMapValue(name, { name,parent });
	return;
}

void ScriptC::Obj::CerVm::VmFunc()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();

	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Func);

	std::stringstream ss;
	auto_c funLen = (*cmd.getCodeParams())["BlockLen"];
	auto_c func = (*cmd.getCodeParams())["param1"];
	auto_c paramSize = (*cmd.getCodeParams())["param2"];
	
	std::string funcName;
	func >> funcName;

	numberT paramSizeT;
	paramSize >> paramSizeT;

	numberT jumpLen;
	funLen >> jumpLen;

	CerRunTime::FuncDescription des;
	
	des.addRess = m_command_codes_index-1;
	des.paramLen = (numberT)paramSizeT;
	des.fileName = getCodeFile();

	for (size_t i = 0; i < paramSizeT; i++) {
		std::string param;
		ss << "param";
		ss << i + 4;
		ss >> param;
		ss.str(""); ss.clear();

		auto_c varName = (*cmd.getCodeParams())[param];
		varName >> param;

		des.params.emplace_back(param);
		param.clear();
	}

	if (funcName.find(":") != funcName.npos){
		std::string interName, funcVName;
		interName = funcName.substr(0, funcName.find(":"));
		funcVName = funcName.substr(funcName.find(":")+1);
		runT->insetInterFunc(interName, funcVName);
	}

	if ((*cmd.getCodeParams()).find("param3") != (*cmd.getCodeParams()).end()) {
		des.hasExport = true;
	}

	runT->setFuncMapValue(funcName, des);

	if (des.hasExport == true) {
		return;
	}

	m_command_codes_index += jumpLen - 2;
	advance();
	takeEat(CommandCode::CommandCodeType::Leave);
}

void ScriptC::Obj::CerVm::VmCall()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();

	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Call);

	std::stringstream ss;
	auto_c this_var;			// this ��������ָ��
	std::string this_name;		// �ֶ����ε�this��
	auto_c func = (*cmd.getCodeParams())["param1"];		// ��Ҫ���õĺ���ȫ��
	auto_c paramSize = (*cmd.getCodeParams())["param2"];	// ��Ҫ���õĺ�������ֵ
	std::string funcName,src_func_name;			// �������ͺ���ԭʼ��
	func >> funcName;

	// �Ƿ��ֶ�����this �󶨼��
	bool is_this_bind = (*cmd.getCodeParams()).find("param3") != (*cmd.getCodeParams()).end();

	// �Ƿ���з���
	bool is_reverse = false;
	is_reverse = cmd.getCodeParams()->find("param6") != cmd.getCodeParams()->end();

	// �Ƿ�Ϊ��ֵ���һλ
	bool is_left_last = false;
	is_left_last = cmd.getCodeParams()->find("param5") != cmd.getCodeParams()->end();

	numberT paramSizeT;
	paramSize >> paramSizeT;

	std::vector<auto_c> valueVec;

	for (size_t i = 0; i < paramSizeT; i++) {
		valueVec.emplace_back(dataSf->back());
		dataSf->pop_back();
	}
	std::string className;
	if (funcName.find("(u)") != funcName.npos) {
		if ((dataSf->back().getAttribute() & (int)NatureType::cls) == false) {
			auto errHis = ErrorHandling::getInstance();
			errHis->throwErr(EType::Vm,"variable is not a interface");
		}

		dataSf->back()[CLASS_NAME] >> className;
		funcName = funcName.substr(3, funcName.size());
		src_func_name = funcName;

		/*
		* 2023.10.23
		* ���⺯����ֹ�ֶ�����
		*/
		if (funcName == SPECIAL_FUNC_GC || funcName == SPECIAL_FUNC_INIT)
		{
			auto errHis = ErrorHandling::getInstance();
			errHis->throwErr(EType::Vm, funcName + "Method prohibits manual calls");
			return;
		}

		/*2023.11.11
		* ����hook ����attr
		*/

		bool hook_attr = false;
		funcName = findClassFunc(className, src_func_name);
		if (funcName.empty())
		{
			funcName = findClassFunc(className, SPECIAL_FUNC_ATTR);
			hook_attr = true;
		}
		else if (is_left_last)
		{
			m_command_codes_index += 2;
		}

		if (funcName.empty())
		{
			auto errHis = ErrorHandling::getInstance();
			errHis->throwErr(EType::Vm, "invalid interface method call");
		}

		/*
		* 2023.11.12
		* �޸����ڱ�����attr�������ò������������
		*/
		if (is_reverse)
		{
			std::reverse(valueVec.begin(), valueVec.end());
		}

		/*
		* 2023.11.11
		* ��ʽ�� attr ����
		*/

		if (hook_attr)
		{
			vmlog(m_command_codes_index - 1 <<
				" - VmRunCode( [*] Replace Function Call \"" +
				src_func_name +
				"\" With \"" +
				SPECIAL_FUNC_ATTR +
				"\") \n\n");

			auto_c param1, param2;
			paramSizeT = 2;
			param1["name"] << src_func_name;
			param1["type"] << "function";
			numberT index = valueVec.size();
			for (auto& i : valueVec)
				param2[--index] = i;

			if (is_left_last)
			{
				auto param1_iter = dataSf->rbegin();
				auto param2_iter = dataSf->rbegin();
				param1_iter++;

				AutoMem::Obj::LetTools tools;
				tools.Swap(*param1_iter, *param2_iter);

				param2[(numberT) - 1] = dataSf->back();
				dataSf->pop_back();
			}

			valueVec.clear();

			valueVec.emplace_back(std::move(param2));
			valueVec.emplace_back(std::move(param1));
		}


		/* 
		* 2023.10.17
		* ͨ��(u)�ĺ���������ֱ�ӻ��գ�֮�����Leave����ֵ֮ǰ����ɾ�� _init ����
		* 2023.11.18
		* fixed �޸���ͨ�������ջ���һ��ջ���ݵ�bug
		* ��ֻ���ڽ��нӿڷ�������ʱ �� û���ֶ���this ������Ĭ�ϰ�
		*/

		// dataSf->pop_back();
		if (!is_this_bind)
		{
			auto del_nature = dataSf->back().getSelfAttribute();
			dataSf->back().setAttribute(del_nature | 16);

			LetObject::reference(&this_var, &dataSf->back());
		}
	}

	/*
	* 2023.10.17
	* Ĭ�ϲ�����Ҫ�ֶ�����this����
	* �ֶ�����thisָ��
	*/
	if (is_this_bind) {
		this_var = (*cmd.getCodeParams())["param3"];
		this_var >> this_name;
		LetObject::reference(&this_var, m_stacks.getVarMapValuePtr(this_name, true));

	}

	/*
	* �жϺ����Ƿ���ڣ������ڱ�����̬��̬˫�ж�
	*/
	auto funcMap = m_stacks.getFuncMapAddress(funcName);
	if (funcMap.addRess == 0) {
		auto errHis = ErrorHandling::getInstance();
		errHis->throwErr(EType::Vm, "invalid interface method call");
	}

	/*
	* �жϺ��������Ƿ��������������̬��̬˫�ж�
	*/
	if (funcMap.params.size() != paramSizeT) {
		auto errHis = ErrorHandling::getInstance();
		errHis->throwErr(EType::Vm, "params error with function");
	}

	/*
	* �жϺ��������Ƿ�Ϊ�ⲿ C++ ��������̬�ж�
	*/
	if (funcMap.hasExport) {
		auto_c ret;
		auto manager = DllFuncReader::getInstance();
		funcPtr func = manager->getFuncFromDll(funcName.c_str());
		std::reverse(std::begin(valueVec), std::end(valueVec));
		manager->callFunc(func, &valueVec, &ret);
		dataSf->emplace_back(std::move(ret));
		return;
	}

	numberT leavePtr = m_command_codes_index;
	leavePtr--;

	auto_c leaveSip;
	leaveSip[0] << leavePtr;
	leaveSip[1] << getCodeFile();

	setCodeFile(funcMap.fileName);


	m_stacks.PushNewSF(funcName);
	this_var.getAttribute();

	sf = m_stacks.GetLastSF();
	runT = sf->getRunTime();
	dataSf = sf->getDataStack();

	runT->setVarMapValue("this", std::move(this_var));

	for (size_t index = 0; index < funcMap.paramLen; index++) {
		runT->setVarMapValue(funcMap.params.at(index), valueVec.at(index));
	}

	runT->getVarMapValue("this").getAttribute();

	dataSf->emplace_back(leaveSip);
	m_command_codes_index = funcMap.addRess;
	advance();
	return;
}

void ScriptC::Obj::CerVm::VmLeave()
{
	/*
	* leave : param1:value		// �ó�ջͷ������Ϊ��ַ��ת��������value�����ջ
	* leave :					// �ó�ջͷ������Ϊ��ַ��ת�����Ϳ�ֵ�����ջ
	*/
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	std::string frameName = sf->getStackFrameName();

	auto_c retVal;
	bool haveRetVal = false;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Leave);
	if (cmd.getCodeParams()->find("param1") != cmd.getCodeParams()->end()) {
		haveRetVal = true;

		/*
		* 2023.10.17
		* ָ�����ݽ����ٽ��п���ת��
		*/
		if (dataSf->back().getSelfAttribute() & (nature)AutoMem::Obj::NatureType::ptr)
			auto_c::reference(&retVal, &dataSf->back());
		else
			retVal = dataSf->back();
		dataSf->pop_back();
	}

	// ɾ������ if while for ��ջ
	while (m_stacks.GetLastSF()->getStackFrameName()[0] == '~')
	{
		CodePopNewSf();
	}

	sf = m_stacks.GetLastSF();
	runT = sf->getRunTime();
	dataSf = sf->getDataStack();

	bool popStack = true;
	numberT retPtr = 0;
	auto_c retSip;

	retSip = *dataSf->begin();

	if (retSip.getType() != LetObject::ObjT::string) {
		if (retSip.getType() == LetObject::ObjT::array)
		{
			std::string file_name;
			retSip[0] >> retPtr;
			retSip[1] >> file_name;
			setCodeFile(file_name);
		}
		else
			retSip >> retPtr;
		
		m_command_codes_index = retPtr;
	}
	else {
		std::string sipV;
		retSip >> sipV;
		auto retV = kstring::stringSplit(sipV, ';');
		retPtr = kstring::stringTo<numberT>(retV[1]);
		std::string filename = retV[0];
		setCodeFile(filename);
		popStack = false;

		dataSf->pop_front();
		m_command_codes_index = retPtr;
	}

	auto_c this_ptr = runT->getVarMapValue("this");

	if (popStack) {
		CodePopNewSf();
	}

	advance();

	sf = m_stacks.GetLastSF();
	runT = sf->getRunTime();
	dataSf = sf->getDataStack();

	/*
	 * 2023.10.17
	 * ͨ�� (u) ���õĺ��� ���ڴ˽����ͷ�
	*/
	if (!dataSf->empty())
	{
		if ((dataSf->back().getSelfAttribute() & 16) != 0)
		{
			auto_c back;
			back = dataSf->back();
			m_destory_data.emplace_back(std::move(back));
			dataSf->pop_back();
		}
	}

	/*
	* 2023.10.23
	* ���������͹��캯����ֵֹ�ķ���
	*/
	if (frameName.find(":") != frameName.npos)
	{
		frameName = frameName.substr(frameName.find(":") + 1);

		if (frameName == SPECIAL_FUNC_GC || frameName == SPECIAL_FUNC_INIT)
		{
			if (haveRetVal)
			{
				auto errHis = ErrorHandling::getInstance();
				errHis->throwErr(EType::Vm, frameName + "prohibit returning values");
			}
			else
				return;
		}
	}

	if (haveRetVal) {
		dataSf->emplace_back(retVal);
		/*
		* 2023.12.10
		* ���ؽӿ�����Ĭ�ϲ��������Զ����շ���
		*/
		//if (dataSf->back().getSelfAttribute() & (int)NatureType::cls)
		//{
		//	dataSf->back()[CLASS_AUTO_GC_CALL] << true;
		//}
	}
	else {
		auto_c space;
		dataSf->emplace_back(space);
	}
}

void ScriptC::Obj::CerVm::VmPush()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Push);
	auto_c obj;
	obj = (*cmd.getCodeParams())["param1"];
	for (auto& i : (*cmd.getCodeParams())) {
		if (i.first == "param2") {
			std::string Pushtype;
			i.second >> Pushtype;
			if (Pushtype == "value") {
				std::string var_name;
				obj >> var_name;
				/*
				* 2023.10.17
				* this �ڵ�������½����κ�ʱ����Ϊָ�뷵��
				*/
				if ((*cmd.getCodeParams()).find("param3") != (*cmd.getCodeParams()).end() || var_name == "this") {
					LetObject::reference(&obj,m_stacks.getVarMapValuePtr(var_name, true));
				}
				else {
					obj = m_stacks.getVarMapValue(var_name, true);
				}
			}
			else if (Pushtype == "array") {
				std::string param3Str;
				cmd.getCodeParams()->find("param3")->second >> param3Str;
				std::stringstream ss(param3Str);
				std::string path;
				std::vector<std::string> allPath;
				while (std::getline(ss, path, ',')) {
					allPath.emplace_back(path);
				}

				for (auto i = allPath.rbegin(); i != allPath.rend(); i++) {
					numberT varindex;
					std::stringstream ss2;
					std::string index, indextype;
					index = i->substr(0, i->rfind(":"));
					ss2 << index;
					ss2 >> varindex;
					indextype = i->substr(i->rfind(":") + 1);

					if (indextype == "(v)") {
						std::string varName;
						auto_c indexC;
						indexC << varindex;
						obj[indexC] >> varName;
						obj[indexC] = m_stacks.getVarMapValue(varName, true);
					}

					else if (indextype == "(u)") {
						auto_c indexC;
						indexC << varindex;
						obj[indexC] = dataSf->back();
						dataSf->pop_back();
					}
				}

				/*
				* 2023.12.10
				* �±�ģʽͳһ�ɲ����ֽ�����д���
				*/

				//std::vector<auto_c> vecR;
				//std::vector<auto_c> vecL;
				//auto_c targetArray;
				//for (auto i = allPath.rbegin(); i != allPath.rend();) {
				//	std::string index, indextype;
				//	index = i->substr(0, i->rfind(":"));
				//	indextype = i->substr(i->rfind(":") + 1);

				//	if (indextype == "(i)") {
				//		vecR.emplace_back(dataSf->back());
				//		dataSf->pop_back();
				//	}

				//	else if (indextype == "(s)") {
				//		vecL.emplace_back(dataSf->back());
				//		dataSf->pop_back();
				//	}

				//	i++;
				//}
				//if (vecR.size() != 0) {
				//	targetArray = dataSf->back();
				//	dataSf->pop_back();
				//}

				//numberT indexI = 0;
				//for (auto i = vecR.rbegin(); i != vecR.rend(); i ++) {
				//	auto_c indexC;
				//	indexC << indexI;
				//	indexI++;
				//	if (vecR.size() != 1) {
				//		obj[indexC] = targetArray[*i];
				//	}
				//	else {
				//		obj = targetArray[*i];
				//	}
				//}

				//if (vecL.size() != 0) {
				//	LetObject::reference(&targetArray,&dataSf->back());
				//	dataSf->pop_back();
				//}

				//for (auto i = vecL.rbegin(); i != vecL.rend(); i++) {
				//	LetObject::reference(&obj, &targetArray[*i]);
				//}

				//break;
			}
			else if (Pushtype == "interface") 
			{
				std::string var_name;
				obj >> var_name;
				 
				if (var_name == "(u)") {
					auto_c param3 = (*cmd.getCodeParams())["param3"];
					std::string param3C;
					param3 >> param3C;
					if (param3C == "(u)") {
						if ((dataSf->back().getSelfAttribute() & (int)NatureType::ptr) == true) {
							auto_c ret = dataSf->back();
							dataSf->pop_back();
							LetObject::reference(&obj, &ret);
						}
						else {
							auto_c ret = dataSf->back();
							dataSf->pop_back();
							obj = ret;
						}
					}
					else {
						auto_c cls;
						if ((dataSf->back().getSelfAttribute() & (int)NatureType::ptr) == true) {
							LetObject::reference(&cls, &dataSf->back());
							dataSf->pop_back();
							LetObject::reference(&obj, &cls[param3C]);
						}
						else {
							auto_c cls = dataSf->back();
							dataSf->pop_back();
							obj = cls[param3C];
						}
					}
				}
				else if (var_name == "(Ru)") {
					auto_c param3 = (*cmd.getCodeParams())["param3"];
					std::string param3C;
					param3 >> param3C;
					if (param3C == "(u)") {
						auto_c ret = dataSf->back();
						dataSf->pop_back();
						obj = ret;
					}
					else {
						auto_c cls = dataSf->back();
						dataSf->pop_back();
						obj = cls[param3C];
					}
				}
				else {
					auto_c pushValue;
					auto interTemplate = m_stacks.getInterMapValue(var_name, true);
					pushValue[CLASS_NAME] << interTemplate.name;
					pushValue[CLASS_PARENT] << interTemplate.parent;
					pushValue[CLASS_AUTO_GC_CALL] << true;
					for (auto& i : interTemplate.var_people) {
						pushValue[i.first] = i.second;
					}

					obj = pushValue;
					obj.setAttribute(obj.getAttribute() | (int)NatureType::cls);
				}
			}
		}
	}

	dataSf->emplace_back(obj);
}

void ScriptC::Obj::CerVm::VmJmp()
{
	/*
	* jmp param1:��Ծ��� 
	*     param2:�Ƿ�ʹ������ 
	*     param3:�ӻ���ַ+eip�ľ�����ת 
	*     param4:��������id
	*	  param5:����������س���ֵ
	* 
	* param2: Ϊ����ʹ�������ջ��Ϊ����
	* param2: Ϊ$ ��ʶ�� param4 ��ΪԤ������ id
	*/
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto cmd = m_current_cmd_code;
	
	numberT jmper = m_command_codes_index -1;

	auto_c ret;
	auto_c key; key << true;
	takeEat(CommandCode::CommandCodeType::Jmp);

	/*
	* 2023.11.2
	* �޸���ת��䣬�ж���������
	* ʵ�ֽӿ�_attr��Ա�����ж�
	*/
	auto param2_finder = cmd.getCodeParams()->find("param2");
	if (param2_finder != cmd.getCodeParams()->end()) {

		std::string param2_str = LetObject::cast<std::string>(param2_finder->second);
		if (param2_str == "$")
		{
			auto param4_finder = cmd.getCodeParams()->find("param4");
			numberT param4_id = LetObject::cast<numberT>(param4_finder->second);
			if (BuiltInCond(param4_id,cmd))
				return;
		}
		else
		{
			auto_c eq = dataSf->back();
			dataSf->pop_back();
			ret = eq == key;

			if (LetObject::cast<bool>(ret)) {
				return;
			}
		}
	}

	if (cmd.getCodeParams()->find("param1") != cmd.getCodeParams()->end())
	{
		ret = (*cmd.getCodeParams())["param1"];
		m_command_codes_index = jmper + LetObject::cast<numberT>(ret);
		advance();
	}
	else if (cmd.getCodeParams()->find("param3") != cmd.getCodeParams()->end())
	{
		std::string str;
		dataSf->back() >> str;
		std::vector<numberT> eipVec;
		std::stringstream ss(str),ns;
		std::string path;
		while (std::getline(ss, path, ':')) {
			numberT num;
			ns << path;
			ns >> num;
			eipVec.emplace_back(num);
			ns.str("");
			ns.clear();
		}

		numberT index;
		ret = (*cmd.getCodeParams())["param3"];
		ret >> index;
		m_command_codes_index = eipVec.at(index) + 1;
		advance();
	}
	return;
}

void ScriptC::Obj::CerVm::VmPushS()
{
	auto_c ret;
	auto cmd = m_current_cmd_code;
	ret = (*cmd.getCodeParams())["param1"];
	takeEat(CommandCode::CommandCodeType::PushS);
	m_stacks.PushNewSF(LetObject::cast<std::string>(ret));
	return;
}

void ScriptC::Obj::CerVm::VmPopS()
{
	/*
	*	PopS  (�˳���ǰջ
	*	PopS param1:"~while" (�˳��� ~while�����򲢻�ȡ��0������ֵ
	*/

	auto_c ret;
	auto cmd = m_current_cmd_code;
	if ((*cmd.getCodeParams()).find("param1") != (*cmd.getCodeParams()).end()) {
		ret = (*cmd.getCodeParams()).find("param1")->second;
	}

	takeEat(CommandCode::CommandCodeType::PopS);

	if (ret.getType() != LetObject::ObjT::string) {
		CodePopNewSf();
	}
	else {
		std::string tarName;
		ret >> tarName;
		while (m_stacks.GetLastSF()->getStackFrameName() != tarName) {
			CodePopNewSf();
		}
	}
}

void ScriptC::Obj::CerVm::VmLens()
{
	/*
	* Lens : ��ȡջ�������Ԫ�ؼ��㲢�������鵽ջ��
	* Lets : param1:name �������name��Ԫ�ز����ص�ջ��
	*/

	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();

	auto_c ret,target;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Lens);
	auto_c name_var;
	if (cmd.getCodeParams()->find("param1") != cmd.getCodeParams()->end()) {
		std::string name;
		(*(cmd.getCodeParams()))["param1"] >> name;
		LetObject::reference(&target,m_stacks.getVarMapValuePtr(name, true));
	}
	else {
		LetObject::reference(&target, &dataSf->back());
	}

	auto_c obj;
	auto vec = target.getIndexs();
	numberT index = 0;
	for (auto& i : vec) {
		obj[index] = i;
		index++;
	}

	dataSf->emplace_back(obj);
}

void ScriptC::Obj::CerVm::VmInc()
{
	/*
	* Inc param1:fileName  ��ת��ָ���ļ���ִ��һ��
	*/
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();

	auto_c ret, target;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Inc);
	std::string fileName;
	(*cmd.getCodeParams())["param1"] >> fileName;

	auto_c resEipDatas;
	unsigned long nowEip;
	nowEip = m_command_codes_index - 1;
	std::string resEipString;
	resEipString = getCodeFile() + ";";
	resEipString += kstring::stringFrom<unsigned long>(nowEip);
	resEipDatas << resEipString;
	setCodeFile(fileName + ".sc");

	/*
	* 2023.11.22
	* require Ĭ�������ַ���eip
	* �����ļ�����leave��Ҫ������eip
	*/
	m_command_codes_index = 1;
	advance();
	dataSf->insert(dataSf->begin(), std::move(resEipDatas));
}

void ScriptC::Obj::CerVm::VmYield()
{
	/*
	* Yield: ����ǰ�����ջ
	*/

	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();

	std::string frameName = sf->getStackFrameName();

	auto_c retVal;
	bool haveRetVal = true;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Yield);

	/*
	* ָ�����ݲ��ٽ��п���ת��
	*/

	if (dataSf->back().getSelfAttribute() & (nature)AutoMem::Obj::NatureType::ptr)
		auto_c::reference(&retVal, &dataSf->back());
	else
		retVal = dataSf->back();
	dataSf->pop_back();

	// yield �ᱣ�������ڲ���if for while ջ
	numberT only_id = m_stacks.GetOnlyId();
	numberT cmmand_index = m_command_codes_index - 1;
	while (m_stacks.GetLastSF()->getStackFrameName()[0] == '~')
	{
		/* ���� if for while ջ */
		m_stacks.SaveLastSF(only_id, cmmand_index, getCodeFile());
	}

	sf = m_stacks.GetLastSF();
	runT = sf->getRunTime();
	dataSf = sf->getDataStack();

	auto_c retSip;
	retSip = *dataSf->begin();

	/* ���� ��ǰ����ջ */
	m_stacks.SaveLastSF(only_id, cmmand_index, getCodeFile());

	sf = m_stacks.GetLastSF();
	runT = sf->getRunTime();
	dataSf = sf->getDataStack();

	numberT retPtr = 0;
	if (retSip.getType() != LetObject::ObjT::string) {
		if (retSip.getType() == LetObject::ObjT::array)
		{
			std::string file_name;
			retSip[0] >> retPtr;
			retSip[1] >> file_name;
			setCodeFile(file_name);
		}
		else
			retSip >> retPtr;

		m_command_codes_index = retPtr;
	}
	else {
		std::string sipV;
		retSip >> sipV;
		auto retV = kstring::stringSplit(sipV, ';');
		retPtr = kstring::stringTo<numberT>(retV[1]);
		std::string filename = retV[0];
		setCodeFile(filename);

		//dataSf->pop_front();

		m_command_codes_index = retPtr;
	}

	advance();

	/*
	* 2023.11.18
	* ���游ջ��thisָ������
	*/
	if (!dataSf->empty())
	{
		if ((dataSf->back().getSelfAttribute() & 16) != 0)
		{
			auto_c this_ref{ dataSf->back() };
			m_stacks.SetBkStackThisRef(only_id, std::move(this_ref));
			dataSf->pop_back();
		}
	}

	/*
	* ���������͹��캯����ֹ���� yield
	*/
	if (frameName.find(":") != frameName.npos)
	{
		frameName = frameName.substr(frameName.find(":") + 1);

		if (frameName == SPECIAL_FUNC_GC || frameName == SPECIAL_FUNC_INIT)
		{
			auto errHis = ErrorHandling::getInstance();
			errHis->throwErr(EType::Vm, frameName + "prohibits calling yield");
		}
	}

	auto_c yield_ret;
	yield_ret[0] << only_id;
	yield_ret[1] = retVal;

	dataSf->emplace_back(yield_ret);
}

void ScriptC::Obj::CerVm::VmResume()
{
	/*
	* Resume: �ָ�ָ����ջ
	*/

	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	takeEat(CommandCode::CommandCodeType::Resume);

	auto_c auto_resume_ret = dataSf->back();
	dataSf->pop_back();

	auto_c auto_only_id = dataSf->back();
	dataSf->pop_back();

	numberT only_id;
	auto_only_id >> only_id;

	/* ��⴫���only id �Ƿ����*/
	if (!m_stacks.HasOnlyId(only_id))
	{
		auto errHis = ErrorHandling::getInstance();
		errHis->throwErr(EType::Vm, "Invalid Yield numeric id");
		return;
	}

	numberT command_eip = m_command_codes_index;
	numberT resume_eip = 0;
	std::string code_file;
	/* �ָ�ջ,���������� */
	while ((resume_eip = m_stacks.LoadSfOnce(only_id, code_file)) > -1)
	{
		auto_c retEip_c;
		retEip_c[0] << command_eip - 1;
		retEip_c[1] << getCodeFile();

		SFPtr sf = m_stacks.GetLastSF();
		auto runT = sf->getRunTime();
		auto dataSf = sf->getDataStack();

		bool has_this = false;
		
		/* ��ת��ǰeip */
		m_command_codes_index = resume_eip;
		setCodeFile(code_file);

		if (m_stacks.GetLastSF()->getStackFrameName()[0] == '~')
			continue;

		/* ����ջ��return eip */
		auto retSip = *dataSf->begin();
		if (retSip.getType() != LetObject::ObjT::string) {
			*dataSf->begin() = retEip_c;
		}

		/* �ָ�ջ��thisָ�� */
		SFPtr parent_sf = sf;
		parent_sf--;
		auto parent_data = parent_sf->getDataStack();

		if (parent_data->empty())
			continue;

		auto del_nature = parent_data->back().getSelfAttribute();
		if ((del_nature & 16) != 0)
		{
			auto_c this_var;
			LetObject::reference(&this_var, &parent_data->back());
			runT->setVarMapValue("this", std::move(this_var));
		}
	}

	/* �������� */
	sf = m_stacks.GetLastSF();
	runT = sf->getRunTime();
	dataSf = sf->getDataStack();
	dataSf->emplace_back(std::move(auto_resume_ret));

	advance();
}

auto_c ScriptC::Obj::CerVm::VmPop()
{
	/*
	* Pop : param1 (var_id) ��ջ���������һ��ֵ����ֵ��var_id
	* Pop : param1 (u) �������ջ��Ϊ��ֵ��������ֵ���������ֵ��ֵ����ֵ
	* Pop : param2 �Ƿ�����ֵ��������
	* Pop : param3 �±�
	* Pop : param4 RePush
	*/

	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Pop);

	auto_c obj = (*cmd.getCodeParams())["param1"];
	std::string var_name;
	obj >> var_name;
	// m_temp_varTable[var_name] = var_val;
	bool create = false;

	if ((*cmd.getCodeParams()).find("param2") != (*cmd.getCodeParams()).end()) {
		create = true;
	}

	if (var_name == "(u)") {
		auto_c ret,var_val;
		if ((dataSf->back().getSelfAttribute() & (int)NatureType::ptr) == true) {
			LetObject::reference(&ret, &dataSf->back());
		}
		else {
			ret = dataSf->back();
		}
		dataSf->pop_back();
		
		var_val = dataSf->back();
		dataSf->pop_back();
		ret = var_val;
		return ret;
	}

	auto_c var_val = dataSf->back();
	dataSf->pop_back();

	if (var_name.find(":") != var_name.npos) {
		std::string interName,varName;
		interName = var_name.substr(0, var_name.find(":"));
		varName = var_name.substr(var_name.find(":")+1);
		runT->insetInterVal(interName, varName, var_val);
	}
	else {
		/*
		* 2023.11.22
		* ���ַ������ڽ������ã���������
		*/
		if(!var_name.empty())
			m_stacks.setVarMapValue(var_name, var_val, !create);
	}

	return ret;
}

auto_c ScriptC::Obj::CerVm::VmAdd()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Add);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::Add);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmSub()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Sub);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::Sub);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmMul()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Mul);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::Mul);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmDiv()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Div);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::Div);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmMod()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Mod);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::Mod);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmLeftO()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::LeftO);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::LeftO);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmRightO()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::RightO);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::RightO);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmGtr()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Gtr);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::Gtr);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmGeq()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Geq);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::Geq);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmLss()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Lss);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::Lss);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmLeq()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Leq);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::Leq);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmEqu()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Equ);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::Equ);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmNeq()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Neq);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::Neq);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmAnd()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::And);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::And);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmXor()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Xor);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::Xor);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmOr()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Or);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::Or);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmDAnd()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::DAnd);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::DAnd);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmDOr()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::DOr);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::DOr);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmNot()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Not);

	auto_c one = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->calc_opera(CodeType::Not);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmMat()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Mat);

	auto_c one = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->calc_opera(CodeType::Mat);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmUnaryAdd()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::UnaryAdd);

	auto_c one = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->calc_opera(CodeType::UnaryAdd);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmUnarySub()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::UnarySub);

	auto_c one = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->calc_opera(CodeType::UnarySub);

	dataSf->emplace_back(calcSf->pop_opera());
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmAryIndex()
{
	/*
	* 2023.12.10
	* �±괦��ͨ�������ֽ�����н�������
	* ����ͨ�� push ��������
	* ���Ч��
	*/
	auto_c ret;
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();
	auto cmd = m_current_cmd_code;

	std::string direct_str;
	auto_c direct;
	direct = (*cmd.getCodeParams())["param1"];
	direct >> direct_str;

	takeEat(CommandCode::CommandCodeType::AryIndex);
	auto_c one = dataSf->back();
	dataSf->pop_back();
	auto_c two = dataSf->back();
	dataSf->pop_back();

	calcSf->push_opera(one);
	calcSf->push_opera(two);
	calcSf->calc_opera(CodeType::AryIndex);

	auto_c push_var;
	if (direct_str == "left")
	{
		LetObject::reference(&push_var, &calcSf->opera_back());
		calcSf->opera_drop();
	}
	else
		push_var = calcSf->pop_opera();
	
	dataSf->emplace_back(std::move(push_var));
	return ret;
}

auto_c ScriptC::Obj::CerVm::VmPass()
{
	auto_c ret;
	takeEat(CommandCode::CommandCodeType::Pass);
	return ret;
}

void ScriptC::Obj::CerVm::VmSwap()
{
	/*
	* Swap Param1:��һ��Ԫ��, Param2:�ڶ���Ԫ��
	*/
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	auto calcSf = sf->getCalcStack();

	auto_c ret;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Swap);

	auto_c param1, param2;
	param1 = cmd.getCodeParams()->find("param1")->second;
	param2 = cmd.getCodeParams()->find("param2")->second;


	numberT param1_numb, param2_numb;
	param1 >> param1_numb;
	param2 >> param2_numb;

	auto param1_iter = dataSf->rbegin();
	auto param2_iter = dataSf->rbegin();

	for (auto i = 0; i < ::llabs(param1_numb); i++)
		param1_iter++;

	for (auto i = 0; i < ::llabs(param2_numb); i++)
		param2_iter++;

	AutoMem::Obj::LetTools tools;
	tools.Swap(*param1_iter, *param2_iter);
}

bool ScriptC::Obj::CerVm::BuiltInCond(numberT id, CommandCode cmd)
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();


	using Opeartor = LetTools::Operator;
	LetTools tools;
	auto_c& target = dataSf->back();
	auto param5_autoc = cmd.getCodeParams()->find("param5")->second;
	std::string param5;
	param5_autoc >> param5;

	/*
	* �ж�ջ����ı������� &&
	* �ж�ջ����ı�������ĳ��Ա���� &&
	* �жϱ����������⺯�� _attr
	* �򲻽��� _attr �������õ���ת
	*/
	if (id == 1)
	{
		bool is_cls = (target.getAttribute() & (int)NatureType::cls) != 0;
		bool havent_value = target.getStrArrayPtr()->find(param5) == target.getStrArrayPtr()->end();
		bool has_func = false;
		if (is_cls && havent_value)
		{
			std::string class_name,func_name;
			target[CLASS_NAME] >> class_name;
			func_name = findClassFunc(class_name, SPECIAL_FUNC_ATTR);
			if (!func_name.empty())
				has_func = true;
		}
		return is_cls && havent_value && has_func;
	}



	auto errHis = ErrorHandling::getInstance();
	errHis->throwErr(EType::System, "Vm::BuiltInCond return default value \n [Please check the C++source code] ");
	return true;
}

void ScriptC::Obj::CerVm::CodeCallFunc(std::string _funcName, std::vector<auto_c> _param, std::string _thisName)
{
#if DebuvmLog && GlobalDebugOpend
	AutoMem::Obj::LetTools tools;
	std::cout << m_command_codes_index;
	vmlog(" - [*] CodeCallFunc( " + _funcName + " , " + _thisName + " , ");
	for (auto& i : _param) {
		tools.print(i);
	}
	vmlog(") /* push new stack */\n\n");
#endif


	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();

	// auto cmd = m_current_cmd_code;
	// takeEat(CommandCode::CommandCodeType::Call);

	auto_c this_var;
	auto_c paramSize;
	std::string funcName = _funcName;

	paramSize << (numberT)_param.size();

	if (!_thisName.empty())
	{
		LetObject::reference(&this_var, m_stacks.getVarMapValuePtr(_thisName, true));
	}

	numberT paramSizeT;
	paramSize >> paramSizeT;

	std::vector<auto_c> valueVec;

	for (size_t i = 0; i < paramSizeT; i++) {
		valueVec.emplace_back(_param.back());
		_param.pop_back();
	}

	auto funcMap = m_stacks.getFuncMapAddress(funcName);
	if (funcMap.addRess == 0) {
		auto errHis = ErrorHandling::getInstance();
		errHis->throwErr(EType::Vm, "invalid interface method call");
	}

	if (funcMap.params.size() != valueVec.size()) {
		auto errHis = ErrorHandling::getInstance();
		errHis->throwErr(EType::Vm, "params count error with function");
	}

	if (funcMap.hasExport) {
		auto_c ret;
		auto manager = DllFuncReader::getInstance();
		funcPtr func = manager->getFuncFromDll(funcName.c_str());
		std::reverse(std::begin(valueVec), std::end(valueVec));
		manager->callFunc(func, &valueVec, &ret);
		dataSf->emplace_back(std::move(ret));
		return;
	}


	numberT leavePtr = m_command_codes_index;
	leavePtr--;

	auto_c leaveSip;
	leaveSip[0] << leavePtr;
	leaveSip[1] << getCodeFile();

	setCodeFile(funcMap.fileName);


	m_stacks.PushNewSF(funcName);
	this_var.getAttribute();

	sf = m_stacks.GetLastSF();
	runT = sf->getRunTime();
	dataSf = sf->getDataStack();

	runT->setVarMapValue("this", std::move(this_var));

	for (size_t index = 0; index < funcMap.paramLen; index++) {
		runT->setVarMapValue(funcMap.params.at(index), valueVec.at(index));
	}

	runT->getVarMapValue("this").getAttribute();

	dataSf->emplace_back(leaveSip);
	m_command_codes_index = funcMap.addRess;
	advance();
	return;
}

void ScriptC::Obj::CerVm::CodeCallFunc(std::string _funcName, std::vector<auto_c> _param, auto_c _thisVar)
{
#if DebuvmLog && GlobalDebugOpend
	AutoMem::Obj::LetTools tools;
	std::cout << m_command_codes_index;
	vmlog(" - [*] CodeCallFunc( " + _funcName + " , " + "<this var>" + " , ");
	for (auto& i : _param) {
		tools.print(i);
	}
	vmlog(") /* push new stack */\n\n");
#endif


	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();

	// auto cmd = m_current_cmd_code;
	// takeEat(CommandCode::CommandCodeType::Call);

	auto_c this_var;
	if (_thisVar.getSelfAttribute() & (int)NatureType::ptr)
		LetObject::reference(&this_var, &_thisVar);
	else
		this_var = _thisVar;

	auto_c paramSize;
	std::string funcName = _funcName;

	paramSize << (numberT)_param.size();
	numberT paramSizeT;
	paramSize >> paramSizeT;

	std::vector<auto_c> valueVec;

	for (size_t i = 0; i < paramSizeT; i++) {
		valueVec.emplace_back(_param.back());
		_param.pop_back();
	}

	auto funcMap = m_stacks.getFuncMapAddress(funcName);
	if (funcMap.addRess == 0) {
		auto errHis = ErrorHandling::getInstance();
		errHis->throwErr(EType::Vm, "invalid interface method call");
	}

	if (funcMap.params.size() != valueVec.size()) {
		auto errHis = ErrorHandling::getInstance();
		errHis->throwErr(EType::Vm, "params count error with function");
	}

	if (funcMap.hasExport) {
		auto_c ret;
		auto manager = DllFuncReader::getInstance();
		funcPtr func = manager->getFuncFromDll(funcName.c_str());
		std::reverse(std::begin(valueVec), std::end(valueVec));
		manager->callFunc(func, &valueVec, &ret);
		dataSf->emplace_back(std::move(ret));
		return;
	}


	numberT leavePtr = m_command_codes_index;
	leavePtr--;

	auto_c leaveSip;
	leaveSip[0] << leavePtr;
	leaveSip[1] << getCodeFile();

	setCodeFile(funcMap.fileName);


	m_stacks.PushNewSF(funcName);
	this_var.getAttribute();

	sf = m_stacks.GetLastSF();
	runT = sf->getRunTime();
	dataSf = sf->getDataStack();

	runT->setVarMapValue("this", std::move(this_var));

	for (size_t index = 0; index < funcMap.paramLen; index++) {
		runT->setVarMapValue(funcMap.params.at(index), valueVec.at(index));
	}

	runT->getVarMapValue("this").getAttribute();

	dataSf->emplace_back(leaveSip);
	m_command_codes_index = funcMap.addRess;
	advance();
	return;
}

void ScriptC::Obj::CerVm::CodePopNewSf()
{
	m_stacks.PopNewSF();
}

void ScriptC::Obj::CerVm::printRunCode()
{
#if DebuvmLog && GlobalDebugOpend
	AutoMem::Obj::LetTools tools;
	std::cout << getCodeFile() + " : ";
	std::cout << m_command_codes_index;
	vmlog(" - VmRunCode(" + m_current_cmd_code.getCodeTypeStr());
	for (auto& i : *(m_current_cmd_code.getCodeParams())) {
		vmlog(" , " + i.first + ":");

		tools.print(i.second);
	}
	vmlog(")\n\n");
#endif
}

bool ScriptC::Obj::CerVm::isRunOver()
{
	bool index = false;
	index = !(m_command_codes_index < commandSize());
	return index;
}

bool ScriptC::Obj::CerVm::isLeaveStackPop()
{
	SFPtr sf = m_stacks.GetLastFuncSf();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	std::string frameName = sf->getStackFrameName();

	bool popStack = true;
	numberT retPtr = 0;
	auto_c retSip;

	retSip = *dataSf->begin();

	if (retSip.getType() == LetObject::ObjT::string) {
		return false;
	}

	return true;
}

bool ScriptC::Obj::CerVm::CmpCodeType(CommandCode::CommandCodeType type1, CommandCode::CommandCodeType type2)
{
	return (type1 == type2);
}

bool ScriptC::Obj::CerVm::CmpCurrentType(CommandCode::CommandCodeType type)
{
	if (isRunOver())
		return false;

	auto cur_type = m_current_cmd_code.getCodeType();
	return cur_type == type;
}

ScriptC::Obj::CerVm::VectorStr ScriptC::Obj::CerVm::isCallGc()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();

	VectorStr ret;

	auto var_list = runT->getVarMap();
	for (auto it = var_list->begin(); it != var_list->end(); it++)
	{
		if (it->first != "this")
		{
			if (it->second.getAttribute() == (int)NatureType::cls)
			{
				bool auto_gc_called;
				it->second[CLASS_AUTO_GC_CALL] >> auto_gc_called;
				if (auto_gc_called == true)
					ret.push_back(it->first);
				
			}
		}
	}

	return ret;
}

ScriptC::Obj::CerVm::VectorStr ScriptC::Obj::CerVm::isCallGc(SFPtr sf)
{
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();

	VectorStr ret;

	auto var_list = runT->getVarMap();
	for (auto it = var_list->begin(); it != var_list->end(); it++)
	{
		if (it->first != "this")
		{
			if (it->second.getAttribute() == (int)NatureType::cls)
			{
				bool auto_gc_called;
				it->second[CLASS_AUTO_GC_CALL] >> auto_gc_called;
				if (auto_gc_called == true)
					ret.push_back(it->first);

			}
		}
	}

	return ret;
}

ScriptC::Obj::CerVm::VectorStr ScriptC::Obj::CerVm::isCallGcR()
{
	SFPtr sf = m_stacks.GetLastSF();
	VectorStr ret;

	while (sf->getStackFrameName()[0] == '~')
	{
		/*
		* 2023.11.19
		* fixed : �޸�������ͬջ��ͬ����������ȡ�����ջ����bug
		*/
		VectorStr vec = isCallGc(sf);
		ret.insert(ret.begin(), vec.begin(), vec.end());
		sf--;
	} 

	/*
	* 2023.11.20
	* fixed : �޸�require�ķ�����ջLeave���Զ��������պ���
	*/
	if (isLeaveStackPop())
	{
		VectorStr vec = isCallGc(sf);
		ret.insert(ret.begin(), vec.begin(), vec.end());
	}

	return ret;
}

bool ScriptC::Obj::CerVm::getCallGcVar(SFPtr sf)
{
	bool hasVar = false;
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();

	for (auto it = dataSf->begin(); it != dataSf->end(); it++)
	{
		if (it->getAttribute() == (int)NatureType::cls)
		{
			bool auto_gc_called;
			(*it)[CLASS_AUTO_GC_CALL] >> auto_gc_called;
			if (auto_gc_called == true)
			{
				auto_c this_ref;
				(*it)[CLASS_AUTO_GC_CALL] << false;
				LetObject::reference(&this_ref, &(*it));
				m_destory_var.emplace_back(std::move(this_ref));
				hasVar = true;
			}
		}
	}

	return hasVar;
}

bool ScriptC::Obj::CerVm::getCallGcVarR()
{
	bool hasVar = false;
	SFPtr sf = m_stacks.GetLastSF();
	while (sf->getStackFrameName()[0] == '~')
	{
		hasVar |= getCallGcVar(sf);
		sf--;
	}

	/*
	* 
	* ��ֹrequire�ķ�����ջLeave���Զ��������պ���
	*/
	if (isLeaveStackPop())
	{
		hasVar |= getCallGcVar(sf);
	}

	return hasVar;
}

bool ScriptC::Obj::CerVm::GcCallBack()
{
	VectorStr var_list;
	if (!isRunOver())
	{
		// ��ȡ��ǰջ���нӿڱ�����
		if (CmpCurrentType(CommandCode::CommandCodeType::PopS))
			var_list = isCallGc();

		// ��ȡ��ǰջ���˼������нṹ������
		if (CmpCurrentType(CommandCode::CommandCodeType::Leave))
			var_list = isCallGcR();
	}
	
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();


	std::string var_name;
	std::string func_name;

	// ѭ����ȡ��ǰ���б���
	while (true)
	{
		// δ��õ��ӿڱ���
		if (var_list.empty())
			return false;

		// ��ȡ������
		var_name = *var_list.begin();

		// ��ȡ����ָ��
		auto_c var;


		/*
		* 2023.11.19
		* return ��������ջ
		* Fixed: �޸� Gcɾ����ջ��Ӱ��if�ڲ�return����ֵ������
		*/
		while (true && sf != m_stacks.GetBaseSF())
		{
			bool gc_call = false;
			runT = sf->getRunTime();
			dataSf = sf->getDataStack();

			var = runT->getVarMapValue(var_name);
			var[CLASS_AUTO_GC_CALL] >> gc_call;

			if (!gc_call)
			{
				/*
				* 2023.11.19
				* �Զ�ִ��Gc�������ֱ�����ٱ���
				*/
				sf--;
				runT->removeVarMapValue(var_name);
				continue;
			}

			// ���������־
			auto var_map = runT->getVarMap();
			if (var_map != nullptr)
			{
				auto iter = var_map->find(var_name);
				iter->second[CLASS_AUTO_GC_CALL] << false;
			}
			break;
		}
		

		// ��ȡ����������
		std::string className = LetObject::cast<std::string>(var[CLASS_NAME]);
		std::string gcName = SPECIAL_FUNC_GC;
		func_name = findClassFunc(className, gcName);

		// δ�ҵ������������
		auto func_des = m_stacks.getFuncMapAddress(func_name);
		if (func_des.addRess != 0)
			break;

		var_list.erase(var_list.begin());
	}

	// �������_gc����
	CodeCallFunc(func_name, {}, std::move(var_name));
	
	return true;
}

bool ScriptC::Obj::CerVm::GcCallBackDataSf()
{
	if (!isRunOver())
	{
		// ��ȡ��ǰջ���нӿڱ�����
		if (CmpCurrentType(CommandCode::CommandCodeType::PopS))
			getCallGcVar(m_stacks.GetLastSF());

		// ��ȡ��ǰջ���˼������нṹ������
		if (CmpCurrentType(CommandCode::CommandCodeType::Leave))
			getCallGcVarR();
	}

	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();

	auto_c var;
	std::string func_name;

	// ѭ����ȡ��ǰ���б���
	while (true)
	{
		// ���ջ�ӿڱ���
		if (!m_destory_var.empty())
		{
			auto it = m_destory_var.begin();
			// ��ȡ������
			LetObject::reference(&var, &(*it));
			m_destory_var.pop_front();
		}
		else if (!m_destory_data.empty())
		{
			auto it = m_destory_data.begin();
			var = *it;
			m_destory_data.pop_front();
		}
		else
			return false;

		// ��ȡ����������
		std::string className = LetObject::cast<std::string>(var[CLASS_NAME]);
		std::string gcName = SPECIAL_FUNC_GC;
		func_name = findClassFunc(className, gcName);

		// δ�ҵ������������
		auto func_des = m_stacks.getFuncMapAddress(func_name);
		if (func_des.addRess != 0)
			break;
	}

	// �������_gc����
	CodeCallFunc(func_name, {}, std::move(var));

	return true;
}

std::string ScriptC::Obj::CerVm::findClassFunc(std::string className , std::string funcName)
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();

	std::string func = className + ":" + funcName;
	auto funcMap = m_stacks.getFuncMapAddress(func);
	while (funcMap.addRess == 0 && !className.empty())
	{
		className = runT->getInterMapParent(className);

		func = className + ":" + funcName;
		funcMap = m_stacks.getFuncMapAddress(func);
	}

	if (className.empty())
		func.clear();

	return func;
}

bool ScriptC::Obj::CerVm::takeEat(CommandCode::CommandCodeType type)
{
	auto errHis = ErrorHandling::getInstance();
	errHis->setErrInfo(m_current_cmd_code.getDebugInfo());
	if (m_current_cmd_code.getCodeType() == type) {
		advance();
		return true;
	}

	errHis->throwErr(EType::Vm,"Code is not type of "+ m_current_cmd_code.getCodeTypeStr());
	return false;
}

void ScriptC::Obj::CerVm::advance()
{
	m_current_cmd_code = commandAt(m_command_codes_index);
	if (m_command_codes_index < commandSize()) {
		m_command_codes_index++;
	}
}


CerVm* ScriptC::Obj::CerVm::create(std::vector<CommandCode> codes)
{
	throw("DisAbled");
	return nullptr;
}

CerVm* ScriptC::Obj::CerVm::create(std::map<std::string, std::vector<CommandCode>> codes)
{
	extern std::string G_mainFile;

	bool init = false;
	CerVm* vm = new CerVm();
	vm->m_codes = codes;
	vm->m_command_codes = &vm->m_codes.find(G_mainFile)->second;

	if (vm) {
		init = vm->initVm();
	}

	if (!vm || !init) {
		delete vm;
		vm = nullptr;
	}

	return vm;
}

void ScriptC::Obj::CerVm::setCodeFile(std::string str)
{
	if (str != m_code_file)
	{
		auto_c filename;
		filename << str;

		auto sf = m_stacks.GetBaseSF();
		auto runT = sf->getRunTime(); 
		runT->setVarMapValue(SYSTEM_FILE_NAME, filename);
	}
	
	m_code_file = str;
	m_command_codes = &m_codes.find(str)->second;
}

std::string ScriptC::Obj::CerVm::getCodeFile()
{
	return m_code_file;
}

CommandCode ScriptC::Obj::CerVm::commandAt(size_t pos)
{
	commandNullptrThrow();
	return m_command_codes->at(pos);
}

size_t ScriptC::Obj::CerVm::commandSize()
{
	commandNullptrThrow();
	return m_command_codes->size();
}

void ScriptC::Obj::CerVm::commandNullptrThrow()
{
	if (m_command_codes == nullptr)
	{
		auto errHis = ErrorHandling::getInstance();
		errHis->throwErr(EType::Vm, "commandNullptrThrow Called \n Please Check C++ Code");
	}
}
