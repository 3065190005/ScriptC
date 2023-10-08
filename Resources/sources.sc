let result = 123;
let abcdef = 456;

result = abcdef % result;

function getNumB():
	return 2.5;
end

require "io";

let io = new StdIo;

interface Class {
	let num = 114514.1919810;
	let str = "Hello double world";

	// 特殊方法 (析构函数)
	function _gc():
		io.print("\n gc called " + this.str + "\n");
	end

	function getNum():
		return 1.63;
	end
	function getNumB():
		return "Hello Class getFuncB()\n";
	end

	function PrintNum():
		io.print(this.num);
	end
}

function getNum():
	return 1; 
end 

function test() : 
	return 0;
end

let str = "Hello World!";
let assignValue = 123;
let space = test();
let array = getNum();
array[0] = [0,1,1,2,3,0,7];


let cls = new Class;
cls.num[0] = !cls.getNum();

if (str == "Hello World") :
	cls.num[0] = 1;
elif (assignValue == 456) :
	cls.num[0] = 2;
else:
	cls.num[0] = 3;
end


let whileIndex = 0;
while (whileIndex < 10):
	cls.num[0] = cls.num[0] * 2;
	whileIndex = whileIndex + 1;
	if(whileIndex < 5):
		continue;
	elif(whileIndex >= 5):
		break;
	end
end

let forResu = 0;
whileIndex = [0,1,2,3,4,5,6,7,8,9];
for i in whileIndex:
	forResu = forResu + i;
end


let maps = 0;
maps[0] = "*********\n";
maps[1] = "****x****\n";
maps[2] = "*********\n";

io.print(maps);

cls.num = 11111;
cls.str = "Hello World";
io.print(cls.PrintNum()<cls>);

// 这是注释

return forResu;