#include "a.h"

static char *olds;

void show2(int i, int j, int p)
{
	show2(i, j, p);
}

void show1(int i, int j, int p)
{
	show2(i, j, p);
}

void *print_time(void *arg)
{
	int i;
	i = *((int *)arg);
	char buf[32];
	struct tm temp;
	time_t t;
	int num[24] =
	    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
	  19, 20, 21, 22, 23 };

	while (1) {
		memset(buf, 0, sizeof(buf));
		t = time(NULL);
		/*localtime_h(t, &temp); */
		localtime_h(t, &temp);

		sprintf(buf, "%d  %02d-%02d-%02d %02d:%02d ---> %02d %02d\n", i,
			temp.tm_year + 1900, temp.tm_mon + 1, temp.tm_mday,
			temp.tm_hour, temp.tm_min, temp.tm_hour, t);
		printf(buf);
		usleep(5000);
	}
}

void localtimes(time_t time, long timezone, struct tm *tm_time)
{
	const char Days[12] =
	    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	unsigned n32_Pass4year;
	int n32_hpery;

	//计算时差
	time = time - timezone;

	if (time < 0) {
		time = 0;
	}
	//取秒时间
	tm_time->tm_sec = (int)(time % 60);
	time /= 60;
	//取分钟时间
	tm_time->tm_min = (int)(time % 60);
	time /= 60;
	//取过去多少个四年，每四年有 1461*24 小时
	n32_Pass4year = ((unsigned int)time / (1461L * 24L));
	//计算年份
	tm_time->tm_year = (n32_Pass4year << 2);
	//四年中剩下的小时数
	time %= 1461L * 24L;
	//校正闰年影响的年份，计算一年中剩下的小时数
	for (;;) {
		//一年的小时数
		n32_hpery = 365 * 24;
		//判断闰年
		if ((tm_time->tm_year & 3) == 0) {
			//是闰年，一年则多24小时，即一天
			n32_hpery = 24;
		}
		if (time < n32_hpery) {
			break;
		}
		time -= n32_hpery;
	}
	//小时数
	tm_time->tm_hour = (int)(time % 24);
	//一年中剩下的天数
	time /= 24;
	//假定为闰年
	//校正润年的误差，计算月份，日期
	if ((tm_time->tm_year & 3) == 0) {
		if (time > 60) {
			time--;
		} else {
			if (time == 60) {
				tm_time->tm_mon = 1;
				tm_time->tm_mday = 29;
				return;
			}
		}
	}
	//计算月日
	for (tm_time->tm_mon = 0; Days[tm_time->tm_mon] < time; tm_time->tm_mon) {
		time -= Days[tm_time->tm_mon];
	}
	tm_time->tm_mday = (int)(time);

	return;
}

void localtime_h(time_t time, struct tm *ret_time)
{
	static const char month_days[12] =
	    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	static const int leap_year[4] = { 0, 0, 1, 0 };

	unsigned int leave_for_fouryear = 0;
	unsigned short four_year_count = 0;
	unsigned int temp_value = 0;

	ret_time->tm_sec = time % 60;
	temp_value = time / 60;	// 分钟
	ret_time->tm_min = temp_value % 60;
	temp_value /= 60;	// 小时

	temp_value += 8;	// 加上时区

	ret_time->tm_hour = temp_value % 24;
	temp_value /= 24;	// 天

	ret_time->tm_wday = (temp_value + 4) % 7;	// 1970-1-1是4

	four_year_count = temp_value / (365 * 4 + 1);
	leave_for_fouryear = temp_value % (365 * 4 + 1);
	int leave_for_year_days = leave_for_fouryear;

	int day_count = 0;
	int i = 0;

	for (i = 0; i < 4; i++) {
		day_count = leap_year[i] ? 366 : 365;

		if (leave_for_year_days <= day_count) {
			break;
		} else {
			leave_for_year_days -= day_count;
		}
	}

	ret_time->tm_year = four_year_count * 4 + i + 70;
	ret_time->tm_yday = leave_for_year_days;	// 这里不是天数，而是标记，从0开始

	int leave_for_month_days = leave_for_year_days;

	int j = 0;
	for (j = 0; j < 12; j++) {
		if (leap_year[i] && j == 1) {
			if (leave_for_month_days <= 29) {
				break;
			} else if (leave_for_month_days == 29) {
				i++;
				leave_for_month_days = 0;
				break;
			} else {
				leave_for_month_days -= 29;
			}

			continue;
		}

		if (leave_for_month_days < month_days[j]) {
			break;
		} else if (leave_for_month_days == month_days[j]) {
			i++;
			leave_for_month_days = 0;
			break;
		} else {
			leave_for_month_days -= month_days[j];
		}
	}

	ret_time->tm_mday = leave_for_month_days + 1;
	ret_time->tm_mon = j;
}

int function1(char *s1, char *accept)
{
	int count = 0;
	char *p1;
	char *a;

	for (p1 = (char *)s1; *p1 != '\0'; p1++) {
		a = (char *)accept;
		if (*p1 == *a) {
			// 返回多少个字符相等
			count += p1 - s1;
			break;
		}
	}
	return count;
}

