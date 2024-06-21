#include <iomanip>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

// 定义Token类型
enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    DELIMITER,
    WHITESPACE,
    COMMENT,
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
        tokens.push_back({TokenType::OPERATOR, std::string(1, source[position]),
                          line, column});
        advance();
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
        return std::string("+-*/=").find(c) != std::string::npos;
    }

    bool isDelimiter(char c) {
        return std::string(";,(){}").find(c) != std::string::npos;
    }
};

int main() {

    std::string test_code = R"(
if(flag==1)
    while(i<=100)
        i=c[i]*d;
else 
    z[i,j]=a+k;
s=t;
    )";

    Lexer lexer(test_code);
    std::vector<Token> tokens = lexer.tokenize();

    std::cout << std::setw(7) << "单词" << std::setw(10) << "属性" << std::endl;

    for (const auto &token : tokens) {
        std::cout << std::setw(7) << token.value << std::setw(10)
                  << static_cast<int>(token.type) << std::endl;
    }

    system("pause");
    return 0;
}
