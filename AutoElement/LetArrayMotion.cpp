#include "LetArrayMotion.h"
#include <algorithm>

namespace Cervice {
	namespace Obj {
		numberT LetArrayMotion::ArrayCmp(LetObject* cmp1, LetObject* cmp2)
		{
			if (cmp1->getType() != LetObject::ObjT::array || cmp2->getType() != LetObject::ObjT::array) {
				return 2;
			}

			auto numPtr1 = cmp1->getNumArrayPtr()->size();
			auto strPtr1 = cmp1->getStrArrayPtr()->size();

			auto numPtr2 = cmp2->getNumArrayPtr()->size();
			auto strPtr2 = cmp2->getStrArrayPtr()->size();

			numberT lens = 2;
			if (numPtr1 == numPtr2) {
				if (strPtr1 == strPtr2) {
					lens = 0;
				}
				else if (strPtr1 > strPtr2) {
					lens = -1;
				}
				else{
					lens = 1;
				}
			}
			else if (numPtr1 > numPtr2) {
				lens = -1;
			}
			else {
				lens = 1;
			}

			numberT numCmp = cmpNum(cmp1, cmp2);
			numberT strCmp = cmpStr(cmp1, cmp2);

			if (numCmp != 0)
				return numCmp;
			else if (strCmp != 0)
				return strCmp;
			else if (numCmp == 0 && numCmp == strCmp) {
				if (lens != 2) {
					return lens; 
				}
				return numCmp;
			}

			return 2;
		}

		numberT LetArrayMotion::cmpNum(LetObject* cmp1, LetObject* cmp2)
		{
			if (cmp1->getType() != LetObject::ObjT::array || cmp2->getType() != LetObject::ObjT::array) {
				return 2;
			}

			auto cmp1Num = cmp1->getNumArrayPtr();
			auto cmp2Num = cmp2->getNumArrayPtr();

			auto cmp1NumCount = cmp1Num->size();
			auto cmp2NumCount = cmp2Num->size();

			size_t numSize = cmp1NumCount > cmp2NumCount ? cmp2NumCount : cmp1NumCount;

			numberT numCmp = 2;

			if (numSize == 0) {
				if (cmp1NumCount > cmp2NumCount) numCmp = -1;
				else if (cmp1NumCount < cmp2NumCount) numCmp = 1;
				else numCmp = 0;
			}

			numberT strCmp = 2;
			auto iptr = cmp1Num->begin();
			auto jptr = cmp2Num->begin();

			if (numSize == 0) {
				if (cmp1NumCount > cmp2NumCount) numCmp = -1;
				else if (cmp1NumCount < cmp2NumCount) numCmp = 1;
				else numCmp = 0;
			}

			for (auto in = 0; in < numSize; in++, iptr++, jptr++) {

				auto [i, j] = std::pair(iptr->first, jptr->first);

				auto cmp1Type = cmp1Num->at(i).getType();
				auto cmp2Type = cmp2Num->at(j).getType();

				if (cmp1Type != cmp2Type) {
					if (cmp1Type > cmp2Type) { numCmp = -1; break; }
					if (cmp1Type < cmp2Type) { numCmp = 1; break; }
				}

				if (cmp1Type == LetObject::ObjT::array) {
					numCmp = ArrayCmp(&cmp1Num->at(i), &cmp2Num->at(j));
					if (numCmp == 0) continue;
					break;
				}

				auto_c cmpRet;
				cmpRet = cmp1Num->at(i) == cmp2Num->at(j);
				if (LetObject::cast<bool>(cmpRet)) {
					numCmp = 0;
					continue;
				}

				cmpRet = cmp1Num->at(i) > cmp2Num->at(j);
				if (LetObject::cast<bool>(cmpRet)) {
					numCmp = -1;
					break;
				}

				cmpRet = cmp1Num->at(i) < cmp2Num->at(j);
				if (LetObject::cast<bool>(cmpRet)) {
					numCmp = 1;
					break;
				}
			}

			return numCmp;
		}

