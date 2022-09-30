include "system";

interface StdTime{
	// (void) -> number|null
	function time():
	 	return __system__time__time__();
	end

	// (void) -> number|null
	function timeAsM():
	 	return __system__time__timeAsM__();
	end

	// (void) -> number|null
	function clock():
	 	return __system__time__clock__();
	end

	// (void) -> number|null
	function clockAsM():
	 	return __system__time__clockAsM__();
	end

	// sec:number, typ:number -> number|null
	function cast(sec,typ):
	 	return __system__time__cast__(sec,typ);
	end

	// millisec:number, typ:number -> number|null
	function castAsM(millisec,typ):
	 	return __system__time__castAsM__(millisec,typ);
	end

	// (void) -> number|null
	function join():
	 	return __system__time__join__();
	end

	// id:number -> number|null
	function start(id):
	 	return __system__time__start__(id);
	end

	// id:number -> number|null
	function pause(id):
	 	return __system__time__pause__(id);
	end

	// id:number -> number|null
	function over(id):
	 	return __system__time__over__(id);
	end

	// id:number -> number|null
	function overAsM(id):
	 	return __system__time__overAsM__(id);
	end

	// (void) -> string|null
	function date():
	 	return __system__time__date__();
	end

	// (void) -> string|null
	function dateAsM():
	 	return __system__time__dateAsM__();
	end

	function toDate(sec):
	 	return __system__time__toDate__(sec);
	end

	function toDateAsM(millisec):
	 	return __system__time__toDateAsM__(millisec);
	end

	let year = 6;
	let month = 5;
	let day = 4;
	let hour = 3;
	let minute = 2;
	let min = 2;
	let second = 1;
	let sec = 1;
	let millisecond = 0;
	let msec = 0;
	let ms = 0;
}