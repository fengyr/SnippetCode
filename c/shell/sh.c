#include <stdlib.h>

#define NULL        ((void *)0)

#define        INTR        2        /* 进程终止信号*/ 
#define        QUIT        3        /* 同上*/ 

#define LINSIZ 1000         /* 命令参数行缓冲空间*/ 
#define ARGSIZ 50           /* Token指针列表 */ 
#define TRESIZ 100          /* 语法树所有节点空间*/ 

#define QUOTE 0200          /* 引用字符标志位，表明该字符前缀为斜杠或引号，限制字符集为ascii，八进制 */ 

/* 以下为语法树节点的属性标识，用于DFLG字段*/ 
#define FAND 1                  /* And: 该命令以后台异步进程方式执行*/ 
#define FCAT 2                  /* Catenate: 以追加方式重定向输出，相当于>> 符号*/ 
#define FPIN 4                  /* Pipe in: 命令输入重定向到管道线，即从左子树流入*/ 
#define FPOU 8                  /* Pipe out: 命令输出重定向到管道线，即向右子树流出*/ 
#define FPAR 16                 /* Parentheses: 标识复合命令中最后一个简单命令，即右括弧前的命令。Shell会为每个外部 
                                  命令（非内置）fork一个子进程上执行，但复合命令中最后一个子命令仍在原来的进程上执行。*/ 
#define FINT 32                 /* Interrupt: 如果命令是后台异步执行，则忽略进程终止信号。*/ 
#define FPRS 64                 /* Print string: 以字符串的形式打印后台进程的pid */ 

/* 以下为语法树节点的类型声明，用于DTYP字段 */ 
#define TCOM 1                  /* Command: 简单命令*/ 
#define TPAR 2                  /* Parentheses: 复合命令，包含在圆形括弧中的命令序列集合 */ 
#define TFIL 3                  /* Filter: 过滤器或者管道线，由单独的'|'或'^'符号表示 */ 
#define TLST 4                  /* List: 命令序列，多个简单命令集合，由单独的; 或'&'或'\n'符号分隔 */ 

/* 以下为语法树节点的各个字段，指定了节点大小*/ 
#define DTYP 0                  /* 节点类型，唯一标识*/ 
#define DLEF 1                  /* 左子树节点，视当前节点的类型而定*/ 
#define DRIT 2                  /* 右子树节点，视当前节点的类型而定*/ 
#define DFLG 3                  /* 节点属性，影响所属命令的状态和执行方式*/ 
#define DSPR 4                  /* 若是复合命令类型，该字段为子语法树节点*/ 
#define DCOM 5                  /* 命令参数字符串 */ 

#define        ENOMEM  12               /* 命令执行失败，内存空间不足*/ 
#define        ENOEXEC 8                /* 命令执行失败，找不到可执行文件*/ 

char        *dolp;                      /* Dollar: 指向以'$'开头的特殊变量*/ 
char        pidp[6];                    /* 字符串形式存储进程pid */ 
char        **dolv;                     /* 以空格分隔的命令行参数序列*/ 
int        dolc;                        /* 命令行参数个数*/ 
char        *promp;                     /* 输入提示符，不为空即交互模式，为空即非交互模式*/ 
char        *linep;                     /* 命令参数缓冲指针，存放下一个字符位置*/ 
char        *elinep;                    /* 命令参数缓冲区末端*/ 
char        **argp;                     /* Token列表指针，存放下一个token的位置*/ 
char        **eargp;                    /* Token列表末端*/ 
int        *treep;                      /* 语法树列表指针，存放下一个节点位置*/ 
int        *treeend;                    /* 语法树列表末端*/ 
char        peekc;                      /* 预读一个字符缓冲*/ 
char        gflg;                       /* 两种用途:  列表指针溢出标记，通配符标记*/ 
char        error;                      /* 错误计数*/ 
char        acctf;                      /*  */ 
char        uid;                        /* User id */ 
char        setintr;                    /* 忽略中断信号标识*/ 
char        *arginp;                    /* 当存在选项'-c'时，从该指针指向的字串中扫描命令参数*/ 
int        onelflg;                     /* 当存在选项'-t'时，从标准输入中扫描命令参数*/ 

/* 中断信号消息列表*/ 
char        *mesg[] = { 
    0, 
    "Hangup", 
    0, 
    "Quit", 
    "Illegal instruction", 
    "Trace/BPT trap", 
    "IOT trap", 
    "EMT trap", 
    "Floating exception", 
    "Killed", 
    "Bus error", 
    "Memory fault", 
    "Bad system call", 
    0, 
    "Sig 14", 
    "Sig 15", 
    "Sig 16", 
    "Sig 17", 
    "Sig 18", 
    "Sig 19", 
}; 

