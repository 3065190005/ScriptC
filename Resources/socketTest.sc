	require ("socket");
	require ("io");
	let io = new StdIo;
	let socket = new StdSocket;

	let type = 1;
	if(type == 1):
		let result = false;

		let listen = socket.create(socket.ipv4,socket.tcp);
		io.println("create : " + listen);

		result = socket.bind(listen,"127.0.0.1",9997);
		io.println(result);

		result = socket.listen(listen,10);
		io.println("listen :" + result);

		let client = socket.accept(listen)[0];
		io.println("client socket :" + client);

		result = socket.send(client,"Hello ScriptC",100);
		io.println("send lens :" + result);

		result = socket.recv(client,100);
		io.println("recv buf :" + result);

		
		result = socket.close(client);
		io.println("close client socket :"+result);

		result = socket.close(listen);
		io.println("close listen socket :"+result);

		result = socket.close(2333);
		io.println("close error socket :"+result);

	elif(type == 2):
		let result = false;

		let client = socket.create(socket.ipv4,socket.tcp);
		io.println("create : " + client);

		result = socket.connect(client,"127.0.0.1",9955);
		io.println("connect : " + result);


		result = socket.send(client,"Hello ScriptC",100);
		io.println("send lens :" + result);

		result = socket.recv(client,100);
		io.println("recv buf :" + result);

		result = socket.close(client);
		io.println("close client socket :"+result);

	elif(type == 3):
		let result = false;

		let client = socket.create(socket.ipv4,socket.udp);
		io.println("create : " + client);

		result = socket.bind(client,"127.0.0.1",9933);
		io.println(result);

		result = socket.sendto(client,"Hello Udp",100,"127.0.0.1",9934);
		io.println(result);


		result = socket.recvfrom(client,100);
		io.println(result);
				
		result = socket.close(client);
		io.println(result);
	elif(type ==4):
		let result = false;

		let listen = socket.create(socket.ipv4,socket.tcp);
		io.println("create : " + listen);

		result = socket.bind(listen,"127.0.0.1",9997);
		io.println(result);

		result = socket.listen(listen,10);
		io.println("listen :" + result);

		let recv_limit = 3;

		while(true):
			io.println("select block 3s");
			result = socket.select([listen],3000);
			for i in result:
				if(i["r"] != null):
					result = socket.accept(i["s"]);
					let client = result[0];
					io.println("client socket :" + client);

					result = socket.send(client,"Hello ScriptC",100);
					io.println("send lens :" + result);

					result = socket.recv(client,100);
					io.println("recv buf :" + result);

					result = socket.close(client);
					io.println("close client socket :"+result);

					recv_limit = recv_limit - 1;
				end
			end

			if(recv_limit == 0):
					break;
			end
		end

		result = socket.close(listen);
		io.println("close listen socket :"+result);
		
	end

	io.print("return");
	return;
	// ---