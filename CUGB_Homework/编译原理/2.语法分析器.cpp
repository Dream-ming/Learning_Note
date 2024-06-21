#include <iomanip>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

// 定义Token类型
enum class TokenType {
    KEYWORD,
    IDENTIFIER, // 标识符
    NUMBER,
    OPERATOR,
    DELIMITER, // 界符
    WHITESPACE,
    COMMENT, // 注释
    UNKNOWN
};

// 定义Token结构体
struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
};

// 关键字集合
const std::unordered_map<std::string, TokenType> keywords = {
    {"if", TokenType::KEYWORD},    {"else", TokenType::KEYWORD},
    {"while", TokenType::KEYWORD}, {"return", TokenType::KEYWORD},
    {"int", TokenType::KEYWORD},   {"float", TokenType::KEYWORD}};

class Lexer {
  public:
    Lexer(const std::string &source)
        : source(source), position(0), line(1), column(1) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        while (position < source.length()) {
            char current_char = source[position];

            if (isspace(current_char)) { // 空格，回车，制表符
                handleWhitespace(tokens);
            } else if (isdigit(current_char)) { // 数字
                handleNumber(tokens);
            } else if (isalpha(current_char) ||
                       current_char == '_') { // 标识符（变量）
                handleIdentifierOrKeyword(tokens);
            } else if (isOperator(current_char)) { // 运算符: +,-,*,/,%,=,==,<=
                handleOperator(tokens);
            } else if (isDelimiter(current_char)) { // 界符: () , ; [] {}
                handleDelimiter(tokens);
            } else {
                handleUnknown(tokens);
            }
        }
        return tokens;
    }

    void showTokens(const std::vector<Token> &tokens) {
        std::cout << std::setw(7) << "单词" << std::setw(6) << "属性"
                  << std::setw(4) << "行" << std::setw(4) << "列" << std::endl;

        for (const auto &token : tokens) {
            std::cout << std::setw(7) << token.value << std::setw(6)
                      << static_cast<int>(token.type) << std::setw(4)
                      << token.line << std::setw(4) << token.column
                      << std::endl;
        }
    }

  private:
    std::string source;
    size_t position;
    int line;
    int column;

    char peek() {
        return position + 1 < source.length() ? source[position + 1] : '\0';
    }

    void advance() {
        position++;
        column++;
    }

    void handleWhitespace(std::vector<Token> &tokens) {
        while (position < source.length() && isspace(source[position])) {
            if (source[position] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            position++;
        }
    }
    // isspace、isdigit是c++自带的
    void handleNumber(std::vector<Token> &tokens) {
        size_t start_pos = position;
        while (position < source.length() && isdigit(source[position])) {
            advance();
        }
        tokens.push_back({TokenType::NUMBER,
                          source.substr(start_pos, position - start_pos), line,
                          column});
    }
    // isalnum(是否字母或数字)
    void handleIdentifierOrKeyword(std::vector<Token> &tokens) {
        size_t start_pos = position;
        while (position < source.length() &&
               (isalnum(source[position]) || source[position] == '_')) {
            advance();
        }
        std::string value = source.substr(start_pos, position - start_pos);
        TokenType type =
            keywords.count(value) ? keywords.at(value) : TokenType::IDENTIFIER;
        tokens.push_back({type, value, line, column});
    }

    void handleOperator(std::vector<Token> &tokens) {
        size_t start_pos = position;
        while (position < source.length() && (isOperator(source[position]))) {
            advance();
        }
        tokens.push_back({TokenType::OPERATOR,
                          source.substr(start_pos, position - start_pos), line,
                          column});
    }

    void handleDelimiter(std::vector<Token> &tokens) {
        tokens.push_back({TokenType::DELIMITER,
                          std::string(1, source[position]), line, column});
        advance();
    }

    void handleComment(std::vector<Token> &tokens) {
        while (position < source.length() && source[position] != '\n') {
            advance();
        }
    }

    void handleUnknown(std::vector<Token> &tokens) {
        tokens.push_back({TokenType::UNKNOWN, std::string(1, source[position]),
                          line, column});
        advance();
    }

    bool isOperator(char c) {
        return std::string("+-*/<>=").find(c) != std::string::npos;
    }

    bool isDelimiter(char c) {
        return std::string(";,(){}").find(c) != std::string::npos;
    }
};

class Parser {
  public:
    Parser(std::vector<Token> &tokens) : tokens(tokens), position(0), num(0) {}

    void parserStart() { stmts(); }

  private:
    std::vector<Token> tokens;
    size_t position;
    size_t num;

    void nextStep() {
        std::cout << num << "\t";
        ++num;
    }

    Token current_token() { return tokens[position]; }

    void show() {
        std::cout << tokens[position].value << "\t\t\t";
        for (int i = position + 1; i < tokens.size(); ++i) {
            std::cout << tokens[i].value;
        }
        std::cout << std::endl;
    }

    void stmts() {
        nextStep();
        std::cout << "stmts -> stmt rest0\t\t";
        show();
        stmt();
        rest0();
        std::cout << "\n分析成功" << std::endl;
    }

    void stmt() {
        if (current_token().value == "if") {
            nextStep();
            std::cout << "stmt -> if(bool) stmt else stmt\t";
            show();

            position++; // (
            nextStep();
            std::cout << "stmt -> if(bool) stmt else stmt\t";
            show();

            getBool();

            nextStep();
            std::cout << "stmt -> if(bool) stmt else stmt\t";
            show();
            position++; // )
            stmt();

            nextStep();
            std::cout << "stmt -> if(bool) stmt else stmt\t";
            show();
            position++; // else
            stmt();
        } else if (current_token().value == "while") {
            nextStep();
            std::cout << "stmt -> while(bool) stmt\t";
            show();
            position++; // (
            nextStep();
            std::cout << "stmt -> while(bool) stmt\t";
            show();
            getBool();
            nextStep();
            std::cout << "stmt -> while(bool) stmt\t";
            show();
            position++; // )
            stmt();
        } else {
            nextStep();
            std::cout << "stmt -> loc = expr;\t\t";
            show();
            loc();
            nextStep();
            std::cout << "stmt -> loc = expr;\t\t";
            show();
            position++;
            expr();
            nextStep();
            std::cout << "stmt -> loc = expr;\t\t";
            show();
            position++;
        }
    }

