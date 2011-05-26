<?php
/*
 * Fake "Threading" in PHP by using fork()
 * along with a socket pair for passing
 * serialized data around
 */

function async($thunk) {
    // By creating a socket pair, we have a channel of communication
    // between PHP processes
    socket_create_pair(AF_UNIX, SOCK_STREAM, 0, $sockets);

    list($parent, $child) = $sockets;

    if (($pid = pcntl_fork()) == 0) { // We're in the child process now
        socket_close($child);
        // Call the thunk, serialize the returned value, and send it
        // through the channel we created (ie. to the parent process)
        socket_write($parent, serialize(call_user_func($thunk)));
        socket_close($parent);

        exit; // We're done so we can exit here
    }

    socket_close($parent);

    return array($pid, $child); // This can treated as a "thread handle"
}

function wait($proc) {
    pcntl_waitpid($proc[0], $status); // Wait for the process to finish

    // Read the data from the channel, and then unserialize it into
    // a PHP object that can be returned
    $output = unserialize(socket_read($proc[1], 4096));
    socket_close($proc[1]);

    return $output;
}

//
// Usage Example
//

$thread = async(function () {
    // Note that variables can be passed through with the "use" construct
    // (ie. $thread = async(function() use($local_var) { ...

    sleep(2); // to show that we really are running asynchronously

    echo "Hello from Child Thread!\n";

    return array('hello', 'world!');
});

echo "Hello from Main Thread\n";

$output = wait($thread);

print_r($output);

// Prints:
//
// Hello from Main Thread
// (and then 2 seconds later)
// Hello from Child Thread
// Array
// (
//     [0] => hello
//     [1] => world!
// )
