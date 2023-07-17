	require "os";
	require "io";
	require "window";
	require "index";

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

	let rect = [20,15,640,480];
	let style = 0; 
	
	ret = window.getWebBoxSize();
	ret = window.setWebBoxSize(rect[0],rect[1],rect[2],rect[3]);
	io.println(ret);

	ret = window.getWebBoxStyle();
	io.println(ret);

	// ret = window.setWebBoxStyle(style);
	// io.println(ret);


	ret = window.htmlBox(title,html);
	io.print(ret);

	ret = window.urlBox("title2","www.baidu.com");
	io.print(ret);


	ret = window.htmlView(title,html);
	io.print(ret);


	ret = window.urlView("title2","www.baidu.com");
	io.print(ret);


	window.showControl();
	os.system("pause");
	return;