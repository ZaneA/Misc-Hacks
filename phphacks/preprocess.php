<?php
$rep = array(
             '/sub\s+\{\s+\((.*?)\)\s+=\s+_@;/' => function ($matches) { return "function ({$matches[1]}) {"; },
             '/def/' => function ($matches) { return 'function'; },
             '/fn/' => function ($matches) { return 'function'; },
             '/qw\((.*?)\)/' => function ($matches) { return "explode(' ', '{$matches[1]}')"; },
             '/\[\s?([[:alnum:]]+:\s?.*)\s?]/' => function ($matches) { return 'array('.preg_replace('/([[:alnum:]]+):\s?(.*?),?\s?/', "'$1' => $3", $matches[1]).')'; }
);

$fp = fopen(__FILE__, 'r'); fseek($fp, __COMPILER_HALT_OFFSET__); $script = stream_get_contents($fp);
foreach ($rep as $match => $replacement) { $script = preg_replace_callback($match, $replacement, $script); }
eval($script); __halt_compiler();

fn hello($text) { echo "{$text}\n"; } 
def hello2() { } 

// Short assoc array syntax, useful for faking named arguments
print_r([ named: 'named', args: "args" ]);

// qw from perl
foreach (qw(hello world) as $word) {
        // More perl syntax just for kicks
        $say = sub {
                ($word) = _@;
                hello($word);
        };

        $say($word);
}
