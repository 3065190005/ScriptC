require ("extern_system\\system_array");

interface StdArray{

    // arr|array -> number|null
    function size(arr):
        return __system__array__size__(arr);
    end

    // arr1|array , arr2|array -> number|null
    function cmp(arr1,arr2):
        return __system__array__cmp__(arr1,arr2);
    end

    // arr|array -> any|null
    function max(arr):
        return __system__array__max__(arr);
    end

    // arr|array -> any|null
    function min(arr):
        return __system__array__min__(arr);
    end

    // arr|array , val|any -> array|null
    function append(arr,val):
        return __system__array__append__(arr,val);
    end

    // arr|array , val|any -> number|null
    function count(arr,val):
        return __system__array__count__(arr,val);
    end

    // arr|array , val|any -> array|null
    function extend(arr,val):
        return __system__array__extend__(arr,val);
    end

    // arr|array -> val|any
    function back(arr):
        return __system__array__back__(arr);
    end

    // arr|array -> val|any
    function front(arr):
        return __system__array__front__(arr);
    end

    // arr|array , val|any -> number|string|null
    function index(arr,val):
        return __system__array__index__(arr,val);
    end

    // arr|array , index|number, val|any -> array|null
    function insert(arr,index,val):
        return __system__array__insert__(arr,index,val);
    end

    // arr|array , index|number|string -> array|null
    function pop(arr,index):
        return __system__array__pop__(arr,index);
    end

    // arr|array , val|any -> array|null
    function remove(arr,val):
        return __system__array__remove__(arr,val);
    end

    // arr|array -> array|null
    function reverse(arr):
        return __system__array__reverse__(arr);
    end

    // num_min|number , num_max|number -> array|null
    function range(num_min,num_max):
        return __system__array__range__(num_min,num_max);
    end

    // arr|array -> array|null
    function childs(arr):
        return __system__array__childs__(arr);
    end
}