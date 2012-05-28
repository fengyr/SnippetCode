/*
 *本函数读出站点名，站点ID，站点IP地址，站点IP对应端口
 *返回-1表示读取错误，返回0表示读取了空数据，返回1表示读取成功，
 */
# include "readdeploy.h"

int get_deploy(char *ip, char *id){
     FILE *fp;
     char line[256],add[256];
     
     sprintf(add,"%s %s%s",ip,AUTHSH,id);
     sprintf(line,"/root/.auth.sh %s",add);
     fp = popen(line,"r");
     pclose(fp);
     return 1;
}

int deploy_head(HEADINF **inf)
{
     int     i, j;
     int     flag;
     FILE    *stream;
     char    name[30] = {0}, add[20] = {0}, line[256], ebuf[256];
     int     nbytes;
     int     id, port;
     int     regular, lines;
     regex_t reg;
     char    line_reg[256] = "\\[[^:]\\{1,\\}:[0-9]\\{1,3\\}:22\\(4\\|5\\).[0-9]\\{1,3\\}.[0-9]\\{1,3\\}.[0-9]\\{1,3\\}:[0-6][0-9]\\{4\\}\\]";
     regmatch_t pm[10];
     const size_t nmatch = 10;
     HEADINF *next;
     
     *inf = NULL;
     if((next = malloc(sizeof(HEADINF)))== NULL){
          perror("malloc() error");
          return -1;
     }
     memset(next,0, sizeof(next));
     flag = FLAG_HEAD;
     
     regular = regcomp(&reg, line_reg, 0);
     if(regular != 0){
          regerror(regular, &reg, ebuf, sizeof(ebuf));
          fprintf(stderr, "%s pattern '%s' \n",ebuf,line_reg);
          return -1;
     }
     if((stream = fopen(FILENAME, "r")) == NULL){
          perror(FILENAME);
          return -1;
     }
     lines = 0;
     while(fgets(line, 256, stream)){
          lines++;
          nbytes = strlen(line);
          if( nbytes == 256 && line[255] != '\n'){
               printf("line too long,\"%s\" file error!\n",FILENAME);
               fclose(stream);
               return -1;
          }
          if( line[nbytes-1] != '\n' ){
               printf("read error!\n");
               fclose(stream);
               return -1;
          }
          else {
               line[nbytes-1] = 0;
               nbytes--;
          }
          if( memchr(line, '#', nbytes) ){
               continue;
          }
          if( !memchr(line, '[', nbytes) || !memchr(line,']',nbytes)){
               continue;
          }
          for(i = 0; i < nbytes; i++){
               if( line[i] == ' ' ){
                    continue;
               }
               regular = regexec(&reg, &line[i], nmatch, pm, 0);
               if( regular == REG_NOMATCH ){
                    if( flag == FLAG_HEAD ){
                         flag = FLAG_ERROR;
                         break;
                    }
               }
               else if( regular != 0 ){
                    regerror(regular, &reg, ebuf, sizeof(ebuf));
                    fprintf(stderr, "%s pattern '%s' \n",ebuf,line_reg);
                    fclose(stream);
                    return -1;
               }
               else {
                    if( flag == FLAG_OVER ){
                         if((next->next = malloc(sizeof(HEADINF))) == NULL){
                              perror("malloc() error");
                              fclose(stream);
                              return -1;
                         }
                         memset(next->next, 0, sizeof(HEADINF));
                         next = next->next;
                    }
                    for(j = 0, i = i+1; line[i] != ':'; j++, i++){
                         name[j] = line[i];
                    }
                    name[j] = 0;
                    if((next->stationname = malloc(strlen(name)+1)) == NULL){
                         perror("malloc() error");
                         fclose(stream);
                         return -1;
                    }
                    memcpy(next->stationname, name, strlen(name)+1);
                    next->id = atoi(&line[++i]);
                    i += 4;
                    for(j = 0; line[i] != ':'; j++,i++){
                         add[j] = line[i];
                    }
                    add[j] = 0;
                    if((next->addr = malloc(strlen(add)+1)) == NULL){
                         perror("malloc() error");
                         fclose(stream);
                         return -1;
                    }
                    memcpy(next->addr, add, strlen(add)+1);
                    next->port = atoi(&line[++i]);
                    next->next = NULL;
                    if(flag == FLAG_HEAD){
                         *inf = next;
                         flag = FLAG_OVER;
                    }
               }
               break;
          }
          if(flag == FLAG_ERROR){
               printf("Information of the line %05d have error!\n",lines);
               flag = FLAG_HEAD;
          }
          memset(line, 0, sizeof(line));
     }
     regfree(&reg);
     if(*inf == NULL){
          fclose(stream);
          return 0;
     }
     fclose(stream);
     return 1;
}
/*
 *释放HEADINF结构的空间
 */
