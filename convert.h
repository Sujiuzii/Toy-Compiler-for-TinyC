#ifndef CONVERT_H
#define CONVERT_H

//词法分析器中相关数据结构的定义
typedef enum dfaStat
{
	InitialStat,    //初始状态
	LetterStat,     //字符
	DigitStat,      //数字
	GTStat,         //大于
	LTStat,         //小于
	EqualStat, 		//==
	equalStat,		// =
	AssignmentStat, //等于
	SemiColonStat,  //冒号
	PlusStat,       //加号
	MinusStat,// -
	TimesStat,// *
	DividedbyStat, // /

	orStat,   // |
	andStat,  // &
	OrStat,   // ||
	AndStat,  // &&

	LeftParenStat,  //左圆括号
	RightParenStat, //右括号
	LeftBraceStat,  //左花括号
	RightBraceStat, //右花括号
	DotStat,        //逗号
	Others,
}dfastat;

typedef enum TokenType
{
	Plus, // +
	Minus,// -
	Times,// *
	Dividedby, // /

	Or,   // ||
	And,  // &&

	GT,   // >
	LT,   // <
	Equal,// ==

	SemiColon,  // ;
	LeftParen,  // (
	RightParen, // )
	Assignment, // =
	LeftBrace,  // { 
	RightBrace, // }

	If,
	Else,
	Int,
	Char,
	While,
	Continue,
	Break,

	Identifier,  //标识符
	IntLiteral,  //整形字面量
	Main,
	Dot, // ,
	Show,
	Return,
	GOTO,
	IFNZ,
}TokenType;

typedef struct Token
{
	enum TokenType type;
	char text[256];
	struct Token* next;
	struct Token* parent;
}Token;
typedef struct Token* TokenList;

//语法分析器中相关数据结构的定义
typedef enum ASTNodeType
{
	Program,            //程序入口
	IntDeclaration,     //整型变量声明
	ExpressionStmt,     //表达式语句，即表达式后面跟一个分号
	AssignmentASTnode , //赋值语句
    Primary,            //基础表达式
	Addtive,            //加法表达式
	Multiplicative,

	IdentifierASTnode,  //标识符
	IntLiteralASTnode,  //整型字面量

	BodyASTNode,			  //Main函数实体
	IfASTNode,
	ElseASTNode,
	WhileASTNode,
	BooleASTNode,

	GTASTNode,
	LTASTNode,
	EQASTNode,

	ANDASTNode,
	ORASTNode,

	ShowASTNode,
}ASTNodeType;

typedef  struct ASTNode
{
	ASTNodeType nodetype;         //结点的语法类型（赋值，表达式等）
	char text[256];								//结点类型对应的实体名称
	struct ASTNode* parent;				
	struct ASTChildren* children;
}ASTNode;
typedef struct ASTNode* ASTNodeList;    //语法树结构

typedef struct  ASTChildren
{
	struct ASTNode* nodelist;
	struct ASTChildren* next;
}ASTChildren;
typedef struct ASTChildren* ASTChildrenList;

typedef struct Result
{
	int result;
}Result;
typedef struct Result* ResultList;  //用于存储语法树的计算结果

typedef struct IdTable
{
	char text[256];
	int value;
	int flag;
	struct IdTable* next;
}IdTable;
typedef struct IdTable* IdTableList;

typedef struct IRnode
{
	char a[256];
	int flag;     //flag=0 变量在寄存器上 ;  flag = 1 变量在内存上
	int Regester; //寄存器序列号
	int Memory;   //在内存中的地址
	struct IRnode* next;
}IRnode;
typedef struct IRnode* IRList;

#endif
