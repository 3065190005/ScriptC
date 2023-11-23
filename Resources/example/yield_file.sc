require("io");

function requireFunc():
	io.println("require Func Call");
	let value = co_yield("require Func co_yield");
	io.println("require Func co_yield " + value);
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