require ("system");

interface StdSocket{

	// family|number, type|number -> boolean|number
	function create(family, type):
		return __system__socket__socket_create__(family, type);
	end

	// socket|number, ip|string, port|number -> boolean|number
	function bind(socket,ip,port):
		 return __system__socket__socket_bind__(socket, ip, port);
	end

	// socket|number, count|number -> boolean|number
	function listen(socket,count):
		 return __system__socket__socket_listen__(socket, count);
	end

	// socket|number -> array|number
	function accept(socket):
		 return __system__socket__socket_accept__(socket);
	end

	// socket|number ,ip|string, port|number -> boolean|number
	function connect(socket,ip,port):
		 return __system__socket__socket_connect__(socket, ip, port);
	end

	// socket|number, buf|string, lens|number -> number
	function send(socket,buf,lens):
		 return __system__socket__socket_send__(socket, buf, lens);
	end

	// socket|number, lens|number -> number
	function recv(socket,lens):
		 return __system__socket__socket_recv__(socket, lens);
	end

	// socket|number, buf|string, lens|number, ip|string, port|number -> number
	function sendto(socket,buf,lens,ip,port):
		 return __system__socket__socket_sendto__(socket, buf, lens, ip, port);
	end

	// socket|number, lens|number -> array|number
	function recvfrom(socket,lens):
		 return __system__socket__socket_recvfrom__(socket, lens);
	end

	// socket|number -> boolean
	function close(socket):
		 return __system__socket__socket_close__(socket);
	end

	// sockets|array, tm|number -> array|number
	function select(sockets,tm):
		 return __system__socket__socket_select__(sockets, tm);
	end

	// (void) -> string
	function gethostname():
		 return __system__socket__socket_gethostname__();
	end

	// host|string -> array|number
	function gethostbyname(host):
		 return __system__socket__socket_gethostbyname__(host);
	end

	let ipv4 = 2;
	let ipv6 = 23;
	let tcp = 1;
	let udp = 2;
}