		numberT LetArrayMotion::cmpStr(LetObject* cmp1, LetObject* cmp2)
		{
			if (cmp1->getType() != LetObject::ObjT::array || cmp2->getType() != LetObject::ObjT::array) {
				return 2;
			}

			auto cmp1Str = cmp1->getStrArrayPtr();
			auto cmp2Str = cmp2->getStrArrayPtr();

			auto cmp1StrCount = cmp1Str->size();
			auto cmp2StrCount = cmp2Str->size();

			size_t strSize = cmp1StrCount > cmp2StrCount ? cmp2StrCount : cmp1StrCount;

			numberT strCmp = 2;
			auto iptr = cmp1Str->begin();
			auto jptr = cmp2Str->begin();

			if (strSize == 0) {
				if (cmp1StrCount > cmp2StrCount) strCmp = -1;
				else if (cmp1StrCount < cmp2StrCount) strCmp = 1;
				else strCmp = 0;
			}

			for (auto in = 0; in < strSize; in++, iptr++, jptr++) {

				auto [i, j] = std::pair(iptr->first, jptr->first);

				auto cmp1Type = cmp1Str->at(i).getType();
				auto cmp2Type = cmp2Str->at(j).getType();

				if (cmp1Type != cmp2Type) {
					if (cmp1Type > cmp2Type) { strCmp = -1; break; }
					if (cmp1Type < cmp2Type) { strCmp = 1; break; }
				}

				if (cmp1Type == LetObject::ObjT::array) {
					strCmp = ArrayCmp(&cmp1Str->at(i), &cmp2Str->at(j));
					if (strCmp == 0) continue;
					break;
				}

				auto_c cmpRet;
				cmpRet = (cmp1Str->at(i) == cmp2Str->at(j));
				if (LetObject::cast<bool>(cmpRet)) {
					strCmp = 0;
					continue;
				}

				cmpRet = cmp1Str->at(i) > cmp2Str->at(j);
				if (LetObject::cast<bool>(cmpRet)) {
					strCmp = -1;
					break;
				}

				cmpRet = cmp1Str->at(i) < cmp2Str->at(j);
				if (LetObject::cast<bool>(cmpRet)) {
					strCmp = 1;
					break;
				}
			}

			return strCmp;
		}

		LetObject LetArrayMotion::ArrayMax(LetObject* ary)
		{
			auto numPtr = ary->getNumArrayPtr();
			auto strPtr = ary->getStrArrayPtr();

			size_t numSize = numPtr->size();
			size_t strSize = strPtr->size();

			if (numSize == 0 && strSize == 0) {
				return auto_c(false, false);
			}

			LetObject ret;
			if (numSize != 0) ret = numPtr->begin()->second;
			else ret = strPtr->begin()->second;

			if (numSize + strSize == 0) {
				return ret;
			}

			auto retType = ret.getType();

			auto iptr = numPtr->begin();
			for (auto in = 0; in < numPtr->size(); in++,iptr++) {
				auto i = iptr->first;
				auto comType = numPtr->at(i).getType();
				if (comType > retType) {
					retType = comType;
					ret = numPtr->at(i);
					continue;
				}

				if (comType == retType) {
					if (comType == LetObject::ObjT::array && ArrayCmp(&ret,&numPtr->at(i)) == 1) {
						ret = numPtr->at(i);
						continue;
					}
					else{
						auto_c boolType = numPtr->at(i) > ret;
						if (LetObject::cast<bool>(boolType)) {
							ret = numPtr->at(i);
							continue;
						}
					}
				}

			}

			auto jptr = strPtr->begin();
			for (auto in = 0; in < strPtr->size(); in++, jptr++) {
				auto i = jptr->first;
				auto comType = strPtr->at(i).getType();
				if (comType > retType) {
					ret = strPtr->at(i);
					retType = comType;
					continue;
				}

				if (comType == retType) {
					if (comType == LetObject::ObjT::array && ArrayCmp(&ret, &strPtr->at(i)) == 1) {
						ret = strPtr->at(i);
						continue;
					}
					else {
						auto_c boolType = strPtr->at(i) > ret;
						if (LetObject::cast<bool>(boolType)) {
							ret = strPtr->at(i);
							continue;
						}
					}
				}
			}

			return ret;
		}

