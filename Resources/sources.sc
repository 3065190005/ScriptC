require "thread";
	require "io";
	
	let io = new StdIo;
	let thread = new StdThread;

	let id = thread.create("lua_file.lua");

	thread.set(id,"C_number",111);
	thread.set(id,"C_string","aaa");
	thread.set(id,"C_boolean",true);
	thread.run(id);

	let type = 2;

	if (type == 1):
		thread.join(id);
		let result = thread.get(id,"result_number");
		io.print(result);
	elif (type == 2):
		thread.wait(id);
		let state = thread.state(id);
		io.print(thread.detach(id));
		io.print(state);
	elif (type == 3):
		thread.stop(id);
		let whileIndex = [0,1,2,3,4,5,6,7,8,9];
		let forResu = 0;
		for i in whileIndex:
			forResu = forResu + i;
		end
		thread.resume(id);
		io.println(forResu);
		thread.join(id);
	elif (type == 4):
		thread.kill(id);
		io.println("kill thread");
	end

	thread.clear(id);
	
	return;