/* 记录系统时间*/ 
struct stime { 
    int proct[2]; 
    int cputim[2]; 
    int systim[2]; 
} timeb; 

char        line[LINSIZ];         /* 命令参数缓冲区*/ 
char        *args[ARGSIZ];        /* Token列表缓冲区*/ 
int        trebuf[TRESIZ];        /* 语法树节点缓冲区*/ 

void main1(); 
void word(); 
char getc(); 
int *syntax(char **p1, char **p2); 
int *syn1(char **p1, char **p2); 
int *syn2(char **p1, char **p2); 
int *syn3(char **p1, char **p2); 
void execute(int *t, int *pf1, int *pf2); 
void texec(char *f, int *at); 
void err(char *s); 
void prs(char *as); 
void prn(int n); 
void pwait(int i, int *t); 
void acct(int *t); 
void enacct(char *as); 
void put(int c); 

/* Shell入口*/ 
int main(int c, char **av) 
{ 
    int f; 
    char *acname, **v; 

    for(f=2; f<15; f++)                 /* close_on_exec，因为shell可能来自外部进程或自身fork */ 
        close(f); 
    if((f=dup(1)) != 2)                 /* 将stdout重定向到stderr，dup()将返回尚未打开的最小文件句柄*/ 
        close(f); 
    dolc = getpid();                    /* 记录当前进程pid */ 
    for(f=4; f>=0; f--) { 
        pidp[f] = dolc%10+'0';          /* itoa */ 
        dolc = dolc/10; 
    } 
    v = av; 
    acname = "/usr/adm/sha";            /* SHA加密序列，目前Unix版本已废弃*/ 
    promp = "% ";                       /* 普通用户交互提示符*/ 
    if(((uid = getuid())&0377) == 0) 
        promp = "# ";                   /* root用户交互提示符*/ 
    acctf = open(acname, 1); 
    if(c > 1) { 
        promp = 0;                      /* 进入非交互模式*/ 
        if (*v[1]=='-') {               /* 存在选项*/ 
            **v = '-';                  /* 用第一个序列字符临时标记*/ 
            if (v[1][1]=='c' && c>2)    /* '-c'选项，从arginp中扫描*/ 
                arginp = v[2]; 
            else if (v[1][1]=='t')      /* '-t'选项，从stdin中扫描*/ 
                onelflg = 2; 
        } else {                        /* 直接从命令文件中扫描*/ 
            close(0); 
            f = open(v[1], 0);          /* 重定向stdin到文件*/ 
            if(f < 0) { 
                prs(v[1]); 
                err(": cannot open"); 
            } 
        } 
    } 
    if(**v == '-') { 
        setintr++;                      /* 非交互模式下带选项，则初始化忽略所有信号*/ 
        signal(QUIT, 1); 
        signal(INTR, 1); 
    } 
    dolv = v+1; 
    dolc = c-1; 

loop: 	/* 每次loop执行一次shell语句解释，直到遇上换行符，无限循环除非整个shell进程终止 */
    if(promp != 0) 
        prs(promp);                     /* 打印交互提示符*/ 
    peekc = getc();                     /* 对于I/O流，预读一个字符，以便将来再次读取*/ 
    main1();                            /* 进入解释器 */ 
    goto loop; 

    return 0;
} 

/* 
 * 该函数包含一次shell交互的整个解释过程 
 * Preprocessor: 对输入的命令参数进行预处理
 * Lexical scanning: 对命令参数进行词法分析，分割成token列表 
 * Syntax parser: 构建语法树 
 * Semantic analyzer: 设置语法树节点属性，I/O等 
 * Executor: 为每个命令分配进程并执行 
 */ 
void main1() 
{ 
    char c, *cp; 
    int *t; 

    argp = args; 
    eargp = args+ARGSIZ-5;          /* 初始化token列表*/ 
    linep = line; 
    elinep = line+LINSIZ-5;         /* 初始化命令参数缓冲区*/ 
    error = 0;                      /* 错误计数清零*/ 
    gflg = 0;                       /* 溢出标志位清零*/ 
    do { 
        cp = linep; 
        word(); 
    } while(*cp != '\n');           /* 扫描所有命令参数，生成token列表，遇上换行符终止*/ 
    treep = trebuf; 
    treeend = &trebuf[TRESIZ];      /* 语法树缓冲区清零*/ 
    if(gflg == 0) { 
        if(error == 0) { 
            setexit(); 
            if (error) 
                return; 
            t = syntax(args, argp);         /* Scanner没有发生缓冲溢出，没有错误计数，对token列表建立语法树 */ 
        } 
        if(error != 0) 
            err("syntax error"); else 		/* 语法分析发生错误，提示并退出shell */
                execute(t, NULL, NULL);     /* 没有错误计数，从根节点开始进行语义分析 */ 
    } 
} 

