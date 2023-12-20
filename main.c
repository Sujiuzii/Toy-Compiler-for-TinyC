#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "convert.h"

extern TokenList list;
extern TokenList IR_list;
extern FILE* sFile;
extern void TokenReader();
extern int cmpstring(char[], char[]);

IdTableList IdHead = NULL;
TokenList pList;
IRList IRList_Head = NULL;
FILE* IRFile;

ASTNodeList assignmentStatment();
ASTNodeList primary();
ASTNodeList intDeclare();
ASTNodeList intNodeList();
ASTNodeList boolStatement();
ASTNodeList addtiveStatment();
ASTNodeList multiplicativeStatement();
ASTNodeList relStatement();
ASTNodeList andrelStatement();
ASTNodeList orrelStatement();

ASTNodeList Body();

void varDeclarationBody(ASTNodeList node, TokenList tokentemp);
void ifBody(ASTNodeList node, TokenList tokentemp);
void showBody(ASTNodeList node, TokenList tokentemp);
void assignmentBody(ASTNodeList node, TokenList tokentemp);
void whileBody(ASTNodeList node, TokenList tokentemp);

ResultList evaluate(ASTNodeList node);
void dumpAST(ASTNodeList p, int deepth);
IRList IR(ASTNodeList node);

int continueSign = 1;
int breakSign = 1;
int ircount = 1;
int labelcount = 1;

char ASTNodeTypeString[30][256] = {
    "Program",
    "IntDeclaration",
    "ExpressionStmt",
    "Assignment",
    "Primary",
    "Add/Minus",
    "Times/Diviedby",
    "Identifier",
    "IntLiteral",
    "Body",
    "If",
    "Else",
    "While",
    "Boole",
    "GT",
    "LT",
    "EQ",
    "And",
    "Or",
    "Show",
};

void addASTNode(ASTNodeList node, ASTNodeList child) {
  if (node != NULL && child != NULL) {
    ASTChildrenList childlist = (ASTChildrenList)malloc(sizeof(ASTChildren));
    childlist->nodelist = child;
    childlist->next = NULL;
    if (node->children == NULL) {
      node->children = childlist;
    } else {
      ASTChildrenList p = node->children;
      while (p->next != NULL) {
        p = p->next;
      }
      p->next = childlist;
    }
    child->parent = node;
  }
  return;
}

IdTableList findIdentifier(char a[]) {
  IdTableList p = IdHead;
  while (p != NULL) {
    if (cmpstring(a, p->text)) {
      return p;
    }
    p = p->next;
  }
  return p;
}

void insertIdNode(char a[], int value, int k) {
  IdTableList idNode = (IdTableList)malloc(sizeof(IdTable));
  strcpy(idNode->text, a);
  idNode->value = value;
  idNode->flag = k;
  idNode->next = IdHead;
  IdHead = idNode;
  return;
}

int hundred(int n) {
  int re = 1;
  if (n == 0) {
    return 1;
  }
  while (n > 0) {
    re *= 10;
    n--;
  }
  return re;
}

int literalToNumber(char a[]) {
  int lenth = strlen(a) - 1;
  int i = lenth;
  int b = 0;
  while (i >= 0) {
    b += (int)(a[i] - '0') * hundred(lenth - i);
    i--;
  }
  return b;
}

// 加法表达式
ASTNodeList addtiveStatment() {
  ASTNodeList child1 = multiplicativeStatement();
  ASTNodeList node = child1;
  TokenList tokentmp;

  while (1) {
    tokentmp = pList;
    if (tokentmp != NULL && (tokentmp->type == Plus)) {
      pList = pList->next;
      ASTNodeList child2 = multiplicativeStatement();
      if (child2 != NULL) {
        node = (ASTNodeList)malloc(sizeof(ASTNode));
        strcpy(node->text, tokentmp->text);
        node->nodetype = Addtive;
        node->children = NULL;
        node->parent = NULL;
        addASTNode(node, child1);
        addASTNode(node, child2);
        child1 = node;
      } else {
        printf("invalid Addtive expression,expecting the right part\n");
        node = NULL;
      }
    } 
    else if (tokentmp != NULL && (tokentmp->type == Minus)) {
      pList = pList->next;
      ASTNodeList child2 = multiplicativeStatement();
      if (child2 != NULL) {
        node = (ASTNodeList)malloc(sizeof(ASTNode));
        strcpy(node->text, tokentmp->text);
        node->nodetype = Addtive;
        node->children = NULL;
        node->parent = NULL;
        addASTNode(node, child1);
        addASTNode(node, child2);
        child1 = node;
      } else {
        printf("invalid Addtive expression,expecting the right part\n");
        node = NULL;
      }
    }
    else {
      break;
    }
  }
  return node;
}