		LetObject LetArrayMotion::ArrayMin(LetObject* ary)
		{
			auto numPtr = ary->getNumArrayPtr();
			auto strPtr = ary->getStrArrayPtr();

			size_t numSize = numPtr->size();
			size_t strSize = strPtr->size();

			if (numSize == 0 && strSize == 0) {
				return auto_c(false, false);
			}

			LetObject ret;
			if (numSize != 0) ret = numPtr->begin()->second;
			else ret = strPtr->begin()->second;

			if (numSize + strSize == 0) {
				return ret;
			}

			auto retType = ret.getType();
			for (auto iptr = numPtr->begin(); iptr != numPtr->end(); iptr++) {
				auto i = iptr->first;
				auto comType = numPtr->at(i).getType();
				if (comType < retType) {
					retType = comType;
					ret = numPtr->at(i);
					continue;
				}

				if (comType == retType) {
					if (comType == LetObject::ObjT::array && ArrayCmp(&ret, &numPtr->at(i)) == -1) {
						ret = numPtr->at(i);
						continue;
					}
					else {
						auto_c boolType = numPtr->at(i) < ret;
						if (LetObject::cast<bool>(boolType)) {
							ret = numPtr->at(i);
							continue;
						}
					}
				}

			}

			for (auto iptr = strPtr->begin(); iptr != strPtr->end(); iptr++) {
				auto i = iptr->first;
				auto comType = strPtr->at(i).getType();
				if (comType < retType) {
					ret = strPtr->at(i);
					retType = comType;
					continue;
				}

				if (comType == retType) {
					if (comType == LetObject::ObjT::array && ArrayCmp(&ret, &strPtr->at(i)) == -1) {
						ret = strPtr->at(i);
						continue;
					}
					else {
						auto_c boolType = strPtr->at(i) < ret;
						if (LetObject::cast<bool>(boolType)) {
							ret = strPtr->at(i);
							continue;
						}
					}
				}
			}
			return ret;
		}
		bool LetArrayMotion::ArrayAppend(LetObject* arr, LetObject*val)
		{
			if (arr->getType() != LetObject::ObjT::array) {
				return false;
			}

			numberT index = 0;
			for (auto& i : *(arr->getNumArrayPtr())) {
				index = i.first > index ? i.first : index;
			}

			index += 1;
			arr->getNumArrayPtr()->insert({ index,*(BlockArray*)val });

			return true;
		}

		numberT LetArrayMotion::ArrayCount(LetObject* var, LetObject* va1)
		{
			if (var->getType() != LetObject::ObjT::array) {
				return -1;
			}

			numberT ret = 0;
			auto va1Type = va1->getType();
			auto numPtr = var->getNumArrayPtr();
			auto strPtr = var->getStrArrayPtr();
			for (auto& i : *numPtr) {
				auto type = i.second.getType();
				if (type == va1Type) {
					if (type == LetObject::ObjT::array) {
						if (ArrayCmp(va1, &i.second) == 0) {
							ret++;
							continue;
						}
					}
					else {
						auto_c boolVa = i.second == va1;
						if (LetObject::cast<bool>(boolVa)) {
							ret++;
							continue;
						}
					}
				}
			}

			for (auto& i : *strPtr) {
				auto type = i.second.getType();
				if (type == va1Type) {
					if (type == LetObject::ObjT::array) {
						if (ArrayCmp(va1, &i.second) == 0) {
							ret++;
							continue;
						}
					}
					else {
						auto_c boolVa = i.second == va1;
						if (LetObject::cast<bool>(boolVa)) {
							ret++;
							continue;
						}
					}
				}
			}

			return ret;
		}