/* 
 * 对命令参数扫描并分割token 
 * readc直接从流中读取一个字符，getc对前者的字符进行预处理 
 */
void word() 
{ 
    char c, c1; 

    *argp++ = linep; 	/* 将当前命令参数作为一个token放入列表，以'\0'结束，argp指向下一个token位置 */

loop: 
    switch(c = getc()) { 

        case ' ': 
        case '\t': 
            goto loop; 	/* 过滤空格和tab字符 */

        case '\'': 
        case '"': 
            c1 = c; 
            while((c=readc()) != c1) { 	/* 多读一个字符，判断是否成对（如引号） */
                if(c == '\n') { 
                    error++; 	/* 引用未结束遇上换行符，错误计数 */
                    peekc = c; 	/* 推回预读字符 */
                    return; 
                } 
                *linep++ = c|QUOTE; /* 加上引用标识 */
            } 
            goto pack; /* 引用结束，跳转至pack */

        case '&': 
        case ';': 
        case '<': 
        case '>': 
        case '(': 
        case ')': 
        case '|': 
        case '^': 
        case '\n': 
            *linep++ = c; 
            *linep++ = '\0'; 	/* 元字符等特殊字符单独作为一个token返回 */
            return; 
    } 

    peekc = c; 	/* 若为普通字符，推回预读 */

pack: 
    for(;;) { 
        c = getc(); 
        if(any(c, " '\"\t;&<>()|^\n")) { 	/* 遇上元字符、分隔符等特殊字符 */
            peekc = c; 	/* 推回预读字符 */
            if(any(c, "\"'")) 
                goto loop; 	/* 遇上引用字符，跳转到loop继续扫描 */
            *linep++ = '\0'; 	/* 分割一个token出来 */
            return; 
        } 
        *linep++ = c; 
    } 
} 

/* 分配一个语法树节点，大小根据n指定，为n*4字节 */
int* tree(int n) 
{ 
    int *t; 

    t = treep; 
    treep += n; 
    if (treep>treeend) { 
        prs("Command line overflow\n"); /* 空间不足 */
        error++; 
        reset(); 
    } 
    return(t); 
} 

/*
 * 读取一个字符，并做一些预处理
 * 1.从预读缓存中读取
 * 2.从流中直接读取
 */
char getc() 
{ 
    char c; 

    if(peekc) { 
        c = peekc; 	/* 如果有预读缓存，读取缓存并返回 */
        peekc = 0; 	/* 清除预读缓存 */
        return(c); 
    } 
    if(argp > eargp) { 	/* Token列表溢出 */
        argp -= 10; 
        while((c=getc()) != '\n'); /* 取完换行符前所有字符 */
        argp += 10; 
        err("Too many args"); 
        gflg++; 
        return(c); 
    } 
    if(linep > elinep) { /* 命令参数缓存溢出 */
        linep -= 10; 
        while((c=getc()) != '\n'); /* 取完换行符之前所有字符 */
        linep += 10; 
        err("Too many characters"); 
        gflg++; 
        return(c); 
    } 
getd: 
    if(dolp) { 	/* 存在带'$'前缀的命令参数字符 */
        c = *dolp++; 
        if(c != '\0') 
            return(c); 
        dolp = 0; 	/* 带'$'前缀的参数扫描完毕，dolp清零 */
    } 
    c = readc(); /* 从流中读取一个字符 */	
    if(c == '\\') { 
        c = readc(); 	/* 遇到转义符，继续读取下一个字符 */
        if(c == '\n') 
            return(' '); 	/* 斜杠在行末，返回空格接续下一行 */
        return(c|QUOTE); 	/* 引用该字符 */
    } 
    if(c == '$') { 
        c = readc(); 	/* 遇到'$'，继续读取一个字符 */
        if(c>='0' && c<='9') { 	/* 位置变量 */
            if(c-'0' < dolc) 	/* 位置参数在合法范围内 */
                dolp = dolv[c-'0']; 	/* dolp指向替代的命令参数字符 */
            goto getd; 	/* 跳转getd处理 */
        } 
        if(c == '$') { 	/* 当前进程pid */
            dolp = pidp; 	/* dolp指向之前记录的pid字符串 */
            goto getd; 	/* 跳转getd处理 */
        } 
    } 
    return(c&0177); 	/* 普通字符直接返回，限定在ascii */
} 

