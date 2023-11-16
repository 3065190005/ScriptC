require ("extern_system\\system_direct");

interface StdDirect{
    // path:string -> boolean|number
    function chdir(path):
        return __system__direct__chdir__(path);
    end

    // (void) -> string|null
    function getcwd():
        return __system__direct__getcwd__();
    end

    // path:string -> boolean|number
    function listdir(path):
        return __system__direct__listdir__(path);
    end

    // dir:string | mods|number -> boolean|number
    function chmod(dir, mods):
        return __system__direct__chmod__(dir, mods);
    end

    // dir:string -> boolean|number
    function makedirs(dir):
        return __system__direct__makedirs__(dir);
    end

    // dir:string -> boolean|number
    function mkdir(dir):
        return __system__direct__mkdir__(dir);
    end

    // dir:string -> boolean|number
    function removedirs(dir):
        return __system__direct__removedirs__(dir);
    end

    // dir:string -> boolean|number
    function rmdir(dir):
        return __system__direct__rmdir__(dir);
    end

    // src:string | des:string -> boolean|number
    function rename(src, des):
        return __system__direct__rename__(src, des);
    end

    // dir:string -> array|number
    function stat(dir):
        return __system__direct__stat__(dir);
    end

    // path:string -> string|number
    function abspath(path):
        return __system__direct__abspath__(path);
    end

    // path:string -> string|number
    function relative(path):
        return __system__direct__relative__(path);
    end

    // path:string -> string|number
    function basename(path):
        return __system__direct__basename__(path);
    end

    // paths:array -> string|null
    function commonprefix(paths):
        return __system__direct__commonprefix__(paths);
    end

    // path:string -> string|number
    function dirname(path):
        return __system__direct__dirname__(path);
    end

    // path:string -> boolean
    function exists(path):
        return __system__direct__exists__(path);
    end

    // path:string -> string|number
    function expand(path):
        return __system__direct__expand__(path);
    end

    // file:string -> number
    function getatime(file):
        return __system__direct__getatime__(file);
    end

    // file:string -> number
    function getmtime(file):
        return __system__direct__getmtime__(file);
    end

    // file:string -> number
    function getctime(file):
        return __system__direct__getctime__(file);
    end

    // file:string -> number
    function getsize(file):
        return __system__direct__getsize__(file);
    end

    // path:string -> boolean
    function isabs(path):
        return __system__direct__isabs__(path);
    end

    // path:string -> boolean
    function isfile(path):
        return __system__direct__isfile__(path);
    end

    // path:string -> boolean
    function isdir(path):
        return __system__direct__isdir__(path);
    end

    // path:string -> string|number
    function normcase(path):
        return __system__direct__normcase__(path);
    end

    // path:string -> string|number
    function normpath(path):
        return __system__direct__normpath__(path);
    end

    // path:string -> array|number
    function info(path):
        return __system__direct__info__(path);
    end

    // src:string | des:string -> number
    function samefile(src, des):
        return __system__direct__samefile__(src, des);
    end
}