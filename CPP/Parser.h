#ifndef PARSER_H
#define PARSER_H

static std::unique_ptr<ExprAST> parse_primary()
static std::unique_ptr<ExprAST> parse_number_expr();
static std::unique_ptr<ExprAST> parse_paren_expr();
static std::unique_ptr<ExprAST> parse> parse_identifier_expr();

std::unique_ptr<ExprAST> log_error(cons char *str);
std::unique_ptr<PrototypeAST> log_error(const char *str);

#endif