/*
 * 从流中读取一个字符
 * arginp!=NULL（onelflg==0）：带"-c"选项，从后面附加的字符串中扫描
 * arginp==1（oneflg==0）：带"-c"选项，扫描终止，退出shell进程
 * onelflg==2(arginp==NULL): 带"-t"选项，从stdin中扫描
 * onelflg==1(arginp==NULL): 带"-t"选项，扫描终止，退出shell进程
 */
int readc() 
{ 
    char cc; 
    int c; 

    if (arginp) { /* "-c"选项 */
        if (arginp == (void *)1) 
            exit(-1); 	/* 没有新的命令参数，退出shell进程 */
        if ((c = *arginp++) == 0) { 
            arginp = (void *)1; 	/* 扫描终止 */
            c = '\n'; 	/* 追加换行符返回 */
        } 
        return(c); 
    } 
    if (onelflg==1) 
        exit(0); /* "-t"选项，扫描终止，退出shell进程 */
    if(read(0, &cc, 1) != 1) /* 从标准输入中读取一个字符 */
        exit(-1); 
    if (cc=='\n' && onelflg) 	/* onelflg == 2，"-t"选项 */
        onelflg--; 	/* 扫描一行完毕 */
    return(cc); 
} 

/* 
 * syntax 
 *        empty 
 *        syn1 
 * 
 * 从这里开始构建语法树，遍历之前扫描生成的token列表 
 * p1指向token列表头部，p2指向token列表末尾的下一个 
 * 自顶向下三级结构: syn1、syn2、syn3，函数调用关系如下
 * 1.左子树：左边列表递进一层
 * 2.右子树：右边列表回溯或者递归
 * 3.找不到符号：所有列表递进一层
 * 4.生成节点：直接返回
 */ 
int *syntax(char **p1, char **p2) 
{ 

    while(p1 != p2) { 
        if(any(**p1, ";&\n"))        /* 列表前缀为命令序列分隔字符，过滤掉该token，对syntax回溯中碰到*/ 
            p1++; else 
                return(syn1(p1, p2));        /* 所有列表递进一层至syn1 */ 
    } 
    return(0); 
} 

/* 
 * syn1 
 *        syn2 
 *        syn2 & syntax 
 *        syn2 ; syntax 
 * 
 * syn1生成命令序列节点，查找分隔符， 
 * 自顶向下生成左子树，回溯生成右子树。 
 * p1指向token列表头部，p2指向token列表末尾的下一个。 
 */ 
int *syn1(char **p1, char **p2) 
{ 
    char **p; 
    int *t, *t1; 
    int l; 

    l = 0; 
    for(p=p1; p!=p2; p++) 
        switch(**p) { 

            case '(': 
                l++; 
                continue;        /* 此处等同于break */ 

            case ')': 
                l--; 
                if(l < 0) 
                    error++; 
                continue; 

            case '&': 
            case ';': 
            case '\n': 
                if(l == 0) { /* 没有圆括弧嵌套 */
                    l = **p; 
                    t = tree(4); 
                    t[DTYP] = TLST;        /* 生成命令序列节点*/ 
                    t[DLEF] = (int)syn2(p1, p);        /* 左边列表递进一层生成左子树节点*/ 
                    t[DFLG] = 0;  /* 命令序列节点不设置任何标志位*/ 
                    if(l == '&') { 
                        t1 = (int *)t[DLEF]; 
                        t1[DFLG] |= FAND|FPRS|FINT;        /* 若有后台进程，其属性下推到左子树节点*/ 
                    } 
                    t[DRIT] = (int)syntax(p+1, p2);        /* 右边列表回溯到syntax生成右子树节点*/ 
                    return(t); 
                } 
        } 
    if(l == 0) 
        return(syn2(p1, p2));        /* 没有找到命令分隔符，所有列表递进一层至syn2 */ 
    error++;                /* 如果嵌套不归零发生错误*/ 
} 

/* 
 * syn2 
 *        syn3 
 *        syn3 | syn2 
 * 
 * syn2负责构建管道线节点，搜索管道线符号， 
 * 自顶向下生成左子树，递归生成右子树。 
 * p1指向token列表头部，p2指向token列表末尾的下一个。 
 */ 