		bool LetArrayMotion::ArrayExtend(LetObject* arr, LetObject* tar)
		{
			if (arr->getType() != LetObject::ObjT::array) {
				return false;
			}

			numberT index = 0;
			auto arrPtr = arr->getNumArrayPtr();
			if (arrPtr->size() != 0) {
				index = arrPtr->begin()->first;
				for (auto& i : *arrPtr) {
					index = i.first > index ? i.first : index;
				}
				index += 1;
			}

			if (tar->getType() != LetObject::ObjT::array) {
				arrPtr->insert({ index,*(BlockArray*)tar });
				return true;
			}

			auto tarNum = tar->getNumArrayPtr();
			auto tarStr = tar->getStrArrayPtr();

			for (auto& i : *tarNum) {
				if (ArrayExtend(arr, &i.second) == false) {
					return false;
				}
			}

			for (auto& i : *tarStr) {
				if (ArrayExtend(arr, &i.second) == false) {
					return false;
				}
			}

			return true;
		}

		std::optional<LetObject>
			LetArrayMotion::ArrayBack(LetObject* var)
		{
			if (var->getType() != LetObject::ObjT::array) {
				return {};
			}

			auto numPtr = var->getNumArrayPtr();
			auto strPtr = var->getStrArrayPtr();

			if (numPtr->size() == 0 && numPtr->size() == strPtr->size()) {
				return {};
			}

			std::optional<std::variant<numberT, std::string>> ret;
			for (auto& i : *numPtr) {
				if (!ret.has_value()) {
					ret = i.first;
					continue;;
				}

				if (i.first > std::get<numberT>(*ret)) {
					ret = i.first;
					continue;
				}
			}
			if (ret.has_value()) {
				LetObject let;
				let = numPtr->at(std::get<numberT>(*ret));
				return let;
			}
			for (auto& i : *strPtr) {
				if (!ret.has_value()) {
					ret = i.first;
					continue;;
				}

				if (i.first > std::get<std::string>(*ret)) {
					ret = i.first;
					continue;
				}
			}
			LetObject let;
			let = strPtr->at(std::get<std::string>(*ret));
			return let;
		}

		std::optional<LetObject> LetArrayMotion::ArrayFront(LetObject* var)
		{
			if (var->getType() != LetObject::ObjT::array) {
				return {};
			}

			auto numPtr = var->getNumArrayPtr();
			auto strPtr = var->getStrArrayPtr();

			if (numPtr->size() == 0 && numPtr->size() == strPtr->size()) {
				return {};
			}

			std::optional<std::variant<numberT, std::string>> ret;
			for (auto& i : *numPtr) {
				if (!ret.has_value()) {
					ret = i.first;
					continue;;
				}

				if (i.first < std::get<numberT>(*ret)) {
					ret = i.first;
					continue;
				}
			}
			if (ret.has_value()) {
				LetObject let;
				let = numPtr->at(std::get<numberT>(*ret));
				return let;
			}
			for (auto& i : *strPtr) {
				if (!ret.has_value()) {
					ret = i.first;
					continue;;
				}

				if (i.first < std::get<std::string>(*ret)) {
					ret = i.first;
					continue;
				}
			}
			LetObject let;
			let = strPtr->at(std::get<std::string>(*ret));
			return let;
		}

		std::optional<std::variant<numberT, std::string>> 
			LetArrayMotion::ArrayIndex(LetObject* var, LetObject* tar)
		{
			if (var->getType() != LetObject::ObjT::array) {
				return {};
			}

			auto numPtr = var->getNumArrayPtr();
			auto strPtr = var->getStrArrayPtr();

			auto tarType = tar->getType();

			for (auto& i : *numPtr) {
				auto varType = i.second.getType();
				if (tarType == varType) {
					if (tarType == LetObject::ObjT::array) {
						if (ArrayCmp(&i.second, tar) == 0) {
							return i.first;
						}
					}
					else {
						auto_c bolv = i.second == tar;
						if (LetObject::cast<bool>(bolv)) {
							return i.first;
						}
					}
				}
			}

			for (auto& i : *strPtr) {
				auto varType = i.second.getType();
				if (tarType == varType) {
					if (tarType == LetObject::ObjT::array) {
						if (ArrayCmp(&i.second, tar) == 0) {
							return i.first;
						}
					}
					else {
						auto_c bolv = i.second == tar;
						if (LetObject::cast<bool>(bolv)) {
							return i.first;
						}
					}
				}
			}

			return {};
		}

