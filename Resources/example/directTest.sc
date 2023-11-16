	require("io"); 
	require("direct");

	let io = new StdIo;
	let dir = new StdDirect;

	let run_path = "D:\\";

	let value = dir.getcwd();
	io.println(value);

	value = dir.chdir(run_path + "example\\");
	io.println(value);

	value = dir.listdir(run_path);
	io.println(value);

	value = dir.makedirs(".\\A\\B\\C\\D\\E\\");
	io.println(value);

	value = dir.mkdir(".\\MK");
	io.println(value);

	value = dir.removedirs(".\\A\\B\\C\\");
	io.println(value);

	value = dir.rmdir(".\\MK");
	io.println(value);

	value = dir.rename(".\\A\\B\\C", ".\\A\\B\\ReName");
	io.println(value);

	value = dir.stat(".\\stat.txt");
	io.println(value);

	value = dir.abspath(".\\stat.txt");
	io.println(value);

	io.println(dir.relative(value));
	io.println(dir.basename(value));
	io.println(dir.dirname(value));
	io.println(dir.exists(value));
	io.println(dir.expand("%AppData%"));

	io.println(dir.getatime(value));
	io.println(dir.getmtime(value));
	io.println(dir.getctime(value));
	io.println(dir.getsize(value));
	io.println(dir.isabs(value));
	io.println(dir.isfile(value));
	io.println(dir.isdir(value));

	io.println(dir.info(value));
	io.println(dir.normcase(value));
	io.println(dir.normpath(value));
	io.println(dir.samefile(".\\stat.txt",".\\stat_tar.txt"));

	value[0] = ".\\A\\1";
	value[1] = ".\\A\\2";
	value[2] = ".\\B\\1";
	value[3] = ".\\B\\2";
	value[4] = ".\\B\\3";

	io.println(dir.commonprefix(value));