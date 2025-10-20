#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Token types
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_TYPE,
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_CHAR,
    TOKEN_OPERATOR,
    TOKEN_ASSIGNMENT,
    TOKEN_COMPARISON,
    TOKEN_ARITHMETIC,
    TOKEN_LOGICAL,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_SEMICOLON,
    TOKEN_COMMA,
    TOKEN_DOT,
    TOKEN_EOF,
    TOKEN_UNKNOWN,
    TOKEN_PREPROCESSOR
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char lexeme[256];
    int line;
    int column;
} Token;

// Keyword arrays
const char *keywords[] = {
    "if", "else", "while", "for", "return", "break",
    "continue", "switch", "case", "default", "do"
};

const char *types[] = {
    "int", "float", "double", "char", "void", "bool",
    "short", "long", "signed", "unsigned"
};

const char *operators[] = {
    "+", "-", "*", "/", "%", "++", "--",
    "==", "!=", "<", ">", "<=", ">=",
    "&&", "||", "!", "&", "|", "^", "~",
    "<<", ">>", "=", "+=", "-=", "*=", "/=", "%="
};

// Function prototypes
bool is_keyword(const char *str);
bool is_type(const char *str);
bool is_operator(const char *str);
bool is_whitespace(char c);
bool is_digit(char c);
bool is_alpha(char c);
bool is_alphanumeric(char c);
Token *tokenize(const char *source, int *token_count);
void print_token(const Token *token);
void free_tokens(Token *tokens, int count);
const char *token_type_to_string(TokenType type);
char *read_file(const char *filename);
char *read_multiline_input();
void print_separator();
void print_menu();

