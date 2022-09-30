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
#include "CerVm.h"

#include <sstream>
using namespace Cervice::Obj;

std::map<std::string, size_t> Cervice::Obj::CerVm::m_CodeBaseAddress;

Cervice::Obj::CerVm::CerVm() :
	m_isinit(false),
	m_command_codes_index(0),
	m_current_cmd_code(CodeType::Pass, {})
{
}

Cervice::Obj::CerVm::~CerVm()
{
}

bool Cervice::Obj::CerVm::initVm()
{
	if (m_isinit)
		return true;

	m_stacks.PushNewSF("!console");
	m_isinit = true;
	return true;
}

void Cervice::Obj::CerVm::Command()
{
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
	case Cervice::Obj::CommandCode::CommandCodeType::Inter:
		VmInter();
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Func:
		VmFunc();
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Call:
		VmCall();
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Leave:
		VmLeave();
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Jmp:
		VmJmp();
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::PushS:
		VmPushS();
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::PopS:
		VmPopS();
		break;
	case Cervice::Obj::CommandCode::CommandCodeType::Lens:
		VmLens();
		break;
	case CommandCode::CommandCodeType::Inc:
		VmInc();
		break;
	default:
		break;
	}
}

void Cervice::Obj::CerVm::runTime()
{
	extern std::string G_mainFile;
	auto_c fileName;
	fileName << G_mainFile;
	m_stacks.setVarMapValue("_FILE_NAME_", std::move(fileName), false);

	m_stacks.PushNewSF("!program");

	if (m_command_codes.size() == 0 || !m_isinit) {
		return;
	}

	m_command_codes_index = 0;
	m_current_cmd_code = m_command_codes.at(m_command_codes_index);
	m_command_codes_index = 1;

	do
	{
		Command();
	} while (m_command_codes_index < m_command_codes.size());

	m_stacks.PopNewSF();
}

auto_c Cervice::Obj::CerVm::runTime(std::string result)
{
	m_stacks.PushNewSF("!program");

	if (m_command_codes.size() == 0 || !m_isinit) {
		return auto_c();
	}

	m_command_codes_index = 0;
	m_current_cmd_code = m_command_codes.at(m_command_codes_index);
	m_command_codes_index = 1;

	do
	{
		Command();
	} while (m_command_codes_index < m_command_codes.size());

	SFPtr sf = m_stacks.GetLastSF();
	auto dataSf = sf->getDataStack();
	auto_c ret;

	if (sf->getStackFrameName() != "!program") {
		ret = dataSf->back();
	}

	dataSf->pop_back();

	m_stacks.PopNewSF();

	return ret;
}

void Cervice::Obj::CerVm::VmInter()
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

void Cervice::Obj::CerVm::VmFunc()
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

void Cervice::Obj::CerVm::VmCall()
{
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();

	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Call);

	std::stringstream ss;
	auto_c this_var;
	std::string this_name;
	auto_c func = (*cmd.getCodeParams())["param1"];
	auto_c paramSize = (*cmd.getCodeParams())["param2"];
	std::string funcName;

	if ((*cmd.getCodeParams()).find("param3") != (*cmd.getCodeParams()).end()) {
		this_var = (*cmd.getCodeParams())["param3"];
		this_var >> this_name;
		LetObject::reference(&this_var, m_stacks.getVarMapValuePtr(this_name, true));

	}


	func >> funcName;

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
			errHis->throwErr(EType::Vm,"value is not person with function");
		}
		dataSf->back()["_class_name"] >> className;
		funcName = funcName.substr(3, funcName.size());
		funcName = className + ":" + funcName;
		dataSf->pop_back();
	}

	auto funcMap = m_stacks.getFuncMapAddress(funcName);
	if (funcMap.addRess == 0) {
		auto errHis = ErrorHandling::getInstance();
		errHis->throwErr(EType::Vm, "value is not person with function");
	}

	if (funcMap.params.size() != valueVec.size()) {
		auto errHis = ErrorHandling::getInstance();
		errHis->throwErr(EType::Vm, "params count error with function");
	}

	if (funcMap.hasExport) {
		auto_c ret;
		auto manager = DllFuncReader::getInstance();
		funcPtr func = manager->getFuncFromDll(funcName.c_str());
		manager->callFunc(func, &valueVec, &ret);
		dataSf->emplace_back(std::move(ret));
		return;
	}

	numberT leavePtr = m_command_codes_index;
	leavePtr--;

	auto_c leaveSip;
	leaveSip << leavePtr;


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

