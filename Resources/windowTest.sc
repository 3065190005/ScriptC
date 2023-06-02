include "os";
	include "io";
	include "window";

	let os = new StdOs;
	let io = new StdIo;
	let window = new StdWindow;
	
	window.hideControl();
	let ret = window.msgBox("Title","txt",1);
	io.print(ret);

	ret = window.editBox("Title","txt");
	io.print(ret);

	ret = window.bowserBox();
	io.print(ret);

	ret = window.htmlBox("title1","<h>123</h>");
	io.print(ret);

	ret = window.urlBox("title2","www.baidu.com");
	io.print(ret);

	window.showControl();
	os.system("pause");
	return;