ASTNodeList multiplicativeStatement()
{
  ASTNodeList child1 = primary();
  ASTNodeList node = child1;
  TokenList tokentmp;

  while (1) {
    tokentmp = pList;
    if (tokentmp != NULL && (tokentmp->type == Times)) {
      pList = pList->next;
      ASTNodeList child2 = primary();
      if (child2 != NULL) {
        node = (ASTNodeList)malloc(sizeof(ASTNode));
        strcpy(node->text, tokentmp->text);
        node->nodetype = Multiplicative;
        node->children = NULL;
        node->parent = NULL;
        addASTNode(node, child1);
        addASTNode(node, child2);
        child1 = node;
      } else {
        printf("invalid Multiplicative expression,expecting the right part\n");
        node = NULL;
      }
    } 
    else if (tokentmp != NULL && (tokentmp->type == Dividedby)) {
      pList = pList->next;
      ASTNodeList child2 = primary();
      if (child2 != NULL) {
        node = (ASTNodeList)malloc(sizeof(ASTNode));
        strcpy(node->text, tokentmp->text);
        node->nodetype = Multiplicative;
        node->children = NULL;
        node->parent = NULL;
        addASTNode(node, child1);
        addASTNode(node, child2);
        child1 = node;
      } else {
        printf("invalid Multiplicative expression,expecting the right part\n");
        node = NULL;
      }
    }
    else {
      break;
    }
  }
  return node;
}

// 大于小于表达式
ASTNodeList relStatement() {
  ASTNodeList child1 = addtiveStatment();
  ASTNodeList node = child1;
  TokenList tokentmp = pList;

  while (1) {
    tokentmp = pList;
    if (tokentmp != NULL && tokentmp->type == GT) {
      pList = pList->next;
      ASTNodeList child2 = addtiveStatment();
      if (child2 != NULL) {
        node = (ASTNodeList)malloc((sizeof(ASTNode)));
        node->nodetype = GTASTNode;
        node->children = NULL;
        node->parent = NULL;
        strcpy(node->text, tokentmp->text);
        addASTNode(node, child1);
        addASTNode(node, child2);
        child1 = node;
      } else {
        printf("error:Lack of Rel after EQStatement\n");
        exit(0);
      }
    } else if (tokentmp != NULL && tokentmp->type == LT) {
      pList = pList->next;
      ASTNodeList child2 = addtiveStatment();
      if (child2 != NULL) {
        node = (ASTNodeList)malloc((sizeof(ASTNode)));
        node->nodetype = LTASTNode;
        node->children = NULL;
        node->parent = NULL;
        strcpy(node->text, tokentmp->text);
        addASTNode(node, child1);
        addASTNode(node, child2);
        child1 = node;
      } else {
        printf("error:Lack of Rel after EQStatement\n");
        exit(0);
      }
    }
    else if(tokentmp != NULL && tokentmp->type == Equal){
      pList = pList->next;
      ASTNodeList child2 = addtiveStatment();
      if (child2 != NULL) {
        node = (ASTNodeList)malloc((sizeof(ASTNode)));
        node->nodetype = EQASTNode;
        node->children = NULL;
        node->parent = NULL;
        strcpy(node->text, tokentmp->text);
        addASTNode(node, child1);
        addASTNode(node, child2);
        child1 = node;
      } else {
        printf("error:Lack of Rel after EQStatement\n");
        exit(0);
      }
    }
    else {
      break;
    }
  }
  return node;
}

ASTNodeList andrelStatement() {
  ASTNodeList child1 = relStatement();
  ASTNodeList node = child1;
  TokenList tokentmp = pList;

  while (1) {
    tokentmp = pList;
    if (tokentmp != NULL && tokentmp->type == And) {
      pList = pList->next;
      ASTNodeList child2 = relStatement();
      if (child2 != NULL) {
        node = (ASTNodeList)malloc((sizeof(ASTNode)));
        node->nodetype = ANDASTNode;
        node->children = NULL;
        node->parent = NULL;
        strcpy(node->text, tokentmp->text);
        addASTNode(node, child1);
        addASTNode(node, child2);
        child1 = node;
      } else {
        printf("error:Lack of Rel after ANDStatement\n");
        exit(0);
      }
    } 
    else {
      break;
    }
  }
  return node;
}

