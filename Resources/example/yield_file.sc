require("io");

function requireFunc():
	io.println("require Func Call");
	let value = yield("require Func Yield");
	io.println("require Func Yield " + value);
	return "requireFunc Return";
end

let info = requireFunc();

interface RequireGcTest{
	function _gc():
		io.println("Require Gc Called");
	end
}

if(true):
	let testGc = new RequireGcTest;
	return;
end