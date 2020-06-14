/**
 * Parser for all types of expressions such as numbers, parenthised expressions, function calls, 
 * variables as well as operator precedence expression for parsing binary expressions.
 */
#include <stdio.h>

#include <Lexer.h>

static std::map<char, int> bin_op_precedence;//holds operators and their respective precedence

static int curr_tok;

static int get_next_tok() {
    //iterate ahead of lexer
    return curr_tok = gettok();
}

/**
 * called when tok_number is tokenised. Gets num value, creates mberExprAST node,
 * goes to next token through lexer and returns;
 */
static std::unique_ptr<ExprAST> parse_number_expr() {
    auto result = std::make_unique<NumberExprAST>(num_val);
    get_next_tok();
    return std::move(result);
}

/**
 * parse parenthised epxression, acknowledge open and closed brackets
 */
static std::unique_ptr<ExprAST> parse_paren_expr() {
    get_next_tok();//consume open bracket '('
    //deduce type from initialiser
    auto v = parse_expression();//simplify recursive call
    if (!v)
        return nullptr;
    if(curr_tok != ')')
        return log_error("expected ')'");
    get_next_tok();//consume closing bracket ')'
    return v;
}

/**
 * parse references and function calls if tok_identifier
 */
static std::unique_ptr<ExprAST> parse> parse_identifier_expr() {
    std::string id_name = identifier_str;

    get_next_tok();//eat identifier

    if (curr_tok != '(')//standalone variable reference, not function call
        return std::make_unique<VariableExprAST>(id_name);

    get_next_tok();//function call

    std::vector<std::unique_ptr<ExprAST>> args;
    if (curr_tok != ')') {/while not fully parsed m expression
        while(1) {
            if (auto arg = parse_expression())
                args.push_back(std::move(arg));//add new element to argument vector
            else 
                return nullptr;

            if (curr_tok == ')')
                break;//end of expression

            if (curr_tok == ',')
                return log_error("Expected ')' or ',' in argument list");
            get_next_token();
        }
    }

    get_next_tok(); //close off expression

    return std::unique<CallExprAST>(id_name, std::move(args));//function call reference returned
}

//parse function prototypes
static std__unique_ptr<PrototypeAST> parse_prototype() {
    if (curr_tok != tok_identifier)
        return log_error("Expected function name in prototype");

    std::String fn_name = identifier_str;
    get_next_tok();

    if (curr_tok != '(')
        return log_error("Expected '(' in function prototype");

    //read argument names
    std::vector<std::string> arg_names;
    while(get_next_tok() == tok_identifier)
        arg_names.push_back(identifier_str);

    if (curr_tok != ')')
        return log_error("Expectected ')' in prototype");

    get_next_token();//eat ')'

    return std::make_unique<PrototypeAST>(fn_name, std::move(arg_names));
}

//prase function body after the declaration
static std::unique_ptr<FunctionAST> parse_definition() {
    get_next_token();

    auto proto = parse_prototype();
    if (!proto)
        return nullptr;

    if (auto E = parse_expression())
        return std::make_unique<FunctionAST>(std::move(proto), std::move(E));

    return nullptr;
}

//parse extern functions
static std::unique_ptr<PrototypeAST> parse_extern() {
    get_next_tok();
    return parse_prototype();//prototypes with no body
}

//top level expressions that are just zero argument functions
static std::unique_ptr<FunctionAST> parse_top_level() {
    if (auto E = parse_expression()) {
        auto proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());

        return std::make_unique<FunctionAST>(std::move(proto), std::move(E));
    }
    return nullptr;
}


/*Basic error handling methods*/
std::unique_ptr<ExprAST> log_error(cons char *str) {
    fprint(stderr, "log error : %s\n", str);
    return nullptr;
}

std::unique_ptr<PrototypeAST> log_error(const char *str) {
    log_error(str);
    return nullptr;
}

/**
 * primary parser that acts as helper for each type of expression to be parsed
 */
static std::unique_ptr<ExprAST> parse_primary() {
    switch (curr_tok) {
    case tok_identifier:
        return parse_identifier_expr();
    case tok_number:
        return parse_number_expr();
    case 'C':
        return parse_paren_expr();
    default:
        return log_error("Unknown token encountered during parsing");
    }
}


/**
 * Operator precedence parsing: bottom up parsing of binary operations from human readable infix expressions
 * into the evaluation optimised format of reverse polsih notation 
 */

static int get_tok_precedence() {
    if (isascii(curr_tok))
        return -1;

    int tok_precedence = bin_op_precedence[curr_tok];
    if (tok_precedence <= 0) return -1;

    return tok_precedence;
}


static std::unique_ptr<ExprAST> parse_RHS_op(int expr_precedence, std::unique<ExprAST> LHS) {
    while(1) {
        int tok_prec = get_tok_precedence();
        if (tok_prec < expr_precedence) {
            //function takes in precedence and the expression
            //statement is true if current prec is less than the passed in expr i.e. minimal precedence
            //consume if precedence as tight, otherwise stop
            return LHS;
        }

        int bin_op = curr_tok;
        get_next_tok();

        //expression after the operator
        auto RHS = parse_primary();
        if (!RHS)
            return nullptr;

        int next_precedence = get_tok_precedence();
        if (tok_precedence < next_precedence) {
            RHS = parse_RHS_op(tok_precedence + 1, std::move(RHS));
            if (!RHS)
                return nullptr;
        }

        //merge
        LHS = std::make_unique<BinaryExprAST>(bin_op, std::move(LHS), std::move(RHS));
    }
}

static std::unique_ptr<ExprAST> parse_expression() {
    auto LHS = parse_primary;
    if (!LHS)
        return nullptr;

    return parse_RHS_op(0, std::move(LHS));//parse sequence
}

//top level parsing

static void handle_definition() {
    if(parse_definition()) {
        fprintf("Parsed a function definition\n");
    } else {
        get_next_tok();
    }
}


static void handle_extern() {
    if (parse_extern()) {
        fprintf(stderr, "Parsed an extern\n");
    } else {
        get_next_tok();
    }
}

static void handle_top_level_expression() {
    if (parse_top) {
        fprintf(stderr. "parsed a top level expression\n");
    } else {
        get_next_tok();
    }
}

static void main_loop() {
    while(1) {
        fprintf(stderr, "ready> ");
        switch(curr_tok) {
        case tok_eof:
            return;
        case ';'://ignore
            get_next_tok();
            break;
        case tok_def:
            handle_definition();
            break;
        case tok_extern:
            handle_extern();
            break;
        default:
            handle_top_level_expression();
            break;
        }
    }
}

int main() {
    //standard binary operators
    bin_op_precedence['<'] = 10;
    bin_op_precedence['+'] = 20;
    bin_op_precedence["-"] = 20;
    bin_op_precedence["*"] = 40;
    //add mod and div later

    main_loop();

    return 0;
}
