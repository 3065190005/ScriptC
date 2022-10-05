include "io";
include "os";
let io = new StdIo;
let os = new StdOs;
os.system("cls");

while(true):
	io.println("Please type student age");
	// let age = io.input();
	let age = "Hello World";

	if(os.number(age) == null):
		io.println("input is not number please type number only : " + age);
	else:
		io.println("real age is : " + os.number(age) + " , fake age is : " +(os.number(age)+1));
		return 0;
	end
	
	os.sleep(1);
	os.system("cls");

end