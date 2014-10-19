// File: lex.h
// Author: Scott Walker
// Description: the header file for the lexical analyzer
int isletter(const char* c);
int isnumber(const char* c);
int is_space(const char* c);
int inttype();
int doubletype();
int invalid(const char* c);
int gettype(const char* c);
char* get_lexeme();
enum token_type get_token (FILE* input, char* buffer, size_t* buffersize);
