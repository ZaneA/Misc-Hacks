<?php
function with(&$orig, $new, $cb) { $temp = $orig; $orig = $new; call_user_func($cb); $orig = $temp; }

$blah = 'hello';

with($blah, 'world', function() {
        global $blah; 
        echo $blah."\n";
});

echo $blah."\n";
