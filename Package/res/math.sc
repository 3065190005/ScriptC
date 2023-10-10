require ("system");

interface StdMath{
    // value:number -> number|null
    function abs(value1):
        return __system__math__abs__(value1);
    end

    // value1:number , value2:number -> array|null
    function div(value1,value2):
  	 	return __system__math__div__(value1,value2);
    end

    // value1:number -> number|null
    function acos(value1):
  	 	 return __system__math__acos__(value1);
	end

    // value1:number -> number|null
    function asin(value1):
  	 	 return __system__math__asin__(value1);
	end

    // value1:number -> number|null
    function atan(value1):
  	 	 return __system__math__atan__(value1);
	end

    // value1:number,value2:number -> number|null
    function atan2(value1,value2):
  	 	 return __system__math__atan2__(value1,value2);
	end

    // value1:number -> number|null
    function cos(value1):
  	 	 return __system__math__cos__(value1);
	end

    // value1:number -> number|null
    function cosh(value1):
  	 	 return __system__math__cosh__(value1);
	end

    // value1:number -> number|null
    function sin(value1):
  	 	 return __system__math__sin__(value1);
	end

    // value1:number -> number|null
    function sinh(value1):
  	 	 return __system__math__sinh__(value1);
	end

    // value1:number -> number|null
    function tanh(value1):
  	 	 return __system__math__tanh__(value1);
	end

    // value1:number -> number|null
    function exp(value1):
  	 	 return __system__math__exp__(value1);
	end

    // value1:number -> number|null
    function log(value1):
  	 	 return __system__math__log__(value1);
	end

    // value1:number -> number|null
    function log10(value1):
  	 	 return __system__math__log10__(value1);
	end

    // value1:number,value2:number -> number|null
    function pow(value1,value1):
  	 	 return __system__math__pow__(value1,value1);
	end

    // value1:number -> number|null
    function sqrt(value1):
  	 	 return __system__math__sqrt__(value1);
	end

    // value1:number -> number|null
    function ceil(value1):
  	 	 return __system__math__ceil__(value1);
	end

    // value1:number -> number|null
    function floor(value1):
  	 	 return __system__math__floor__(value1);
	end

    // value1:number -> array|null
    function frexp(value1):
  	 	 return __system__math__frexp__(value1);
	end

    // value1:number -> number|null
    function ldexp(value1,value2):
  	 	 return __system__math__ldexp__(value1,value2);
	end

    // value1:number -> array|null
    function modf(value1):
  	 	 return __system__math__modf__(value1);
	end

}