require ("io");
let io = new StdIo;

require ("string");
let string = new StdString;

let str = "  Hello World : Begin {name} over  ";
str = string.tirm(str);
str = string.replace(str,"{name}","ZhangSan",string.str_all);
let cutArr = string.cut(str,":");
for i in cutArr:
io.println(string.tirm(i));
end

let revertStr = string.reverse(str);
io.println(revertStr);

let atStr = string.at(str,0) + string.at(str,1);
io.println(atStr);


function log(val):
	io.println(val);
end
	
	log(string.isAlpha("abcAAfg"));
	log(string.isAlnum("123456"));
	log(string.hasBlank("123 455"));
	log(string.isBlank("    "));
	log(string.hasCntrl("123\n"));
	log(string.isCntrl("\t\n"));
	log(string.isLower("abcefg"));
	log(string.isUpper("AAFEGSD"));

return;