int *syn2(char **p1, char **p2) 
{ 
    char **p; 
    int l, *t; 

    l = 0; 
    for(p=p1; p!=p2; p++) 
        switch(**p) { 

            case '(': 
                l++; 
                continue;        /* 此处等同于break */ 

            case ')': 
                l--; 
                continue; 

            case '|': 
            case '^': 
                if(l == 0) { /* 没有圆括弧嵌套 */
                    t = tree(4); 
                    t[DTYP] = TFIL;        /* 建立管道线节点*/ 
                    t[DLEF] = (int)syn3(p1, p);        /* 左边列表递进一层生成左子树节点*/ 
                    t[DRIT] = (int)syn2(p+1, p2);        /* 右边递归生成右子树节点*/ 
                    t[DFLG] = 0;        /* 标志位继承自上层节点*/ 
                    return(t); 
                } 
        } 
    return(syn3(p1, p2));        /* 没有找到管道线，所有列表递进一层，不需要判断嵌套是否归零*/ 
} 

/* 
 * syn3 
 *        ( syn1 ) [ < in  ] [ > out ] 
 *        word word* [ < in ] [ > out ] 
 * 
 * syn3负责构建复合命令和简单命令节点，前者以圆括号标记，括号内所有token列表回溯至syn1构建子命令节点； 
 * 简单命令是语法树叶子，其左右子树节点为I/O重定向文件，并存储命令参数字符串，生成节点后直接返回。 
 * p1指向token列表头部，p2指向token列表末尾的下一个。 
 */ 
int *syn3(char **p1, char **p2) 
{ 
    char **p; 
    char **lp, **rp; 
    int *t; 
    int n, l, i, o, c, flg; 

    flg = 0; 
    if(**p2 == ')') 
        flg |= FPAR;        /* 复合命令中最后一个子命令不需要fork子进程*/ 
    lp = 0;        /* 子命令开头*/ 
    rp = 0;        /* 子命令末尾*/ 
    i = 0;        /* 输入文件路径*/ 
    o = 0;        /* 输出文件路径*/ 
    n = 0;        /* 字符计数*/ 
    l = 0;        /* 嵌套层数*/ 
    for(p=p1; p!=p2; p++) 
        switch(c = **p) { 

            case '(': 
                if(l == 0) { 
                    if(lp != 0) 
                        error++; 
                    lp = p+1;        /* 当前嵌套最外层，lp指向子命令第一个字符*/ 
                } 
                l++; 
                continue;        /* 同break */ 

            case ')': 
                l--; 
                if(l == 0) 
                    rp = p;        /* 当前嵌套最外层，rp指向子命令后面的')' */ 
                continue; 

            case '>': 
                p++; 
                if(p!=p2 && **p=='>')        /* 追加模式FCAT标识*/ 
                    flg |= FCAT; else 
                        p--; 
                /* 注意这里没有break，继续往下走*/ 
            case '<': 
                if(l == 0) { 
                    p++; 
                    if(p == p2) { 
                        error++; 	/* 后面没有字符，发生错误 */
                        p--; 
                    } 
                    if(any(**p, "<>(")) 	/* 非法字符 */
                        error++; 
                    if(c == '<') { 
                        if(i != 0) 
                            error++; 
                        i = (int)*p;        /* 重定向到输入文件，只对简单命令有效*/ 
                        continue; 
                    } 
                    if(o != 0) 
                        error++; 
                    o = (int)*p;        /* 重定向到输出文件，只对简单命令有效*/ 
                } 
                continue; 

            default: 
                if(l == 0) 
                    p1[n++] = *p;        /* 命令参数前移清除以往记录，并用n计数*/ 
        } 
    if(lp != 0) { 
        if(n != 0) 
            error++;                /* 复合命令，n必须为0 */ 
        t = tree(5); 
        t[DTYP] = TPAR;        /* 分配复合命令类型节点*/ 
        t[DSPR] = (int)syn1(lp, rp);        /* 括号内所有token列表回溯至syn1构建子命令节点 */ 
        goto out; 
    } 
    if(n == 0) 
        error++;                /* 简单命令，n必须不为0 */ 
    p1[n++] = 0;                /* 字符串结束*/ 
    t = tree(n+5);        /* 分配简单命令节点，大小为5个字段（DSPR字段为空）加上命令字符数*/ 
    t[DTYP] = TCOM; 
    for(l=0; l<n; l++) 
        t[l+DCOM] = (int)p1[l]; 
out: 
    t[DFLG] = flg;        /* 固有属性或者继承自上层节点属性*/ 
    t[DLEF] = i;        /* 对于简单命令，左右子树是重定向的I/O文件*/ 
    t[DRIT] = o;        /* 对于复合命令，左右子树为空*/ 
    return(t); 
} 

