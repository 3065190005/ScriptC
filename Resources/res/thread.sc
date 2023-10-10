require ("system");

interface StdThread{
	// path:string -> boolean|null
	function create(path):
		return __system__thread__thread_create__(path);
	end

	// id|number , name|string , param|(number,string,boolean) -> boolean|null 
	function set(id,name,param):
		return __system__thread__thread_set__(id,name,param);
	end

	// id|number , name|string -> boolean|null 
	function get(id,name):
		return __system__thread__thread_get__(id,name);
	end

	// id|number -> boolean|null 
	function run(id):
		return __system__thread__thread_run__(id);
	end

	// id|number -> boolean|null 
	function state(id):
		return __system__thread__thread_state__(id);
	end

	// id|number -> boolean|null 
	function wait(id):
		return __system__thread__thread_wait__(id);
	end

	// id|number -> boolean|null 
	function stop(id):
		return __system__thread__thread_stop__(id);
	end	

	// id|number -> boolean|null 
	function resume(id):
		return __system__thread__thread_resume__(id);
	end

	// id|number -> boolean|null 
	function kill(id):
		return __system__thread__thread_kill__(id);
	end		

	// id|number -> boolean|null 
	function join(id):
		return __system__thread__thread_join__(id);
	end

	// id|number -> boolean|null 
	function detach(id):
		return __system__thread__thread_detach__(id);
	end

	// id|number -> boolean|null 
	function clear(id):
		return __system__thread__thread_clear__(id);
	end


	let s_unknow = 0;
	let s_start = 2;
	let s_runing = 3;
	let s_wait = 4;
	let s_stop = 5;
}