		bool LetArrayMotion::ArrayInsert(LetObject* var, numberT index, LetObject* tar)
		{
			bool ret = false;
			if (var->getType() != LetObject::ObjT::array) {
				return false;
			}

			ret = ArrayInsertNum(var, index, tar);

			return ret;
		}

		bool LetArrayMotion::ArrayInsertNum(LetObject* var, numberT index, LetObject* tar)
		{
			if (var->getType() != LetObject::ObjT::array) {
				return false;
			}

			auto numPtr = var->getNumArrayPtr();
			if (numPtr->empty()) return false;

			LetObject second;
			for (auto& i : *numPtr) {
				if (i.first > index) {
					second[i.first] = i.second;
				}
			}

			numberT step = 0;
			if (tar->getType() == LetObject::ObjT::array) {
				auto tarPtrNum = *tar->getNumArrayPtr();
				auto tarPtrStr = *tar->getStrArrayPtr();
				std::vector<numberT> tarVecNum;
				std::vector<std::string> tarVecStr;
				for (auto& i : tarPtrNum) {
					tarVecNum.push_back(i.first);
				}
				for (auto& i : tarPtrStr) {
					tarVecStr.push_back(i.first);
				}
				std::sort(tarVecNum.begin(), tarVecNum.end());
				std::sort(tarVecStr.begin(), tarVecStr.end());

				for (auto& j : tarVecNum) {
					step += 1;
					index += 1;
					numPtr->insert({ index ,tarPtrNum.at(j) });
					numPtr->at(index) = tarPtrNum.at(j);
				}

				for (auto& j : tarVecStr) {
					step += 1;
					index += 1;
					numPtr->insert({ index ,tarPtrStr.at(j) });
					numPtr->at(index) = tarPtrStr.at(j);
				}
			}
			else {
				step++;
				numPtr->insert({ index + 1,*(BlockArray*)tar });
			}

			for (auto& i : *second.getNumArrayPtr()) {
				if (numPtr->find(i.first) != numPtr->end()) {
					numPtr->insert({ i.first + step ,i.second });
					numPtr->at(i.first + step) = i.second;
				}
				else
				{
					numPtr->insert({ i.first,i.second });
				}
			}

			return true;
		}

		bool LetArrayMotion::ArrayPop(LetObject* var, std::variant<numberT, std::string> index)
		{
			if (var->getType() != LetObject::ObjT::array) {
				return false;
			}

			auto numPtr = var->getNumArrayPtr();
			auto strPtr = var->getStrArrayPtr();
		
			// number
			if (index.index() == 0) {
				auto numFinder = numPtr->find(std::get<numberT>(index));
				if (numFinder != numPtr->end()) {
					numPtr->erase(numFinder);
				}
			}
			// string
			else {
				auto strFinder = strPtr->find(std::get<std::string>(index));
				if (strFinder != strPtr->end()) {
					strPtr->erase(strFinder);
				}
			}

			return true;
		}

