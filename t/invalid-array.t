# vi:filetype=

use lib 'lib';
use Config ();
use Test::Nginx::Socket;

repeat_each(1);

plan tests => repeat_each() * 3 * blocks();

no_long_string();

run_tests();

__DATA__

=== TEST 1: array map reports the scalar value after a successful array operation
--- config
    location /foo {
        error_page 500 = @invalid_array;
        array_split ',' 'a,b' to=$array;
        set $scalar 'invalid';
        array_map '[$array_it]' $scalar;
        echo 'unexpected success';
    }

    location @invalid_array {
        return 500 "invalid array\n";
    }
--- request
GET /foo
--- error_code: 500
--- response_body
invalid array
--- grep_error_log eval
qr/array_map: invalid array variable value in the 2nd argument: "[^"]*"/
--- grep_error_log_out
array_map: invalid array variable value in the 2nd argument: "invalid"



=== TEST 2: array map op reports the scalar value rather than the directive name
--- config
    location /foo {
        error_page 500 = @invalid_array;
        array_split ',' 'a,b' to=$array;
        set $scalar 'invalid';
        array_map_op set_quote_sql_str $scalar;
        echo 'unexpected success';
    }

    location @invalid_array {
        return 500 "invalid array\n";
    }
--- request
GET /foo
--- error_code: 500
--- response_body
invalid array
--- grep_error_log eval
qr/array_map_op: invalid array variable value in the 2nd argument: "[^"]*"/
--- grep_error_log_out
array_map_op: invalid array variable value in the 2nd argument: "invalid"



=== TEST 3: array join reports the scalar value
--- config
    location /foo {
        error_page 500 = @invalid_array;
        array_split ',' 'a,b' to=$array;
        set $scalar 'invalid';
        array_join ',' $scalar;
        echo 'unexpected success';
    }

    location @invalid_array {
        return 500 "invalid array\n";
    }
--- request
GET /foo
--- error_code: 500
--- response_body
invalid array
--- grep_error_log eval
qr/array_join: invalid array variable value in the 2nd argument: "[^"]*"/
--- grep_error_log_out
array_join: invalid array variable value in the 2nd argument: "invalid"



=== TEST 4: array join rejects a scalar the same size as an array reference
--- config eval
my $scalar = 'a' x (2 * $Config::Config{ptrsize});
sprintf q{
    location /foo {
        error_page 500 = @invalid_array;
        set $scalar '%s';
        array_join ',' $scalar;
        echo 'unexpected success';
    }

    location @invalid_array {
        return 500 "invalid array\n";
    }
}, $scalar;
--- request
GET /foo
--- error_code: 500
--- response_body
invalid array
--- error_log
array_join: invalid array variable value in the 2nd argument:



=== TEST 5: array map op rejects an array reference with a null tag
--- config eval
my $scalar = '0' x (4 * $Config::Config{ptrsize});
sprintf q{
    location /foo {
        error_page 500 = @invalid_array;
        array_split ',' 'a,b' to=$array;
        set_decode_hex $scalar '%s';
        array_map_op set_quote_sql_str $scalar;
        echo 'unexpected success';
    }

    location @invalid_array {
        return 500 "invalid array\n";
    }
}, $scalar;
--- request
GET /foo
--- error_code: 500
--- response_body
invalid array
--- error_log
array_map_op: invalid array variable value in the 2nd argument:



=== TEST 6: array map rejects an invalid tag with a valid array pointer
--- config eval
my $digits = 2 * $Config::Config{ptrsize};
sprintf q{
    location /foo {
        error_page 500 = @invalid_array;
        array_split ',' 'a,b' to=$array;
        set_encode_hex $encoded $array;
        if ($encoded ~ "^.{%d}(.{%d})$") {
            set $bad_hex "%s$1";
        }
        set_decode_hex $scalar $bad_hex;
        array_map '[$array_it]' $scalar;
        echo 'unexpected success';
    }

    location @invalid_array {
        return 500 "invalid array\n";
    }
}, $digits, $digits, '0' x $digits;
--- request
GET /foo
--- error_code: 500
--- response_body
invalid array
--- error_log
array_map: invalid array variable value in the 2nd argument:
