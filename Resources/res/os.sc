require "system";

interface StdOs{
	// abort (void)
	function abort():
		return __system__os__abort__();
	end

	// exit code:number
	function exit(code):
		return __system__os__exit__(code);
	end

	// str value:any
	function str(value):
		return __system__os__str__(value);
	end

	// number value:any
	function number(value):
		return __system__os__number__(value);
	end

	// bool value:any
	function bool(value):
		return __system__os__bool__(value);
	end

	// type value:any
	function type(value):
		return __system__os__type__(value);
	end

	// hex value:string
	function hex(value):
		return __system__os__hex__(value);
	end

	// oct value:string
	function oct(value):
		return __system__os__oct__(value);
	end

	// bin value:string
	function bin(value):
		return __system__os__bin__(value);
	end

	// getenv env:string
	function getenv(env):
		return __system__os__getenv__(env);
	end

	// putenv env:string txt:string
	function putenv(env , txt):
		return __system__os__putenv__(env,txt);
	end

	// system code:string
	function system(code):
		return __system__os__system__(code);
	end

	// sleep code:number
	function sleep(code):
		return __system__os__sleep__(code);
	end

	// argv (void)
	function argv():
		return __system__os__argv__();
	end
}