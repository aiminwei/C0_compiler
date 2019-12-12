/*����C0�ķ�-��-���������ʵ��
author :   Mr.Wei
assignment for compiler,Dec,2016*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define keyn      13                //no of reservered word
#define keywordn  100
#define ilong     50
#define dlong     10
#define strlong   300
#define llong     220
#define tmax      200
#define amax      40
#define cmax      2000
#define ctmax     39
#define calmax    7
#define errtp     48
#define errmax    50
#define stacksize 1000
#define conmax    100              //ÿ�ֳ�������������Ŀ

enum keyword {charsy = 1,constsy,elsesy,floatsy,forsy,ifsy,intsy,mainsy,printfsy,returnsy,scanfsy,voidsy,whilesy,intcon,floatcon,charcon,stringcon,ident,comma,semicolon,
              lparent,rparent,lbrack,rbrack,lbrace,rbrace,plussy,minussy,timessy,divsy,
              eql,neq,lss,leq,gtr,geq,
              become,sq,dq,endsy};
enum objtype    {constant = 1,variable,funztion};
enum stitype   {nottp = 1,intty,floatty,charty,arrayty};

char errmsg[errtp][35] =
{
    "ident too long",
    "digit too long",
    "invalid char",
    "without single quota",
    "string too long",
    "repeat defined",
    "too much symbol",
    "undefined word",
    "unsignedint error",
    "without = ",
    "typedef inconsistent",
    "constdef error",
    "without ;",
    "not ident",
    "without ]",
    "withour )",
    "no_ret_func not factor",
    "without [",
    "array index not int",
    "out of array size",
    "format error",
    "unknown factor",
    "type inconsistent",
    "no type times",
    "no type div",
    "without if",
    "without (",
    "without while",
    "without for",
    "not variable no change",
    "step on one ident",
    "without scanf",
    "without printf",
    "without return",
    "ident not function",
    "paranum inconsistent",
    "ident not array",
    "array can't be assigned",
    "without }",
    "unknown statement",
    "without main",
    "without {",
    "function void type",
    "without void",
    "without main func",
    "redundant code",
    "out of code array",
    "return not inconsistent"
};

int skipend[41];                   //�������ż������飬����Ԫ��ֵ����1��ʾ��Ӧ�ķ��ſ�����Ϊ��������

char key_word[keywordn][ilong] =
{
    "",
    "charsy",
    "constsy",
    "elsesy",
    "floatsy",
    "forsy",
    "ifsy",
    "intsy",
    "mainsy",
    "printfsy",
    "returnsy",
    "scanfsy",
    "voidsy",
    "whilesy",
    "intcon",
    "floatcon",
    "charcon",
    "stringcon",
    "ident",
    "comma",
    "semicolon",
    "lparent",
    "rparent",
    "lbrack",
    "rbrack",
    "lbrace",
    "rbrace",
    "plussy",
    "minussy",
    "timessy",
    "divsy",
    "eql",
    "neq",
    "lss",
    "leq",
    "gtr",
    "geq",
    "become",
    "sq",
    "dq",
    ""
};

char reword[keyn+1][ilong] =                  //���屣����
{
    "NULL\0        ",
    "char\0        ",
    "const\0       ",
    "else\0        ",
    "float\0       ",
    "for\0         ",
    "if\0          ",
    "int\0         ",
    "main\0        ",
    "printf\0      ",
    "return\0      ",
    "scanf\0       ",
    "void\0        ",
    "while\0       ",
};

char code_mnemonic[ctmax][4] =
{
    "LDA",//0
    "LOD",//1
    "LDI",//2
    "INT",//3
    "JMP",//4
    "JPC",//5
    "CAL",//6
    "LDC",//7
    "LDR",//8
    "FLT",//9
    "RED",//10           //dx��ʶ���ͣ�1�����ͣ�2��ʵ�� 3���ַ���
    "WRS",//11
    "EXF",//12
    "LDT",//13
    "MUS",//14
    "WRR",//15
    "STO",//16
    "EQR",//17
    "NER",//18
    "LSR",//19
    "LER",//20
    "GTR",//21
    "GER",//22
    "EQL",//23
    "NEQ",//24
    "LSS",//25
    "LER",//26
    "GRT",//27
    "GEQ",//28
    "ADD",//29
    "SUB",//30
    "ADR",//31
    "SUR",//32
    "MUL",//33
    "DIV",//34
    "MUR",//35
    "DIR",//36
    "WRW",//37
    "MKS"//38
};

typedef struct{                               //���ű�ṹ�彨��
    char name[ilong];                         //���ŵ�����
    enum objtype ty;                          //�÷�������Ӧ�����ͣ������������ͺ�����
    enum stitype typ;                         //�������ͣ����ͣ������ͣ��ַ��ͺ�����
    int  aindex;                              //������Ϣ������ָ�룬����Ϊ-1����Ϊ�����򱣴溯����ռ�ռ�Ĵ�С
    int  addr;                                //����Ǻ����ͼ�¼�ú����������ʼ��ַ����������ͳ����ͼ�¼������ֵ�������¼�����ڳ�����ĵ�ַ������Ǳ����ͼ�¼�����ĺ����ڵ���Ե�ַ��������ַ�����ֻ���ַ�������ʼ��ַ
    int  para;                                //��������Ǻ�������˱�����¼��������������Ϊ-1
}table;

typedef struct{                               //������Ϣ������
    int  asize;                               //����Ĵ�С
    enum stitype typ;                         //����Ԫ�ص���������
}arraytable;

table tab[tmax];                              //���ű�
arraytable atab[amax];                        //������Ϣ������

int tx = 0;
int atx = 0;

double flcon[conmax];                         //�����ͳ�����
char   chcon[conmax];                         //�ַ��ͳ�����
char   str[strlong];                          //�ַ��������洢����

int flc = 0;
int chc = 0;
int strc = 0;

int strr = 0;

FILE * fp;
FILE * fp_re;
FILE * fp_er;
char line[llong];
int lc;
int lleng;
int cc;
int errnum = 0;
int inum;
double fl;
char ch;
char word[ilong] = "";
enum keyword sym;
enum stitype symtp;
int dx = 0;

void ret_funcdef(enum keyword kw);                        //������ʽ������
void statement();
void statementlist();
void call(int i);
void expression();
void getsym();

typedef struct{
    int fct;
    int lev;
    int dx;
}order;

order code[cmax];
int cx = 1;

double stack_C[stacksize];         //����ջ
int gap = -2;                      //��¼ȫ�ֱ��������һ��Ԫ�صĵ�ַ
int ddx = -2;                      //ȫ�ֱ�����ռ�Ĵ�С�ռ�
int mainsize = 0;                  //��������ռ�ռ��С


typedef struct{
    enum stitype typ;            //��������ֵ������
    int i;                       //���������������ֵ
    float f;                     //������ֵ
}item;

int over = 1;                                             //�����ж��������Ƿ����
item ret;                                                 //�����жϺ�������ֵ

void skip()                        //���������
{
    while(skipend[sym] == 0)
    {
        getsym();
    }
    return;
}

void error(int n)
{
    if(errnum >= errmax)
    {
        //fprintf(fp_er,"too much error!\n");
        printf("too much error!");
    }
    else
    {
        //fprintf(fp_er,"err %d :this program error in the line %d,%d:%s\n",errnum+1,lc,cc,errmsg[n]);
        printf("err %d :this program error in the line %d,%d:%s\n",errnum+1,lc,cc,errmsg[n]);
        errnum++;
    }
    return;
}

void emit(int f,int l,int d)
{
    if(cx == cmax)
        error(46);
    else
    {
        code[cx].fct = f;
        code[cx].lev = l;
        code[cx++].dx = d;
    }
    return;
}

void nextc()
{
    int len,i;
    cc++;
    if(cc == lleng)
    {
        for(i = 0;i < llong;i++)
            line[i] = 0;
        if(fgets(line,llong,fp) == NULL)
        {
            ch = 0;
            return;
        }
        lc++;
        len = strlen(line);
        lleng = len;
        cc = 0;
    }
    ch = line[cc];
    return;
}

void getsym()
{
    int ind;
    for(ind = 0;ind < ilong;ind++)
        word[ind] = 0;
    inum = 0;
    fl = 0;

    int k,i,j,m;

    while(ch == ' '||ch == '\t'||ch == '\n')
        nextc();

    if((ch >= 'a' && ch <= 'z')||(ch >= 'A' && ch <= 'Z')||ch == '_')
    {
        k = 0;
        do{
            if(k == ilong)
            {
                error(0);
                break;
            }
            word[k++] = ch;
            nextc();
        }while((ch >= 'a' && ch <= 'z')||(ch >= 'A' && ch <= 'Z')||(ch >= '0' && ch <= '9')||ch == '_');
        if(k == ilong)
        {
            while((ch >= 'a' && ch <= 'z')||(ch >= 'A' && ch <= 'Z')||(ch >= '0' && ch <= '9')||ch == '_')
                nextc();
        }
        else
            word[k] = 0;
        i = 1;
        j = keyn;
        do{
            m = (i+j)/2;
            if(strcmp(word,reword[m]) <= 0)
                j = m - 1;
            if(strcmp(word,reword[m]) >= 0)
                i = m + 1;
        }while(i <= j);
        if(i-1 > j)
            sym = m;
        else
            sym = ident;
    }

    else if(ch >= '0' && ch <= '9')
    {
        k = 0;
        do{
            if(k == dlong)
            {
                error(1);
                break;
            }
            inum = inum * 10+ ch - '0';
            nextc();
            k++;
        }while(ch >= '0' && ch <= '9');
        if(k == dlong)
        {
            while(ch >= '0' && ch <= '9')
                nextc();
        }
        if(ch == '.')
        {
            sym = floatcon;
            fl = inum;
            nextc();
            float e = 1.0;
            while(ch >= '0' && ch <= '9')
            {
                if(k == dlong)
                {
                    error(1);
                    break;
                }
                e = e * 10;
                fl += (ch - '0')/e;
                nextc();
                k++;
            }
            if(k == dlong)
            {
                while(ch >= '0' && ch <= '9')
                    nextc();
            }
            inum = 0;
        }
        else
            sym = intcon;
    }

    else if(ch == '=')
    {
        nextc();
        if(ch == '=')
        {
            sym = eql;
            word[0] = '=';word[1] = '=';word[2] = 0;
            nextc();
        }
        else
        {
            sym = become;
            word[0] = '=';word[1] = 0;
        }

    }
    else if(ch == '<')
    {
        nextc();
        if(ch == '=')
        {
            sym = leq;
            word[0] = '<';word[1] = '=';word[2] = 0;
            nextc();
        }
        else
        {
            sym = lss;
            word[0] = '<';word[1] = 0;
        }

    }
    else if(ch == '>')
    {
        nextc();
        if(ch == '=')
        {
            sym = geq;
            word[0] = '>';word[1] = '=';word[2] =  0;
            nextc();
        }
        else
        {
            sym = gtr;
            word[0] = '>';word[1] = 0;
        }

    }
    else if(ch == '!')
    {
        nextc();
        if(ch != '=')
            error(2);
        else
        {
            sym = neq;
            word[0] = '!';word[1] = '=';word[2] = 0;
            nextc();
        }
    }

    else if(ch == '+'||ch == '-'||ch == '*'||ch == '/')
    {
        if(ch == '+')
            sym = plussy;
        else if(ch == '-')
            sym = minussy;
        else if(ch == '*')
            sym = timessy;
        else
            sym = divsy;
        word[0] = ch;word[2] = 0;
        nextc();
    }

    else if(ch == '('||ch == ')'||ch == '['||ch == ']'||ch == '{'||ch == '}')
    {
        if(ch == '(')
            sym = lparent;
        else if(ch == ')')
            sym = rparent;
        else if(ch == '[')
            sym = lbrack;
        else if(ch == ']')
            sym = rbrack;
        else if(ch == '{')
            sym = lbrace;
        else
            sym = rbrace;
        word[0] = ch;word[1] = 0;
        nextc();
    }

    else if(ch == 39)
    {
        nextc();
        if((ch >= '0' && ch <= '9')||(ch >= 'a' && ch <= 'z')||(ch >= 'A'&&ch <= 'Z')||ch == '_'||ch == '+'||ch == '-'||ch == '*'||ch == '/')
        {
            word[0] = ch;word[1] = 0;
        }
        else
        {
            word[0] = 0;
            error(2);
        }
        nextc();
        if(ch != 39)
            error(3);
        else
            nextc();
        sym = charcon;
    }
    else if(ch == '"')
    {
        nextc();
        strr = strc;
        while(ch != '"')
        {
            if(strc == strlong)
            {
                error(4);
                break;
            }
            str[strc++] = ch;
            nextc();
        }
        if(strc == strlong)
        {
            while(ch != '"')
                nextc();
            strc--;
        }
        str[strc++] = 0;
        sym = stringcon;
        nextc();
    }

    else if(ch == ',')
    {
        sym = comma;
        word[0] = ch;word[1] = 0;
        nextc();
    }
    else if(ch == ';')
    {
        sym = semicolon;
        word[0] = ch;word[1] = 0;
        nextc();
    }
    else if(ch == 0)
    {
        sym = endsy;
        return;
    }
    else
    {
        error(2);
        nextc();
    }
    return;
}

void enter(char *name,enum objtype ty)
{
    int i = tx;
    int j;
    for(j = 0;j<strlen(name);j++)
        if(name[j] >= 'A' && name[j] <= 'Z')
            name[j] = name[j] - 'A' + 'a';
    strcpy(tab[0].name,name);
    if(ty == funztion)
    {
        while(strcmp(tab[i].name,name))
            i--;
    }
    else
    {
        while(strcmp(tab[i].name,name))
        {
            if(tab[i].ty == funztion && tab[i].aindex == -1)
            {
                i = 0;
                break;
            }
            i--;
        }
    }
    if(i != 0)
        error(5);
    else
    {
        tx++;
        if(tx >= tmax)
            error(6);
        else
        {
            strcpy(tab[tx].name,name);
            tab[tx].ty = ty;
            tab[tx].typ = nottp;
            tab[tx].addr = 0;
            tab[tx].aindex = -1;
            tab[tx].para = -1;
        }
    }
    return;
}

int sertable(char *name)
{
    int i = tx;
    int j;
    for(j = 0;j<strlen(name);j++)
        if(name[j] >= 'A' && name[j] <= 'Z')
            name[j] = name[j] - 'A' + 'a';
    strcpy(tab[0].name,name);
    while(strcmp(tab[i].name,name))
        i--;
    if(i == 0)
        error(7);
    return i;
}

void unsignedint()
{
    while(ch == ' '||ch == '\t'||ch == '\n')
        nextc();
    if(!(ch >= '1' && ch <= '9'))
        error(8);
    else
    {
        getsym();
        if(sym != intcon)
            error(8);
    }
    return;
}

void quit_table()
{
    int i,j,k,m;
    for(i = tx;i > 0;i--)
        if(tab[i].ty == funztion)
            break;
    j = tab[i].para;
    for(i = i + 1;j > 0;j--,i++)
        for(m = 0;m<ilong;m++)
            tab[i].name[m] = 0;
    k = i-1;
    for( ;i <= tx;i++)
    {
        for(m = 0;m<ilong;m++)
            tab[i].name[m] = 0;
        tab[i].ty = 0;
        tab[i].typ = 0;
        tab[i].aindex = 0;
        tab[i].addr = 0;
        tab[i].para = 0;
    }
    tx = k;
}

void constantdef()
{
    enum keyword kw;
    if(sym == charsy || sym == intsy || sym == floatsy)
    {
        kw = sym;
        getsym();
        while(sym == ident)
        {
            enter(word,constant);
            getsym();
            if(sym == become)
                getsym();
            else
            {
                error(9);
                if(sym == eql)
                    getsym();
                else
                {
                    skip();
                    if(sym == semicolon)
                        break;
                    else
                    {
                        getsym();
                        continue;
                    }
                }
            }
            if(kw == charsy)
            {
                if(sym != charcon && sym != intcon)
                    error(10);
                else
                {
                    tab[tx].typ = charty;
                    tab[tx].addr = chc;
                    if(sym == charcon)
                        chcon[chc++] = word[0];
                    else
                        chcon[chc++] = inum;
                }
            }
            else if(kw == intsy)
            {
                int sign = 1;
                if(sym == plussy || sym == minussy)
                {
                    if(sym == minussy)
                        sign = -1;
                    getsym();
                }
                if(sym != intcon)
                    error(10);
                else
                {
                    tab[tx].typ = intty;
                    inum = inum*sign;
                    tab[tx].addr = inum;
                }
            }
            else
            {
                int sign1 = 1,sign2 = 1;
                if(sym == plussy || sym == minussy)
                {
                    if(sym == minussy)
                        sign1 = -1;
                    getsym();
                }
                if(sym == plussy || sym == minussy)
                {
                    if(sym == minussy)
                        sign2 = -1;
                    getsym();
                }
                tab[tx].typ = floatty;
                tab[tx].addr = flc;
                if(sym == intcon)
                    flcon[flc++] = (double)inum*sign1*sign2;
                else
                    flcon[flc++] = fl*sign1*sign2;
            }
            getsym();
            if(sym != comma)
                break;
            else
                getsym();
        }
    }
    else
        error(11);
    return;
}

void constantcon()
{
    while(sym == constsy)
    {
        getsym();
        skipend[comma]++;
        constantdef();
        skipend[comma]--;
        if(sym != semicolon)
            error(12);
        else
        {
            getsym();
            //printf("this is constant define statement!\n");
        }
    }
    return;
}

void variablecon()
{
    enum keyword kw;
    int array_size;
    while(sym == intsy || sym == charsy || sym == floatsy)
    {
        kw = sym;
        getsym();
        skipend[comma]++;
        if(sym != ident)
        {
            error(13);
            skip();
            if(sym == semicolon)
                continue;
            else
            {
                while(sym != ident && sym != semicolon)
                    getsym();
            }
        }
        while(sym == ident)
        {
            enter(word,variable);
            getsym();
            if(sym == lparent)
            {
                skipend[comma]--;
                ret_funcdef(kw);
                return;
            }
            else if(sym == lbrack)
            {
                skipend[rbrack]++;
                unsignedint();
                if(sym != intcon)
                    error(22);
                array_size = inum;
                getsym();
                if(sym != rbrack)
                {
                    error(14);
                    skip();
                    skipend[rbrack]--;
                    if(sym == semicolon)
                        break;
                    else if(sym == comma)
                    {
                        getsym();
                        continue;
                    }
                }
                else
                {
                    skipend[rbrack]--;
                    atab[atx].asize = array_size;
                    switch(kw){
                        case intsy:atab[atx].typ = intty;
                                    break;
                        case charsy:atab[atx].typ = charty;
                                    break;
                        case floatsy:atab[atx].typ = floatty;
                                    break;
                        default:error(10);
                    }
                    tab[tx].addr = dx;
                    dx += array_size;
                    tab[tx].typ = arrayty;
                    tab[tx].aindex = atx++;
                    getsym();
                }
            }
            else
            {
                switch(kw){
                    case intsy:tab[tx].typ = intty;
                                break;
                    case charsy:tab[tx].typ = charty;
                                break;
                    case floatsy:tab[tx].typ = floatty;
                                break;
                    default:error(10);
                }
                tab[tx].addr = dx;
                dx += 1;
            }
            if(sym != comma)
                break;
            else
                getsym();
        }
        skipend[comma]--;
        if(sym != semicolon)
            error(12);
        else
        {
            getsym();
            //printf("this is a varibale definition statement!\n");
        }
    }
}

int parameterlist()
{
    int paranum = 0;
    enum keyword kw;
    while(sym == intsy || sym == charsy || sym == floatsy)
    {
        kw = sym;
        getsym();
        if(sym != ident)
        {
            error(13);
            skipend[comma]++;
            skip();
            skipend[comma]--;
            if(sym == comma)
            {
                getsym();
                continue;
            }
            else
                break;
        }
        else
        {
            paranum++;
            enter(word,variable);
            switch(kw){
                case intsy:tab[tx].typ = intty;
                            break;
                case charsy:tab[tx].typ = charty;
                            break;
                case floatsy:tab[tx].typ = floatty;
                            break;
                default:error(10);
            }
            tab[tx].addr = dx;
            dx += 1;
        }
        getsym();
        if(sym == comma)
            getsym();
        else
            break;
    }
    if(sym != rparent)
        error(15);
    return paranum;
}

void factor(item *x)
{
    int i;
    item y;
    if(sym == ident)
    {
        i = sertable(word);
        if(i == 0)
        {
            skip();
        }
        else if(tab[i].ty == funztion)
        {
            if(tab[i].typ == nottp)
            {
                error(16);
                skip();
            }
            else
            {
                call(i);
                //emit("������ת��tab[i].addr��ַ��");
                x->typ = tab[i].typ;
                if(tab[i].typ == intty || tab[i].typ == charty)
                    x->i = ret.i;
                else
                    x->f = ret.f;
            }
        }
        else
        {
            if(tab[i].ty == constant)
            {
                if(tab[i].typ == intty)
                {
                    emit(2,0,tab[i].addr);
                    //emit("����ָ��ȡaddr��ֵ");
                    x->typ = tab[i].typ;
                    getsym();
                }
                else if(tab[i].typ == charty)
                {
                    emit(7,0,tab[i].addr);
                    //emit("����ָ��ȡchcon[addr]��ֵ");
                    x->typ = tab[i].typ;
                    getsym();
                }
                else
                {
                    emit(8,0,tab[i].addr);
                    //emit("����ָ��ȡflcon[addr]��ֵ");
                    x->typ = tab[i].typ;
                    getsym();
                }
            }
            else
            {
                if(tab[i].typ == arrayty)
                {
                    if(i > gap)           //˵���Ǿֲ�����
                        emit(0,1,tab[i].addr);
                    else
                        emit(0,0,tab[i].addr);
                    //emit("��ַaddr���ص�ջ��");
                    getsym();
                    if(sym != lbrack)
                    {
                        error(17);
                        skip();
                    }
                    getsym();
                    expression(&y);
                    if(y.typ != intty)
                        error(18);
                    //if(y.i >= atab[tab[i].aindex].asize)
                    //    error(19);
                    emit(29,0,0);
                    emit(13,0,0);
                    //emit("��ջ������ջ��");
                    //emit("ȡջ����ַ����ֵ);
                    x->typ = atab[tab[i].aindex].typ;
                    if(sym != rbrack)
                    {
                        error(14);
                        skipend[rbrack]++;
                        skip();
                        skipend[rbrack]--;
                    }
                    else
                        getsym();
                }
                else
                {
                    if(i > gap)           //˵���Ǿֲ�����
                        emit(1,1,tab[i].addr);
                    else
                        emit(1,0,tab[i].addr);
                    //emit("����addr����Ӧ��ֵַ���ص�ջ��");
                    x->typ = tab[i].typ;
                    getsym();
                }
            }
        }
    }
    else if(sym == minussy || sym == plussy || sym == intcon || sym == floatcon || sym == charcon)
    {
        if(sym == minussy || sym == plussy)
        {
            int sign = 1;
            if(sym == minussy)
                sign = -1;
            getsym();
            if(sym == intcon)
            {
                inum = inum*sign;
                emit(2,0,inum);
                //emit("����ָ���inum����ջ��");
                x->typ = intty;
                getsym();
            }
            else if(sym == plussy || sym == minussy)
            {
                int sign1 = 1;
                if(sym == minussy)
                    sign1 = -1;
                getsym();
                if(sym != floatcon)
                {
                    if(sym == intcon)
                        fl = (double)inum;
                    else
                    {
                        error(20);
                        fl = 0;
                    }
                }
                fl = fl * sign * sign1;
                flcon[flc++] = fl;
                emit(8,0,flc-1);
                //emit("����ָ���fl����ջ��");
                x->typ = floatty;
                getsym();
            }
            else if(sym == floatcon)
            {
                fl = fl * sign;
                flcon[flc++] = fl;
                emit(8,0,flc-1);
                //emit("����ָ���fl����ջ��");
                x->typ = floatty;
                getsym();
            }
        }
        else
        {
            if(sym == intcon)
            {
                emit(2,0,inum);
                //emit("����ָ���inumֵ����ջ��");
                x->typ = intty;
                getsym();
            }
            else if(sym == floatcon)
            {
                flcon[flc++] = fl;
                emit(8,0,flc-1);
                //emit("����ָ���flֵ����ջ��");
                x->typ = floatty;
                getsym();
            }
            else
            {
                inum = (int)word[0];
                emit(2,0,inum);
                //emit("����ָ���inumֵ����ջ��");
                x->typ = charty;
                getsym();
            }
        }
    }
    else if(sym == lparent)
    {
        getsym();
        skipend[rparent]++;
        expression(x);
        if(sym != rparent)
        {
            error(15);
            skip();
        }
        else
            getsym();
        skipend[rparent]--;
    }
    else
        error(21);
    return;
}

void term(item *x)
{
    item y;
    factor(x);
    if(sym == timessy || sym == divsy)
    {
        while(sym == timessy || sym == divsy)
        {
            if(sym == timessy)
            {
                getsym();
                factor(&y);
                if(x->typ != y.typ)
                {
                    if(x->typ == floatty && y.typ == intty)
                    {
                        y.typ = floatty;
                        emit(9,0,0);
                        //emit("����ָ���ջ��������ת��Ϊ��������");
                    }
                    else if(x->typ == intty && y.typ == floatty)
                    {
                        x->typ = floatty;
                        emit(9,0,1);
                        //emit("����ָ��Ѵ�ջ��������ת��Ϊ��������");
                    }
                    else if((x->typ == intty && y.typ == charty) || (x->typ == charty && y.typ == intty))
                    {
                        x->typ = intty;
                        y.typ = intty;
                    }
                    else
                        error(22);
                }
                if(x->typ == intty)
                    emit(33,0,0);//emit("����ָ���ջ���ʹ�ջ�����");
                else if(x->typ == floatty)
                    emit(35,0,0);//emit("����ָ���ջ���ʹ�ջ�����");
                else
                    error(23);
            }
            else
            {
                getsym();
                factor(&y);
                if(x->typ != y.typ)
                {
                    if(x->typ == floatty && y.typ == intty)
                    {
                        y.typ = floatty;
                        emit(9,0,0);
                        //emit("����ָ���ջ��������ת��Ϊ��������");
                    }
                    else if(x->typ == intty && y.typ == floatty)
                    {
                        x->typ = floatty;
                        emit(9,0,1);
                        //emit("����ָ��Ѵ�ջ��������ת��Ϊ��������");
                    }
                    else if((x->typ == intty && y.typ == charty) || (x->typ == charty && y.typ == intty))
                    {
                        x->typ = intty;
                        y.typ = intty;
                    }
                    else
                        error(22);
                }
                if(x->typ == intty)
                    emit(34,0,0);//emit("����ָ��Ѵ�ջ����ջ�����");
                else if(x->typ == floatty)
                    emit(36,0,0);//emit("����ָ��Ѵ�ջ����ջ�����");
                else
                    error(24);
            }
        }
    }
    return;
}

void expression(item *x)
{
    item y;
    int sign = 1;
    if(sym == plussy || sym == minussy)
    {
        if(sym == minussy)
            sign = -1;
        getsym();
    }
    term(x);
    if(sign == -1)
        emit(14,0,0);//emit("����ָ���ջ��Ԫ��ȡ��");
    while(sym == plussy || sym == minussy)
    {
        if(sym == plussy)
        {
            getsym();
            term(&y);
            if(x->typ != y.typ)
            {
                if(x->typ == floatty && y.typ == intty)
                {
                    y.typ = floatty;
                    emit(9,0,0);
                    //emit("����ָ���ջ��������ת��Ϊ��������");
                }
                else if(x->typ == intty && y.typ == floatty)
                {
                    x->typ = floatty;
                    emit(9,0,1);
                    //emit("����ָ��Ѵ�ջ��������ת��Ϊ��������");
                }
                else if((x->typ == intty && y.typ == charty)||(x->typ == charty && y.typ == intty))
                {
                    x->typ = intty;
                    y.typ = intty;
                }
                else
                    error(22);
            }
            if(x->typ == intty || x->typ == charty)
                emit(29,0,0);//emit("����ָ���ջ���ʹ�ջ�����");
            else if(x->typ == floatty)
                emit(31,0,0);//emit("����ָ���ջ���ʹ�ջ�����");
            else
                error(22);
        }
        else
        {
            getsym();
            term(&y);
            if(x->typ != y.typ)
            {
                if(x->typ == floatty && y.typ == intty)
                {
                    y.typ = floatty;
                    emit(9,0,0);
                    //emit("����ָ���ջ��������ת��Ϊ��������");
                }
                else if(x->typ == intty && y.typ == floatty)
                {
                    x->typ = floatty;
                    emit(9,0,1);
                    //emit("����ָ��Ѵ�ջ��������ת��Ϊ��������");
                }
                else if((x->typ == intty && y.typ == charty)||(x->typ == charty && y.typ == intty))
                {
                    x->typ = intty;
                    y.typ = intty;
                }
                else
                    error(22);
            }
            if(x->typ == intty || x->typ == charty)
                emit(30,0,0);//emit("����ָ��Ѵ�ջ����ջ�����");
            else if(x->typ == floatty)
                emit(32,0,0);//emit("����ָ��Ѵ�ջ����ջ�����");
            else
                error(22);
        }
    }
    return;
}

void condition()
{
    enum keyword kw;
    item x,y;
    expression(&x);
    if(sym == eql || sym == neq || sym == lss || sym == leq || sym == gtr || sym == geq)
    {
        kw = sym;
        getsym();
        expression(&y);
        if(x.typ != y.typ)
        {
            if(x.typ == intty && y.typ == floatty)
            {
                x.typ = floatty;
                emit(9,0,1);
                //emit("����ָ��Ѵ�ջ��������ת��Ϊ��������");
            }
            else if(x.typ == floatty && y.typ == intty)
            {
                y.typ = floatty;
                emit(9,0,0);
                //emit("����ָ���ջ��������ת��Ϊ��������");
            }
            else if((x.typ == intty && y.typ == charty)||(x.typ == charty && y.typ == intty))
            {
                x.typ = intty;
                y.typ = intty;
            }
            else
                error(22);
        }
        if(x.typ == intty || x.typ == charty)
        {
            if(kw == eql)
                emit(23,0,0);
            else if(kw == neq)
                emit(24,0,0);
            else if(kw == lss)
                emit(25,0,0);
            else if(kw == leq)
                emit(26,0,0);
            else if(kw == gtr)
                emit(27,0,0);
            else
                emit(28,0,0);
        }
        //emit("�������ͱȽϴ���");
        else
        {
            if(kw == eql)
                emit(17,0,0);
            else if(kw == neq)
                emit(18,0,0);
            else if(kw == lss)
                emit(19,0,0);
            else if(kw == leq)
                emit(20,0,0);
            else if(kw == gtr)
                emit(21,0,0);
            else
                emit(22,0,0);
        }
        //emit("���ɸ����ͱȽϴ���");
    }
    return;
}

void ifstatement()
{
    int c0,c1;
    if(sym != ifsy)
        error(25);
    getsym();
    if(sym != lparent)
        error(26);
    else
    {
        getsym();
        skipend[rparent]++;
        condition();
        if(sym != rparent)
        {
            error(15);
            skip();
        }
        skipend[rparent]--;
        getsym();
    }
    c0 = cx;
    emit(5,0,0);
    statement();
    c1 = cx;
    emit(4,0,0);
    if(sym == elsesy)
    {
        getsym();
        code[c0].dx = cx;
        statement();
    }
    else
    {
        code[c0].dx = cx;
    }
    code[c1].dx = cx;
    //printf("this is a ifstatement!\n");
    return;
}

void whilestatement()
{
    int c0,c1;
    if(sym != whilesy)
        error(27);
    getsym();
    if(sym != lparent)
        error(26);
    else
    {
        getsym();
        c0 = cx;
        skipend[rparent]++;
        condition();
        if(sym != rparent)
        {
            error(15);
            skip();
        }
        skipend[rparent]--;
        getsym();
    }
    c1 = cx;
    emit(5,0,0);
    statement();
    emit(4,0,c0);
    code[c1].dx = cx;
    //printf("this is a whilestatement!\n");
    return;
}

void forstatement()
{
    int c0,c1,c2;
    item x;
    int i;
    if(sym != forsy)
        error(28);
    getsym();
    if(sym != lparent)
        error(26);
    else
    {
        getsym();
        if(sym != ident)
            error(13);
        i = sertable(word);
        if(tab[i].ty != variable)
        {
            error(29);
            getsym();
        }
        else
        {
            if(i > gap)
                emit(0,1,tab[i].addr);
            else
                emit(0,0,tab[i].addr);
            //emit("addr��ջ��");
            getsym();
            if(sym != become)
                error(9);
            getsym();
            expression(&x);
            if(x.typ != tab[i].typ)
            {
                if(x.typ == intty && tab[i].typ == floatty)
                    emit(9,0,0);//emit("����ָ���ջ��ֵ��Ϊ������");
                else if((x.typ == intty && tab[i].typ == charty)||(x.typ == charty && tab[i].typ == intty))
                {
                    x.typ = tab[i].typ;
                }
                else
                    error(22);
            }
            emit(16,0,0);
            //emit("ջ��ֵ��ֵ����ջ��");
        }
        if(sym != semicolon)
            error(12);
        else
            getsym();

        c0 = cx;
        skipend[semicolon]++;
        condition();
        skipend[semicolon]--;
        c1 = cx;
        emit(5,0,0);
        c2 = cx;
        emit(4,0,0);
        if(sym != semicolon)
            error(12);
        else
            getsym();

        if(sym != ident)
            error(13);
        i = sertable(word);
        if(tab[i].ty != variable)
        {
            error(29);
            getsym();
        }
        else
        {
            if(i > gap)
                emit(0,1,tab[i].addr);
            else
                emit(0,0,tab[i].addr);
            //emit("addr��ջ��");
            getsym();
            if(sym != become)
                error(9);
            getsym();
            if(strcmp(word,tab[i].name))
                error(30);
            if(i > gap)
                emit(1,1,tab[i].addr);
            else
                emit(1,0,tab[i].addr);
            //emit("����ֵ��ջ��");
            getsym();
            if(sym != plussy && sym != minussy)
                error(20);
            else
            {
                if(sym == plussy)
                {
                    unsignedint();
                    emit(2,0,inum);
                    if(tab[i].typ == intty || tab[i].typ == charty)
                        emit(29,0,0);
                    else
                        emit(31,0,0);
                    emit(16,0,0);
                    //emit("����ָ���inumֵ����ջ��");
                    //emit("ջ����ջ�����");
                    //emit("ֵ���ջ����ַ��");
                    getsym();
                }
                else
                {
                    unsignedint();
                    emit(2,0,inum);
                    if(tab[i].typ == intty || tab[i].typ == charty)
                        emit(30,0,0);
                    else
                        emit(32,0,0);
                    emit(16,0,0);
                    //emit("����ָ���inumֵ����ջ��");
                    //emit("ջ����ջ�����");
                    //emit("ֵ���ջ����ַ��");
                    getsym();
                }
            }
            //emit("ջ��ֵ��ֵ����ջ��");
            emit(4,0,c0);
        }
        if(sym != rparent)
        {
            error(15);
            skipend[rparent]++;
            skip();
            skipend[rparent]--;
        }
        getsym();
    }
    code[c2].dx = cx;
    statement();
    emit(4,0,c2+1);
    code[c1].dx = cx;
    //printf("this is a forstatement!\n");
    return;
}

void readstatement()
{
    int i;
    if(sym != scanfsy)
        error(31);
    getsym();
    if(sym != lparent)
    {
        error(26);
        skip();
    }
    else
    {
        do{
            getsym();
            if(sym != ident)
                error(13);
            else
            {
                i = sertable(word);
                if(tab[i].ty != variable)
                    error(29);
                else
                {
                    if(i > gap)
                        emit(0,1,tab[i].addr);
                    else
                        emit(0,0,tab[i].addr);
                    //emit("����ָ��ȡ������ַ��ջ��");
                    if(tab[i].typ == arrayty)
                        error(37);
                    else if(tab[i].typ == intty)
                        emit(10,0,1);
                    else if(tab[i].typ == floatty)
                        emit(10,0,2);
                    else
                        emit(10,0,3);
                    //emit("read");
                    getsym();
                }
            }
        }while(sym == comma);
        if(sym != rparent)
        {
            error(15);
            skipend[rparent]++;
            skip();
            skipend[rparent]--;
            if(sym == rparent)
                getsym();
        }
        else
        {
            getsym();
            //printf("this is a readstatement!\n");
        }
    }
    return;
}

void writestatement()
{
    item x;
    if(sym != printfsy)
        error(32);
    getsym();
    if(sym != lparent)
    {
        error(26);
        skip();
    }
    else
    {
        getsym();
        if(sym == stringcon)
        {
            while(str[strr] != 0)
            {
                emit(11,0,str[strr]);
                strr++;
            }
            //emit(11,0,10);
            //emit("strr���ַ�����ʼ��ַ������ַ���");
            getsym();
            if(sym == comma)
            {
                getsym();
                expression(&x);
                if(x.typ == intty)
                    emit(37,0,0);//emit("���ջ������");
                else if (x.typ == floatty)
                    emit(15,0,0);//emit("���ջ��������");
                else
                    emit(11,0,0);//emit("���ջ���ַ�");
            }
        }
        else
        {
            expression(&x);
            if(x.typ == intty)
                emit(37,0,0);//emit("���ջ������");
            else if (x.typ == floatty)
                emit(15,0,0);//emit("���ջ��������");
            else
                emit(11,0,0);//emit("���ջ���ַ�");
        }
        if(sym != rparent)
        {
            error(15);
            skipend[rparent]++;
            skip();
            skipend[rparent]--;
            if(sym == rparent)
                getsym();
        }
        else
        {
            getsym();
            //printf("this is a writestatement!\n");
        }
    }
    return;
}

void returnstatement()
{
    item x;
    if(sym != returnsy)
        error(33);
    else
    {
        getsym();
        if(sym == lparent)
        {
            getsym();
            expression(&x);
            if(sym != rparent)
            {
                error(15);
                skipend[rparent]++;
                skip();
                skipend[rparent]--;
                if(sym == rparent)
                    getsym();
            }
            else
            {
                if(x.typ != ret.typ)
                {
                    if((x.typ == floatty && ret.typ == intty) || ret.typ == nottp)
                        error(47);
                }
                emit(12,0,1);
                //emit("ret");
                getsym();
            }
        }
        else
        {
            if(ret.typ != nottp)
                error(47);
            emit(12,0,0);
        }
        //printf("this is a returnstatement!\n");
    }
    return;
}

void call(int i)
{
    int tx0;
    item x;
    int paranum;
    tx0 = i;
    if(tab[i].ty != funztion)
    {
        error(34);
        skip();
    }
    emit(38,0,0);
    //emit("���ɳ��򱣴�abp");
    //emit("abp=esp");
    //emit("��������ջջ������aindex");
    paranum = tab[i].para;
    getsym();
    if(sym != lparent)
    {
        error(26);
        skipend[rparent]++;
        skip();
        skipend[rparent]--;
        if(sym == rparent)
            getsym();
    }
    else
    {
        getsym();
        if(sym != rparent)
        {
            skipend[rparent]++;
            expression(&x);//////////////////////////////////////
            paranum -= 1;
            i++;
            if(tab[i].typ != x.typ)
            {
                if(x.typ == intty && tab[i].typ == floatty)
                {
                    x.typ = floatty;
                    emit(9,0,0);
                    //emit("����ָ���ջ������ת��Ϊ������");
                }
                else if((x.typ == intty && tab[i].typ == charty)||(x.typ == charty &&tab[i].typ == intty))
                {
                    x.typ = tab[i].typ;
                }
                else
                    error(22);
            }
            while(sym == comma)
            {
                getsym();
                expression(&x);/////////////////////////////////
                paranum -= 1;
                i++;
                if(tab[i].typ != x.typ)
                {
                    if(x.typ == intty && tab[i].typ == floatty)
                    {
                        x.typ = floatty;
                        emit(9,0,0);
                        //emit("����ָ���ջ������ת��Ϊ������");
                    }
                    else if((x.typ == intty && tab[i].typ == charty)||(x.typ == charty &&tab[i].typ == intty))
                    {
                        x.typ = tab[i].typ;
                    }
                    else
                        error(22);
                }
            }
            skipend[rparent]--;
        }
        if(paranum != 0)
            error(35);
        if(sym != rparent)
            error(15);
        else
        {
            getsym();
            emit(6,tab[tx0].aindex-3-tab[tx0].para,tab[tx0].addr);
            //emit("������תָ���ת��tab[i].addr��");
        }
        //printf("this is a call statement!\n");
    }
    return;
}

void assignment(int i)
{
    item x;
    if(tab[i].ty == constant || tab[i].ty == funztion)
    {
        error(29);
        skip();
    }
    else
    {
        if(i > gap)
            emit(0,1,tab[i].addr);
        else
            emit(0,0,tab[i].addr);
        //emit("���ر�����ַ��ջ��");
        getsym();
        if(sym != become)
        {
            if(sym == lbrack)
            {
                if(tab[i].typ != arrayty)
                    error(36);
                else
                {
                    getsym();
                    expression(&x);
                    if(x.typ != intty)
                        error(18);
                    //asize = atab[tab[i].aindex].asize;
                    //if(x.i >= asize)
                    //    error(19);
                    emit(29,0,0);
                    //emit("ջ���Ӵ�ջ��");
                }
                if(sym != rbrack)
                    error(14);
                else
                {
                    getsym();
                    if(sym != become)
                        error(9);
                }
            }
            else
                error(20);
        }
        else
        {
            if(tab[i].typ == arrayty)
                error(37);
        }
        getsym();
        expression(&x);
        if(tab[i].typ != x.typ)
        {
            if(tab[i].typ == arrayty)
            {
                if(atab[tab[i].aindex].typ == x.typ)
                    ;
                else if(x.typ == intty && atab[tab[i].aindex].typ == floatty)
                {
                    x.typ = floatty;
                    emit(9,0,0);
                    //emit("����ָ���ջ�����͸�Ϊ������");
                }
                else if((x.typ == intty && atab[tab[i].aindex].typ == charty)||(x.typ == charty && atab[tab[i].aindex].typ == intty))
                {
                    x.typ = atab[tab[i].aindex].typ;
                }
                else
                    error(22);
            }
            else if(x.typ == intty && tab[i].typ == floatty)
            {
                x.typ = floatty;
                emit(9,0,0);
                //emit("����ָ���ջ�����͸�Ϊ������");
            }
            else if((x.typ == intty && tab[i].typ == charty)||(x.typ == charty &&tab[i].typ == intty))
            {
                x.typ = tab[i].typ;
            }
            else
                error(22);
        }
        emit(16,0,0);
        //emit("ջ��ֵ��ֵΪ��ջ��");
       // printf("this is a assignment statement!\n");
    }
    return;
}

void stat_skip()
{
    skipend[ident]++;
    skipend[ifsy]++;
    skipend[whilesy]++;
    skipend[forsy]++;
    skipend[lbrace]++;
    skipend[scanfsy]++;
    skipend[printfsy]++;
    skipend[returnsy]++;
    skipend[semicolon]++;
    skip();
    if(sym == semicolon)
        getsym();
    skipend[ident]--;
    skipend[ifsy]--;
    skipend[whilesy]--;
    skipend[forsy]--;
    skipend[lbrace]--;
    skipend[scanfsy]--;
    skipend[printfsy]--;
    skipend[returnsy]--;
    skipend[semicolon]--;
    return;
}

void statement()
{
    int i;
    if(sym == ident)
    {
        i = sertable(word);
        skipend[semicolon]++;
        if(tab[i].ty == funztion)
            call(i);
        else
            assignment(i);
        skipend[semicolon]--;
        if(sym != semicolon)
        {
            error(12);
            stat_skip();
        }
        else
            getsym();
    }
    else if(sym == ifsy || sym == whilesy || sym == forsy)
    {
        if(sym == ifsy)
            ifstatement();
        else if(sym == whilesy)
            whilestatement();
        else
            forstatement();
    }
    else if(sym == lbrace)
    {
        getsym();
        skipend[rbrace]++;
        statementlist();
        if(sym != rbrace)
        {
            error(38);
            stat_skip();
        }
        else
            getsym();
        skipend[rbrace]--;
    }
    else if(sym == scanfsy || sym == printfsy)
    {
        skipend[semicolon]++;
        if(sym == scanfsy)
            readstatement();
        else
            writestatement();
        skipend[semicolon]--;
        if(sym != semicolon)
        {
            error(12);
            stat_skip();
        }
        else
            getsym();
    }
    else if(sym == returnsy)
    {
        skipend[semicolon]++;
        returnstatement();
        skipend[semicolon]--;
        if(sym != semicolon)
        {
            error(12);
            stat_skip();
        }
        else
            getsym();
    }
    else if(sym == semicolon)
    {
        //printf("this is a empty statement!\n");
        getsym();
    }
    else
    {
        error(39);
        stat_skip();
    }
    return;
}

void statementlist()
{
    while(sym == ident || sym == ifsy || sym == whilesy || sym == forsy || sym == lbrace || sym == scanfsy || sym == printfsy || sym == returnsy || sym == semicolon)
    {
        statement();
    }
    return;
}

void comstatement(int tx0)
{
    if(sym == constsy)
        constantcon();
    if(sym == intsy || sym == charsy || sym == floatsy)
        variablecon();
    tab[tx0].aindex = dx;
    statementlist();
    return;
}

void mainfunc()
{
    int tx0;
    dx = 3;
    if(sym != mainsy)
        error(40);
    else
    {
        ret.typ = nottp;
        code[1].dx = cx;
        enter(word,funztion);
        tx0 = tx;
        getsym();
        if(sym != lparent)
            error(26);
        getsym();
        if(sym != rparent)
            error(15);
        getsym();
        if(sym != lbrace)
            error(41);
        else
            getsym();
        skipend[rbrace]++;
        tab[tx0].addr = cx;
        comstatement(tx0);
        skipend[rbrace]--;
        if(sym != rbrace)
            error(38);
        else
        {
            getsym();
            if(ret.typ != nottp)
                error(42);
            emit(12,0,0);
            //emit("ret");
            //printf("this is a main function!\n");
        }
    }
    mainsize = tab[tx0].aindex;
    quit_table();
    over = 0;
    return;
}

void un_ret_funcdef()
{
    dx = 3;
    int paranum,tx0;
    if(sym != voidsy)
        error(43);
    getsym();
    if(sym != ident)
    {
        if(sym == mainsy)
            mainfunc();
        return;
    }
    else
    {
        ret.typ = nottp;
        enter(word,funztion);
        tx0 = tx;
        getsym();
        if(sym != lparent)
            error(26);
        else
            getsym();
        skipend[rparent]++;
        paranum = parameterlist();
        skipend[rparent]--;
        tab[tx0].para = paranum;
        if(sym != rparent)
            error(15);
        else
            getsym();
        if(sym != lbrace)
            error(41);
        else
            getsym();
        skipend[rbrace]++;
        tab[tx0].addr = cx;
        comstatement(tx0);
        skipend[rbrace]--;
        if(sym != rbrace)
            error(38);
        else
        {
            getsym();
            if(ret.typ != nottp)
                error(42);
            emit(12,0,0);
            //emit("ret");
            //printf("this is a un_ret_funcdef statement!\n");
        }
    }
    quit_table();
    return;
}

void ret_funcdef(enum keyword kw)
{
    int paranum,tx0 = tx;
    if(kw == voidsy)
    {
        dx = 3;
        if(sym == charsy ||sym == intsy ||sym == floatsy)
        {
            kw = sym;
            getsym();
            if(sym != ident)
                error(13);
            else
            {
                enter(word,funztion);
                tx0 = tx;
                switch(kw){
                    case intsy:tab[tx].typ = intty;
                                ret.typ = intty;
                                break;
                    case charsy:tab[tx].typ = charty;
                                ret.typ = charty;
                                break;
                    case floatsy:tab[tx].typ = floatty;
                                ret.typ = floatty;
                                break;
                    default:error(22);
                }
                getsym();
            }
        }
        else
            error(22);
    }
    else
    {
        gap = tx0-1;
        ddx = dx;
        dx = 3;
        tab[tx0].ty = funztion;
        switch(kw){
            case intsy:tab[tx0].typ = intty;
                        ret.typ = intty;
                        break;
            case charsy:tab[tx0].typ = charty;
                        ret.typ = charty;
                        break;
            case floatsy:tab[tx0].typ = floatty;
                        ret.typ = floatty;
                        break;
            default:error(22);
        }
    }
    if(sym != lparent)
        error(26);
    else
        getsym();
    skipend[rparent]++;
    paranum = parameterlist();
    skipend[rparent]--;
    tab[tx0].para = paranum;
    if(sym != rparent)
        error(15);
    else
        getsym();
    if(sym != lbrace)
        error(41);
    else
        getsym();
    skipend[rbrace]++;
    tab[tx0].addr = cx;
    comstatement(tx0);
    skipend[rbrace]--;
    if(sym != rbrace)
        error(38);
    else
    {
        getsym();
        if(ret.typ != tab[tx0].typ)
        {
            if(ret.typ == intty && tab[tx0].typ == floatty)
            {
                ret.typ = floatty;
                ret.f = (double)ret.i;
                ret.i = 0;
            }
            else if((ret.typ == intty && tab[tx0].typ == charty)||(ret.typ == charty &&tab[tx0].typ == intty))
            {
                ret.typ = tab[tx0].typ;
            }
            else
                error(22);
        }
        if(ret.typ == nottp)
            emit(12,0,0);
        else
            emit(12,0,1);
        //emit("ret");
        //printf("this is a ret_funcdef statement!\n");
    }
    quit_table();
    ret.typ = nottp;
    ret.f = 0;
    ret.i = 0;
    return;
}

void programme()
{
    emit(4,0,0);
    if(sym == constsy)
    {
        skipend[semicolon]++;
        constantcon();
        skipend[semicolon]--;
    }
    if(sym == intsy || sym == charsy || sym == floatsy)
    {
        skipend[semicolon]++;
        variablecon();
        skipend[semicolon]--;
    }
    if(gap == -2)
        gap = tx;
    if(ddx == -2)
        ddx = dx;
    while(over == 1 && (sym == voidsy || sym == intsy || sym == charsy || sym == floatsy))
    {
        if(sym == voidsy)
            un_ret_funcdef();
        else
            ret_funcdef(voidsy);
    }
    if(sym == endsy && over == 1)
        error(44);
    else if(over == 0 && sym != endsy)
        error(45);
    else
        error(20);
    return;
}

void os_error(int pc)
{
    printf("out of stack in code:%d\n",pc-1);
    return;
}

void interpret()
{
    int calabp[calmax];
    int calnum = 0;
    int i;
    double e;
    char c;
    int esp = 0;                     //ջ��λ��
    int abp = 0;                     //��¼��ǰ����ջ�������ַ
    int pc = 1;
    skipend[endsy] = 1;
    order ic;

    esp += ddx;
    abp = esp;
    esp += 2;
    stack_C[esp] = 0;
    stack_C[esp-1] = 0;
    stack_C[esp-2] = 0;
    esp += mainsize-3;

    ic = code[pc];
    while(pc != 0){
        pc++;
        switch(ic.fct)
        {
            case 0: ++esp;
                    if(esp == stacksize)
                    {
                        os_error(pc);
                        return;
                    }
                    if(ic.lev == 0)
                        stack_C[esp] = ic.dx;
                    else
                        stack_C[esp] = abp+ic.dx;
                    break;
            case 1: ++esp;
                    if(esp == stacksize)
                    {
                        os_error(pc);
                        return;
                    }
                    if(ic.lev == 0)
                        stack_C[esp] = stack_C[ic.dx];
                    else
                        stack_C[esp] = stack_C[abp+ic.dx];
                    break;
            case 2: ++esp;
                    if(esp == stacksize)
                    {
                        os_error(pc);
                        return;
                    }
                    stack_C[esp] = ic.dx;
                    break;
            case 4: pc = ic.dx;
                    break;
            case 5: if((int)stack_C[esp] == 0)
                        pc = ic.dx;
                    break;
            case 6: if(esp+ic.lev >= stacksize)
                    {
                        os_error(pc);
                        return;
                    }
                    abp = calabp[--calnum];
                    stack_C[abp+2] = pc;
                    for(i = 1;i <= ic.lev;i++)
                        stack_C[esp+i] = 0;
                    esp += ic.lev;
                    pc = ic.dx;
                    break;
            case 7: ++esp;
                    if(esp == stacksize)
                    {
                        os_error(pc);
                        return;
                    }
                    stack_C[esp] = (int)chcon[ic.dx];
                    break;
            case 8: ++esp;
                    if(esp == stacksize)
                    {
                        os_error(pc);
                        return;
                    }
                    stack_C[esp] = flcon[ic.dx];
                    break;
            case 9: if(ic.dx == 0)
                        stack_C[esp] = (double)stack_C[esp];
                    else
                        stack_C[esp-1] = (double)stack_C[esp-1];
                    break;
            case 10:esp--;
                    if((int)stack_C[esp+1]>=stacksize)
                    {
                        os_error(pc);
                        return;
                    }
                    if(ic.dx == 1)
                    {
                        scanf("%d",&i);
                        stack_C[(int)stack_C[esp+1]] = i;
                    }
                    else if(ic.dx == 2)
                    {
                        scanf("%lf",&e);
                        stack_C[(int)stack_C[esp+1]] = e;
                    }
                    else if(ic.dx == 3)
                    {
                        scanf("%c",&c);
                        stack_C[(int)stack_C[esp+1]] = (int)c;
                    }
                    break;
            case 11:if(ic.dx == 0)
                        printf("%c",(int)stack_C[esp--]);
                    else
                        printf("%c",ic.dx);
                    break;
            case 12:if(ic.dx == 0)
                    {
                        stack_C[abp] = 0;
                        esp = abp  - 1;
                        pc = (int)stack_C[abp + 2];
                        abp = (int)stack_C[abp + 1];
                    }
                    else
                    {
                        stack_C[abp] = stack_C[esp];
                        esp = abp;
                        pc = (int)stack_C[abp + 2];
                        abp = (int)stack_C[abp + 1];
                    }
                    break;
            case 13:stack_C[esp] = stack_C[(int)stack_C[esp]];
                    break;
            case 14:stack_C[esp] = -1 * stack_C[esp];
                    break;
            case 15:esp--;
                    printf("%f\n",stack_C[esp+1]);
                    break;
            case 16:esp-=2;
                    stack_C[(int)stack_C[esp+1]] = stack_C[esp+2];
                    break;
            case 17:esp--;
                    if(stack_C[esp] == stack_C[esp+1])
                        stack_C[esp] = 1;
                    else
                        stack_C[esp] = 0;
                    break;
            case 18:esp--;
                    if(stack_C[esp] != stack_C[esp+1])
                        stack_C[esp] = 1;
                    else
                        stack_C[esp] = 0;
                    break;
            case 19:esp--;
                    if(stack_C[esp] < stack_C[esp+1])
                        stack_C[esp] = 1;
                    else
                        stack_C[esp] = 0;
                    break;
            case 20:esp--;
                    if(stack_C[esp] <= stack_C[esp+1])
                        stack_C[esp] = 1;
                    else
                        stack_C[esp] = 0;
                    break;
            case 21:esp--;
                    if(stack_C[esp] > stack_C[esp+1])
                        stack_C[esp] = 1;
                    else
                        stack_C[esp] = 0;
                    break;
            case 22:esp--;
                    if(stack_C[esp] >= stack_C[esp+1])
                        stack_C[esp] = 1;
                    else
                        stack_C[esp] = 0;
                    break;
            case 23:esp--;
                    if((int)stack_C[esp] == (int)stack_C[esp+1])
                        stack_C[esp] = 1;
                    else
                        stack_C[esp] = 0;
                    break;
            case 24:esp--;
                    if((int)stack_C[esp] != (int)stack_C[esp+1])
                        stack_C[esp] = 1;
                    else
                        stack_C[esp] = 0;
                    break;
            case 25:esp--;
                    if((int)stack_C[esp] < (int)stack_C[esp+1])
                        stack_C[esp] = 1;
                    else
                        stack_C[esp] = 0;
                    break;
            case 26:esp--;
                    if((int)stack_C[esp] <= (int)stack_C[esp+1])
                        stack_C[esp] = 1;
                    else
                        stack_C[esp] = 0;
                    break;
            case 27:esp--;
                    if((int)stack_C[esp] > (int)stack_C[esp+1])
                        stack_C[esp] = 1;
                    else
                        stack_C[esp] = 0;
                    break;
            case 28:esp--;
                    if((int)stack_C[esp] >= (int)stack_C[esp+1])
                        stack_C[esp] = 1;
                    else
                        stack_C[esp] = 0;
                    break;
            case 29:esp--;
                    stack_C[esp] = (int)stack_C[esp]+(int)stack_C[esp+1];
                    break;
            case 30:esp--;
                    stack_C[esp] = (int)stack_C[esp]-(int)stack_C[esp+1];
                    break;
            case 31:esp--;
                    stack_C[esp] = stack_C[esp]+stack_C[esp+1];
                    break;
            case 32:esp--;
                    stack_C[esp] = stack_C[esp]-stack_C[esp+1];
                    break;
            case 33:esp--;
                    stack_C[esp] = (int)((int)stack_C[esp]*(int)stack_C[esp+1]);
                    break;
            case 34:esp--;
                    stack_C[esp] = (int)((int)stack_C[esp]/(int)stack_C[esp+1]);
                    break;
            case 35:esp--;
                    stack_C[esp] = stack_C[esp]*stack_C[esp+1];
                    break;
            case 36:esp--;
                    stack_C[esp] = stack_C[esp]/stack_C[esp+1];
                    break;
            case 37:esp--;
                    printf("%d\n",(int)stack_C[esp+1]);
                    break;
            case 38:++esp;
                    if(esp + 2 >= stacksize)
                    {
                        os_error(pc);
                        return;
                    }
                    stack_C[esp] = 0;
                    stack_C[esp+1] = abp;
                    stack_C[esp+2] = 0;
                    calabp[calnum++] = esp;
                    esp += 2;
                    break;
            default:;
        }
        ic = code[pc];
        //fprintf(fp_er,"%d\n",pc);
    }
}

int main()
{
    int i;
    char path[40];
    printf("please input the file path:\n");
    scanf("%s",path);
    if(strlen(path)>=40)
    {
        printf("too long path!");
        return 0;
    }
    fp = fopen(path,"r");
    fp_re = fopen("14061103_code.txt","w");
    //fp_er = fopen("14061103_errorlist.txt","w");
    fgets(line,llong,fp);
    lleng = strlen(line);
    lc = 1;
    cc = 0;
    ch = line[cc];
    dx = 0;
    ret.typ = nottp;

    getsym();
    programme();

    printf("complete compiler!\n");
    printf("The Pcode result has been saved in the 14061103_code.txt!\n");

    for(i = 1;i<cx;i++)
        fprintf(fp_re,"%-4d %s  %d %4d\n",i,code_mnemonic[code[i].fct],code[i].lev,code[i].dx);

    printf("Then run the programme!\n");

    if(errnum == 0)
        interpret();
    else
        printf("This programme seems to be wrong,please check errors and then correct and rebulid!\n");

    fclose(fp);
    fclose(fp_re);
    //fclose(fp_er);
    return 0;
}
