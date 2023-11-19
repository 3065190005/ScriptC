

# ScriptC

## 基础介绍

脚本语言：ScriptC是一门脚本语言，用来嵌入C++或单独存在
可扩展性：非常通用且简单的扩展机制接口，由C++提供这些功能，ScriptC可以使用他们
其他特性：
	面向过程编程和函数编程
	弱类型，和js一样，ScriptC是一门弱类型语言
	数组：通过数组，ScriptC可以实现哈希表，集合，数组
	接口：Interface关键字可以很方便的定义各种数据结构


## 第一个ScriptC程序


### 环境变量
**将根目录添加到ScriptC系统环境变量中**

### HelloWorld
```sc
require ("io");
let io = new StdIo;
io.println("Hello World");
```

运行后会在屏幕上输出 “Hello World!”


## 基本语法


### 脚本式编程
我们可以将ScriptC程序保存到以sc结尾的文件并执行。

```sc
require ("io");
let io = new StdIo;
io.println("Hello World!");
io.println("Hello ScriptC!");
```

将上面代码保存到sc文件并执行

```console
Hello World!
Hello ScriptC!
```


### 注释
ScriptC中只有 单行注释，可以通过使用**两个斜杠**进行单行注释
```sc
// this well be not compile
```


### 标识符
标识符用于定义用户自己的变量函数或接口。
标识符可以由 字母 a 到 z 或 A 到 Z 或下划线 _ 开头 后面加上任意0个或多个字母下划线或数字(0~9)
下面列出了合法标识符
```sc
mohd         zara      aBc     move_name    a_123
myname50     _temp     Jsbd       a23b9        retVal
```


### 关键字

ScriptC一共有23个关键字

|  	 	   |       |           |        |         |
| -------- | ----- | :-------: | ---- | ------- |
| continue | break |    else   | export | return |
| elif     | end   |   false   | for  | true    |
| function | if    |    in     | let  | while   |
| null     | undef | interface | new  | require |
| override     | yield | resume |   |  |


### 特殊变量

|  	特殊变量名 	   |    说明   |
| -------- | ----- |
| this | 调用函数时通过捕获符\<\>修改this与变量的绑定关系 |
| \_\_FILE\_NAME\_\_ | 当前正在运行的文件名 |
| \_\_MAIN\_NAME\_\_ | 首个运行的文件名 |
| _class_name | 变量的接口名 |
| _class_parent | 变量口的父接口名 |
| _class_auto_gc_call | 变量是否自动调用特殊函数_gc |


