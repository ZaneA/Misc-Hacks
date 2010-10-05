<?php
// Map functions to shorter versions

foreach (explode(' ', 'strtolower:lc strtoupper:uc array_map:map') as $funcmap) {
        list($func, $alias) = explode(':', $funcmap);
        eval("function $alias () { return call_user_func_array('$func', func_get_args()); }");
}

echo lc('Hello, World');
