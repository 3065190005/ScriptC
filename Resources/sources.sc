let result = 123;
let abcdef = 456;

result = abcdef % result;

function getNumB():
	return 2.5;
end

require ("io");

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
		io.println(this.num);
	end

	function subNum():
		this.num = "This Change!";
	end
}

interface SubClass override Class{
	let num = 1000;
	
	function subNum():
		this.num = "Sub Class Overide This Change!";
	end

	function _init(value):
		this.num = value;
	end

	function _attr(info, name):
		for i in info:
			io.println(i);
		end

		if (info.type == "function"):
			for i in name:
				io.println(i);
			end
		else:
			io.println(name);
		end

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
cls.PrintNum()<cls>;
cls.subNum();
cls.PrintNum()<cls>;
io.println("\n\n --- sub class called ---\n");

let subcls = new SubClass;
subcls.PrintNum()<subcls>;
subcls.subNum();
subcls.PrintNum()<subcls>;

io.println("\n\n --- sub class init ---\n");
let init_cls = new SubClass("init change !");
io.println(init_cls.num);

io.println("\n\n --- sub class attr --- \n");

init_cls.getAAA("Abc");
let space_s = init_cls.numbert;
init_cls.getAAA("CCC") = "value";
init_cls.numbert = "numberT value";

// 这是注释
io.println("program over");


io.println("\n\n --- function Yield Test --- \n");

function yieldFunc(param):
	io.println("yieldFunc " + param);
	param = yield(param + " yield");
	io.println(param);
	return param + " return";
end

let yield_value = yieldFunc("This is Param");
io.println(yield_value[1]);
yield_value = resume(yield_value[0], yield_value[1] + " resume");
io.println(yield_value);


io.println("\n\n --- function Yield this Test --- \n");

function yieldFuncThis():
	io.println("yieldFuncThis " + this);
	this = "yieldFuncThis" + this;
	this = yield(this + " yield");
	io.println(this);
	return this + " return";
end

let this_value = "This is this Value";
let yield_value_this = yieldFuncThis()<this_value>;
io.println(yield_value_this[1]);
this_value = resume(yield_value_this[0], yield_value_this[1] + " resume");
io.println(this_value);


io.println("\n\n --- interface Yield Test --- \n");
interface YieldClass{
	let str = "Default";
	let child = "Child";
	function yieldFunc():
		let value = "yeildFunc call";
		this.str = this.str + " Yield";

		io.println(value);
		io.println(this.str);

		value = yield(value + " Yield");

		io.println(value);
		io.println(this.str);

		return value + " Return";
	end
}

let yield_cls = new YieldClass;
let value = yield_cls.yieldFunc();
io.println(value[1]);
io.println(yield_cls.str);

yield_cls.str = yield_cls.str + " Resume";
value = resume(value[0], value[1] + "Resume");

io.println(value);

io.println("\n\n --- Right interface Yield Test --- \n");
function getRight():
	return new YieldClass;
end

value = getRight().yieldFunc();
io.println(value[1]);
value = resume(value[0], value[1] + "Resume");
io.println(value);

io.println("\n\n --- Yield Sell Buy Apples --- \n");

function sell(max):
	let count = yield(max);
	while(count >= 0):
		io.println("Sell Apple: " + count);
		count = yield(count);
	end
end

function buy(max):
	let count = sell(max);
	while(count[1] >= 0):
		count = resume(count[0], count[1]);
		io.println("Buy Apple: " + count[1]);
		count[1] = count[1] - 1;
	end
end

let person = buy(5);


return forResu;