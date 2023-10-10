require ("system");

interface StdIo{

	// base print str:any
	function print(str):
		__system__io__print__(str);
	end

	// print + enter str:any
	function println(str):
		__system__io__print__(str);
		__system__io__print__("\n");
	end

	// base input (void)
	function input():
		let ret = 0;
		ret = __system__io__input__();
		return ret;
	end

	// file exist file:number
	function fexist(file):
		let ret = 0;
		ret = __system__io__fexist__(file);
		return ret;
	end

	// file create file:string
	function fcreate(file):
		let ret = 0;
		ret = __system__io__fcreate__(file);
		return ret;
	end

	// file remove file:string
	function fremove(file):
		let ret = 0;
		ret = __system__io__fremove__(file);
		return ret;
	end

	// file rename source:string,target:string
	function frename(source,target):
		let ret = 0;
		ret = __system__io__frename__(source,target);
		return ret;
	end

	// create temp file
	function tmpfile():
		let ret = 0;
		ret = __system__io__tmpfile__();
		return ret;
	end

	// get error string
	function errget():
		let ret = 0;
		ret = __system__io__errget__();
		return ret;
	end

	// set error string str:string
	function errset(str):
		__system__io__errset__(str);
	end

	// file seek id:number , offset:number, origin:number
	function fseek(id,offset,origin):
		return __system__io__fseek__(id,offset,origin);
	end

	// file tell id:number
	function ftell(id):
		return __system__io__ftell__(id);
	end

	// file read id:number,lens:number
	function fread(id,lens):
		return __system__io__fread__(id,lens);
	end

	// file write id:number,str:string,lens:number
	function fwrite(id,str,lens):
		return __system__io__fwrite__(id,str,lens);
	end

	// file close id:number
	function fclose(id):
		return __system__io__fclose__(id);
	end

	// file fopen file:string,mode:string
	function fopen(file,mode):
		return __system__io__fopen__(file,mode);
	end


	let seek_beg = 0;
	let seek_cur = 1;
	let seek_end = 2;

	let read_all = -2;
	let read_line = -1;

	let write_all = -1;
}