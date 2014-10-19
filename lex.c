// File: lex.c
// Author: Scott Walker
// Description: the implementation of the lexical analyzer

#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0

//                     0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23
const int table[48][25] = 
                    {{ 0,  0, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46,  2, 46, 46, 46, 46, 46, 46}, // 0
                     {46,  1,  3, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 1
                     {46,  2, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46,  2, 46, 46, 46, 46, 46, 46}, // 2 
                     {45,  3, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45}, // 3 
                     {46, 46, 46,  5, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 4 
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 5
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 6
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 7
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 8 
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 9
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 10 
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 11 
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 12
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 13
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 14
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 15
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 16
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 17
                     {46, 46, 46,  6, 23, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 18
                     {46, 46, 46,  7, 46, 24, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 19
                     {46, 46, 46, 11, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 20
                     {46, 46, 46, 14, 46, 46, 46, 46, 36, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 21
                     {46, 46, 46, 10, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 22
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 23
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 24
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 25
                     {46, 46, 46, 28, 46, 46, 46, 46, 46, 37, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 26
                     {46, 46, 46, 29, 46, 46, 46, 46, 36, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 27
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 28
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 29
                     {46, 46, 46, 13, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 30
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 31
                     {46, 46, 46, 25, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 32
                     {46, 46, 46, 15, 46, 46, 46, 46, 46, 46, 46, 30, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 33
                     {46, 46, 46, 17, 46, 46, 46, 46, 46, 46, 46, 46, 46, 31, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 34
                     {46, 46, 46, 16, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 35
                     {46, 46, 46, 13, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 36
                     {46, 46, 46, 12, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 37
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 38
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 39
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 40
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 41
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 42
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 43
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 44
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 45
                     {46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46}, // 46
                     { 0,  1, 45,  4, 18, 19, 22, 20, 27, 26, 21, 33, 35, 34, 32, 38, 39, 40,  2, 41, 42, 43, 44, 46, 45} // 47
                    };
//                     0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23

enum token_type {ERROR, IDENT, INTTYPE, DBLTYPE, NUMLIT,
        PLUS, MINUS, MULT, DIV, MOD, EXP, NOT, AND, OR, 
        XOR, ASSIGN, LT, GT, SHIFTL, SHIFTR, PLUSPLUS, PLUSEQ,
        MINUSMINUS, MINUSEQ, MULTEQ, DIVEQ, MODEQ, EXPEQ, NOTEQ,
        LOGAND, ANDEQ, LOGOR, OREQ, XOREQ, EQUALTO, SHIFTLEQ,
        LTE, SHIFTREQ, GTE, TILDA, RPAREN, LPAREN, SEMI, QUEST,
        COLON, COMMA, EOFT};

char * token_names [48] = {"ERROR\0", "IDENT\0", "INTTYPE\0", "DBLTYPE\0", "NUMLIT\0", 
    "PLUS\0", "MINUS\0", "MULT\0", "DIV\0", "MOD\0", "EXP\0", "NOT\0", "AND\0", "OR\0", 
    "XOR\0", "ASSIGN\0", "LT\0", "GT\0", "SHIFTL\0", "SHIFTR\0", "PLUSPLUS\0", "PLUSEQ\0", 
    "MINUSMINUS\0", "MINUSEQ\0", "MULTEQ\0", "DIVEQ\0", "MODEQ\0", "EXPEQ\0", "NOTEQ\0", 
    "LOGAND\0", "ANDEQ\0", "LOGOR\0", "OREQ\0", "XOREQ\0", "EQUALTO\0", "SHIFTLEQ\0", 
    "LTE\0", "SHIFTREQ\0", "GTE\0", "TILDA\0", "RPAREN\0", "LPAREN\0", "SEMI\0", "QUEST\0", 
    "COLON\0", "COMMA\0", "EOFT\0"};

char lexeme[32] = "\0";

// This function checks to see if a character is a letter
// Paramters: a character c
// Returns: 1 if a letter, 0 if not
int isletter(const char* c)
{
        if ((*c >= 65 && *c <= 90) || (*c >= 97 && *c <= 122))
        {
            return 1;
        }
        return 0;
}

// This function checks to see if a character is a number
// Parameters: a character c
// Returns: 1 if a number, 0 if not
int isnumber(const char* c)
{
        if (*c >= 48 && *c <= 57)
        {
            return 1;
        }
        return 0;
}

// This function checks to see if ahcaracter is a number
// Paramters: a character c
// Returns: 1 if a number, 0 if not
int isnum(const char c)
{
    if (c >= 48 && c <= 57)
    {
        return 1;
    }
    return 0;
}

// This function checks to see if a character is whitespace
// Parameters: a character c
// Returns: 1 if whitespace, 0 if not
int is_space(const char* c)
{
    if (*c == ' ' || *c == '\n' || *c == '\t')
    {
        return 1;
    }
    return 0;
}

// This function checks to see if a character array contains 'int'
// Parameters: none because it uses a global
// Returns: 1 if array is 'int', 0 if not
int inttype()
{
    if (lexeme[0] == 'i' && lexeme[1] == 'n' && lexeme[2] == 't')
    {
        return 1;
    }
    return 0;
}

// This function checks to see if a character array contains 'double'
// Parameters: none because it uses a global
// Returns: 1 if array is 'double', 0 if not
int doubletype()
{
    if (lexeme[0] == 'd' && lexeme[1] == 'o' && lexeme[2] == 'u' && lexeme[3] == 'b' && lexeme[4] == 'l' && lexeme[5] == 'e')
    {
        return 1;
    }
    return 0;
}

// This function checks to see if a character is valid for this lexical analyzer
// Parameters: a character c
// Returns: 0 if character is valid, 1 if not
int invalid(const char* c)
{
    if (isletter(c) || isnumber(c))
    {
        return 0;
    }
    char valid[27] = {0, 32, 33, 37, 38, 40, 41, 42, 43, 44, 45, 46, 47, 58, 59, 60, 61, 62, 63, 94, 95, 124, 126, '\0', '\t', '\n'};
    int i = 0;
    for (i; i < 27; i++)
    {
        if (*c == valid[i])
        {
            return 0;
        }
    }
    return 1;
}

// This function assigns a type integer to a certain character
// Parameters: a character c
// Returns: an integer code
int gettype(const char* c)
{ 
    switch (*c)
    {
        case 'a':
            return 0;
        case '0':
            return 1;
        case '.':
            return 2;
        case '=':
            return 3;
        case '+':
            return 4;
        case '-':
            return 5;
        case '*':
            return 6;
        case '/':
            return 7;
        case '<':
            return 8;
        case '>':
            return 9;
        case '%':
            return 10;
        case '&':
            return 11;
        case '^':
            return 12;
        case '|':
            return 13;
        case '!':
            return 14;
        case '~':
            return 15;
        case '(':
            return 16;
        case ')':
            return 17;
        case '_':
            return 18;
        case ';':
            return 19;
        case '?':
            return 20;
        case ':':
            return 21;
        case ',':
            return 22;
        default:
            return 23;
    }
}

// This function accesses the global lexeme variable
// Parameters: none
// Returns: a pointer to the lexeme variable
char* get_lexeme()
{
    return lexeme;
}

// This function breaks down the contents of a file into lexemes and prints their corresponding code
// Parameters: a file pointer, a character array pointer, a pointer to the size of the buffer
enum token_type get_token (FILE* input, char* buffer, size_t* buffersize)
{
    int type = 0;
    static int lineno = 0;
    static ssize_t linesize = -1;
    static int pos = 0;
    int state = 47;
    int next_state = 47;
    // check to see if file is opened
    if (input == NULL)
    {
        return EOFT;
    }
    // check to see if a new line needs to be read in
    if (pos >= linesize - 1)
    {
        linesize = -1;
        linesize = getline(&buffer, buffersize, input);
        if (linesize == -1)
        {
            return EOFT;
        }
        buffer[linesize] = '\0';
        lineno++;
        printf("\t%i: %s", lineno, buffer);
        pos = 0;
    }
    // remove leading whitespace
    while (isspace(buffer[pos])) pos++;
    int i = 0, j = 0;
    state = 47;
    // generate the lexemes and return their code
    while(1)
    {
        char c = buffer[pos];
        if (isletter(&c))
        {
            c = 'a';
        }
        else if (isnumber(&c))
        {
            c = '0';
        }
        else if (is_space(&c))
        {
            type = 23;
        }
        else if (invalid(&c))
        {
            next_state = 45;
        }
        type = gettype(&c);
        // check to see if the next state indicates that the lexeme has ended
        next_state = table[state][type];
        if (DEBUG) printf("\t\t\tnext state is %i state is %i type is %i\n", next_state, state, type);
        // lexeme over, find its type
        
        if (next_state >= 45)
        {
            if (state == 46 && DEBUG)
            {
                printf("state error on %c(%i) at %i,%i\n", buffer[pos], c, lineno, pos);
                printf("state is %i and next state is %i\n", state, next_state);
            }
            if (type == 23 && state == 46)
            {
                state = 45;
            }

            switch (state)
            {
                case 0:
                        if (inttype())
                        {
                            lexeme[i] = '\0';
                            return INTTYPE;
                        }
                        else if (doubletype())
                        {
                            lexeme[i] = '\0';
                            return DBLTYPE;
                        }
                        lexeme[i] = '\0';
                        return IDENT;
                case 1:
                    lexeme[i] = '\0';
                    return NUMLIT;
                case 3:
                    lexeme[i] = '\0';
                    return NUMLIT;
                case 4:
                    lexeme[i] = '\0';
                    return ASSIGN;
                case 5:
                    lexeme[i] = '\0';
                    return EQUALTO;
                case 6:
                    lexeme[i] = '\0';
                    return PLUSEQ;
                case 7:
                    lexeme[i] = '\0';
                    return MINUSEQ;
                case 8:
                    lexeme[i] = '\0';
                    return LTE;
                case 9:
                    lexeme[i] = '\0';
                    return GTE;
                case 10:
                    lexeme[i] = '\0';
                    return MULTEQ;
                case 11:
                    lexeme[i] = '\0';
                    return DIVEQ;
                case 12:
                    lexeme[i] = '\0';
                    return SHIFTREQ;
                case 13:
                    lexeme[i] = '\0';
                    return SHIFTLEQ;
                case 14:
                    lexeme[i] = '\0';
                    return MODEQ;
                case 15:
                    lexeme[i] = '\0';
                    return ANDEQ;
                case 16:
                    lexeme[i] = '\0';
                    return XOREQ;
                case 17:
                    lexeme[i] = '\0';
                    return OREQ;
                case 18:
                    lexeme[i] = '\0';
                    return PLUS;
                case 19:
                    lexeme[i] = '\0';
                    return MINUS;
                case 20:
                    lexeme[i] = '\0';
                    return DIV;
                case 21:
                    lexeme[i] = '\0';
                    return MOD;
                case 22:
                    lexeme[i] = '\0';
                    return MULT;
                case 23:
                    lexeme[i] = '\0';
                    return PLUSPLUS;
                case 24:
                    lexeme[i] = '\0';
                    return MINUSMINUS;
                case 25:
                    lexeme[i] = '\0';
                    return NOTEQ;
                case 26:
                    lexeme[i] = '\0';
                    return GT;
                case 27:
                    lexeme[i] = '\0';
                    return LT;
                case 28:
                    lexeme[i] = '\0';
                    return GTE;
                case 29:
                    lexeme[i] = '\0';
                    return LTE;
                case 30:
                    lexeme[i] = '\0';
                    return LOGAND;
                case 31:
                    lexeme[i] = '\0';
                    return LOGOR;
                case 32:
                    lexeme[i] = '\0';
                    return NOT;
                case 33:
                    lexeme[i] = '\0';
                    return AND;
                case 34:
                    lexeme[i] = '\0';
                    return OR;
                case 35:
                    lexeme[i] = '\0';
                    return XOR;
                case 36:
                    lexeme[i] = '\0';
                    return SHIFTL;
                case 37:
                    lexeme[i] = '\0';
                    return SHIFTR;
                case 38:
                    lexeme[i] = '\0';
                    return TILDA;
                case 39:
                    lexeme[i] = '\0';
                    return LPAREN;
                case 40:
                    lexeme[i] = '\0';
                    return RPAREN;
                case 41:
                    lexeme[i] = '\0';
                    return SEMI;
                case 42:
                    lexeme[i] = '\0';
                    return QUEST;
                case 43:
                    lexeme[i] = '\0';
                    return COLON;
                case 44:
                    lexeme[i] = '\0';
                    return COMMA;
                case 45:
                    printf("Error at %i,%i: invalid character found %c\n", lineno, pos, buffer[pos - 1]);
                    state = 47;
                    type = 0;
                    next_state = 47;
                    //lexeme[i] = buffer[pos];
                    //i++;
                    lexeme[i] = '\0';
                    //pos++;
                    return ERROR;
                default:
                    if (DEBUG) printf("\t\t\tnow state = table[%i][%i] or %i\n", state, type, table[state][type]);
//                    if (DEBUG) printf("next state %i\n", next_state);
//                    if (c == ' ')
//                    {
//                        printf("space error\n");
//                    }
                    //printf("Error at %i,%i: invalid character found %c\n", lineno, pos, buffer[pos - 1]);
                    //state = 47;
                    //type = 0;
                    //lexeme[i] = buffer[pos];
                    //i++;
                    //lexeme[i] = '\0';
                    //pos++;
                    //return ERROR;
            }
        }
        if (DEBUG) printf("\t\t\tnow state = table[%i][%i] or %i\n", state, type, table[state][type]);
        // get the next token state from the lookup table
        state = table[state][type];
        lexeme[i] = buffer[pos];
        i++;
        pos++;
    }
}
