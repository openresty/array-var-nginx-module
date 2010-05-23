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



=== TEST 2: array split (empty split sep)
--- config
    location /foo {
        array_split '' $arg_names to=$names;
        array_join '+' $names;
        echo $names;
    }
--- request
GET /foo?names=Bob
--- response_body
B+o+b



=== TEST 3: array split (empty split/join sep)
--- config
    location /foo {
        array_split '' $arg_names to=$names;
        array_join '' $names;
        echo [$names];
    }
--- request
GET /foo?names=Bob
--- response_body
[Bob]



=== TEST 4: array split (empty split + empty input)
--- config
    location /foo {
        array_split '' $arg_names to=$names;
        array_join '+' $names;
        echo [$names];
    }
--- request
GET /foo?
--- response_body
[]



=== TEST 5: array split/join (single item)
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



=== TEST 6: array split/join (empty array)
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



=== TEST 7: array split/join (multi-char sep)
--- config
    location /foo {
        array_split '->' $arg_names to=$names;
        array_join '(+)' $names;
        echo "$names";
    }
--- request
GET /foo?names=a->b->c
--- response_body
a(+)b(+)c



=== TEST 8: array split/join (list of empty values)
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



=== TEST 9: array map
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



=== TEST 10: array map (in-place)
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



=== TEST 11: array map (copy)
--- config
    location /foo {
        array_split ',' $arg_names to=$names;
        array_map '[$array_it]' $names to=$names2;
        array_join '+' $names;
        array_join '+' $names2;
        echo "$names";
        echo "$names2";
    }
--- request
GET /foo?names=bob,marry,nomas
--- response_body
bob+marry+nomas
[bob]+[marry]+[nomas]



=== TEST 12: array map (empty values)
--- config
    location /foo {
        array_split ',' $arg_names to=$names;
        array_map '[$array_it]' $names;
        array_join '+' $names;
        echo "$names";
    }
--- request
GET /foo?names=,marry,nomas
--- response_body
[]+[marry]+[nomas]



=== TEST 13: non-in-place join
--- config
    location /foo {
        array_split ',' $arg_names to=$names;
        array_join '+' $names to=$res;
        array_join '-' $names to=$res2;
        echo $res;
        echo $res2;
    }
--- request
GET /foo?names=bob,marry,nomas
--- response_body
bob+marry+nomas
bob-marry-nomas



=== TEST 14: non-in-place join
--- config
    location /foo {
        array_split ',' $arg_names to=$names;
        array_join '+' $names to=$res;
        array_join '-' $names to=$res2;
        echo $res;
        echo $res2;
    }
--- request
GET /foo?names=bob,marry,nomas
--- response_body
bob+marry+nomas
bob-marry-nomas



=== TEST 15: map op (in-place)
--- config
    location /foo {
        array_split ',' $arg_names to=$names;
        array_map_op set_quote_sql_str $names;
        array_join '+' $names to=$res;
        echo $res;
    }
--- request
GET /foo?names=bob,marry,nomas
--- response_body
'bob'+'marry'+'nomas'



=== TEST 16: map op (copy)
--- config
    location /foo {
        array_split ',' $arg_names to=$names;
        array_map_op set_quote_sql_str $names to=$list;
        array_join '+' $list to=$res;
        echo $res;
    }
--- request
GET /foo?names=bob,marry,nomas
--- response_body
'bob'+'marry'+'nomas'



=== TEST 17: map op (quote special chars)
--- config
    location /foo {
        array_split ',' $arg_names to=$names;
        array_map_op set_quote_sql_str $names;
        array_join '+' $names to=$res;
        echo $res;
    }
--- request
GET /foo?names=',\
--- response_body
'\''+'\\'



=== TEST 18: $array_it gets cleared after array map
--- config
    location /foo {
        array_split ',' $arg_names to=$names;
        array_map '[$array_it]' $names;
        echo "[$array_it]";
    }
--- request
GET /foo?names=bob,marry,nomas
--- response_body
[]