		bool LetArrayMotion::ArrayRemove(LetObject* var, LetObject* tar)
		{
			if (var->getType() != LetObject::ObjT::array) {
				return false;
			}

			auto numPtr = var->getNumArrayPtr();
			auto strPtr = var->getStrArrayPtr();

			std::vector<numberT> numVec;
			std::vector<std::string> strVec;

			for (auto& i : *numPtr) numVec.push_back(i.first);
			for (auto& i : *strPtr) strVec.push_back(i.first);

			std::sort(numVec.begin(), numVec.end());
			std::sort(strVec.begin(), strVec.end());

			auto tarType = tar->getType();

			for (auto& i : numVec) {
				
				auto finder = numPtr->find(i);
				auto findType = finder->second.getType();

				if (findType != tarType) continue;
				bool is = false;
				if (findType == LetObject::ObjT::array) {
					 is = ArrayCmp(&finder->second, tar) == 0;
				}
				else {
					auto_c boolva = finder->second == tar;
					is = LetObject::cast<bool>(boolva);
				}

				if (is) {
					numPtr->erase(finder);
					return true;
				}
			}

			for (auto& i : strVec) {
				auto finder = strPtr->find(i);
				auto findType = finder->second.getType();

				if (findType != tarType) continue;
				bool is = false;
				if (findType == LetObject::ObjT::array) {
					is = ArrayCmp(&finder->second, tar) == 0;
				}
				else {
					auto_c boolva = finder->second == tar;
					is = LetObject::cast<bool>(boolva);
				}

				if (is) {
					strPtr->erase(finder);
					return true;
				}
			}
			
			return true;
		}

		bool LetArrayMotion::ArrayReverse(LetObject* var)
		{
			if (var->getType() != LetObject::ObjT::array) {
				return false;
			}

			auto numPtr = var->getNumArrayPtr();
			auto strPtr = var->getStrArrayPtr();

			std::vector<numberT> numVec,numVecR;
			std::vector<std::string> strVec,strVecR;

			for (auto& i : *numPtr) numVec.push_back(i.first);
			for (auto& i : *strPtr) strVec.push_back(i.first);

			std::sort(numVec.begin(), numVec.end());
			std::sort(strVec.begin(), strVec.end());

			numVecR = numVec;
			strVecR = strVec;

			std::reverse(numVecR.begin(), numVecR.end());
			std::reverse(strVecR.begin(), strVecR.end());

			for (auto i = 0; i < numVec.size() / 2; i++) {
				numberT index = numVec.at(i);
				numberT indexR = numVecR.at(i);

				auto ptr = numPtr->extract(index);
				auto ptrR = numPtr->extract(indexR);
				ptr.key() = indexR;
				ptrR.key() = index;
				numPtr->insert(std::move(ptr));
				numPtr->insert(std::move(ptrR));
			}

			for (auto i = 0; i < strVec.size() / 2; i++) {
				std::string index = strVec.at(i);
				std::string indexR = strVecR.at(i);

				auto ptr = strPtr->extract(index);
				auto ptrR = strPtr->extract(indexR);
				ptr.key() = indexR;
				ptrR.key() = index;
				strPtr->insert(std::move(ptr));
				strPtr->insert(std::move(ptrR));
			}

			return true;
		}

		LetObject Cervice::Obj::LetArrayMotion::ArrayRange(numberT min, numberT max)
		{
			LetObject ret;

			for (; min <= max; min++) {
				ret[min] << min;
			}

			return ret;
		}

		LetObject Cervice::Obj::LetArrayMotion::ArrayChilds(LetObject* var)
		{
			LetObject ret;
			if (var->getType() != LetObject::ObjT::array) {
				return ret;
			}

			auto numPtr = var->getNumArrayPtr();
			auto strPtr = var->getStrArrayPtr();

			std::vector<numberT> numVec;
			std::vector<std::string> strVec;

			for (auto& i : *numPtr) numVec.push_back(i.first);
			for (auto& i : *strPtr) strVec.push_back(i.first);

			std::sort(numVec.begin(), numVec.end());
			std::sort(strVec.begin(), strVec.end());

			if (numVec.empty() && strVec.empty()) 
				return ret;
			
			numberT index = 0;

			for (auto& i : numVec) {
				ret[index++] << i;
			}

			for (auto& i : strVec) {
				ret[index++] << i;
			}

			return ret;
		}
	}
}