ASTNodeList orrelStatement() {
  ASTNodeList child1 = andrelStatement();
  ASTNodeList node = child1;
  TokenList tokentmp = pList;

  while (1) {
    tokentmp = pList;
    if (tokentmp != NULL && tokentmp->type == Or) {
      pList = pList->next;
      ASTNodeList child2 = andrelStatement();
      if (child2 != NULL) {
        node = (ASTNodeList)malloc((sizeof(ASTNode)));
        node->nodetype = ORASTNode;
        node->children = NULL;
        node->parent = NULL;
        strcpy(node->text, tokentmp->text);
        addASTNode(node, child1);
        addASTNode(node, child2);
        child1 = node;
      } else {
        printf("error:Lack of Rel after ORStatement\n");
        exit(0);
      }
    } 
    else {
      break;
    }
  }
  return node;
}

// 布尔表达式
ASTNodeList boolStatement() {
  TokenList tokentmp = pList;
  ASTNodeList node = (ASTNodeList)malloc(sizeof(ASTNode));
  node->nodetype = BooleASTNode;
  node->children = NULL;
  node->parent = NULL;
  strcpy(node->text, "Boole");

  ASTNodeList child1 = orrelStatement();
  addASTNode(node, child1);
  return node;
}

// 赋值语句
ASTNodeList assignmentStatment() {
  ASTNodeList node = NULL;
  TokenList tokentmp = pList;

  if (tokentmp != NULL && tokentmp->type == Identifier) {
    pList = pList->next;
    node = (ASTNodeList)malloc(sizeof(ASTNode));
    node->nodetype = AssignmentASTnode;
    strcpy(node->text, tokentmp->text);
    node->children = NULL;
    node->parent = NULL;
    tokentmp = pList;
    TokenList tmp = pList;
    if (tokentmp != NULL && tokentmp->type == Assignment) {
      pList = pList->next;
      ASTNodeList child = addtiveStatment();
      if (child == NULL) {
        printf(
            "invalid assignment statement in assignmentStatment,expecting "
            "semicolon\n");
        node = NULL;
      } else {
        addASTNode(node, child);
        tokentmp = pList;
        if (tokentmp != NULL && tokentmp->type == SemiColon) {
          pList = pList->next;
        } else {
          printf(
              "invalid statement in assignmentStatment,expecting semicolon\n");
        }
      }
    } else {
      pList = tmp;
      node = NULL;
    }
  }
  return node;
}

//基础表达式
ASTNodeList primary() {
  ASTNodeList node = NULL;
  TokenList tokentmp = pList;

  if (tokentmp != NULL) {
    if (tokentmp->type == IntLiteral) {
      pList = pList->next;
      node = (ASTNodeList)malloc(sizeof(ASTNode));
      node->nodetype = IntLiteralASTnode;
      strcpy(node->text, tokentmp->text);
      node->children = NULL;
      node->parent = NULL;
    } else if (tokentmp->type == Identifier) {
      pList = pList->next;
      node = (ASTNodeList)malloc(sizeof(ASTNode));
      node->nodetype = IdentifierASTnode;
      strcpy(node->text, tokentmp->text);
      node->children = NULL;
      node->parent = NULL;
    } else if (tokentmp->type == LeftParen) {
      pList = pList->next;
      node = orrelStatement();
      if (node != NULL) {
        tokentmp = pList;
        if (tokentmp != NULL && tokentmp->type == RightParen) {
          pList = pList->next;
        } else {
          printf("expecting right parenthesis in expression\n");
          node = NULL;
        }
      } else {
        printf(
            "expecting an additive expression inside parenthesis in "
            "expression\n");
        node = NULL;
      }
    }
  }
  return node;
}

ASTNodeList intDeclare() {
  ASTNodeList node = NULL;
  TokenList tokentmp = pList;

  if (tokentmp != NULL && tokentmp->type == Int) {
    tokentmp = pList;
    pList = pList->next;
    if (pList != NULL && pList->type == Identifier) {
      tokentmp = pList;
      pList = pList->next;
      node = (ASTNodeList)malloc(sizeof(ASTNode));
      strcpy(node->text, tokentmp->text);
      node->nodetype = IntDeclaration;
      node->children = NULL;
      node->parent = NULL;
      tokentmp = pList;
      if (tokentmp != NULL && tokentmp->type == Assignment) {
        pList = pList->next;
        ASTNodeList child = addtiveStatment();
        if (child == NULL) {
          printf("invalide variable initialization,expecting an expression\n");
          node = NULL;
        } else {
          addASTNode(node, child);
        }
      }
    } else {
      printf("variable name expected!\n");
      node = NULL;
    }
  }
  return node;
}

// 遍历语法树，并打印语法树
void dumpAST(ASTNodeList p, int deepth) {
  if (p == NULL) {
    return;
  }
  char a[100];
  memset(a, '\0', 100);
  for (int i = 0; i < deepth; i++) {
    a[i] = '\t';
  }
  printf("%s%s\t%s\n", a, ASTNodeTypeString[p->nodetype], p->text);
  ASTChildrenList children = p->children;
  while (children != NULL) {
    dumpAST(children->nodelist, deepth + 1);
    children = children->next;
  }
  return;
}