/* 用函数指针f扫描所有字符 */
void scan(int *at, int (*f)()) 
{ 
    char *p, c; 
    int *t; 

    t = at+DCOM; 
    while(p = (char *)*t++) 
        while(c = *p) 
            *p++ = (*f)(c); 
} 

/* 查找通配符*/
int tglob(int c) 
{ 
    if(any(c, "[?*")) 
        gflg = 1;  
    return(c); 
} 

/* 解除引用标识*/ 
int trim(int c) 
{ 
    return(c&0177); 
} 

/*
 * 该函数从根节点开始深度优先遍历整个语法树，为每个命令创建进程并执行 
 * pf1和pf2是管道线I/O句柄，pf1表示命令输入端，pf2表示命令输出端 
 * pf1[0]: 管道线输入端句柄 
 * pf1[1]: 闲置 
 * pf2[0]: 闲置 
 * pf2[1]: 管道线输出端句柄 
 */ 
void execute(int *t, int *pf1, int *pf2) 
{ 
    int i, f, pv[2]; 
    int *t1; 
    char *cp1, *cp2; 
    extern int errno; 

    if(t != 0) 
        switch(t[DTYP]) { 

            case TCOM: 
                cp1 = (char *)t[DCOM]; 
                /* 以下为内置命令*/ 
                if(equal(cp1, "chdir")) { 
                    if(t[DCOM+1] != 0) { 
                        if(chdir(t[DCOM+1]) < 0) 
                            err("chdir: bad directory"); 
                    } else 
                        err("chdir: arg count"); 
                    return; 
                } 
                if(equal(cp1, "shift")) { 
                    if(dolc < 1) { 
                        prs("shift: no args\n"); 
                        return; 
                    } 
                    dolv[1] = dolv[0]; 
                    dolv++; 
                    dolc--; 
                    return; 
                } 
                if(equal(cp1, "login")) { 
                    if(promp != 0) { 
                        close(acctf); 
                        execv("/bin/login", t+DCOM); 
                    } 
                    prs("login: cannot execute\n"); 
                    return; 
                } 
                if(equal(cp1, "newgrp")) { 
                    if(promp != 0) { 
                        close(acctf); 
                        execv("/bin/newgrp", t+DCOM); 
                    } 
                    prs("newgrp: cannot execute\n"); 
                    return; 
                } 
                if(equal(cp1, "wait")) { 
                    pwait(-1, 0); 
                    return; 
                } 
                if(equal(cp1, ":")) 
                    return; 
                /* 注意，这里没有break，筛选掉内置命令后，外部命令会继续往下走*/ 
            case TPAR: 
                f = t[DFLG]; 
                i = 0; 
                if((f&FPAR) == 0)        /* 除了复合命令中最后一个子命令，其它一律fork子进程*/ 
                    i = fork(); 
                if(i == -1) { 
                    err("try again"); 
                    return; 
                } 
                if(i != 0) {        /* 父进程代码*/ 
                    if((f&FPIN) != 0) { 
                        close(pf1[0]);        /* fork之后，子进程获得父进程的管道线句柄拷贝，我们要在父进程中关闭它，以免资源泄漏*/ 
                        close(pf1[1]); /* 注意，这里只关闭pf1，因为pf1、pf2都是pv的镜像，因此关闭pf1就是关闭pf2 */ 
                    } 
                    if((f&FPRS) != 0) {/* 打印子进程pid */ 
                        prn(i); 
                        prs("\n"); 
                    } 
                    if((f&FAND) != 0)        /* 后台异步进程无需等待*/ 
                        return; 
                    if((f&FPOU) == 0)        /* 节点为管道线末端，等待子进程终止*/ 
                        pwait(i, t); 
                    return; 
                } 
                /* 以下为子进程代码，或者FPAR属性节点命令进程*/ 
                if(t[DLEF] != 0) { 
                    close(0); 
                    i = open(t[DLEF], 0);        /* 重定向stdin到左子树节点*/ 
                    if(i < 0) { 
                        prs((char *)t[DLEF]); 
                        err(": cannot open"); 
                        exit(-1); 
                    } 
                } 
                if(t[DRIT] != 0) { 
                    if((f&FCAT) != 0) { 
                        i = open(t[DRIT], 1); 
                        if(i >= 0) { 
                            seek(i, 0, 2);                /* 追加模式*/ 
                            goto f1; 
                        } 
                    } 
                    i = creat(t[DRIT], 0666);        /* 创建新的文件句柄*/ 
                    if(i < 0) { 
                        prs((char *)t[DRIT]); 
                        err(": cannot create"); 
                        exit(-1); 
                    } 
f1: 
                    close(1); 
                    dup(i);                /* 重定向stdout到右子树节点*/ 
                    close(i); 
                } 
                if((f&FPIN) != 0) { 
                    close(0); 
                    dup(pf1[0]);        /* 重定向stdin到管道线输入端*/ 
                    close(pf1[0]); 
                    close(pf1[1]);        /* 子进程关闭管道线句柄*/ 
                } 
                if((f&FPOU) != 0) { 
                    close(1); 
                    dup(pf2[1]);        /* 重定向stdout  到管道线输出*/ 
                    close(pf2[0]); 
                    close(pf2[1]);        /* 子进程关闭管道线句柄*/  
                } 
                if((f&FINT)!=0 && t[DLEF]==0 && (f&FPIN)==0) { 
                    close(0);        /* 若忽略中断，且不存在管道线输入，则关闭stdin并重定向到位桶*/ 
                    open("/dev/null", 0);        /* 以免来自其它进程stdin干扰，但stdout是保留的*/ 
                } 
                if((f&FINT) == 0 && setintr) { 
                    signal(INTR, 0);        /* 无FINT标识，则恢复中断信号响应（默认是忽略的） */ 
                    signal(QUIT, 0); 
                } 
                if(t[DTYP] == TPAR) {        /* 如果是复合类型，则执行子命令，同时下推FINT标识*/ 
                    if(t1 = (int *)t[DSPR]) 
                        t1[DFLG] |= f&FINT; 
                    execute(t1, NULL, NULL); 
                    exit(0); 
                } 
                close(acctf); 
                gflg = 0; 
                scan(t, &tglob);        /* 扫描通配符*/ 
                if(gflg) { 
                    t[DSPR] = (int)"/etc/glob"; 
                    execv(t[DSPR], t+DSPR);        /* 包含通配符的命令在/etc/glob中执行*/ 
                    prs("glob: cannot execute\n"); 
                    exit(-1); 
                } 
                scan(t, &trim);        /* 解除引用标识*/ 
                *linep = 0; 
                texec((char *)t[DCOM], t);        /* 最先执行当前目录下的命令*/ 
                cp1 = linep; 
                cp2 = "/usr/bin/"; 
                while(*cp1 = *cp2++) 	/* strcpy，cp1停在'\0'位置 */
                    cp1++; 
                cp2 = (char *)t[DCOM]; 
                while(*cp1++ = *cp2++); 	/* strcpy，'\0'结束 */
                texec(linep+4, t);        /* 接着执行/bin目录下的命令*/ 
                texec(linep, t);        /* 最后执行/usr/bin目录下的命令*/ 
                prs((char *)t[DCOM]); 	/* 若进程异常退出，表示找不到命令，退出shell */
                err(": not found"); 
                exit(-1); 

            case TFIL: 
                f = t[DFLG]; 
                pipe(pv);        /* 创建管道线，pv[0]为输入句柄，pv[1]为输出句柄*/ 
                t1 = (int *)t[DLEF]; 
                t1[DFLG] |= FPOU | (f&(FPIN|FINT|FPRS)); 
                execute(t1, pf1, pv);        /* 对于左子树节点，pv[1]作为管道线输出句柄传入，pv[0]闲置*/ 
                t1 = (int *)t[DRIT]; 
                t1[DFLG] |= FPIN | (f&(FPOU|FINT|FAND|FPRS));        /* 只有管道线末端命令才能继承FAND属性，与FPOU排斥*/ 
                execute(t1, pv, pf2);        /* 对于右子树节点，pv[0]作为管道线输入句柄传入，pv[1]闲置*/ 
                return; 

            case TLST: 
                f = t[DFLG]&FINT;                /* 对于命令序列节点，下推FINT属性到左右子树*/ 
                if(t1 = (int *)t[DLEF]) 
                    t1[DFLG] |= f; 
                execute(t1, NULL, NULL); 		/* 先左后右依次执行 */
                if(t1 = (int *)t[DRIT]) 
                    t1[DFLG] |= f; 
                execute(t1, NULL, NULL); 
                return; 

        } 
} 

