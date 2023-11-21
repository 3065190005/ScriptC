require ("extern_system\\system_dialog");

interface StdDialog{
	// (void) -> boolean;
	function hideConsole():
		return __system__dialog__hideConsole__(); 
	end

	// (void) -> boolean;
	function showConsole():
		return __system__dialog__showConsole__();
	end

	// title|str , txt|str, type|number -> boolean|number;
	function msgBox(title,txt,type):
		return __system__dialog__msgBox__(title,txt,type);
	end

	// title|str , txt|str -> boolean|str;
	function editBox(title,txt):
		return __system__dialog__editBox__(title,txt);
	end


	// (void) -> boolean|str;
	function bowserBox():
		return __system__dialog__bowserBox__();
	end

	// title|str , html|str -> boolean;
	function ieHtml(title,html):
		return __system__dialog__ieHtml__(title,html);
	end

	// title|str , url|str -> boolean;
	function ieUrl(title,url):
		return __system__dialog__ieUrl__(title,url);
	end

	// title|str , html|str -> boolean;
	function edgeHtml(title,html):
		return __system__dialog__edgeHtml__(title,html);
	end

	// title|str , url|str -> boolean;
	function edgeUrl(title,url):
		return __system__dialog__edgeUrl__(title,url);
	end

	//----------
	// (void) -> array;
	function getWebBoxSize():
		return __system__dialog__getWebBoxSize__();
	end

	// x|number, y|number, width|number, height|number -> boolean;
	function setWebBoxSize(x,y,width,height):
		return __system__dialog__setWebBoxSize__(x,y,width,height);
	end

	// (void) -> number;
	function getWebBoxStyle():
		return __system__dialog__getWebBoxStyle__();
	end

	// style|number -> boolean;
	function setWebBoxStyle(style):
		return __system__dialog__setWebBoxStyle__(style);
	end

	let none = 0;
	let title = 12582912;
	let menu = 13107200;
	let menupro = 13303808;
	let normal = 13565952;
}