ASTNodeList intNodeList() {
  ASTNodeList node = NULL;
  TokenList tokentmp = pList;

  if (pList != NULL && pList->type == Identifier) {
    tokentmp = pList;
    pList = pList->next;
    node = (ASTNodeList)malloc(sizeof(ASTNode));
    strcpy(node->text, tokentmp->text);
    node->nodetype = IntDeclaration;
    node->children = NULL;
    node->parent = NULL;
    tokentmp = pList;
    if (tokentmp != NULL && tokentmp->type == Assignment) {
      pList = pList->next;
      ASTNodeList child = addtiveStatment();
      if (child == NULL) {
        printf("invalide variable initialization,expecting an expression\n");
        node = NULL;
      } else {
        addASTNode(node, child);
      }
    }
  } else {
    printf("variable name expected!\n");
    node = NULL;
  }
  return node;
}

ResultList evaluate(ASTNodeList node) {
  ResultList resultList = NULL;
  ASTChildrenList child = NULL;
  ResultList childList1 = NULL;
  ResultList childList2 = NULL;
  IdTableList tmpIdNode = NULL;
  ASTNodeList parent = NULL;
  int tmp, tmp1, tmpcontinue, tmpbreak;

  switch (node->nodetype) {
    case Program:
      child = node->children;
      while (child != NULL) {
        evaluate(child->nodelist);
        child = child->next;
      }
      break;

    case BodyASTNode:
      child = node->children;
      while (child != NULL) {
        evaluate(child->nodelist);
        child = child->next;
      }
      break;

    case IntDeclaration:
      child = node->children;
      if (child != NULL && continueSign == 1 && breakSign == 1) {
        childList1 = evaluate(child->nodelist);
        insertIdNode(node->text, childList1->result, 1);
      } else {
        insertIdNode(node->text, 0, 0);
      }
      break;

    case IntLiteralASTnode:
      resultList = (ResultList)malloc(sizeof(Result));
      resultList->result = literalToNumber(node->text);
      break;

    case IdentifierASTnode:
      tmpIdNode = findIdentifier(node->text);
      if (tmpIdNode == NULL) {
        printf("%s not identified\n", node->text);
        exit(0);
      }
      if (tmpIdNode->flag == 0) {
        printf("%s not assignment\n", node->text);
        exit(0);
      }
      resultList = (ResultList)malloc(sizeof(Result));
      resultList->result = tmpIdNode->value;
      break;

    case LTASTNode:
      child = node->children;
      tmp = evaluate(child->nodelist)->result;
      child = child->next;
      tmp1 = evaluate(child->nodelist)->result;
      resultList = (ResultList)malloc(sizeof(Result));
      if (tmp < tmp1) {
        resultList->result = 1;
      } else {
        resultList->result = 0;
      }
      break;

    case GTASTNode:
      child = node->children;
      tmp = evaluate(child->nodelist)->result;
      child = child->next;
      tmp1 = evaluate(child->nodelist)->result;
      resultList = (ResultList)malloc(sizeof(Result));
      if (tmp > tmp1) {
        resultList->result = 1;
      } else {
        resultList->result = 0;
      }
      break;

    case EQASTNode:
      child = node->children;
      tmp = evaluate(child->nodelist)->result;
      child = child->next;
      tmp1 = evaluate(child->nodelist)->result;
      resultList = (ResultList)malloc(sizeof(Result));
      if (tmp == tmp1) {
        resultList->result = 1;
      } else {
        resultList->result = 0;
      }
      break;

    case ANDASTNode:
      child = node->children;
      tmp = evaluate(child->nodelist)->result;
      child = child->next;
      tmp1 = evaluate(child->nodelist)->result;
      resultList = (ResultList)malloc(sizeof(Result));
      if(tmp && tmp1){
        resultList->result = 1;
      }
      else{
        resultList->result = 0;
      }
      break;

    case ORASTNode:
      child = node->children;
      tmp = evaluate(child->nodelist)->result;
      child = child->next;
      tmp1 = evaluate(child->nodelist)->result;
      resultList = (ResultList)malloc(sizeof(Result));
      if(tmp || tmp1){
        resultList->result = 1;
      }
      else{
        resultList->result = 0;
      }
      break;

    case Addtive:
      /*  TO DO  */
      /*  提供int cmpstring(char a[], char b[])函数可调用，具体用法看定义  */
      child = node->children;
      tmp = evaluate(child->nodelist)->result;
      child = child->next;
      tmp1 = evaluate(child->nodelist)->result;
      if(cmpstring(node->text, "+"))
        tmp += tmp1;
      else
        tmp -= tmp1;
      resultList = (ResultList)malloc(sizeof(Result));
      resultList->result = tmp;
      break;

    case Multiplicative:
      child = node->children;
      tmp = evaluate(child->nodelist)->result;
      child = child->next;
      tmp1 = evaluate(child->nodelist)->result;
      if(cmpstring(node->text, "*"))
        tmp *= tmp1;
      else
        tmp /= tmp1;
      resultList = (ResultList)malloc(sizeof(Result));
      resultList->result = tmp;
      break;

    case AssignmentASTnode:
      child = node->children;
      /*  TO DO  */
      /*  提供IdTableList findIdentifier(char a[])函数可用，具体用法看定义  */
      tmpIdNode = findIdentifier(node->text);
      if (tmpIdNode == NULL) {
        printf("%s not identified\n", node->text);
        exit(0);
      }
      tmpIdNode->flag = 1;
      tmpIdNode->value = evaluate(child->nodelist)->result;
      resultList = (ResultList)malloc(sizeof(Result));
      resultList->result = tmpIdNode->value;
      break;

    case ShowASTNode:
      /*  TO DO  */
      child = node->children;
      int tmpflag = 1;
      while (child != NULL) {
        if(tmpflag == 1){
          printf("%d\n", evaluate(child->nodelist)->result);
          tmpflag--;
        }
        else
          evaluate(child->nodelist);
        child = child->next;
      }
      break;

    case IfASTNode:
      /*  TO DO  */
      child = node->children;
      if(evaluate(child->nodelist)->result){
        child = child->next;
        evaluate(child->nodelist);
      }
      else{
        child = child->next->next;
        if(child != NULL){
          evaluate(child->nodelist);
        }
      }
      break;

    case ElseASTNode:
      /*  TO DO  */
      child = node->children;
      evaluate(child->nodelist);
      break;

    case WhileASTNode:
      child = node->children;
      while(evaluate(child->nodelist)->result){
        child = child->next;
        evaluate(child->nodelist);
        child = node->children;
      }
      break;

    case BooleASTNode:
      child = node->children;
      resultList = evaluate(child->nodelist);
      break;

    default:
      break;
  }
  return resultList;
}

