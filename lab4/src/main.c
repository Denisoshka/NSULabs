#include <stdio.h>
#include <ctype.h>

#define kMaxStackSize 1000
#define kMaxExpressionSize 1000

enum kCalculationResult{
    kSyntaxError = 1,
    kDivisionByZero,
    kOtherError,
};

typedef struct IntStack{
    int Array[kMaxStackSize];
    int QuantityOfElemensts;
}IntStack;

IntStack CreateIntStack(void){
    IntStack blank ={
            .QuantityOfElemensts = 0,
            };
    return blank;
}

int IsEmptyIntStack( const IntStack * Stack ){
    return Stack->QuantityOfElemensts == 0;
}

int PopIntStack( IntStack * Stack ){
    Stack->QuantityOfElemensts--;
    return Stack->Array[Stack->QuantityOfElemensts];
}

void PushIntStack(IntStack * Stack, const int PushedElement ){
    Stack->Array[Stack->QuantityOfElemensts] = PushedElement;
    Stack->QuantityOfElemensts++;
}

int GetIntStackSize( IntStack * Stack ){
    return Stack->QuantityOfElemensts;
}

typedef struct CharStack{
     char Array[kMaxStackSize];
    int QuantityOfElemensts;
}CharStack;

CharStack CreateCharStack(void){
    CharStack blank ={
            .QuantityOfElemensts = 0,
    };
    return blank;
}

int IsEmptyCharStack( const CharStack * Stack ){
    return Stack->QuantityOfElemensts == 0;
}

char PopCharStack( CharStack * Stack ){
    Stack->QuantityOfElemensts--;
    return Stack->Array[Stack->QuantityOfElemensts];
}

char GetCharStack( const CharStack * Stack ){
    return Stack->Array[Stack->QuantityOfElemensts - 1];
}

int IsFullCharStack( const CharStack * Stack ){
    return Stack->QuantityOfElemensts == kMaxStackSize;
}

void PushCharStack(CharStack * Stack, const int PushedElement ){
    Stack->Array[Stack->QuantityOfElemensts] = PushedElement;
    Stack->QuantityOfElemensts++;
}

typedef struct ExpressionArray{
    char Array[kMaxExpressionSize];
    int ArrayLen;
}ExpressionArray;

ExpressionArray CreateExpressionString(void){
    ExpressionArray Blank = {
            .ArrayLen = 0,
    };
    return Blank;
}

int ScanExpression( ExpressionArray * Expression)
{
    FILE * StreamIn = fopen( "in.txt", "r");
    if ( StreamIn == NULL ){
        fprintf( stderr, "__LINE__ %d\n", __LINE__);
        return 1;
    }

    int Iteration = 0;
    for (char Symbol; Iteration < kMaxExpressionSize; Iteration++)
    {
        if ( 1 != fread( &Symbol, sizeof(char), 1, StreamIn) )
        {
            fprintf( stderr, "__LINE__ %d\n", __LINE__);
            fclose( StreamIn );
            return 1;
        }

        if( Symbol != '\n'){
            Expression->Array[Iteration] = Symbol;
        }
        else{
            break;
        }
    }
    Expression->ArrayLen = Iteration;
    
    fclose( StreamIn );
    return 0;
}

int GetNumber( const char symbol ){
    return symbol - '0';
}

int GetNumberFromExpression( const ExpressionArray *Expression, int * Index )
{
    int result = 0;
    for ( ;( *Index < Expression->ArrayLen && isdigit( Expression->Array[*Index] ) ); (*Index)++){
        result *= 10;
        result += GetNumber( Expression->Array[*Index] );
    }
    (*Index)--;
    return result;
}

int GetPriority( const char Symbol )
{
    switch (Symbol)
    {
        case '-': return 1;
        case '+': return 1;
        case '*': return 2;
        case '/': return 2;
        default: return 0;
    }
}

int IsOperation(const char Symbol) {
    return Symbol == '+' || Symbol == '-' || Symbol == '*' || Symbol == '/';
}