/* 
 * 执行命令 
 * f为可执行文件路径
 * at为语法树节点
 */
void texec(char *f, int *at) 
{ 
    extern int errno; 
    int *t; 

    t = at; 
    execv(f, t+DCOM);        /* 重新加载并执行命令，不需要设置环境*/ 
    if (errno==ENOEXEC) { 
        if (*linep) 
            t[DCOM] = (int)linep; 
        t[DSPR] = (int)"/bin/sh"; 
        execv(t[DSPR], t+DSPR);        /* 找不到命令可执行文件*/ 
        prs("No shell!\n"); 
        exit(-1); 
    } 
    if (errno==ENOMEM) { 
        prs((char *)t[DCOM]); 
        err(": too large");        /* 内存不足*/ 
        exit(-1); 
    } 
} 

/* 打印错误，非交互模式则清空stdin缓存并退出shell进程*/ 
void err(char *s) 
{ 
    prs(s); 
    prs("\n"); 
    if(promp == 0) { 
        seek(0, 0, 2); 
        exit(-1); 
    } 
} 

/* 打印字符串*/ 
void prs(char *as) 
{ 
    char *s; 

    s = as; 
    while(*s) 
        put(*s++); 
} 

/* 输出一个字符*/ 
void put(int c) 
{ 

    write(2, &c, 1); 
} 