void varDeclarationBody(ASTNodeList node, TokenList tokentmp) {
  ASTNodeList child = intDeclare();
  if (child != NULL) {
    addASTNode(node, child);
  } else {
    printf("error:IntDeclare\n");
    exit(0);
  }

  while (1) {
    if (pList != NULL && pList->type == Dot) {
      pList = pList->next;
      ASTNodeList child = intNodeList();
      if (child != NULL) {
        addASTNode(node, child);
      } else {
        printf("error:IntDeclare\n");
        exit(0);
      }
    } else if (pList != NULL && pList->type == SemiColon) {
      pList = pList->next;
      break;
    } else {
      printf("invalide statement in varDeclaration,expecting semicolon\n");
    }
  }
}

void assignmentBody(ASTNodeList node, TokenList tokentmp) {
  ASTNodeList child = assignmentStatment();
  if (child != NULL) {
    addASTNode(node, child);
  } else {
    printf("error : assignmentStatment\n");
    exit(0);
  }
}

void showBody(ASTNodeList node, TokenList tokentmp) {
  /*  TO DO  */
  tokentmp = pList;
  ASTNodeList shownode;
  if (tokentmp != NULL && tokentmp->type == Show) {
    pList = pList->next;
    shownode = (ASTNodeList)malloc(sizeof(ASTNode));
    shownode->nodetype = ShowASTNode;
    strcpy(shownode->text, tokentmp->text);
    tokentmp = pList;
    if (tokentmp != NULL && tokentmp->type == LeftParen) {
      pList = pList->next;
      tokentmp = pList;  
      ASTNodeList child = addtiveStatment();
      if(child != NULL){
        addASTNode(shownode, child);
      }
      else{
        printf("error:show\n");
        exit(0);
      }
      if (pList != NULL && pList->type == RightParen) {
          pList = pList->next;
        }
      else {
        printf("error:Lack of RightParen\n");
        exit(0);
      }
      if(pList != NULL && pList->type == SemiColon){
        pList = pList->next;
      }
      else{
        printf("error:Lack of Semicolon\n");
        exit(0);
      }
    } else {
      printf("error:Lack of LeftParen\n");
      exit(0);
    }
    addASTNode(node, shownode);
  }
}

