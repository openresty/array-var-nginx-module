# vi:filetype=perl

use lib 'lib';
use Test::Nginx::Socket;

#repeat_each(3);
repeat_each(1);

plan tests => repeat_each() * 2 * blocks();

no_long_string();

run_tests();

#no_diff();

__DATA__

=== TEST 1: array split/join
--- config
    location /foo {
        array_split ',' $arg_names to=$names;
        array_join '+' $names;
        echo $names;
    }
--- request
GET /foo?names=Bob,Marry,John
--- response_body
Bob+Marry+John



=== TEST 2: array split/join (single item)
--- config
    location /foo {
        array_split ',' $arg_names to=$names;
        array_join '+' $names;
        echo $names;
    }
--- request
GET /foo?names=nomas
--- response_body
nomas



=== TEST 3: array split/join (empty array)
--- config
    location /foo {
        array_split ',' $arg_names to=$names;
        array_join '+' $names;
        echo "[$names]";
    }
--- request
GET /foo?
--- response_body
[]



=== TEST 4: array split/join (list of empty values)
--- config
    location /foo {
        array_split ',' $arg_names to=$names;
        array_join '+' $names;
        echo "[$names]";
    }
--- request
GET /foo?names=,,,
--- response_body
[+++]


=== TEST 4: array map
--- config
    location /foo {
        array_split ',' $arg_names to=$names;
        array_map 'hi' $names;
        array_join '+' $names;
        echo "[$names]";
    }
--- request
GET /foo?names=,,,
--- response_body
[hi+hi+hi+hi]


=== TEST 4: array map
--- config
    location /foo {
        array_split ',' $arg_names to=$names;
        array_map '[$array_it]' $names;
        array_join '+' $names;
        echo "$names";
    }
--- request
GET /foo?names=bob,marry,nomas
--- response_body
[bob]+[marry]+[nomas]