/* itoa */ 
void prn(int n) 
{ 
    int a; 

    if(a=n/10) 
        prn(a); 
    put(n%10+'0'); 
} 

/* 字符串中是否包含某个字符*/ 
int any(int c, char *as) 
{ 
    char *s; 

    s = as; 
    while(*s) 
        if(*s++ == c) 
            return(1); 
    return(0); 
} 

/* 字符串比较*/ 
int equal(char *as1, char *as2) 
{ 
    char *s1, *s2; 

    s1 = as1; 
    s2 = as2; 
    while(*s1++ == *s2) 
        if(*s2++ == '\0') 
            return(1); 
    return(0); 
} 

/*
 * 父进程等待子进程终止
 * i为子进程pid，-1表示等待所有子进程
 * t为语法树节点
 */
void pwait(int i, int *t) 
{ 
    int p, e; 
    int s; 

    if(i != 0) 
        for(;;) { 
            times(&timeb); 
            time(timeb.proct); 
            p = wait(&s); 
            if(p == -1) 	/* 等待失败 */
                break; 
            e = s&0177; 	/* 保留状态环境 */
            if(mesg[e] != 0) { 	/* 子进程异常终止 */
                if(p != i) { 
                    prn(p); 
                    prs(": "); 
                } 
                prs(mesg[e]); 	/* 打印对应的消息 */
                if(s&0200) 	/* 内核奔溃 */
                    prs(" -- Core dumped"); 
            } 
            if(e != 0) 
                err(""); 	/* 等待正常终止 */
            if(i == p) { 
                acct(t); 
                break; 
            } else 
                acct(0); 
        } 
} 

void acct(int *t) 
{ 
    if(t == 0) 
        enacct("**gok"); else 
            if(*t == TPAR) 
                enacct("()"); else 
                    enacct((char *)t[DCOM]); 
} 

void enacct(char *as) 
{ 
    struct stime timbuf; 
    struct { 
        char cname[14]; 
        char shf; 
        char uid; 
        int datet[2]; 
        int realt[2]; 
        int bcput[2]; 
        int bsyst[2]; 
    } tbuf; 
    int i; 
    char *np, *s; 

    s = as; 
    times(&timbuf); 
    time(timbuf.proct); 
    lsub(tbuf.realt, timbuf.proct, timeb.proct); 
    lsub(tbuf.bcput, timbuf.cputim, timeb.cputim); 
    lsub(tbuf.bsyst, timbuf.systim, timeb.systim); 
    do { 
        np = s; 
        while (*s != '\0' && *s != '/') 
            s++; 
    } while (*s++ != '\0'); 
    for (i=0; i<14; i++) { 
        tbuf.cname[i] = *np; 
        if (*np) 
            np++; 
    } 
    tbuf.datet[0] = timbuf.proct[0]; 
    tbuf.datet[1] = timbuf.proct[1]; 
    tbuf.uid = uid; 
    tbuf.shf = 0; 
    if (promp==0) 
        tbuf.shf = 1; 
    seek(acctf, 0, 2); 
    write(acctf, &tbuf, sizeof(tbuf)); 
}