void ifBody(ASTNodeList node, TokenList tokentmp) {
  /*  TO DO  */
  /*  提供ASTNodeList boolStatement()函数和
  void addASTNode(ASTNodeList node, ASTNodeList child)可用  */
  tokentmp = pList;
  ASTNodeList ifnode, elsenode;
  if (tokentmp != NULL && tokentmp->type == If) {
    pList = pList->next;
    ifnode = (ASTNodeList)malloc(sizeof(ASTNode));
    ifnode->nodetype = IfASTNode;
    strcpy(ifnode->text, tokentmp->text);
    tokentmp = pList;
    if (tokentmp != NULL && tokentmp->type == LeftParen) {
      pList = pList->next;
      tokentmp = pList;  
      ASTNodeList child = boolStatement();
      if(child != NULL){
        addASTNode(ifnode, child);
      }
      else{
        printf("error:condition\n");
        exit(0);
      }
      tokentmp = pList;
      if (tokentmp != NULL && tokentmp->type == RightParen) {
        pList = pList->next;
        tokentmp = pList;
        if (tokentmp != NULL && tokentmp->type == LeftBrace) {
          pList = pList->next;
          tokentmp = pList;
          ASTNodeList child = Body();
          if (child != NULL) {
            addASTNode(ifnode, child);
            if (pList != NULL && pList->type == RightBrace) {
              pList = pList->next;
            } else {
              printf("error:Lack of RightBrace\n");
              exit(0);
            }
          }
        } else {
          printf("error:Lack of LeftBrace\n");
          exit(0);
        }
      } else {
        printf("error:Lack of RightParen\n");
        exit(0);
      }
    } else {
      printf("error:Lack of LeftParen\n");
      exit(0);
    }
    tokentmp = pList;
    if (tokentmp != NULL && tokentmp->type == Else){
      elsenode = (ASTNodeList)malloc(sizeof(ASTNode));
      elsenode->nodetype = ElseASTNode;
      strcpy(elsenode->text, tokentmp->text);
      pList = pList->next;
      tokentmp = pList;
      if (tokentmp != NULL && tokentmp->type == LeftBrace) {
        pList = pList->next;
        tokentmp = pList;
        ASTNodeList child = Body();
        if (child != NULL) {
          addASTNode(elsenode, child);
          if (pList != NULL && pList->type == RightBrace) {
            pList = pList->next;
          } else {
            printf("error:Lack of RightBrace\n");
            exit(0);
          }
        }
      } else {
          printf("error:Lack of LeftBrace\n");
          exit(0);
        }
        addASTNode(ifnode, elsenode);
    }
    addASTNode(node, ifnode);
  }
}

void whileBody(ASTNodeList node, TokenList tokentmp)
{
  tokentmp = pList;
  // continueSign = 1;
  // breakSign = 1;
  ASTNodeList whilenode;
  if (tokentmp != NULL && tokentmp->type == While) {
    pList = pList->next;
    whilenode = (ASTNodeList)malloc(sizeof(ASTNode));
    whilenode->nodetype = WhileASTNode;
    strcpy(whilenode->text, tokentmp->text);
    tokentmp = pList;
    if (tokentmp != NULL && tokentmp->type == LeftParen) {
      pList = pList->next;
      tokentmp = pList;  
      ASTNodeList child = boolStatement();
      if(child != NULL){
        addASTNode(whilenode, child);
      }
      else{
        printf("error:condition\n");
        exit(0);
      }
      tokentmp = pList;
      if (tokentmp != NULL && tokentmp->type == RightParen) {
        pList = pList->next;
        tokentmp = pList;
        if (tokentmp != NULL && tokentmp->type == LeftBrace) {
          pList = pList->next;
          tokentmp = pList;
          ASTNodeList child = Body();
          if (child != NULL) {
            addASTNode(whilenode, child);
            if (pList != NULL && pList->type == RightBrace) {
              pList = pList->next;
            } else {
              printf("error:Lack of RightBrace\n");
              exit(0);
            }
          }
        } else {
          printf("error:Lack of LeftBrace\n");
          exit(0);
        }
      } else {
        printf("error:Lack of RightParen\n");
        exit(0);
      }
    } else {
      printf("error:Lack of LeftParen\n");
      exit(0);
    }
    addASTNode(node, whilenode);
  }
}