bool is_keyword(const char *str) {
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool is_type(const char *str) {
    int num_types = sizeof(types) / sizeof(types[0]);
    for (int i = 0; i < num_types; i++) {
        if (strcmp(str, types[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool is_operator(const char *str) {
    int num_operators = sizeof(operators) / sizeof(operators[0]);
    for (int i = 0; i < num_operators; i++) {
        if (strcmp(str, operators[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool is_alphanumeric(char c) {
    return is_alpha(c) || is_digit(c);
}

const char *token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_TYPE: return "TYPE";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_INTEGER: return "INTEGER";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_STRING: return "STRING";
        case TOKEN_CHAR: return "CHAR";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_ASSIGNMENT: return "ASSIGNMENT";
        case TOKEN_COMPARISON: return "COMPARISON";
        case TOKEN_ARITHMETIC: return "ARITHMETIC";
        case TOKEN_LOGICAL: return "LOGICAL";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_DOT: return "DOT";
        case TOKEN_PREPROCESSOR: return "PREPROCESSOR";
        case TOKEN_EOF: return "EOF";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN";
    }
}

Token *tokenize(const char *source, int *token_count) {
    int capacity = 100;
    Token *tokens = malloc(capacity * sizeof(Token));
    *token_count = 0;

    int line = 1;
    int column = 1;
    int i = 0;
    int len = strlen(source);

    while (i < len) {
        if (is_whitespace(source[i])) {
            if (source[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
            continue;
        }

        if (*token_count >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens, capacity * sizeof(Token));
        }

        Token *current_token = &tokens[*token_count];
        current_token->line = line;
        current_token->column = column;

        // Preprocessor directives
        if (source[i] == '#') {
            int start = i;
            while (i < len && source[i] != '\n') {
                i++;
                column++;
            }
            strncpy(current_token->lexeme, &source[start], i - start);
            current_token->lexeme[i - start] = '\0';
            current_token->type = TOKEN_PREPROCESSOR;
            (*token_count)++;
            continue;
        }

        // Single-line comments
        if (source[i] == '/' && i + 1 < len && source[i + 1] == '/') {
            while (i < len && source[i] != '\n') {
                i++;
                column++;
            }
            continue;
        }

        // Multi-line comments
        if (source[i] == '/' && i + 1 < len && source[i + 1] == '*') {
            i += 2;
            column += 2;
            while (i < len && !(source[i] == '*' && i + 1 < len && source[i + 1] == '/')) {
                if (source[i] == '\n') {
                    line++;
                    column = 1;
                } else {
                    column++;
                }
                i++;
            }
            if (i < len) {
                i += 2;
                column += 2;
            }
            continue;
        }

        // Strings
        if (source[i] == '"') {
            int start = i;
            i++;
            column++;
            while (i < len && source[i] != '"') {
                if (source[i] == '\\' && i + 1 < len) {
                    i += 2;
                    column += 2;
                } else {
                    i++;
                    column++;
                }
            }
            if (i < len) {
                i++;
                column++;
            }
            strncpy(current_token->lexeme, &source[start], i - start);
            current_token->lexeme[i - start] = '\0';
            current_token->type = TOKEN_STRING;
            (*token_count)++;
            continue;
        }

        // Characters
        if (source[i] == '\'') {
            int start = i;
            i++;
            column++;
            while (i < len && source[i] != '\'') {
                if (source[i] == '\\' && i + 1 < len) {
                    i += 2;
                    column += 2;
                } else {
                    i++;
                    column++;
                }
            }
            if (i < len) {
                i++;
                column++;
            }
            strncpy(current_token->lexeme, &source[start], i - start);
            current_token->lexeme[i - start] = '\0';
            current_token->type = TOKEN_CHAR;
            (*token_count)++;
            continue;
        }

        // Numbers
        if (is_digit(source[i]) || (source[i] == '.' && i + 1 < len && is_digit(source[i + 1]))) {
            int start = i;
            bool has_dot = false;

            while (i < len && (is_digit(source[i]) || source[i] == '.' ||
                              source[i] == 'e' || source[i] == 'E' ||
                              source[i] == '+' || source[i] == '-')) {
                if (source[i] == '.') {
                    if (has_dot) break;
                    has_dot = true;
                }
                i++;
                column++;
            }

            strncpy(current_token->lexeme, &source[start], i - start);
            current_token->lexeme[i - start] = '\0';
            current_token->type = has_dot ? TOKEN_FLOAT : TOKEN_INTEGER;
            (*token_count)++;
            continue;
        }

        // Identifiers and keywords
        if (is_alpha(source[i])) {
            int start = i;
            while (i < len && is_alphanumeric(source[i])) {
                i++;
                column++;
            }

            strncpy(current_token->lexeme, &source[start], i - start);
            current_token->lexeme[i - start] = '\0';

            if (is_keyword(current_token->lexeme)) {
                current_token->type = TOKEN_KEYWORD;
            } else if (is_type(current_token->lexeme)) {
                current_token->type = TOKEN_TYPE;
            } else {
                current_token->type = TOKEN_IDENTIFIER;
            }
            (*token_count)++;
            continue;
        }

        // Operators and punctuation
        char single_char[2] = {source[i], '\0'};
        char double_char[3] = {0};

        if (i + 1 < len) {
            double_char[0] = source[i];
            double_char[1] = source[i + 1];
            double_char[2] = '\0';
        }

        if (is_operator(double_char)) {
            strcpy(current_token->lexeme, double_char);
            current_token->type = TOKEN_OPERATOR;
            i += 2;
            column += 2;
            (*token_count)++;
            continue;
        }

        if (is_operator(single_char)) {
            strcpy(current_token->lexeme, single_char);
            current_token->type = TOKEN_OPERATOR;
            i++;
            column++;
            (*token_count)++;
            continue;
        }

        // Punctuation
        switch (source[i]) {
            case '(':
                current_token->lexeme[0] = '(';
                current_token->lexeme[1] = '\0';
                current_token->type = TOKEN_LPAREN;
                break;
            case ')':
                current_token->lexeme[0] = ')';
                current_token->lexeme[1] = '\0';
                current_token->type = TOKEN_RPAREN;
                break;
            case '{':
                current_token->lexeme[0] = '{';
                current_token->lexeme[1] = '\0';
                current_token->type = TOKEN_LBRACE;
                break;
            case '}':
                current_token->lexeme[0] = '}';
                current_token->lexeme[1] = '\0';
                current_token->type = TOKEN_RBRACE;
                break;
            case '[':
                current_token->lexeme[0] = '[';
                current_token->lexeme[1] = '\0';
                current_token->type = TOKEN_LBRACKET;
                break;
            case ']':
                current_token->lexeme[0] = ']';
                current_token->lexeme[1] = '\0';
                current_token->type = TOKEN_RBRACKET;
                break;
            case ';':
                current_token->lexeme[0] = ';';
                current_token->lexeme[1] = '\0';
                current_token->type = TOKEN_SEMICOLON;
                break;
            case ',':
                current_token->lexeme[0] = ',';
                current_token->lexeme[1] = '\0';
                current_token->type = TOKEN_COMMA;
                break;
            case '.':
                current_token->lexeme[0] = '.';
                current_token->lexeme[1] = '\0';
                current_token->type = TOKEN_DOT;
                break;
            default:
                current_token->lexeme[0] = source[i];
                current_token->lexeme[1] = '\0';
                current_token->type = TOKEN_UNKNOWN;
                break;
        }

        i++;
        column++;
        (*token_count)++;
    }

    // Add EOF token
    if (*token_count >= capacity) {
        capacity += 1;
        tokens = realloc(tokens, capacity * sizeof(Token));
    }
    Token *eof_token = &tokens[*token_count];
    strcpy(eof_token->lexeme, "EOF");
    eof_token->type = TOKEN_EOF;
    eof_token->line = line;
    eof_token->column = column;
    (*token_count)++;

    return tokens;
}

void print_token(const Token *token) {
    printf("Line %3d, Col %3d: %-15s %s\n",
           token->line, token->column,
           token_type_to_string(token->type),
           token->lexeme);
}

void free_tokens(Token *tokens, int count) {
    free(tokens);
}

char *read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(length + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);

    return buffer;
}

char *read_multiline_input() {
    printf("\nEnter your C code (type 'END' on a new line to finish):\n");
    printf("--------------------------------------------------\n");

    int capacity = 1024;
    int length = 0;
    char *buffer = malloc(capacity);
    char line[256];

    while (fgets(line, sizeof(line), stdin)) {
        // Check if user typed END to finish
        if (strcmp(line, "END\n") == 0 || strcmp(line, "end\n") == 0) {
            break;
        }

        int line_len = strlen(line);

        // Resize buffer if needed
        while (length + line_len >= capacity) {
            capacity *= 2;
            buffer = realloc(buffer, capacity);
        }

        strcpy(buffer + length, line);
        length += line_len;
    }

    buffer[length] = '\0';
    return buffer;
}

void print_separator() {
    printf("==================================================\n");
}

void print_menu() {
    printf("\n");
    print_separator();
    printf("          C LEXICAL ANALYZER (TOKENIZER)         \n");
    print_separator();
    printf("1. Enter code directly\n");
    printf("2. Load from file\n");
    printf("3. Exit\n");
    print_separator();
    printf("Choose an option: ");
}

int main() {
    printf("\n*** Welcome to the C Lexical Analyzer ***\n");

    while (1) {
        print_menu();

        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        getchar(); // Consume newline

        char *source_code = NULL;

        if (choice == 1) {
            // Direct input
            source_code = read_multiline_input();

            if (strlen(source_code) == 0) {
                printf("No code entered!\n");
                free(source_code);
                continue;
            }

            printf("\n--------------------------------------------------\n");
            printf("Source code received:\n");
            printf("%s\n", source_code);

        } else if (choice == 2) {
            // File input
            char filename[256];
            printf("Enter filename: ");
            if (fgets(filename, sizeof(filename), stdin)) {
                filename[strcspn(filename, "\n")] = '\0'; // Remove newline
                source_code = read_file(filename);

                if (!source_code) {
                    continue;
                }

                printf("\nSource code from '%s':\n", filename);
                printf("%s\n", source_code);
            }

        } else if (choice == 3) {
            printf("\nThank you for using the C Lexical Analyzer!\n");
            break;

        } else {
            printf("Invalid choice! Please select 1, 2, or 3.\n");
            continue;
        }

        // Tokenize the source code
        printf("\nTokens:\n");
        print_separator();

        int token_count;
        Token *tokens = tokenize(source_code, &token_count);

        for (int i = 0; i < token_count; i++) {
            print_token(&tokens[i]);
        }

        printf("\nTotal tokens: %d\n", token_count);

        free_tokens(tokens, token_count);
        free(source_code);
    }

    return 0;
}