void Cervice::Obj::CerVm::VmLeave()
{
	/*
	* leave : param1:value		// 拿出栈头数据作为地址跳转并将推送value到外层栈
	* leave :					// 拿出栈头数据作为地址跳转并推送空值到外层栈
	*/
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();

	auto_c retVal;
	bool haveRetVal = false;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Leave);
	if (cmd.getCodeParams()->find("param1") != cmd.getCodeParams()->end()) {
		haveRetVal = true;
		retVal = dataSf->back();
		dataSf->pop_back();
	}

	// 删除所有 if while for 堆栈
	while (m_stacks.GetLastSF()->getStackFrameName()[0] == '~')
	{
		m_stacks.PopNewSF();
	}

	sf = m_stacks.GetLastSF();
	runT = sf->getRunTime();
	dataSf = sf->getDataStack();

	bool popStack = true;
	numberT retPtr = 0;
	auto_c retSip;

	retSip = *dataSf->begin();

	if (retSip.getType() != LetObject::ObjT::string) {
		numberT fileAddress = (getBaseAddress() - 1);
		retSip >> retPtr;
		m_command_codes_index = fileAddress + retPtr;
	}
	else {
		std::string sipV;
		retSip >> sipV;
		auto retV = kstring::stringSplit(sipV,';');
		retPtr = kstring::stringTo<numberT>(retV[1]);
		std::string filename = retV[0];
		setBaseAddress(filename);
		popStack = false;

		dataSf->pop_front();
		m_command_codes_index = retPtr;
	}


	if (popStack) {
		m_stacks.PopNewSF();
	}

	advance();

	sf = m_stacks.GetLastSF();
	runT = sf->getRunTime();
	dataSf = sf->getDataStack();

	if (haveRetVal) {
		dataSf->emplace_back(retVal);
	}
	else {
		auto_c space;
		dataSf->emplace_back(space);
	}
}