    void rest0() {
        nextStep();
        if (position < tokens.size()) {
            std::cout << "rest0 -> stmt rest0\t\t";
            show();
            stmt();
            rest0();
        } else {
            std::cout << "rest0 -> null\t\t";
        }
    }

    void getBool() {
        position++;
        nextStep();
        std::cout << "bool -> equality\t\t";
        show();
        equality();
    }

    void loc() {
        nextStep();
        std::cout << "loc -> id resta\t\t\t";
        show();
        id();
        resta();
    }

    void id() {
        nextStep();
        std::cout << "id -> " << current_token().value << "    \t\t\t";
        show();
        position++;
    }

    void resta() {
        nextStep();
        if (current_token().value == "[") {
            std::cout << "resta -> [elist]\t\t";
            show();
            position++;
            elist();
            nextStep();
            std::cout << "resta -> [elist]\t\t";
            show();
            position++;
        } else {
            std::cout << "resta -> null\t\t\t";
            show();
        }
    }

    void elist() {
        nextStep();
        std::cout << "elist -> expr rest1\t\t";
        show();
        expr();
        rest1();
    }

    void rest1() {
        nextStep();
        if (current_token().value == ",") {
            std::cout << "rest1 -> ,expr rest1\t\t";
            show();
            position++;
            expr();
            rest1();
        } else {
            std::cout << "rest1 -> null\t\t\t";
            show();
        }
    }

    void equality() {
        nextStep();
        std::cout << "equality -> rel rest4\t\t";
        show();
        rel();
        rest4();
    }

    void rel() {
        nextStep();
        std::cout << "rel -> expr rop_expr\t\t";
        show();
        expr();
        rop_expr();
    }

    void rest4() {
        nextStep();
        if (current_token().value == "==") {
            std::cout << "rest4 -> ==rel rest4\t\t";
            show();
            position++;
            rel();
            rest4();
        } else if (current_token().value == "!=") {
            std::cout << "rest4 -> !=rel rest4\t\t\t";
            show();
            position++;
            rel();
            rest4();
        } else {
            std::cout << "rest4 -> null\t\t\t";
            show();
        }
    }

    void expr() {
        nextStep();
        std::cout << "expr -> term rest5\t\t";
        show();
        term();
        rest5();
    }

    void rop_expr() {
        nextStep();
        if (current_token().value == "<") {
            std::cout << "rop_expr -> <expr\t\t";
            show();
            position++;
            expr();
        } else if (current_token().value == "<=") {
            std::cout << "rop_expr -> <=expr\t\t";
            show();
            position++;
            expr();
        } else if (current_token().value == ">") {
            std::cout << "rop_expr -> >expr\t\t";
            show();
            position++;
            expr();
        } else if (current_token().value == ">=") {
            std::cout << "rop_expr -> >=expr\t\t";
            show();
            position++;
            expr();
        } else {
            std::cout << "rop_expr -> null\t\t";
            show();
        }
    }

    void term() {
        nextStep();
        std::cout << "term -> unary rest6\t\t";
        show();
        unary();
        rest6();
    }

    void rest5() {
        nextStep();
        if (current_token().value == "+") {
            std::cout << "rest5 -> +term rest5\t\t";
            show();
            position++;
            term();
            rest5();
        } else if (current_token().value == "-") {
            std::cout << "rest5 -> -term rest5\t\t";
            show();
            position++;
            term();
            rest5();
        } else {
            std::cout << "rest5 -> null\t\t\t";
            show();
        }
    }

    void unary() {
        nextStep();
        std::cout << "unary -> factor\t\t\t";
        show();
        factor();
    }

    void rest6() {
        nextStep();
        if (current_token().value == "*") {
            std::cout << "rest6 -> *unary rest6\t\t";
            show();
            position++;
            unary();
            rest6();
        } else if (current_token().value == "/") {
            std::cout << "rest6 -> /unary rest6\t\t";
            show();
            position++;
            unary();
            rest6();
        } else {
            std::cout << "rest6 -> null\t\t\t";
            show();
        }
    }

    void factor() {
        nextStep();
        if (current_token().value == "(") {
            std::cout << "factor -> (expr)\t\t\t";
            show();
            expr();
        } else if (current_token().type == TokenType::NUMBER) {
            std::cout << "factor -> num\t\t\t";
            show();
            getNum();
        } else {
            std::cout << "factor -> loc\t\t\t";
            show();
            loc();
        }
    }

    void getNum() {
        nextStep();
        std::cout << "num -> " << current_token().value << "\t\t\t";
        show();
        position++;
    }
};

int main() {

    std::string test_code = R"(
{if(flag==1)
    while(i<=100)
        i=c[i]*d;
else 
    z[i,j]=a+k;
s=t;}
    )";

    std::cout << "输入源代码:" << std::endl << test_code << std::endl;

    Lexer lexer(test_code);
    std::vector<Token> tokens = lexer.tokenize();
    // lexer.showTokens(tokens);

    int num = 0;
    std::cout << "步骤\t文法\t\t\t\t分析字符\t\t剩余串\n";
    Parser parser(tokens);
    parser.parserStart();

    system("pause");
    return 0;
}
