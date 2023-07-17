require "io";
let io = new StdIo;

require "string";
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

return;