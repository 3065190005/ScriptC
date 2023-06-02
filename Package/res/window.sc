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
}