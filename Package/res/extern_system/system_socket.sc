// socket
function export __system__socket__socket_create__(family, type) end
function export __system__socket__socket_bind__(socket, ip, port) end
function export __system__socket__socket_listen__(socket, count) end
function export __system__socket__socket_accept__(socket) end
function export __system__socket__socket_connect__(socket, ip, port) end
function export __system__socket__socket_send__(socket, buf, lens) end
function export __system__socket__socket_recv__(socket, lens) end
function export __system__socket__socket_sendto__(socket, buf, lens, ip, port) end
function export __system__socket__socket_recvfrom__(socket, lens) end
function export __system__socket__socket_close__(socket) end
function export __system__socket__socket_select__(list, tm) end
function export __system__socket__socket_gethostname__() end
function export __system__socket__socket_gethostbyname__(host) end