int Calculate( IntStack *NumberStack, const char Operation){
    if ( IsEmptyIntStack( NumberStack) ){return kSyntaxError;}
    const int NewNumber = PopIntStack( NumberStack);

    if ( IsEmptyIntStack( NumberStack) ){return kSyntaxError;}
    const int OldNumber = PopIntStack( NumberStack);

    switch ( Operation){
        case '+':
            PushIntStack( NumberStack, OldNumber + NewNumber);
            break;
        case '-':
            PushIntStack( NumberStack, OldNumber - NewNumber);
            break;
        case '*':
            PushIntStack( NumberStack, OldNumber * NewNumber);
            break;
        case '/':
            if ( NewNumber == 0 ){
                return kDivisionByZero;
            }
            PushIntStack( NumberStack, OldNumber / NewNumber);
            break;
        default:
            return kSyntaxError;
    }
    return 0;
}

int CalculateExpression(const ExpressionArray * Expression, IntStack * NumbersStack, CharStack * OperationsStack ){
    if ( Expression->Array[0] == ')' ){return kSyntaxError;}

    for (int Index = 0; Index < Expression->ArrayLen; Index++){
        if ( isdigit(Expression->Array[Index]) ){
            PushIntStack( NumbersStack, GetNumberFromExpression( Expression, &Index));
        }else if ( Expression->Array[Index] == '(' ){
            PushCharStack( OperationsStack, Expression->Array[Index] );
        }else if ( Expression->Array[Index] == ')' ){
            if ( Expression->Array[ Index - 1 ] == '(' ) { return kSyntaxError; }
            if (IsEmptyCharStack( OperationsStack)) { return kSyntaxError; }

            char Operation = PopCharStack( OperationsStack);
            for ( int CalculationError; Operation != '(' ; ) {
                if ( (CalculationError = Calculate(NumbersStack, Operation)) ) { return CalculationError; }
                if (IsEmptyCharStack( OperationsStack)) { return kSyntaxError; }
                Operation = PopCharStack( OperationsStack);
            }
        }else if ( IsOperation(Expression->Array[Index]) ){
            if ( !IsEmptyCharStack( OperationsStack ) ) {
                for ( int CalculationError; GetPriority(GetCharStack(OperationsStack) ) >= GetPriority(Expression->Array[Index]) && !IsEmptyCharStack( OperationsStack);) {
                    if ( (CalculationError = Calculate(NumbersStack, PopCharStack( OperationsStack))) ) { return CalculationError; }
                }
                if ( IsFullCharStack( OperationsStack) ) { fprintf( stderr, "__LINE__ %d", __LINE__ ); return kOtherError; }
                PushCharStack( OperationsStack, Expression->Array[Index]);
            }else{
                if ( IsFullCharStack( OperationsStack) ) { fprintf( stderr, "__LINE__ %d", __LINE__ ); return kOtherError; }
                PushCharStack( OperationsStack, Expression->Array[Index]);
            }
        }else{ return kSyntaxError; }
    }
    for (int CalculationError; !IsEmptyCharStack( OperationsStack) ; ){
        if ( (CalculationError = Calculate(NumbersStack, PopCharStack(OperationsStack))) ){ return CalculationError; }
    }
    return 0;
}

int PrintExpressionResult( IntStack * NumberStack, CharStack * OperationStack, const int Flag){
    FILE * StreamOut = fopen( "out.txt", "w");
    if ( StreamOut == NULL ){fprintf( stderr, "__LINE__ %d", __LINE__);return 1;}

    if ( Flag || !IsEmptyCharStack(OperationStack ) || GetIntStackSize(NumberStack ) != 1 ){
        if ( Flag == kSyntaxError ){ fprintf( StreamOut, "syntax error\n");}
        if ( Flag == kDivisionByZero){ fprintf( StreamOut, "division by zero\n");}
    }else{ fprintf( StreamOut, "%d\n", PopIntStack( NumberStack) ); }

    fclose( StreamOut);
    return 0;
}

int main(void)
{
    ExpressionArray Expression = CreateExpressionString();
    if ( ScanExpression( &Expression) ){ return 0; }

    IntStack NumbersStack = CreateIntStack();
    CharStack OperationsStack = CreateCharStack();
    const int CalculationError = (Expression.ArrayLen == 0) ? kSyntaxError :CalculateExpression( &Expression, &NumbersStack, &OperationsStack );
    PrintExpressionResult( &NumbersStack, &OperationsStack, CalculationError );

    return 0;
}
