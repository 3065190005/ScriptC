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
}