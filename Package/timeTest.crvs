	include "io";
	function log(val):
		let io = new StdIo;
		io.println(val);
	end

	include "time";
	let tm = new StdTime;
	
	log(tm.time());
	log(tm.timeAsM());
	log(tm.clock());
	log(tm.clockAsM());
	log(tm.cast(120,tm.min));
	log(tm.castAsM(120000,tm.sec));
	let schedule = tm.join();
	log(schedule);
	log(tm.over(schedule));
	log(tm.overAsM(schedule));
	log(tm.date());
	log(tm.dateAsM());
	log(tm.toDate(1664374474));
	log(tm.toDateAsM(1664374474789));

	return 0;