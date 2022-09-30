include "io";
	include "os";
	let os = new StdOs;
	let io = new StdIo;

	function log(str):
		os.system("echo " + str);
	end

	function logv(str):
		io.println(str);
	end
	
	log("arg getting");
	let arg = os.argv();
	logv(arg);
	
	log("putenv");
	let penv = os.putenv("Path","HelloWorld");
	logv(penv);

	log("getenv");
	let genv = os.getenv("Path");
	logv(genv);

	log("hex");
	let hexV = os.hex("0xFFFF");
	logv(hexV);

	log("oct");
	let octV = os.oct("0o7777");
	logv(octV);

	log("bin");
	let binV = os.bin("0b1111");
	logv(binV);

	log("type");
	let typeV = os.type(os) + " " + os.type(octV) + " " + os.type(arg);
	logv(typeV);

	log("static cast bool string number");
	let boolV = os.bool("true");
	logv(boolV);

	let numberV = os.number("999");
	logv(numberV);

	let stringV = os.str(numberV);
	logv(stringV);

	if(1 == 2):
		log("exit 1");
		os.exit(1);
	elif(2 == 3):
		log("abort");
		os.abort();
	end