int free_head(HEADINF **inf)
{
     HEADINF *next;
     while((*inf)->next){
          next = (*inf)->next;
          free((*inf)->stationname);
          (*inf)->stationname = NULL;
          free((*inf)->addr);
          (*inf)->addr = NULL;
          (*inf)->next = NULL;
          free(*inf);
          *inf = next;
     }
     *inf = NULL;
     return 1;
}

/*
 *本函数读取1、球机ID，
 *         2、球机位置名，
 *         3、球机组播地址，
 *         4、视频端口，
 *         5、控制端口，
 *         6、dmduID，
 *         7、报警ID，
 *         8、云台地址，
 *         9、控制卡类型名称
 *
 *返回-1表示读取错误，返回0表示读取了空数据，返回1表示读取成功.
 */
int deploy_all(ALLINF **inf)
{
     int     i, j;
     int     flag;
     FILE    *stream;
     char    name[ALLMAX][30] = {0}, videoadd[ALLMAX][20] = {0};
     char    line[256], ebuf[256], type[ALLMAX][10]={0};
     int     nbytes, lines = 0, qt = 0;
     int     id[ALLMAX], videoport[ALLMAX], controlport[ALLMAX], dmduid[ALLMAX];
     PTC     protocol[ALLMAX];
     int     alarmid[ALLMAX], panaddr[ALLMAX];
     int     regular;
     regex_t reg;
     char    line_reg[512] = "[0-9]\\{5\\}:[^:]\\{1,\\}:22\\(4\\|5\\).[0-9]\\{1,3\\}.[0-9]\\{1,3\\}.[0-9]\\{1,3\\}:[0-9]\\{5\\}:[0-9]\\{5\\}:[0-9]\\{1,\\}:[0-9\\-]\\{1\\}[0-9]\\{0,\\}:[0-9\\-]\\{1\\}[0-9]\\{0,\\}:[^:]\\{1,\\}:dm642_\\(cif\\|d1\\)";
     regmatch_t pm[10];
     const size_t nmatch = 10;
     
     flag = FLAG_HEAD;
     
     regular = regcomp(&reg, line_reg, 0);
     if(regular != 0){
          regerror(regular, &reg, ebuf, sizeof(ebuf));
          fprintf(stderr, "%s pattern '%s' \n",ebuf,line_reg);
          return -1;
     }
     if((stream = fopen(FILENAME, "r")) == NULL){
          perror(FILENAME);
          return -1;
     }
     while(fgets(line, 256, stream)){
          lines++;
          nbytes = strlen(line);
          if( nbytes == 256 && line[255] != '\n'){
               printf("line too long,\"%s\" file error!\n",FILENAME);
               fclose(stream);
               return -1;
          }
          if( line[nbytes-1] != '\n' ){
               printf("read error!\n");
               fclose(stream);
               return -1;
          }
          else {
               line[nbytes-1] = 0;
               nbytes--;
          }
          if( memchr(line, '#', nbytes)
              || memchr(line, '[', nbytes)
              || memchr(line, ']', nbytes)){
               continue;
          }
          for(i = 0; i < nbytes; i++){
               if( line[i] == ' ' ){
                    continue;
               }
               if( !memcmp(line, "DIR", 3)
                   || !memcmp(line, "TIME", 4)
                   || !memcmp(line, "RATE", 4)
                   || !memcmp(line, "<", 1)){
                    break;
               }
               regular = regexec(&reg, &line[i], nmatch, pm, 0);
               if( regular == REG_NOMATCH ){
                    if( flag == FLAG_HEAD ){
                         flag = FLAG_ERROR;
                         break;
                    }
               }
               else if( regular != 0 ){
                    regerror(regular, &reg, ebuf, sizeof(ebuf));
                    fprintf(stderr, "%s pattern '%s' \n",ebuf,line_reg);
                    fclose(stream);
                    return -1;
               }
               else {
                    id[qt] = atoi(line);
                    for(j = 0, i = i+6; line[i] != ':'; j++, i++){
                         name[qt][j] = line[i];
                    }
                    for(j = 0, i = i+1; line[i] != ':'; j++, i++){
                         videoadd[qt][j] = line[i];
                    }
                    videoport[qt] = atoi(&line[++i]);
                    i += 6;
                    controlport[qt] = atoi(&line[i]);
                    i += 6;
                    dmduid[qt] = atoi(&line[i]);
                    for(; line[i] != ':'; i++);
                    i++;
                    alarmid[qt] = atoi(&line[i]);
                    for(; line[i] != ':'; i++);
                    i++;
                    panaddr[qt] = atoi(&line[i]);
                    for(; line[i] != ':'; i++);
                    i++;
                    if(memcmp(&line[i], "PEL-P", 5) == 0){
                         protocol[qt] = Pel_p;
                    }
                    else if(memcmp(&line[i], "KTD", 3) == 0){
                         protocol[qt] = Ktd;
                    }
                    else if(memcmp(&line[i], "DMD", 3) == 0){
                         protocol[qt] = Dmd;
                    }
                    else{
                         protocol[qt] = Na;
                    }
                    for(; line[i] != ':'; i++);
                    i++;
                    for(j = 0; line[i] != 0; j++, i++){
                         type[qt][j] = line[i];
                    }
                    qt++;
                    if(qt == ALLMAX-1){
                         printf("Informating number out-of range!\n");
                         flag = FLAG_OVER;
                         break;
                    }
               }
               break;
          }
          if(flag == FLAG_OVER){
               break;
          }
          if(flag == FLAG_ERROR){
               flag = FLAG_HEAD;
               printf("Information of the line %05d have error!\n",lines);
          }
          memset(line, 0, sizeof(line));
     }
     regfree(&reg);
     if(qt == 0){
          fclose(stream);
          return 0;
     }
     for(i = 0; i < qt; i++){
          for(j = 0; j < qt; j++){
               if(j != i){
                    if(id[i] == id[j]
                       || memcmp(videoadd[i], videoadd[j],
                                 (strlen(videoadd[i]) > strlen(videoadd[j])
                                  ? strlen(videoadd[i]) : strlen(videoadd[j]))) == 0
                       || dmduid[i] == dmduid[j]){
                         printf("%d,%d Information repeat!\n", id[i],id[j]);
                         fclose(stream);
                         return -1;
                    }
                    if(alarmid[i] != -1 && alarmid[j] != -1){
                         if(alarmid[i] == alarmid[j]){
                              printf("%d,%d Information repeat!\n",id[i],id[j]);
                              fclose(stream);
                              return -1;
                         }
                    }
               }
          }
     }
     
     if((*inf = malloc(sizeof(ALLINF)))== NULL){
          perror("malloc() error");
          fclose(stream);
          return -1;
     }
     memset(*inf, 0, sizeof(*inf));
     (*inf)->quantity = qt;
     if(((*inf)->id = malloc(qt*sizeof(int))) == NULL){
          perror("malloc() error");
          fclose(stream);
          return -1;
     }
     if(((*inf)->name = malloc(qt*sizeof(char *))) == NULL){
          perror("malloc() error");
          fclose(stream);
          return -1;
     }
     if(((*inf)->videoaddr = malloc(qt*sizeof(char *))) == NULL){
          perror("malloc() error");
          fclose(stream);
          return -1;
     }
     if(((*inf)->videoport = malloc(qt*sizeof(int))) == NULL){
          perror("malloc() error");
          fclose(stream);
          return -1;
     }
     if(((*inf)->controlport = malloc(qt*sizeof(int))) == NULL){
          perror("malloc() error");
          fclose(stream);
          return -1;
     }
     if(((*inf)->dmduid = malloc(qt*sizeof(int))) == NULL){
          perror("malloc() error");
          fclose(stream);
          return -1;
     }
     if(((*inf)->alarmid = malloc(qt*sizeof(int))) == NULL){
          perror("malloc() error");
          fclose(stream);
          return -1;
     }
     if(((*inf)->panaddr = malloc(qt*sizeof(int))) == NULL){
          perror("malloc() error");
          fclose(stream);
          return -1;
     }
     if(((*inf)->protocol = malloc(qt*sizeof(PTC))) == NULL){
          perror("malloc() error");
          fclose(stream);
          return -1;
     }
     if(((*inf)->type = malloc(qt*sizeof(char *))) == NULL){
          perror("malloc() error");
          fclose(stream);
          return -1;
     }
     for(i = 0; i < qt; i++){
          (*inf)->id[i] = id[i];
          if(((*inf)->name[i] = malloc(strlen(name[i])+1)) == NULL){
               perror("malloc() error");
               fclose(stream);
               return -1;
          }
          memcpy((*inf)->name[i], name[i], strlen(name[i])+1);
          if(((*inf)->videoaddr[i] = malloc(strlen(videoadd[i])+1)) == NULL){
               perror("malloc() error");
               fclose(stream);
               return -1;
          }
          memcpy((*inf)->videoaddr[i], videoadd[i], strlen(videoadd[i])+1);
          (*inf)->videoport[i] = videoport[i];
          (*inf)->controlport[i] = controlport[i];
          (*inf)->dmduid[i] = dmduid[i];
          (*inf)->alarmid[i] = alarmid[i];
          (*inf)->panaddr[i] = panaddr[i];
          (*inf)->protocol[i] = protocol[i];
          if(((*inf)->type[i] = malloc(strlen(type[i])+1)) == NULL){
               perror("malloc() error");
               fclose(stream);
               return -1;
          }
          memcpy((*inf)->type[i], type[i], strlen(type[i])+1);
     }
     fclose(stream);
     return 1;
}

/*
 *释放ALLINF结构的空间
 */
int free_all(ALLINF **inf)
{
     int i;
     free((*inf)->id);
     (*inf)->id = NULL;
     free((*inf)->videoport);
     (*inf)->videoport = NULL;
     free((*inf)->controlport);
     (*inf)->controlport = NULL;
     free((*inf)->dmduid);
     (*inf)->dmduid = NULL;
     free((*inf)->alarmid);
     (*inf)->alarmid = NULL;
     free((*inf)->panaddr);
     (*inf)->panaddr = NULL;
     
     for(i = 0; i < (*inf)->quantity; i++){
          free((*inf)->name[i]);
          (*inf)->name[i] = NULL;
          free((*inf)->videoaddr[i]);
          (*inf)->videoaddr[i] = NULL;
          free((*inf)->type[i]);
          (*inf)->type[i] = NULL;
     }
     free((*inf)->name);
     (*inf)->name = NULL;
     free((*inf)->videoaddr);
     (*inf)->videoaddr = NULL;
     free((*inf)->type);
     (*inf)->type = NULL;
     free(*inf);
     *inf = NULL;
     return 1;
}