ASTNodeList Body() {
  ASTNodeList node = NULL;
  node = (ASTNodeList)malloc(sizeof(ASTNode));
  node->nodetype = BodyASTNode;
  strcpy(node->text, "Body");
  node->children = NULL;
  node->parent = NULL;
  TokenList tokentmp = pList;
  ASTNodeList tempBody;

  if (tokentmp != NULL && tokentmp->type == Int) {
    varDeclarationBody(node, tokentmp);
    tempBody = Body();
    addASTNode(node, tempBody);
  } 
  else if (tokentmp != NULL && tokentmp->type == Identifier) {
    assignmentBody(node, tokentmp);
    tempBody = Body();
    addASTNode(node, tempBody);
  } else if (tokentmp != NULL && tokentmp->type == If) {
    ifBody(node, tokentmp);
    tempBody = Body();
    addASTNode(node, tempBody);
  } else if (tokentmp != NULL && tokentmp->type == Show) {
    showBody(node, tokentmp);
    tempBody = Body();
    addASTNode(node, tempBody);
  } 
  else if(tokentmp != NULL && tokentmp->type == While){
    whileBody(node, tokentmp);
    tempBody = Body();
    addASTNode(node, tempBody);
  }
  else if (pList != NULL && pList->type == RightBrace) {
    return NULL;
  } else {
    printf("error:unknown statement in main body");
    exit(0);
  }
  return node;
}

ASTNodeList Programming() {
  TokenList tokentmp = pList;
  ASTNodeList node = NULL;

  if (tokentmp != NULL && tokentmp->type == Main) {
    pList = pList->next;
    node = (ASTNodeList)malloc(sizeof(ASTNode));
    node->nodetype = Program;
    node->children = NULL;
    node->parent = NULL;
    strcpy(node->text, tokentmp->text);
    tokentmp = pList;
    if (tokentmp != NULL && tokentmp->type == LeftParen) {
      pList = pList->next;
      tokentmp = pList;
      if (tokentmp != NULL && tokentmp->type == RightParen) {
        pList = pList->next;
        tokentmp = pList;
        if (tokentmp != NULL && tokentmp->type == LeftBrace) {
          pList = pList->next;
          tokentmp = pList;
          ASTNodeList child = Body();
          if (child != NULL) {
            addASTNode(node, child);
            if (pList != NULL && pList->type == RightBrace) {
              pList = pList->next;
            } else {
              printf("error:Lack of RightBrace\n");
              exit(0);
            }
          }
        } else {
          printf("error:Lack of LeftBrace\n");
          exit(0);
        }
      } else {
        printf("error:Lack of RightParen\n");
        exit(0);
      }
    } else {
      printf("error:Lack of LeftParen\n");
      exit(0);
    }
  }
  return node;
}

