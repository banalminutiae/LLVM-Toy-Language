/**
 * Abstract syntax tree: Each object for each language construct should be organised 
 * sensibly for later compilation stages to easily work with.
 * Objects represented: expressions, prototypes, function objects
 */

//base class for expressions
class ExprAST {
public:
    virtual ~ExprAST() {}
};

//number literal expression class
class NumberExprAST: public ExprAST {
    double val;

public:
    NumberExprAst(double val): val(val) {}//member initialisation
};                                        //init val param to val var

//more subclasses
class VariableExprAST: public ExprAST {
    std::string name;
public:
    VariableExprAST(const std::string &name) : name(name) {}
};
    
//binary operator expr subclass
class BinaryExprAST: public ExprAST {
    //+, _ etc
    char op;
    //owns an object through the ptr, discards when out of scope
    std::unique_ptr<ExprAST> LHS, RHS;
public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS) :
        op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {};//move: safe transfer of objects
};

//function calls
class CallExprAST: public ExprAST {
    std::string callee;
    std::vector<std::unique_ptr<ExprAST>> args;
public:
    CallExprAst(cost std::string &callee, std:vector<std::unique_ptr<ExprAst>> args) :
        callee(callee), args(std::move(args)) {}
};
    
//prototypes for functions and their parameters
class PrototypeAST: public ExprAST {
    std::string name;
    std::vector<std::string> args;

public:
    PrototypeAST(const std::string &name, std::vector<std::string> args) :
        name(name), args(std::move(args)) {}

    //getter
    const std::string &get_name() const {return name; }
};

//function definition itself
class FunctionAST: public ExprAST {
    std::unique_ptr<PrototypeAST> proto;
    std::unique_ptr<ExprAST> body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<ExprAST> body) :
        proto(std::move(proto), Body(std::move(proto)) {}
};