void Cervice::Obj::CerVm::VmPush()
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
				if ((*cmd.getCodeParams()).find("param3") != (*cmd.getCodeParams()).end()) {
					std::string var_name;
					obj >> var_name;
					LetObject::reference(&obj,m_stacks.getVarMapValuePtr(var_name, true));
				}
				else {
					std::string var_name;
					obj >> var_name;
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

				std::vector<auto_c> vecR;
				std::vector<auto_c> vecL;
				auto_c targetArray;
				for (auto i = allPath.rbegin(); i != allPath.rend();) {
					std::string index, indextype;
					index = i->substr(0, i->rfind(":"));
					indextype = i->substr(i->rfind(":") + 1);

					if (indextype == "(i)") {
						vecR.emplace_back(dataSf->back());
						dataSf->pop_back();
					}

					else if (indextype == "(s)") {
						vecL.emplace_back(dataSf->back());
						dataSf->pop_back();
					}

					i++;
				}
				if (vecR.size() != 0) {
					targetArray = dataSf->back();
					dataSf->pop_back();
				}

				numberT indexI = 0;
				for (auto i = vecR.rbegin(); i != vecR.rend(); i ++) {
					auto_c indexC;
					indexC << indexI;
					indexI++;
					if (vecR.size() != 1) {
						obj[indexC] = targetArray[*i];
					}
					else {
						obj = targetArray[*i];
					}
				}

				if (vecL.size() != 0) {
					LetObject::reference(&targetArray,&dataSf->back());
					dataSf->pop_back();
				}

				for (auto i = vecL.rbegin(); i != vecL.rend(); i++) {
					LetObject::reference(&obj, &targetArray[*i]);
				}

				break;
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
					pushValue["_class_name"] << interTemplate.name;
					pushValue["_class_parent"] << interTemplate.parent;
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

void Cervice::Obj::CerVm::VmJmp()
{
	/*
	* jmp param1:相对距离 param2:是否使用条件 param3:从基地址+eip的绝对跳转
	*/
	SFPtr sf = m_stacks.GetLastSF();
	auto runT = sf->getRunTime();
	auto dataSf = sf->getDataStack();
	
	numberT jmper = m_command_codes_index -1;

	auto_c ret;
	auto_c key; key << true;
	auto cmd = m_current_cmd_code;
	takeEat(CommandCode::CommandCodeType::Jmp);

	if (cmd.getCodeParams()->find("param2") != cmd.getCodeParams()->end()) {
		auto_c eq = dataSf->back();
		dataSf->pop_back();
		ret = eq == key;
		if (LetObject::cast<bool>(ret)) {
			return;
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
		m_command_codes_index = getBaseAddress() + eipVec.at(index);
		advance();
	}
	return;
}

void Cervice::Obj::CerVm::VmPushS()
{
	auto_c ret;
	auto cmd = m_current_cmd_code;
	ret = (*cmd.getCodeParams())["param1"];
	takeEat(CommandCode::CommandCodeType::PushS);
	m_stacks.PushNewSF(LetObject::cast<std::string>(ret));
	return;
}

void Cervice::Obj::CerVm::VmPopS()
{
	/*
	*	PopS  (退出当前栈
	*	PopS param1:"~while" (退出到 ~while作用域并获取第0个绝对值
	*/

	auto_c ret;
	auto cmd = m_current_cmd_code;
	if ((*cmd.getCodeParams()).find("param1") != (*cmd.getCodeParams()).end()) {
		ret = (*cmd.getCodeParams()).find("param1")->second;
	}

	takeEat(CommandCode::CommandCodeType::PopS);

	if (ret.getType() != LetObject::ObjT::string) {
		m_stacks.PopNewSF();
	}
	else {
		std::string tarName;
		ret >> tarName;
		while (m_stacks.GetLastSF()->getStackFrameName() != tarName) {
			m_stacks.PopNewSF();
		}
	}
}

void Cervice::Obj::CerVm::VmLens()
{
	/*
	* Lens : 获取栈中最近的元素计算并返回数组到栈中
	* Lets : param1:name 计算变量name的元素并返回到栈中
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

void Cervice::Obj::CerVm::VmInc()
{
	/*
	* Inc param1:fileName  跳转到指定文件并执行一遍
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
	unsigned long nowEip, jumperEip;
	nowEip = m_command_codes_index - 1;
	std::string resEipString;
	resEipString = m_BaseAddress + ";";
	resEipString += kstring::stringFrom<unsigned long>(nowEip);
	resEipDatas << resEipString;
	setBaseAddress(fileName + ".sc");
	jumperEip = getBaseAddress();
	m_command_codes_index = jumperEip;
	advance();
	dataSf->insert(dataSf->begin(), std::move(resEipDatas));
}

auto_c Cervice::Obj::CerVm::VmPop()
{
	/*
	* Pop : param1 (var_id) 从栈返回最近的一个值并赋值给var_id
	* Pop : param1 (u) 将最近的栈作为左值并将除左值以外最近的值赋值给左值
	* Pop : param2 是否新左值变量声明
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
		m_stacks.setVarMapValue(var_name, var_val, !create);
	}

	return ret;
}

auto_c Cervice::Obj::CerVm::VmAdd()
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

auto_c Cervice::Obj::CerVm::VmSub()
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

auto_c Cervice::Obj::CerVm::VmMul()
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

auto_c Cervice::Obj::CerVm::VmDiv()
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

auto_c Cervice::Obj::CerVm::VmMod()
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

auto_c Cervice::Obj::CerVm::VmLeftO()
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

auto_c Cervice::Obj::CerVm::VmRightO()
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

auto_c Cervice::Obj::CerVm::VmGtr()
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

auto_c Cervice::Obj::CerVm::VmGeq()
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

auto_c Cervice::Obj::CerVm::VmLss()
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

auto_c Cervice::Obj::CerVm::VmLeq()
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

auto_c Cervice::Obj::CerVm::VmEqu()
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

auto_c Cervice::Obj::CerVm::VmNeq()
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

auto_c Cervice::Obj::CerVm::VmAnd()
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

auto_c Cervice::Obj::CerVm::VmXor()
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

auto_c Cervice::Obj::CerVm::VmOr()
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

auto_c Cervice::Obj::CerVm::VmDAnd()
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

auto_c Cervice::Obj::CerVm::VmDOr()
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

auto_c Cervice::Obj::CerVm::VmNot()
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

auto_c Cervice::Obj::CerVm::VmMat()
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

auto_c Cervice::Obj::CerVm::VmUnaryAdd()
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

auto_c Cervice::Obj::CerVm::VmUnarySub()
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

auto_c Cervice::Obj::CerVm::VmPass()
{
	auto_c ret;
	takeEat(CommandCode::CommandCodeType::Pass);
	return ret;
}

bool Cervice::Obj::CerVm::takeEat(CommandCode::CommandCodeType type)
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

void Cervice::Obj::CerVm::advance()
{
	m_current_cmd_code = m_command_codes.at(m_command_codes_index);
	if (m_command_codes_index < m_command_codes.size()) {
		m_command_codes_index++;
	}
}


CerVm* Cervice::Obj::CerVm::create(std::vector<CommandCode> codes)
{
	bool init = false;
	CerVm* vm = new CerVm();
	vm->m_command_codes = codes;

	if(vm){
		init = vm->initVm();
	}

	if (!vm || !init) {
		delete vm;
		vm = nullptr;
	}

	return vm;
}

void Cervice::Obj::CerVm::setBaseAddress(std::string str)
{
	m_BaseAddress = str;
}

size_t Cervice::Obj::CerVm::getBaseAddress()
{
	return m_CodeBaseAddress[m_BaseAddress];
}
