require ("io");

let io = new StdIo;

io.println("BeginTest : input Test File name");
let name = io.input();
io.println("file name : " + name);

io.println("BeginTest : input New Test File name");
let wname = io.input();
io.println("file new name : " + wname);

let ret = 0;
io.print("file exist : ");
ret = io.fexist(name);
io.println(ret);

io.print("file create : ");
ret = io.fcreate(name);
io.println(ret);

io.print("file frename : ");
ret = io.frename(name,wname);
io.println(ret);

io.print("file remove : ");
ret = io.fremove(wname);
io.println(ret);

io.print("temp file id : ");
ret = io.tmpfile();
io.println(ret);

io.print("close file " + ret);
ret = io.fclose(ret);
io.println(ret);

io.println("set error str : aaa");
ret = io.errset("aaa");

io.print("get error str : ");
ret = io.errget();
io.println(ret);

let rets = 0;
io.println("open file:");
ret = io.fopen("abcdef.txt","a+");
io.println(ret);

io.println("write file: aaaa\\nbbbb\\ncccc\\nddddd ");
rets = io.fwrite(ret,"aaaa\nbbbb\ncccc\nddddd",io.write_all);
io.println("write lens : " + rets);

io.println("tell file:");
rets = io.ftell(ret);
io.println(rets);

let str1 = 0;
let str2 = 0;

io.println("seek file");
rets = io.fseek(ret,io.seek_beg,2);
io.println(rets);

io.println("read file line");
str1 = io.fread(ret,io.read_line);

io.println("read file all");
str2 = io.fread(ret,io.read_all);

io.println("read line : " + str1);
io.println("read all : " + str2);

io.println("file close");
rets = io.fclose(ret);
io.println(rets);