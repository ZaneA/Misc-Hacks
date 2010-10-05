<?php
// Demonastrates method calling within string interpolation
// Increases memory usage by ~300kb

$df = get_defined_functions();
foreach (array_merge($df['internal'], $df['user']) as $func)
        $$func = $func;

echo "String length of \"hello world\" is {$strlen('hello world')}\n";
