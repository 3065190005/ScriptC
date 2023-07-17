require "system";

interface StdType{
	// str|string -> boolean;
	function isAlpha(str):
		return __system__type__isAlpha__(str); 
	end

	// str|string -> boolean;
	function isAlnum(str):
		return __system__type__isAlnum__(str);
	end

	// str|string -> boolean;
	function hasBlank(str):
		return __system__type__hasBlank__(str); 
	end

	// str|string -> boolean;
	function isBlank(str):
		return __system__type__isBlank__(str); 
	end

	// str|string -> boolean;
	function hasCntrl(str):
		return __system__type__hasCntrl__(str); 
	end

	// str|string -> boolean;
	function isCntrl(str):
		return __system__type__isCntrl__(str);
	end

	// str|string -> boolean;
	function isLower(str):
		return __system__type__isLower__(str); 
	end

	// str|string -> boolean;
	function isUpper(str):
		return __system__type__isUpper__(str); 
	end
}