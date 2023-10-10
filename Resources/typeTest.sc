	require ("io");
	function log(val):
		let io = new StdIo;
		io.println(val);
	end

	require ("type");
	let tp = new StdType;
	
	log(tp.isAlpha("abcAAfg"));
	log(tp.isAlnum("123456"));
	log(tp.hasBlank("123 455"));
	log(tp.isBlank("    "));
	log(tp.hasCntrl("123\n"));
	log(tp.isCntrl("\t\n"));
	log(tp.isLower("abcefg"));
	log(tp.isUpper("AAFEGSD"));

	return 0;