	require ("os");
	require ("io");
	require ("dialog");
	require ("example\\index");

	let os = new StdOs;
	let io = new StdIo;
	let dialog = new StdDialog;
	
	dialog.hideConsole();
	let ret = dialog.msgBox("Title","txt",1);
	io.print(ret);

	ret = dialog.editBox("Title","txt");
	io.print(ret);

	ret = dialog.bowserBox();
	io.print(ret);

	let rect = [20,15,640,480];
	let style = 0; 
	
	ret = dialog.getWebBoxSize();
	ret = dialog.setWebBoxSize(rect[0],rect[1],rect[2],rect[3]);
	io.println(ret);

	ret = dialog.getWebBoxStyle();
	io.println(ret);

	// ret = dialog.setWebBoxStyle(style);
	// io.println(ret);


	ret = dialog.ieHtml(title,html);
	io.print(ret);

	ret = dialog.ieUrl("title2","www.baidu.com");
	io.print(ret);


	ret = dialog.edgeHtml(title,html);
	io.print(ret);


	ret = dialog.edgeUrl("title2","https://www.baidu.com/");
	io.print(ret);


	dialog.showConsole();
	os.system("pause");
	return;