IRList IR(ASTNodeList node){
	IRList irList = NULL; // 中间代码链表
	ASTChildrenList child = NULL;
	IRList childList1 = NULL;
	IRList childList2 = NULL;
	IdTableList tmpIdNode = NULL;
	char L1tmp[100];
	char L2tmp[100];

	char IRtmp[256];
	// int L1;
	// int L2;
	switch (node->nodetype)
	{
	case Program:
		child = node->children;
		while (child != NULL)
		{
			IR(child->nodelist);
			child = child->next;
		}
		break;

	case BodyASTNode:
		child = node->children;
		while (child != NULL)
		{
			IR(child->nodelist);
			child = child->next;
		}
		break;

	case AssignmentASTnode:

		tmpIdNode = findIdentifier(node->text);
		if (tmpIdNode == NULL)
		{
			printf("%s not identified\n", node->text);
			exit(0);
		}
		child = node->children;
		childList1 = IR(child->nodelist);
		if (childList1 != NULL)
		{
			printf("\t%s = %s\n", node->text, childList1->a);
			fprintf(IRFile, "\t%s = %s\n", node->text, childList1->a);
			tmpIdNode->flag = 1;
		}
		break;

	case IntDeclaration:
		child = node->children;
		if (child != NULL)
		{
			childList1 = IR(child->nodelist);
			insertIdNode(node->text, 0, 1);
			printf("\t%s = %s\n", node->text, childList1->a);
			fprintf(IRFile, "\t%s = %s\n", node->text, childList1->a);
		}
		else
		{
			insertIdNode(node->text, 0, 0);
		}
		break;

	case IdentifierASTnode:
		tmpIdNode = findIdentifier(node->text);
		if (tmpIdNode == NULL)
		{
			printf("%s not identified\n", node->text);
			exit(0);
		}
		if (tmpIdNode->flag == 0)
		{
			printf("%s not assignment\n", node->text);
			exit(0);
		}
		irList = (IRList)malloc(sizeof(IRnode));
		strcpy(irList->a, node->text);
		break;

	case IntLiteralASTnode:
		irList = (IRList)malloc(sizeof(IRnode));
		strcpy(irList->a, node->text);
		break;

	case Addtive:
    // // TODO
  case Multiplicative:
    // TODO
    child = node->children;
    childList1 = IR(child->nodelist);
    child = child->next;
    childList2 = IR(child->nodelist);
    sprintf(IRtmp, "T%d", ircount);
    ircount++;
    printf("\t%s = %s %s %s\n", IRtmp, childList1->a, node->text, childList2->a);
    fprintf(IRFile, "\t%s = %s %s %s\n", IRtmp, childList1->a, node->text, childList2->a);
    irList = (IRList)malloc(sizeof(IRnode));
    strcpy(irList->a, IRtmp);
    break;

  case ShowASTNode:
    // TODO
    child = node->children;
    while (child != NULL){
      childList1 = IR(child->nodelist);
      if(childList1 != NULL){
        printf("\t%s = %s + 0\n", childList1->a, childList1->a);
        fprintf(IRFile, "\t%s = %s + 0\n", childList1->a, childList1->a);
      }
      child = child->next;
    }
    break;

  case IfASTNode:
    // TODO
    child = node->children;
    childList1 = IR(child->nodelist);
    sprintf(L1tmp, "L%d", labelcount);
    labelcount++;
    sprintf(L2tmp, "L%d", labelcount);
    labelcount++;
    printf("\tIFNZ %s GOTO %s\n", childList1->a, L1tmp);
    fprintf(IRFile, "IFNZ %s GOTO %s\n", childList1->a, L1tmp);
    child = child->next;
    if (child->next != NULL) {
      IR(child->next->nodelist);
    }
    printf("\tGOTO %s\n", L2tmp);
    fprintf(IRFile, "\tGOTO %s\n", L2tmp);
    printf("%s:\n", L1tmp);
    fprintf(IRFile, "%s:\n", L1tmp);
    IR(child->nodelist);
    printf("%s:\n", L2tmp);
    fprintf(IRFile, "%s:\n", L2tmp);
    break;

  case ElseASTNode:
    // TODO
    child = node->children;
    IR(child->nodelist);
    break;

  case LTASTNode:
    // // TODO
  case GTASTNode:
    // // TODO
  case EQASTNode:
    // // TODO
  case ANDASTNode:
    // // TODO
  case ORASTNode:
    // TODO
    child = node->children;
    childList1 = IR(child->nodelist);
    child = child->next;
    childList2 = IR(child->nodelist);
    sprintf(IRtmp, "T%d", ircount);
    ircount++;
    printf("\t%s = %s %s %s\n", IRtmp, childList1->a, node->text, childList2->a);
    fprintf(IRFile, "\t%s = %s %s %s\n", IRtmp, childList1->a, node->text, childList2->a);
    irList = (IRList)malloc(sizeof(IRnode));
    strcpy(irList->a, IRtmp);
    break;
  
  case WhileASTNode:
    // TODO
    sprintf(L1tmp, "L%d", labelcount);
    labelcount++;
    sprintf(L2tmp, "L%d", labelcount);
    labelcount++;
    child = node->children;
    printf("%s:\n", L1tmp);
    fprintf(IRFile, "%s:\n", L1tmp);
    childList1 = IR(child->nodelist);
    printf("\tIFZ %s GOTO %s\n", childList1->a, L2tmp);
    fprintf(IRFile, "\tIFZ %s GOTO %s\n", childList1->a, L2tmp);
    child = child->next;
    IR(child->nodelist);
    printf("\tGOTO %s\n", L1tmp);
    fprintf(IRFile, "\tGOTO %s\n", L1tmp);
    printf("%s:\n", L2tmp);
    fprintf(IRFile, "%s:\n", L2tmp);
    break;

  case BooleASTNode:
    // TODO
    child = node->children;
    irList = IR(child->nodelist); 
    break;

	default:
		break;
	}
	return irList;
}


int main(int argc, char** argv) {
  // sFile = fopen("./text_compile/test.txt", "r");
  sFile = fopen(argv[1], "r");
  IRFile = fopen("./IR.txt", "w");

  if (sFile == NULL) {
    printf("打开文件失败!\n");
    return 0;
  }
  list = NULL;
  TokenReader();

  // TokenList p = list;
  // printf("-词法分析\n");
  // while (p != NULL) {
  //   printf("Token = '%s'\tType = % d\n", p->text, p->type);
  //   p = p->next;
  // }

  // printf(
  //     "\n\n-语法分析\n\n"
  //     "--打印语法树\n");
  pList = list;
  ASTNodeList AST = Programming();

  if (AST != NULL) {
    dumpAST(AST, 0);
    printf("\n--显示show函数中的变量值\n");
    evaluate(AST);
    printf("\n--打印中间代码\n\n");
    IR(AST);
  }
  fclose(sFile);
  fclose(IRFile);
  return 0;
}
