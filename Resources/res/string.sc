require ("extern_system\\system_string");

interface StdString{

    // str:string ,delim:string -> array|null
    function split(str,delim):
        return __system__str__split__(str,delim);
    end 

    // str:string ,delim:string -> array|null
    function cut(str,delim):
        return __system__str__cut__(str,delim);
    end 

    // str:string ,pos:number ,lens:number -> string|null
    function sub(str,pos,lens):
        return __system__str__sub__(str,pos,lens);
    end 

    // str:string ,src:string ,des:string ,cnt:number -> string|null
    function replace(str,src,des,cnt):
        return __system__str__replace__(str,src,des,cnt);
    end 

    // str:string -> string|null
    function reverse(str):
        return __system__str__reverse__(str);
    end 

    // str:string -> string|null
    function lower(str):
        return __system__str__lower__(str);
    end 

    // str:string -> string|null
    function upper(str):
        return __system__str__upper__(str);
    end 

    // str:string -> string|null
    function tirm(str):
        return __system__str__tirm__(str);
    end 

    // str:string -> string|null
    function up(str):
        return __system__str__up__(str);
    end 

    // str:string -> string|null
    function low(str):
        return __system__str__low__(str);
    end 

    // str:string ,pos:number ,lens:number ,txt:string -> string|null
    function fill(str,pos,lens,txt):
        return __system__str__fill__(str,pos,lens,txt);
    end 

    // str:string ,src:string -> number|null
    function count(str,src):
        return __system__str__count__(str,src);
    end 

    // str:string ,index:number -> string|null
    function at(str,index):
        return __system__str__at__(str,index);
    end 

    // str:string ,index:number ,lens:number -> string|null
    function erase(str,index,lens):
        return __system__str__erase__(str,index,lens);
    end 

    // str:string ,index:number ,txt:string -> string|null
    function insert(str,index,txt):
        return __system__str__insert__(str,index,txt);
    end 

    // str:string -> string|null
    function lens(str):
        return __system__str__lens__(str);
    end 

    // str:string ,txt:string -> array|null
    function find_all(str,txt):
        return __system__str__find_all__(str,txt);
    end 

    // str:string ,txt:string -> number|null
    function find_first(str,txt):
        return __system__str__find_first__(str,txt);
    end 

    // str:string ,txt:string ,cnt:number -> array|null
    function find_N(str,txt,cnt):
        return __system__str__find_N__(str,txt,cnt);
    end 

    // str:string ,txt:string -> array|null
    function find_not_all(str,txt):
        return __system__str__find_not_all__(str,txt);
    end 

    // str:string ,txt:string -> array|null
    function find_not_first(str,txt):
        return __system__str__find_not_first__(str,txt);
    end 

    // str:string ,txt:string ,cnt:number -> array|null
    function find_not_N(str,txt,cnt):
        return __system__str__find_not_N__(str,txt,cnt);
    end 

    // str:string ,txt:string -> number|null
    function rfind_first(str,txt):
        return __system__str__rfind_first__(str,txt);
    end 

    // str:string ,txt:string ,cnt:number -> array|null
    function rfind_N(str,txt,cnt):
        return __system__str__rfind_N__(str,txt,cnt);
    end 

    // str:string ,txt:string -> array|null
    function rfind_not_first(str,txt):
        return __system__str__rfind_not_first__(str,txt);
    end 

    // str:string ,txt:string ,cnt:number -> array|null
    function rfind_not_N(str,txt,cnt):
        return __system__str__rfind_not_N__(str,txt,cnt);
    end 


    // str|string -> boolean;
	function isAlpha(str):
		return __system__str__isAlpha__(str); 
	end

	// str|string -> boolean;
	function isAlnum(str):
		return __system__str__isAlnum__(str);
	end

	// str|string -> boolean;
	function hasBlank(str):
		return __system__str__hasBlank__(str); 
	end

	// str|string -> boolean;
	function isBlank(str):
		return __system__str__isBlank__(str); 
	end

	// str|string -> boolean;
	function hasCntrl(str):
		return __system__str__hasCntrl__(str); 
	end

	// str|string -> boolean;
	function isCntrl(str):
		return __system__str__isCntrl__(str);
	end

	// str|string -> boolean;
	function isLower(str):
		return __system__str__isLower__(str); 
	end

	// str|string -> boolean;
	function isUpper(str):
		return __system__str__isUpper__(str); 
	end

    let str_all = -1;
}