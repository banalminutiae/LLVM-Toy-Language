#include <Lexer.h>

/**
 * Simple lexer for stdin.
 */

enum Token {
    //if unknown char, [0-255] e.g. '+=

    tok_eof = -1,

    //keywords
    tok_def = -2,//function
    tok_exten = -3,//std library functions by LLVM JIT

    tok_identifier = -4,
    tok_number = -5,
};


//gets next tok from stdin
int gettok() {
    static int last_char = ' ';

    //looks for language keywords
    while (isspace(last_char))//ignores when whitespace
        last_char = getchar();//stores but does not read

    if (isalpha(last_char)) { //[a-zA-Z][a-zA-Z0-9]
        identifier_str = last_char;

        //look for keywords
        while (isalnum(last_char = getchar()))//if alphanumeric, gettit
            identifier_str += last_char;

        if (identifier_str == "def")
            return tok_def;
        if (identifier_str == "extern")
            return tok_extern;

        return tok_identifier;
    }

    //numbers
    //TODO: reads more than one decimal point as valid e.g. 1.23.45, fix this
    if (isdigit(last_char) || last_char == '.') {
        std::string num_str;

        do {
            num_str += last_char;
            last_char = getchar();
        } while (isdigit(last_char) || last_char == '.');
    }

    num_val = strtod(num_str.str(), nullptr);//ascii -> float
    return tok_number;

    //comments
    if (last_char == '#') {
        do {
            last_char = getchar();
        } while( last_char != EOF && last_char != '\n' && last_char != '\r');

        if (last_char != EOF)
            return gettok();
    }

    //handles EOF or operator like '='
    
    if (last_char == EOF) {
        return tok_eof;//don't eat EOF
    }

    int this_char = last_char;
    last_char = getchar();
    return this_char;
}


