// io
function export __system__io__input__() end
function export __system__io__print__(str) end

function export __system__io__fexist__(file) end
function export __system__io__fcreate__(file) end
function export __system__io__fremove__(file) end
function export __system__io__frename__(source,target) end
function export __system__io__tmpfile__() end

function export __system__io__errget__() end
function export __system__io__errset__(str) end

function export __system__io__fseek__(id,offset,origin) end
function export __system__io__ftell__(id) end

function export __system__io__fread__(id,lens) end
function export __system__io__fwrite__(id,str,lens) end

function export __system__io__fclose__(id) end
function export __system__io__fopen__(file,mode) end

// os
function export __system__os__abort__() end
function export __system__os__exit__(value1) end
function export __system__os__str__(value1) end
function export __system__os__number__(value1) end
function export __system__os__bool__(value1) end
function export __system__os__type__(value1) end
function export __system__os__hex__(value1) end
function export __system__os__oct__(value1) end
function export __system__os__bin__(value1) end
function export __system__os__getenv__(value1) end
function export __system__os__putenv__(value1,value2) end
function export __system__os__system__(value1) end
function export __system__os__sleep__(value1) end
function export __system__os__argv__() end

// math
function export __system__math__abs__(value1) end
function export __system__math__div__(value1,value2) end
function export __system__math__acos__(value1) end
function export __system__math__asin__(value1) end
function export __system__math__atan__(value1) end
function export __system__math__atan2__(value1,value2) end
function export __system__math__cos__(value1) end
function export __system__math__cosh__(value1) end
function export __system__math__sin__(value1) end
function export __system__math__sinh__(value1) end
function export __system__math__tanh__(value1) end
function export __system__math__exp__(value1) end
function export __system__math__log__(value1) end
function export __system__math__log10__(value1) end
function export __system__math__pow__(value1,value2) end
function export __system__math__sqrt__(value1) end
function export __system__math__ceil__(value1) end
function export __system__math__floor__(value1) end
function export __system__math__frexp__(value1) end
function export __system__math__ldexp__(value1,value2) end
function export __system__math__modf__(value1) end

// string
function export __system__str__split__(str,delim) end
function export __system__str__cut__(str,delim) end
function export __system__str__sub__(str,pos,lens) end
function export __system__str__replace__(str,src,des,cnt) end
function export __system__str__reverse__(str) end
function export __system__str__lower__(str) end
function export __system__str__upper__(str) end
function export __system__str__tirm__(str) end
function export __system__str__up__(str) end
function export __system__str__low__(str) end
function export __system__str__fill__(str,pos,lens,txt) end
function export __system__str__count__(str,src) end
function export __system__str__at__(str,index) end
function export __system__str__erase__(str,index,lens) end
function export __system__str__insert__(str,index,txt) end
function export __system__str__lens__(str) end
function export __system__str__find_all__(str,txt) end
function export __system__str__find_first__(str,txt) end
function export __system__str__find_N__(str,txt,cnt) end
function export __system__str__find_not_all__(str,txt) end
function export __system__str__find_not_first__(str,txt) end
function export __system__str__find_not_N__(str,txt,cnt) end
function export __system__str__rfind_first__(str,txt) end
function export __system__str__rfind_N__(str,txt,cnt) end
function export __system__str__rfind_not_first__(str,txt) end
function export __system__str__rfind_not_N__(str,txt,cnt) end


// array
function export __system__vec__size__(arr) end
function export __system__vec__cmp__(arr1,arr2) end
function export __system__vec__max__(arr) end
function export __system__vec__min__(arr) end
function export __system__vec__append__(arr,val) end
function export __system__vec__count__(arr,val) end
function export __system__vec__extend__(arr,val) end
function export __system__vec__back__(arr) end
function export __system__vec__front__(arr) end
function export __system__vec__index__(arr,val) end
function export __system__vec__insert__(arr,index,val) end
function export __system__vec__pop__(arr,index) end
function export __system__vec__remove__(arr,val) end
function export __system__vec__reverse__(arr) end
function export __system__vec__range__(num_min,num_max) end
function export __system__vec__childs__(arr) end

// time
function export __system__time__time__() end
function export __system__time__timeAsM__() end
function export __system__time__clock__() end
function export __system__time__clockAsM__() end
function export __system__time__cast__(sec,typ) end
function export __system__time__castAsM__(millisec,typ) end
function export __system__time__join__() end
function export __system__time__start__(id) end
function export __system__time__pause__(id) end
function export __system__time__over__(id) end
function export __system__time__overAsM__(id) end
function export __system__time__date__() end
function export __system__time__dateAsM__() end
function export __system__time__toDate__(sec) end
function export __system__time__toDateAsM__(millisec) end

// type
function export __system__type__isAlpha__(str) end
function export __system__type__isAlnum__(str) end
function export __system__type__hasBlank__(str) end
function export __system__type__isBlank__(str) end
function export __system__type__hasCntrl__(str) end
function export __system__type__isCntrl__(str) end
function export __system__type__isLower__(str) end
function export __system__type__isUpper__(str) end

// window
function export __system__fltk_window__hideControl__() end
function export __system__fltk_window__showControl__() end
function export __system__fltk_window__msgBox__(title,txt,type) end
function export __system__fltk_window__editBox__(title,txt) end
function export __system__fltk_window__bowserBox__() end
function export __system__fltk_window__htmlBox__(title,html) end
function export __system__fltk_window__urlBox__(title,url) end