## 数据类型
null 			  未声明存储空间 （当需要删除变量时只需要赋值为null即可
undef 			有存储空间但是没有值	( 当需要删除值时值需要赋值为undef即可
boolean			布尔变量包含两个值 true或false
number			数字类型 浮点或整形
string			由一对双引号来表示字符串类型
array			由中括号包裹起来的数据
interface		 在ScriptC中interface是一个可以用来声明各种变量和函数的集合体

*ScriptC中**string**是**基本数据类型**，无法通过下标符号**[]**获取单个字符
如要获取字符串中某个下标的字符可以使用**at**函数，详细请看**StdString 字符串处理库***

**实例**

```sc
let result1 = null;					// 未声明空间
let result2 = undef;				// 声明空间但值不可用或没有值
let result3 = true;					// 布尔类型true
let result4 = 123456;				// 数字类型 123456
let result5 = "Hello ScriptC"; 		// 字符串类型 "Hello ScriptC"
let result6 = [1,"Joy",true];		// 数组类型，其内容为 数字(1),字符串(Joy),布尔(true)

interface struct{
	let number = 0;
}

let result 7 = new struct;			// 接口类型，其内部具有成员变量number
```


## 作用域
```sc
// main.sc
let value = 0;	// 声明全局变量

function func():
	let value = 1; // 声明局部变量
	value = value + 1;	// 使用局部变量
end

function func2():
	value = value + 1;	// 使用全局变量
end

func2();	// value = 1
value = value + 1; // 使用全局变量 value = 2
```

## 运算符

### 算术运算符
**下表显示了 ScriptC 语言支持的所有算术运算符。假设变量 A 的值为 10，变量 B 的值为 20，则**  

| 运算符 | 描述 | 实例 |
| --- | --- | --- |
| + | 把两个操作数相加 | A + B 将得到 30 |
| \- | 从第一个操作数中减去第二个操作数 | A - B 将得到 -10 |
| \* | 把两个操作数相乘 | A \* B 将得到 200 |
| / | 分子除以分母 | B / A 将得到 2 |
| % | 取模运算符，整除后的余数 | B % A 将得到 0 |
### 关系运算符

**下表显示了 ScriptC支持的所有关系运算符。假设变量  A  的值为 10，变量 B 的值为 20，则**  

| 运算符 | 描述 | 实例 |
| --- | --- | --- |
| \== | 检查两个操作数的值是否相等，如果相等则条件为真。 | (A == B) 为假。 |
| != | 检查两个操作数的值是否相等，如果不相等则条件为真。 | (A != B) 为真。 |
| \> | 检查左操作数的值是否大于右操作数的值，如果是则条件为真。 | (A > B) 为假。 |
| < | 检查左操作数的值是否小于右操作数的值，如果是则条件为真。 | (A < B) 为真。 |
| \>= | 检查左操作数的值是否大于或等于右操作数的值，如果是则条件为真。 | (A >= B) 为假。 |
| <= | 检查左操作数的值是否小于或等于右操作数的值，如果是则条件为真。 | (A <= B) 为真。 |


### 逻辑运算符

**下表显示了ScriptC支持的所有关系逻辑运算符。假设变量 A 的值为 1，变量 B 的值为 0，则**  

| 运算符 | 描述 | 实例 |
| --- | --- | --- |
| && | 称为逻辑与运算符。如果两个操作数都非零，则条件为真。 | (A && B) 为假。 |
| || | 称为逻辑或运算符。如果两个操作数中有任意一个非零，则条件为真。 | (A || B) 为真。 |
| ! | 称为逻辑非运算符。用来逆转操作数的逻辑状态。如果条件为真则逻辑非运算符将使其为假。 | !(A && B) 为真。 |


### 位运算符
**bit相关操作 会强制截断将浮点转换成8字节的整形进行运算**

| 运算符 | 描述 | 实例 |
| --- | --- | --- |
| & | 
按位与操作，按二进制位进行"与"运算。运算规则：

0&0\=0;   
0&1\=0;    
1&0\=0;     
1&1\=1;

 | (A & B) 将得到 12，即为 0000 1100 |
| | | 

按位或运算符，按二进制位进行"或"运算。运算规则：

0|0\=0;   
0|1\=1;   
1|0\=1;    
1|1\=1;

 | (A | B) 将得到 61，即为 0011 1101 |
| ^ | 

异或运算符，按二进制位进行"异或"运算。运算规则：

0^0\=0;   
0^1\=1;   
1^0\=1;  
1^1\=0;

 | (A ^ B) 将得到 49，即为 0011 0001 |
| ~ | 

取反运算符，按二进制位进行"取反"运算。运算规则：

~1\=-2;   
~0\=-1;

 | (~A ) 将得到 -61，即为 1100 0011，一个有符号二进制数的补码形式。 |
| << | 二进制左移运算符。将一个运算对象的各二进制位全部左移若干位（左边的二进制位丢弃，右边补0）。 | A << 2 将得到 240，即为 1111 0000 |
| \>> | 二进制右移运算符。将一个数的各二进制位全部右移若干位，正数左补0，负数左补1，右边丢弃。 | A >> 2 将得到 15，即为 0000 1111 |


### 赋值运算符

| 运算符 | 描述 | 实例 |
| --- | --- | --- |
| \= | 简单的赋值运算符，把右边操作数的值赋给左边操作数 | C = A + B 将把 A + B 的值赋给 C |


### 杂项运算符

| 运算符 | 描述 | 实例 |
| --- | --- | --- |
| . | 获取接口的变量或函数 | a.value 获取a接口的value成员 , a.func() 调用a接口 的func函数 |
| <> | 捕获符 | func()<value> 修改func函数运行时的this与value变量绑定 |



### 运算符优先级

| 类别  | 运算符  | 结合性  |
| --- | --- | --- |
| 后缀  | () \[\] ; . <>   | 从左到右  |
| 一元  | \+ - ! ~ | 从右到左  |
| 乘除  | \* / %  | 从左到右  |
| 加减  | \+ -  | 从左到右  |
| 移位  | << >>  | 从左到右  |
| 关系  | < <= > >=  | 从左到右  |
| 相等  | \== !=  | 从左到右  |
| 位与 AND  | &  | 从左到右  |
| 位异或 XOR  | ^  | 从左到右  |
| 位或 OR  | |  | 从左到右  |
| 逻辑与 AND  | &&  | 从左到右  |
| 逻辑或 OR  | ||  | 从左到右  |
| 赋值  | \=   | 从右到左  |
| 逗号  | ,  | 从左到右  |


## 判断

***判断语句运算符***
| 语句 | 描述 |
| --- | --- |
| if...end 语句 | 一个 **if 语句** 由一个布尔表达式后跟一个或多个语句组成。 |
| if...elif...else...end 语句 | 一个 **if 语句** 后可跟任意多个可选的 **elif 语句**，else 语句在前面所有布尔表达式为假时执行。 |
| 嵌套 if 语句 | 您可以在一个 **if** 或 **elif 和 else** 语句内使用另一个 **if elif else** 语句。 |


***实例***
```sc
let value = 0;

if (value == 1) :
	value = "if";
elif (value == 2):
	value = "elif";
else:
	value = "else";
end
```



## 循环

### 循环类型
**ScriptC提供了两种循环（条件循环和变量循环）**  
| 循环类型 | 描述 |
| --- | --- |
| while...end 循环 | 当给定条件为真时，重复语句或语句组。它会在执行循环主体之前测试条件。 |
| for...in...end 循环 | 对数组变量进行从头到尾的变量获取循环 |
| 嵌套循环 | 您可以在 while 或 for 循环内使用一个或多个循环。 |


### 循环控循环控制语句制语句
| 控制语句 | 描述 |
| --- | --- |
| break 语句 | 终止**循环**或 **switch** 语句，程序流将继续执行紧接着循环或 switch 的下一条语句。 |
| continue 语句 | 告诉一个循环体立刻停止本次循环迭代，重新开始下次循环迭代。 |


**实例**

```sc
let count = 0;
let index = 0;

// 当index 小于10时进行循环
while(index < 10):
	count = count + index;
	index = index + 1;
end


count = 0;
index = [0,1,2,3,4,5];

// 循环 index 内的所有元素 并赋值给i
for i in index:
	count = count + i;
end
```



## 函数

### 定义函数
**函数形式如下**
```sc
function func_name( parameter list ):
	body of the function
end
```

在 ScriptC 语言中，函数由一个函数头和一个函数主体组成。下面列出一个函数的所有组成部分：
+ function		 函数声明关键字
+ func_name		 符合命名规则的函数名
+ parameter list	 符合命名规则的参数，不同参数之间使用逗号(,)分割
+ 函数主体		 函数具体要执行的代码
+ end		函数声明结束关键字


**实例**
```sc

// 函数声明
function add(x,y):
	return x + y;
end

// 函数调用
let result = add(1,2); // 结果等于3

```

## 接口
ScriptC允许通过使用关键字interface将不同变量和函数进行包装来生成自定义数据类型

### 简单的定义和使用
**接口形式如下**
```sc
interface interface_name{
	interface body
}
```

+ interface 		接口声明关键字
+ interface_name 	符合命名规则的接口名
+ 接口内部成员	  接口内部成员只允许存在函数和变量

**实例**
```sc

// 定义 struct接口
interface struct{
	let number = 0;
	
	function add(x,y):
		return x + y;
	end
}

// 使用 struct接口
// 通过关键字 new + 接口名 来实例化接口
let value = new struct;
value.number = value.add(1,2);		// value.number == 3;
```

### 多接口嵌套
接口内部只能声明变量和函数，但是对于变量的赋值没有限制
```sc
interface structA{
	let number = 1;
	
	function Afunc():
		return 1 + 1;
	end
}

interface structB{
	let number = 2;
	let struct = new structA;
	
	function Bfunc():
		return 2 + 2;
	end
}

let value = new structB;
value.struct.number = 123;				// 赋值123给structA的成员 number
value.number = value.struct.Afunc();	// 调用 structA的Afunc方法 并赋值给StructB的成员number
```


### 接口重写
接口可以通过 override 关键字进行重写
```sc
interface Base{
	let age = 18;
	let sex = "boy";
	
	function fakeAge():
		return this.age + 1;
	end
	
	function realAge():
		return this.age;
	end
}

interface Child override Base{
	let age = 19;
	
	function fakeAge():
		return this.age * 2;
	end
}

let person = new Child;
let age = person.age; 				// age = 19
let fake_age = person.fakeAge(); 	// fake_age = 38
let real_age = person.realAge();	// real_age = 19
let sex = person.sex;				// sex = "boy"
```


### 特殊函数 
### gc
特殊函数 gc，可以在接口变量被销毁前自动调用
**该函数只会在变量被局部变量管理时才会调用，成员则不会进行调用**
**this指针指向当前将要被销毁的变量**
**_析构函数不允许使用return返回值_**
**该方法禁止手动调用**

```sc
	require ("io");
	let io = new StdIo;

	interface structA{
		let number = 1;

		// 特殊函数
		function _gc():
			this.number = 3;
			io.print(this.number + " Gc");
		end
	}

	let value = new structA;
	// 结束时会自动调用 structA接口的_gc函数 并输出 "3 Gc"
```
### init
当使用new关键字通过传参语法实例化一个接口时会调用特殊函数 init
**this指针指向当前实例化对象**
_**初始化函数不允许使用return返回值**_
**该方法禁止手动调用**

```sc
	interface Base{
		let name = null;
		let age = null;

		// 特殊函数
		function _init(name,age):
			this.name = name;
			this.age = age;
		end
	}
	
	interface Baby{
		// 特殊函数
		function _init(name):
			this.name = name;
			this.age = 1;
		end
	}

	interface Dancer override Base{
		function onDance():
				// ... something
		end
	}

	interface Singer{
		function Func():
		end
	}

	let baby = new Baby("Tom"); // 调用 Baby:_init(name = "tom")
	let base = new Base("Bob",27); // 调用 Base:_init(name = "Bob", age = 27)
	let dancer = new Dancer("Jane", 30); // 调用 Base:_init(name = "Jane", age = 30)
	// let error_baby = new Baby("Tom",27); // error ，Base的初始化函数已被覆盖
	let person = new Baby; // 不会调用初始化函数
	// let singer = new Singer(); // 报错 Singer没有实现初始化函数
	let singer = new Singer; // 实例化Singer接口
```

### attr
当调用一个不存在的方法或成员时会自动拦截并调用特殊方法attr
**this指针指向当前调用的对象**
_**拦截函数允许使用return返回值**_
**该方法允许手动调用，但** _**不推荐**_ 

```sc
	interface Person{
		let name = "Bob";
		let age = 30;

		// 特殊函数
		function _attr(info,argv):
			
		end

		function setName(name):
			this.name = name;
		end		

		function getName():
			return this.name;
		end
	}

	let person = new Person;
	
	// 调用 Person:getName();
	let call_name = person.getName();
	
	// 返回Person:name - "Bob"
	let name = person.name;	 
	
	// 拦截调用 Person:_attr(info = ["name":"sex", "type":"member"], argv = null)
	let sex = person.sex; 

	// 拦截调用 Person:_attr(info = ["name":"getSex", "type":"function"], argv = null)
	let call_sex = person.getSex();
	
	// 拦截调用 Person:_attr(info = ["name":"setAge", "type" : "function"], argv = [0:17]);
	let call_age = person.setAge(17);
	
	// 拦截调用 Person:_attr(info = ["name":"getAge", "type" : "function"], argv = [0:"param", -1:"value"]);
	person.getInstance("param") = "value";

	// 拦截调用 Person:_attr(info = ["name":"isChild ", "type":"member"], argv = false);
	person.isChild = false;

```

**_不推荐_**
通过下标运算符可以禁止 attr 拦截不存在的成员变量
**需要注意：当使用下标运算符赋值后，则不会再拦截已赋值的成员**
```sc
	interface Person{
		function _attr(info,argv):
		end
	}
	
	let person = new Person;
	person["name"] = "Tom";  // person.name = "Tom"; 不会调用 _attr
	person.name = "Bob"; // 不会调用拦截函数 _attr;
	
```


### 特殊变量 this 和捕获符<>

接口可以通过捕获符**<>**来修改**函数**或**方法**内的**this**的绑定关系

```sc
interface structA{
	let number = 0;
	function setNumber(x):
		this.number = x;
	end
}

interface structB{
	function setNumber(x,y):
		this.number = x + y;
	end
}

let value1 = new structA;
let value2 = new structB;

let result = 0;
value1.setNumber(5)<value1>;	// value1.number = 5;
value2.setNumber(1,2)<value1>;	// value1.number = 3;
value1.setNumber(0);		// value1.number = 0;
// value1.setNumber(0)<result>; // error 绑定成功但result不是成员且内部没有成员变量number

```
**当绑定的变量没有相关接口成员或方法将会出现错误**
**当未手动绑定则会默认将this与调用变量进行绑定**


## 协程
通过使用yield和resume可以实现函数之间的切换

### yield关键字
```sc
yield(expr)
```
+  expr 任意合法表达式

yield关键字可以用来暂停当前函数执行并获取一个值，其中参数expr为任意表达式
当调用yield关键字会返回一个数组，其中元素[0]为返回的协程id，元素[1]为expr表达式的值
```sc
	// require ...
	
	function func(str):
		yield(str + " yield");
		return null;
	end
	
	let var = func("Hello World");
	io.println(var); // var = [0:<number>, 1:"Hello World yield"]
```

### resume关键字
```sc
	resume(id, expr)
```
+ id 要恢复的协程id
+ expr 任意合法表达式

resume 关键字可以用来恢复一个被暂停的函数并发送一个值，其中id为yield关键字返回协程id，expr为任意合法表达式

```sc
	//require ...
	function func(str):
		io.println(str);
		str = yield("Yield Str");
		io.println(str);
		return "Return Str";
	end
	
	let coroutine = func("Hello World");
	io.println(coroutine[1]);
	let resume_str = resume(coroutine[0], "Resume Str");
	io.println(resume_str);
```

输出如下
```sc
Hello World
Yield Str
Resume Str
Return Str
```

### 简单的协程使用示例
```sc
require("io");
let io = new StdIo;	

function sell(max):
	let count = yield(max);
	while(count > 0):
		io.println("Sell : " + count);
		count = yield(count);
	end
end

function buy(max):
	let count = sell(max);
	while(count[1] > 0):
		count = resume(count[0], count[1]);
		io.println("Buy : " + count[1]);
		count[1] = count[1] - 1;
	end
end
let person = buy(5);
```
下面是输出结果
```sc
Sell : 5
Buy : 5
Sell : 4
Buy : 4
Sell : 3
Buy : 3
Sell : 2
Buy : 2
Sell : 1
Buy : 1
```


### 注意事项
yield的使用和return基本一直，不可在部分函数中使用 如：特殊函数init或gc
yield必须在函数内部使用


## 头文件
### 引用头文件的语法  
```sc 
require (file_string);
```

+ require  头文件引入关键字
+ file_string  要引入的文件名字符串

### 引用头文件的操作  
头文件在引入时会将要引入的文件代码全部执行一遍  
如果文件被多次引用，将只会保留第一次引入代码，不执行之后的代码
```sc
require ("io");	// 引入io.sc 头文件标准库
require ("io");	// 无视，不执行
```

### 有条件引用  
头文件的执行是属于代码的一部分，可以配合条件语句进行选择性引入
```sc
let value = 1;
if(value == 1):
	require ("io");
else:
	require ("os");
end
```

**条件判断并不能防止头文件的引入，但可以阻止头文件的运行**

## 输入/输出

### 常用的标准输入输出函数  

**ScriptC可以通过引入标准库io.sc来调用标准输入和标准输出**
ScriptC一般使用print和input来获取标准输入输出，具体的使用如下

```sc
require ("io");
let io = new StdIo;
io.print("Hello World");
let string = io.input();
io.println(string);
```

**实例解析**  
+ require ("io")	引入标准输入输出头文件 io.sc
+ let io = new StdIo;	io.sc内部文件均在StdIo接口中实现
+ io.print("Hello World");	调用标准输出并输出字符串 "Hello World"
+ let string = io.input();		从键盘获取标准输入并赋值给string变量
+ io.println(string);		println是print的带换行符版本，print默认不带换行符

**所有标准库内部的函数和变量都将被一个接口所包含，如果要使用某个标准库函数需要先创建该接口实例**  


## 文件读取

**本章节内容的函数都存在于io.sc标准库中。**
### 打开文件

可以通过fopen来打开现有或新文件，这个函数会返回一个number类型的文件Id，之后使用返回的Id对该文件进行操作  
```sc
function fopen(filename , mode)
```
打开成功返回number类型的非0文件Id，否则返回false，该函数存在于io.sc

**filename**是需要打开的文件名，**mode**表示该文件的访问模式值可以是下列值中的一个  

| 模式 | 描述 |
| --- | --- |
| r | 打开一个已有的文本文件，允许读取文件。 |
| w | 打开一个文本文件，允许写入文件。如果文件不存在，则会创建一个新文件。在这里，您的程序会从文件的开头写入内容。如果文件存在，则该会被截断为零长度，重新写入。 |
| a | 打开一个文本文件，以追加模式写入文件。如果文件不存在，则会创建一个新文件。在这里，您的程序会在已有的文件内容中追加内容。 |
| r+ | 打开一个文本文件，允许读写文件。 |
| w+ | 打开一个文本文件，允许读写文件。如果文件已存在，则文件会被截断为零长度，如果文件不存在，则会创建一个新文件。 |
| a+ | 打开一个文本文件，允许读写文件。如果文件不存在，则会创建一个新文件。读取会从文件的开头开始，写入则只能是追加模式。 |

如果处理的是二进制文件，则需使用下面的访问模式来取代上面的访问模式  
``` "rb", "wb", "ab", "rb+", "r+b", "wb+", "w+b", "ab+", "a+b" ```

### 关闭文件  
为了关闭文件，需要使用fclose函数。  
``` sc 
fclose(fileId)
```

+ fileId 	通过fopen获取到的文件Id

成功关闭文件返回true，否则返回false。

### 写入文件
如果要写入文件，需要使用fwrite函数。
```sc 
fwrite(fileId , Txt , lens)
```

+ fileId 		通过fopen返回的文件Id
+ Txt	    	要写入的字符内容
+ lens		     要写入的长度
**lens**可以通过 **io.sc** 内部的**write_all**变量指定写入剩余所有内容。

### 读取文件  
为了读取文件，需要使用fread函数  
```sc
fread(fileId , lens);
```
+ fileId	通过fopen返回的文件Id
+ lens		读取的文件长度
+ 
**lens**可以通过**io.sc**内部的**read_all**来读取剩余所有内容，或使用**read_line**来读取剩余最近的一行内容。

### 实例
```sc
require ("io");
let io = new StdIo;

let fileId = io.fopen("student.txt","wb+");	// 使用wb+模式打开 student.txt文件
io.fwrite(fileId,"Hello World",io.wirte_all); // 写入"Hello World"所有内容到fileId文件内
let string = io.fread(fileId , io.read_all);  // 读取fileId的所有内容并赋值给string变量
io.fclose(fileId);				// 关闭fileId
```
更多细节请查看 **StdIo 标准输入/输出** 标准库

## 错误处理

**错误处理函数均在io.sc标准库中**
### 设置错误字符串  
在ScriptC中可以通过使用**errset**来设置错误字符串
```sc 
errset(errstring)
```

+ errstring 	需要设置的错误字符串
如果在设置前有其他错误字符串，重新调用将覆盖该缓冲区

### 获取错误字符串
如果要获取已设置的错误字符串可以调用**errget**函数
```sc 
errget()
```
如果有错误字符串，将返回字符串并将缓冲区设置为空，否则返回布尔值false

### 实例  
```sc
require ("io");
let io = new StdIo;

io.errset("ErrCode 1");
let err = errget(); // err = "ErrCode 1";
```
更多细节请查看 **StdIo 标准输入/输出** 标准库

## 命令行参数
**命令行参数是指在运行前通过控制台传给ScriptC的命令行参数**  

### 获取命令行参数
可以通过调用**os.sc**标准库中的**argv**函数来获取所有命令行参数  
以下情况默认传入参数为 ```"1" "2" "3"```
```sc
require ("os");
let os = new StdOs;

let args = os.argv();
```

+ require "os"			引入标准库**os.sc**
+ let os = new StdOs	**os.sc**标准库所有代码均在**StdOs**接口内
+ let args = os.argv()		获取所有命令行参数，并返回一个数组类型

更多细节请查看 **StdOs  操作系统** 标准库

# 标准库
## StdIo 标准输入/输出
```sc
require ("io");
let io = new StdIo;
```

标准输出 : 变量 -> (void)
print(value)	         value:any	  -> (void)

 标准输出 :变量 -> (void)
println(value)		value:any	-> (void)

标准输入 : (void) -> 标准输入返回值
input()			   (void)	-> value:string

文件是否存在		文件名 -> 	存在返回true 否则返回false
fexist(value)		value:string	-> boolean

文件创建		: 文件名 -> 创建返回true 否则返回false
fcreate(value)		value:string	->  boolean

fremove 文件删除	 文件名 -> 删除返回true 否则返回false
fremove(value)		value:string -> boolean

创建临时文件		: (void) -> 成功返回id 失败返回false
tmpfile()			(void)	-> number|boolean

设置错误号  : 		内容 ->	 (void)
errset(value)		value:string -> (void)

获取错误号  : 		(void) -> 有返回内容字符串，否则返回false
errget()			(void) -> string|boolean

移动文件位置 			 文件ID ,位置, 偏移 		-> 成功返回true 失败返回false
fseek(va1,va2,va3)		  va1:number , va2:number , va3 :number 	-> boolean

获取当前文件位置 		: 文件ID -> 成功返回位置 失败返回false
ftell(value)			value:number   ->  number|boolean

文件写入 :		文件ID 内容 大小(小于0 全部) -> 成功返回输入字节数 失败返回false
fwrite(va1,va2,va3)		val1:number , va2:string , va3:number -> nubmer|boolean

文件读取  		文件ID 大小(小于-2 剩余全部 等于-1 剩余最近行) -> 成功返回读取内容 失败返回false
fread(va1,va2)			va1:number , va2:number 	-> 	number|boolean

关闭文件 		: 文件ID -> 成功返回true 失败返回false
fclose(value)			value:number 	->   boolean

打开文件 : 文件名 打开方式 -> 成功返回文件Id 失败返回false
fopen(va1,va2)		va1:string , va2:string	-> 	number|boolean

**内置变量**  
```sc
let seek_beg = 0;
let seek_cur = 1;
let seek_end = 2;
let read_all = -2;
let read_line = -1;
let write_all = -1;
```



## StdOs  操作系统
```sc
require ("os");
let os = new StdOs;
```

终止程序 : (void) -> (void)
abort()			(void) 	->  (void)

退出程序	: 退出码 -> (void)
exit(value)		value:number

转换字符串	 : 参数 -> 成功返回字符串，失败返回null
str(value)		value:string|number|boolean		-> string|null

转换数字	 : 参数 -> 成功返回数字，失败返回null
number(value)	value:string|number|boolean		-> number|null

转换布尔 	: 参数 -> 成功返回布尔值，失败返回null
bool(value)		value:string|number|boolean		-> boolean|null

返回参数类型 : 参数 -> 返回指定变量类型
type(value)		value:any	->  string

获取16进制 : 参数 -> 成功返回number，失败返回null
hex(value)		value:string	-> number|null
<u>***hex("0xffff");***</u>

获取8进制  : 参数 -> 成功返回number, 失败返回null
oct(value)		value:string	->number|null
<u>***oct("0o7777");***</u>

获取2进制  : 参数 -> 成功返回number, 失败返回null
bin(value)		value:string	->number|null
<u>***bin("0b1111");***</u>

获取环境 : 环境名字 -> 成功返回环境内容字符串，失败返回null
getenv(value)	value:string	-> string|null

创建修改删除环境变量 : 名称 , 参数 -> 成功返回true 失败返回false
putenv(va1,va2)	va1:string , va2:string	-> boolean
<u>***putenv("Path",""); // delete Path***</u>

系统命令 : 命令 -> 执行成功返回true 失败返回false
system(value)	value:string	-> boolean

睡眠 : 时间 -> 执行成功返回睡眠时间，失败返回null
sleep(value)	value:number  -> number|boolean

获取启动参数 : (void) -> 返回数组
argv()		(void)	-> array



## StdMath 数学库
```sc
require ("math");
let math = new StdMath;
```

abs		返回绝对值 : 数字 -> 成功返回数字的绝对值，否则返回null
value:number -> number|null

div		积分除法	  : 分子，分母 -> 成功返回 数组(商，余数)，否则返回null
value1:number , value2:number -> array|null

acos		返回参数的反余弦弧度		: 参数 -> 成功返回数字，否则返回null
value1:number -> number|null

asin		返回参数的正弦弧线弧度	: 参数 -> 成功返回数字，否则返回null
value1:number -> number|null

atan		返回参数的反正切值，以弧度为单位 : 参数 -> 成功返回数字， 否则返回null
value1:number -> number|null

atan2	返回参数1 / 参数2的以弧度为单位的反正切值，根据这两个值，以确定正确的象限上的标志	: 参数1，参数2 -> 成功返回数字，否则返回null
value1:number,value2:number -> number|null

cos		返回的弧度角参数的余弦值	: 参数 -> 成功返回数字， 否则返回null
value1:number -> number|null

cosh		返回参数1的双曲余弦 : 参数 -> 成功返回数字，否则返回null
value1:number -> number|null

sin		返回一个弧度角参数1的正弦 : 参数1  -> 成功返回数字，否则返回null
value1:number -> number|null

sinh		返回参数1的双曲正弦	: 参数1  -> 成功返回数字，否则返回null
value1:number -> number|null

tanh		返回参数1的双曲正切  :  参数1  -> 成功返回数字，否则返回null
value1:number -> number|null

exp		返回e值的第参数1次幂  :  参数1  -> 成功返回数字，否则返回null
value1:number -> number|null

log		返回自然对数的参数1（基准 - e对数） :  参数1  -> 成功返回数字，否则返回null
value1:number -> number|null

log10	返回参数1的常用对数（以10为底）	:  参数1  -> 成功返回数字，否则返回null
value1:number -> number|null

pow		返回参数1的参数2次方	:  参数1 , 参数2 -> 成功返回数字，否则返回null
value1:number,value2:number -> number|null

sqrt		返回参数1的平方根	  :  参数1  -> 成功返回数字，否则返回null
value1:number -> number|null

ceil		返回大于或等于参数1的最小整数值  :  参数1  -> 成功返回数字，否则返回null
value1:number -> number|null

floor	返回的最大整数值小于或等于参数1  :  参数1  -> 成功返回数字，否则返回null
value1:number -> number|null

frexp	返回值是尾数指向的整数指数是指数。得到的值是参数1 * 2 ^ exponent ： 参数1  ->  成功返回数组，否则返回null
value1:number -> array|null

ldexp	返回参数1乘以参数2指数的幂	： 参数1 , 参数2 -> 成功返回数字，否则返回null
value1:number -> number|null

modf		返回参数1的小数与整数部分	:  参数1 -> 成功返回数组，否则返回null
value1:number -> array|null



## StdString 字符串处理库
```sc
require ("string");
let s = StdString;
```

split   拆分字符串 : 源字符串，分隔符 -> 拆分成功返回数组，否则返回null
str:string ,delim:string -> array|null
function split(str,delim)

cut      分割指定字符串 : 源字符串，分隔符 -> 拆分成功返回数组，否则返回null
str:string ,delim:string -> array|null
function cut(str,delim)

sub      剪切指定字符串 : 目标字符串，开始位置，个数(-1剩余全部) -> 剪切成功返回字符串，否则返回null
str:string ,pos:number ,lens:number -> string|null
function sub(str,pos,lens)

replace  替换指定内容 : 字符串，源内容，替换内容，替换次数(-1全部) -> 替换成功返回替换后的字符串，否则返回null
str:string ,src:string ,des:string ,cnt:number -> string|null
function replace(str,src,des,cnt)

at              获取字符串下标 : 字符串，下标 -> 返回指定下标字符，失败返回null
str:string ,index:number -> string|null
function at(str,index)

erase    删除指定字符串 : 字符串，下标，长度 -> 返回删除后的字符串，失败返回null
str:string ,index:number ,lens:number -> string|null
function erase(str,index,lens)

insert   在某个位置插入字符串 : 字符串，下标，内容 -> 返回插入后总字符串，失败返回null
str:string ,index:number ,txt:string -> string|null
function insert(str,index,txt)

lens            获取字符串总长度 : 字符串 -> 返回字符串长度,失败返回null
str:string -> string|null
function lens(str)

reverse  字符串反转     : 字符串 -> 返回翻转后的字符串，失败返回null
str:string -> string|null
function reverse(str)

fill  字符串填充        ： 字符串，位置，长度，内容 -> 返回填充后的总字符串，失败返回null
str:string ,pos:number ,lens:number ,txt:string -> string|null
function fill(str,pos,lens,txt)

lower   将字符串转换为大写 : 字符串 -> 返回转换字符串，失败返回null
str:string -> string|null
function lower(str)

upper   将字符串转换为大写 : 字符串 -> 返回转换字符串，失败返回null
str:string -> string|null
function upper(str)

tirm     删除头尾空白符 : 字符串 -> 返回处理后的字符串，失败返回null
str:string -> string|null
function tirm(str)

up      将首字母大写 : 字符串 -> 返回处理后的字符串，失败返回null
str:string -> string|null
function up(str)

low     将首字母小写 : 字符串 -> 返回处理后的字符串，失败返回null
str:string -> string|null
function low(str)

count   返回字符串出现的次数 : 字符串，目标 -> 返回目标出现次数，失败返回null
str:string ,src:string -> number|null
function count(str,src)

find_all        正向查找全部下标                : 字符串，内容 -> 查找成功返回数组，失败返回null
str:string ,txt:string -> array|null
function find_all(str,txt)

find_first      正向查找第一个下标              ：字符串，内容 -> 查找成功返回数字，失败返回null
str:string ,txt:string -> number|null
function find_first(str,txt)

find_N          正向查找N个下标         ：字符串，内容，次数 -> 查找成功返回数组，失败返回null
str:string ,txt:string ,cnt:number -> array|null
function find_N(str,txt,cnt)

find_not_all    正向查找全部not下标     ：字符串，内容 -> 查找成功返回二维数组，失败返回null
str:string ,txt:string -> array|null
function find_not_all(str,txt)

find_not_first  正向查找第一个not下标   : 字符串，内容 -> 查找成功返回数组，失败返回null
str:string ,txt:string -> array|null
function find_not_first(str,txt)

find_not_N      正向查找N个not下标              : 字符串，内容，次数 -> 查找成功返回二维数组，失败返回null
str:string ,txt:string ,cnt:number -> array|null
function find_not_N(str,txt,cnt)

rfind_frist              反向查找第一个下标             :       字符串，内容 -> 成功返回数字，失败返回null
str:string ,txt:string -> number|null
function rfind_first(str,txt)

rfind_N                  反向查找N个下标                        :       字符串，内容，次数 -> 成功返回数组，失败返回nullstr:string ,txt:string ,cnt:number -> array|null
function rfind_N(str,txt,cnt)

rfind_not_first  反向查找第一个not下标  :       字符串，内容    -> 成功返回数组，失败返回null
str:string ,txt:string -> array|null
function rfind_not_first(str,txt)

rfind_not_N              反向查找N个not下标             :       字符串，内容，次数 -> 成功返回二维数组，失败返回null
str:string ,txt:string ,cnt:number -> array|null
function rfind_not_N(str,txt,cnt)

**内置变量**  
```sc
let str_all = -1;
```



## StdArray 数组处理库
```sc
require ("array");
let array = new StdArray;
```

返回数组总个数              ：数组 -> 成功返回个数，否则返回null
arr|array -> number|null
size(arr)

比较两个数组                    ：数组1，数组2 -> 成功对比结果返回数字，否则返回null (相等返回0,1大返回数组-1，否则返回1)
arr1|array , arr2|array -> number|null
cmp(arr1,arr2)

返回数组列表最大值                  ：数组 -> 成功返回最大值，否则返回null
arr|array -> any|null
max(arr)

返回数组列表最小元素            ：数组 -> 成功返回最小值，否则返回null
arr|array -> any|null
min(arr)

在数组末尾添加元素                  ：数组，任意 -> 成功返回添加后的数组，否则返回null
arr|array , val|any -> array|null
append(arr,val)

检测数组某个元素的个数      ：数组，任意 -> 成功返回个数，否则返回null
arr|array , val|any -> number|null
count(arr,val)

在数组末尾添加                      ：数组，任意 -> 成功返回添加后的新数组，否则返回null
arr|array , val|any -> array|null
extend(arr,val)

返回数组最后一个元素            ：数组 -> 成功返回末尾元素，否则返回null
arr|array -> val|any
back(arr)

返回数组第一个元素                  ：数组 -> 成功返回开头元素，否则返回null
arr|array -> val|any
front(arr)

查找数组指定元素第一次位置  ：数组，任意 -> 成功查找返回下标，否则返回null
arr|array , val|any -> number|string|null
index(arr,val)

将指定内容插入指定数组位置  ：数组，下标，任意 -> 成功返回插入后的新数组，否则返回null
arr|array , index|number, val|any -> array|null
insert(arr,index,val)

删除数组指定内容                ：数组，下标    -> 成功返回删除后的新数组，否则返回null
arr|array , index|number|string -> array|null
pop(arr,index)

移除数组指定的第一个值  ：数组，任意 -> 成功返回移除后的新数组，否则返回null
arr|array , val|any -> array|null
remove(arr,val)

反序数组                                        ：数组  -> 成功返回新数组，否则返回null
arr|array -> array|null
reverse(arr)

范围生成数组   ：数字1，数组2 -> 成功返回基于数组1和数字2范围区间的数组，否则返回null
num_min|number , num_max|number -> array|null
range(num_min,num_max)

返回数组所有下标                ：数组 -> 成功将该数组的所有下标作为数组返回，失败返回null
arr|array -> array|null
childs(arr)



## StdTime 时间库
```sc
require ("time");
let tm = new StdTime;
```

获得时间戳（秒 : （void） -> 返回数字时间戳，否则返回null
(void) -> number|null
time();
 	
    
获得时间戳（毫秒 : （void） -> 返回数字时间戳，否则返回null
(void) -> number|null
timeAsM();
 	
获得程序运行时间（秒 : （void） -> 返回数字时间，否则返回null
(void) -> number|null
clock();
 	
获得程序运行时间（毫秒 : （void） -> 返回数字时间，否则返回null
(void) -> number|null
clockAsM();
 	
将秒转换成某个时间 (6年，5月，4日，3时，2分，1秒 0毫秒 : 源数字，类型 -> 转换成功返回数字，否则返回null
sec:number, typ:number -> number|null
cast(sec,typ);
 	
将毫秒转换成某个时间 (6年，5月，4日，3时，2分，1秒 0毫秒 : 源数字，类型 -> 转换成功返回数字，否则返回null
millisec:number, typ:number -> number|null
castAsM(millisec,typ);
 	
创建一个计时器 : (void) -> 设置成功返回数字计时器Id，否则返回null
(void) -> number|null
join();
 	
继续一个计时器 ： 数字计时器id -> 继续成功返回true,否则返回false
id:number -> number|null
start(id);
 	
暂停一个计时器 ： 数字计时器id -> 暂停成功返回true,否则返回false
id:number -> number|null
pause(id);
 	
获取一个计时器 ： 数字计时器id -> 成功返回计数秒，否则返回null
id:number -> number|null
over(id);
_**该值是静止通过计算获取的不受Sleep等函数影响**_
 	
获取一个计时器 ： 数字计时器id -> 成功返回计数毫秒，否则返回null
id:number -> number|null
overAsM(id);
_**该值是静止通过计算获取的不受Sleep等函数影响**_
 	
获取系统日期 ： (void) -> 成功返回日期字符串，否则返回null
(void) -> string|null
date();
 	
获取系统日期 (毫秒 ： (void) -> 成功返回日期字符串，否则返回null
(void) -> string|null
dateAsM();
 	
将秒时间戳转换成日期 ： 数字秒时间戳 -> 成功返回日期字符串，否则返回null
sec:number -> string|null
toDate(sec);
 	
将毫秒时间戳转换成日期 ： 数字毫秒时间戳 -> 成功返回日期字符串，否则返回null
millisec:number -> string|null
toDateAsM(millisec);


**内置变量**  
```sc
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
```



## StdType 类型库
```sc
require ("type");
let tp = new StdType;
```

检测字符串是否纯字母: 是则返回true，否则返回false
str|string -> boolean;
isAlpha(str);

检测字符串是否纯数字: 是则返回true，否则返回false
str|string -> boolean;
isAlnum(str);

检测字符串是否含有空格: 是则返回true，否则返回false
str|string -> boolean;
hasBlank(str);

检测字符串是否纯空格: 是则返回true，否则返回false
str|string -> boolean;
isBlank(str);

检测字符串是否含有控制符: 是则返回true，否则返回false
str|string -> boolean;
hasCntrl(str);

检测字符串是否全控制符： 是则返回true，否则返回false
str|string -> boolean;
isCntrl(str);

检测字符串是否全小写: 是则返回true，否则返回false
str|string -> boolean;
isLower(str);

检测字符串是否全大写： 是则返回true，否则返回false
str|string -> boolean;
isUpper(str);

## StdWindow 窗口库
```sc
require ("window");
let win = new StdWindow;
```

隐藏控制台：成功返回true，否则返回false
void -> boolean;
hideControl();

显示控制台：成功返回true，否则返回false
void -> boolean;
showControl();

创建消息框：成功返回按钮id，否则返回false
title|str, txt|str ,type|number -> number|boolean
msgBox(title,txt,type)

创建输入框：成功返回输入内容，否则返回false
title|str, txt|str -> str|boolean
editBox(title,txt)

创建选择框：成功返回路径，否则返回false
(void) -> str|boolean
bowserBox()

通过IE内核 创建html窗口：成功返回true，否则返回false
title|str, html|str -> boolean
htmlBox(title,html)

通过IE内核 创建url窗口：成功返回true，否则返回false
title|str, url|str -> boolean
urlBox(title,url)

通过Edge内核 创建html窗口：成功返回true，否则返回false
title|str, html|str -> boolean
htmlView(title,html)

通过Edge内核 创建url窗口：成功返回true，否则返回false
title|str, url|str -> boolean
urlView(title,url)

setWebStyle   设置窗口样式	: 成功返回true，否则返回false
style|number -> boolean
setWebBoxStyle(style)
[详细参考winuser窗口样式](https://learn.microsoft.com/zh-cn/windows/win32/winmsg/window-styles)


getWebStyle   获取窗口样式	: 成功返回number，否则返回false
(void) -> number|boolean
getWebBoxStyle()

setWebBoxSize   设置窗口位置大小	: 成功返回true，否则返回false
x|number, y|number, w|number, h|number -> boolean
setWebBoxSize(x,y,w,h)


getWebBoxSize   获取窗口位置大小	: 成功返回array，否则返回false
(void) -> array|boolean
getWebBoxSize()

```
	win.none 无任何控件样式
	win.title 仅标题窗口样式
	win.menu 标题和关闭按钮样式
	win.menupro 标题，关闭，最小化，最大化样式
	win.normal menupro基础上增加窗口大小控制
```




## StdThread 线程库
```sc
require ("thread");
let thread = new StdThread;
```

create 创建基于lua代码的线程 i	：成功返回number ，失败返回null
path|string  ->  number|null
create(path)


set 设置执行时的全局变量	：成功返回number ，失败返回null
id|number ,  name|string , param|(boolean,number,string)  ->  number|null
set(id,name,param)


get 结束时获取指定全局变量值	：成功返回number ，失败返回null
id|number ,  name|string   ->  (boolean,number,string) |null
get(id,name)


run 运行线程		：运行成功返回true ，失败返回null
id|number -> boolean|null
run(id)


state 获取线程状态		：运行成功返回true ，失败返回null
id|number -> boolean|null
state(id)


wait 等待线程结束		：运行成功返回true ，失败返回null
id|number -> boolean|null
wait(id)
_**执行join获detach后该函数不可用**_


stop 挂起线程		：运行成功返回true ，失败返回null
id|number -> boolean|null
stop(id)
_**执行join获detach后该函数不可用**_
_**挂起线程注意输出等带锁操作造成死锁  不建议使用**_


resume 挂起线程		：运行成功返回true ，失败返回null
id|number -> boolean|null
resume(id)
_**执行join获detach后该函数不可用**_
_**挂起线程注意输出等带锁操作造成死锁  不建议使用**_


kill 挂起线程		：运行成功返回true ，失败返回null
id|number -> boolean|null
kill(id)
_**执行join获detach后该函数不可用**_
_**挂起线程注意输出等带锁操作造成死锁  不建议使用**_


join 连接线程		：运行成功返回true ，失败返回null
id|number -> boolean|null
join(id)


detach 分离线程		：运行成功返回true ，失败返回null
id|number -> boolean|null
detach(id)


clear 释放执行完毕的线程子资源		：运行成功返回true ，失败返回null
id|number -> boolean|null
clear(id)


```
	// 线程状态
	thread.s_unknow 未初始化
	thread.s_start  开始运行
	thread.s_runing 正在运行
	thread.s_wait  线程已挂起
	thread.s_stop  线程已结束
```


## StdSocket 套接字库
```sc
require ("socket");
let socket = new StdSocket;
```

create 创建socket ：成功返回socket id，否则返回错误码id
family|number, type|number -> boolean|number
create(family, type)


bind 绑定socket ：成功返回true ，否则返回错误码id
socket|number, ip|string, port|number -> boolean|number
bind(socket, ip, port)


listen 监听socket ：成功返回true ，否则返回错误码id
socket|number, count|number -> boolean|number
listen(socket, count)
  

accept 接受socket ：成功返回array数组包含ip sock以及port ，否则返回错误码id
socket|number -> array|number
accept(socket)


connect 连接socket ：成功返回true ，否则返回错误码id
socket|number ,ip|string, port|number -> boolean|number
connect(socket, ip, port)


send 发送字段Tcp ：成功返回发送长度 ，否则返回错误码id
socket|number, buf|string, lens|number -> number
send(socket, buf, lens)


recv 接受字段Tcp ：成功返回接受字符串 ，否则返回错误码id
socket|number, lens|number -> number
recv(socket, lens)


sendto 发送字段Udp ：成功返回发送长度 ，否则返回错误码id
socket|number, buf|string, lens|number, ip|string, port|number -> number
sendto(socket, buf, lens, ip, port)


recvfrom 接受字段Udp ：成功返回array ，否则返回错误码id
socket|number, lens|number -> array|number
recvfrom(socket, lens)
  

close 关闭socket ：成功返回true ，否则返回false
socket|number -> boolean
close(socket)
  

select 检测一组一维数组socket并返回 ：成功返回二位数组（可能为空） ，否则返回错误码id
sockets|array, tm|number -> array|number
select(list, tm)
  

gethostname 获取主机名 ：成功返回字符串 ，否则返回错误码id
(void) -> string
gethostname()


gethostbyname 通过主机名获取ip ：成功返回数组 ，否则返回错误码id
host|string -> array|number
gethostbyname(host)

```
	// socket创建family
	socket.ipv4		创建ipv4
	socket.ipv6		创建ipv6

	// socket创建type
	socket.tcp		基于tcp连接
	socket.udp		基于udp连接
```



## StdDirect 目录库

```sc
require ("direct");
let dir = new StdDirect;
```
改变当前工作目录                ：字符串 -> 成功true，否则返回错误码
path:string -> boolean|number
chdir(path)


返回当前工作目录                ：void -> 成功字符串，否则返回null
(void) -> string|null
getcwd()


返回指定的文件夹包含的文件或文件夹的名字的列表。                ：字符串 -> 成功数组，否则返回错误码
path:string -> boolean|number
listdir(path)


设置目录或文件权限      : 字符串 | 数字 -> 成功返回true，否则返回错误码
dir:string | mods|number -> boolean|number
chmod(dir,mods)


递归创建目录。          ：字符串 -> 成功返回true，否则返回错误码
dir:string -> boolean|number
makedirs(dir)


创建目录。              ：字符串 -> 成功true，否则返回错误码
dir:string -> boolean|number
mkdir(dir)


递归删除目录。          ：字符串 -> 成功true，否则返回错误码
dir:string -> boolean|number
removedirs(dir)


删除目录。              ：字符串 -> 成功true，否则返回错误码
dir:string -> boolean|number
rmdir(dir)


重命名目录名。          ：字符串 | 字符串 -> 成功true，否则返回错误码
src:string | des:string -> boolean|number
rename(src, des)


方法用于在给定的路径上执行一个系统 stat 的调用          ：字符串 -> 成功数组，否则返回错误码
dir:string -> array|number
stat(dir)


返回绝对路径    ：字符串 -> 成功字符串，否则返回错误码
path:string -> string|number
abspath(path)


返回相对路径    ：字符串 -> 成功字符串，否则返回错误码
path:string -> string|number
relative(path)


返回文件名      ：字符串 -> 成功字符串，否则返回错误码
path:string -> string|number
basename(path)


返回多个路径中，所有共有的最长的路径    ：数组 -> 成功字符串，否则返回null
paths:array -> string|null
commonprefix(paths)


返回文件路径    ：字符串 -> 成功字符串，否则返回错误码
path:string -> string|number
dirname(path)


路径是否存在    ：字符串 -> 成功true，否则返回false
path:string -> boolean
exists(path)


根据环境变量的值替换%字符串%值  ：字符串 -> 成功字符串，否则返回错误码
path:string -> string|number
expand(path)


返回最近访问时间        ：字符串 -> 成功数字，否则返回错误码
file:string -> number
getatime(file)


返回最近文件修改时间    ：字符串 -> 成功数字，否则返回错误码
file:string -> number
getmtime(file)


返回文件创建时间        ：字符串 -> 成功数字，否则返回错误码
file:string -> number
getctime(file)


返回文件大小    ：字符串 -> 成功数字，否则返回错误码
file:string -> number
getsize(file)


判断是否为绝对路径      ：字符串 -> 成功true，否则返回false
path:string -> boolean
isabs(path)


判断路径是否为文件      ：字符串 -> 成功true，否则返回false
path:string -> boolean
isfile(path)


判断路径是否为目录      ：字符串 -> 成功true，否则返回false
path:string -> boolean
isdir(path)


将路径字符串的大写和正斜杠转换  ：字符串 -> 成功字符串，否则返回错误码
path:string -> string|number
normcase(path)


规范路径字符串形式      ：字符串 -> 成功字符串，否则返回错误码
path:string -> string|number
normpath(path)


获得相对信息    ：字符串 -> 成功数组，否则返回错误码
path:string -> array|number
info(path)


判断目录或文件是否相同 ：字符串|字符串 -> 成功返回数字，否则返回错误码
src:string | des:string -> number
samefile(src, des) array|number
info（str_dir)


判断目录或文件是否相同 ：字符串|字符串 -> 成功返回数字，否则返回错误码
src:string | des:string -> number
samefile（str_src, str_des)

