include "os";
include "window";
let os = new StdOs;
let win = new StdWindow;

win.hideControl();
os.sleep(5000);
win.showControl();

return;