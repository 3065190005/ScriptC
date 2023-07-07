include "system";

interface StdWindow{
	// (void) -> boolean;
	function hideControl():
		return __system__fltk_window__hideControl__(); 
	end

	// (void) -> boolean;
	function showControl():
		return __system__fltk_window__showControl__();
	end

	// title|str , txt|str, type|number -> boolean|number;
	function msgBox(title,txt,type):
		return __system__fltk_window__msgBox__(title,txt,type);
	end

	// title|str , txt|str -> boolean|str;
	function editBox(title,txt):
		return __system__fltk_window__editBox__(title,txt);
	end


	// (void) -> boolean|str;
	function bowserBox():
		return __system__fltk_window__bowserBox__();
	end

	// title|str , html|str -> boolean;
	function htmlBox(title,html):
		return __system__fltk_window__htmlBox__(title,html);
	end

	// title|str , url|str -> boolean;
	function urlBox(title,url):
		return __system__fltk_window__urlBox__(title,url);
	end

	// title|str , html|str -> boolean;
	function htmlView(title,html):
		return __system__fltk_window__htmlView__(title,html);
	end

	// title|str , url|str -> boolean;
	function urlView(title,url):
		return __system__fltk_window__urlView__(title,url);
	end

	//----------
	// (void) -> array;
	function getWebBoxSize():
		return __system__fltk_window__getWebBoxSize__();
	end

	// x|number, y|number, width|number, height|number -> boolean;
	function setWebBoxSize(x,y,width,height):
		return __system__fltk_window__setWebBoxSize__(x,y,width,height);
	end

	// (void) -> number;
	function getWebBoxStyle():
		return __system__fltk_window__getWebBoxStyle__();
	end

	// style|number -> boolean;
	function setWebBoxStyle(style):
		return __system__fltk_window__setWebBoxStyle__(style);
	end

	let none = 0;
	let title = 12582912;
	let menu = 13107200;
	let menupro = 13303808;
	let normal = 13565952;
}