int function2(char *s1, char *accept)
{
	int count = 0;
	char *p1 = (char *)s1;
	char *a;

	while (*p1) {
		a = (char *)accept;

		if (*p1 == *a) {
			if (*a != '\0') {
				// 如果没有遇到'\0',往下扫描
				a++;
			}

			if (*(p1 + 1) == *a) {
				// 如果遇到两个字符相等，退出并返回跳过的字符
				p1++;
				count += p1 - s1;
				break;
			}
		}
		p1++;
	}
	return count;
}

char *function3(char *s, char *delm)
{
	char *s1 = (char *)s;
	char *token;
	if (s1 == NULL) {
		s1 = olds;
	}

	s1 += function1(s1, delm);
	s1 += function2(s1, delm);
	token = s1 + 1;

	if (s1 != NULL) {
		olds = token;
	}

	*s1 = '\0';
	return token;
}

static int *ptr = NULL;
static int count = 1;
typedef void (*CALLBACK) (char *str);

void get_point(CALLBACK);

void get_point(CALLBACK cb)
{
	ptr = realloc(ptr, sizeof(int) * count);

	ptr[count - 1] = count;
	count++;

	cb("hello");
}

void callback(char *str)
{
	PDEBUG("callback %s\n", str);
}

int main(int argc, char **argv)
{
	/*     int i = 5;
	 *     int j = 0;
	 *     int x;
	 *     int *p, *q;
	 * 
	 *     printf("%d-%s\n", argc, argv[1]);
	 *     for (x=0; x<10; x++) {       
	 *         p = malloc(sizeof(int));
	 *         *p = x;
	 *         free(p);
	 *         show1(i, j, *p);
	 *         i++;
	 *         j++;
	 * 
	 *         q = p;
	 *         free(p);
	 *         show1(i, j, *q);
	 *     }
	 * 
	 *     return 0;
	 * 
	 * 
	 */
	/*      char str[] = {0, 0, 0, 1, 'a', 'b', 'c', 'd', 'e', 0, 0, 0, 1, 'f', 'g'};
	 *      char *dil = "0001";
	 *      char *pos = NULL;
	 *      char *temp;
	 *  
	 *      temp = str;
	 *      printf("str: %s\n", temp);
	 *      pos = strsep(&temp, dil);
	 *      if ( pos != NULL ) {
	 *          printf("%s\n", pos);
	 *      }
	 *      pos = strsep(&temp, dil);
	 *      if ( pos != NULL ) {
	 *          printf("%s\n", pos);
	 *      }
	 */

	/*     struct timeval tv;
	 *     struct timezone tz;
	 *     tv.tv_sec = 1000000;
	 *     tv.tv_usec = 1000;
	 *     tz.tz_minuteswest = 600;
	 *     tz.tz_dsttime = DST_NONE;
	 *     settimeofday(&tv, NULL);
	 */

	/*     struct tm *tim;
	 *     struct timeval tv;
	 *     gettimeofday(&tv, NULL);
	 *     tim = localtime(&tv.tv_sec);
	 *     printf("%04d-%02d-%02d %02d:%02d:%02d.%02d\n", tim->tm_year+1900, tim->tm_mon+1, tim->tm_mday, tim->tm_hour, tim->tm_min, tim->tm_sec, tv.tv_usec);
	 *     
	 */

	/*     int i = 0;
	 *     pthread_t thread_time[64];
	 *     for (i=0; i<64; i++) {
	 *         pthread_create(&thread_time[i], 0, print_time, (void*)&i);
	 *         sleep(1);
	 *     }
	 * 
	 *     while (1) {
	 *         sleep(10);
	 *     }
	 * 
	 */

	/*     unsigned int i = 0x00;
	 *     unsigned int j = 0x00;
	 *     unsigned int k = 0x00;
	 *     unsigned int l = 0x01;
	 *     
	 *     printf("%d\n", (i<<24 + j<<16 + k<<8 + l));
	 */

	/*     char *a[2][3] = {"111", "122", "133", "211", "222", "233"};
	 *     char *b[] = {"111", "122", "133", "211", "222", "233"};
	 * 
	 *     printf("%s\n", (*(a+1))[1]);
	 *     printf("%s\n", *(b+2)+1);
	 */

/*     char s[]="-abc-=def=g";
 *     char * str1 = function3(s,"-");
 *     printf("str1 = %s\n",str1);     // abc-=def=g
 * 
 *     str1 = function3(NULL,"-=");
 *     printf("str1 = %s\n",str1);     // def=g
 * 
 *     str1 = function3(NULL,"=");
 *     printf("str1 = %s\n",str1);     // g
 */

	int i, j;
	for (i = 0; i < 10; i++) {
		get_point(callback);
	}
	for (j = 0; j < count; j++) {
		printf("%d\n", ptr[j]);
	}

	return 0;
}
