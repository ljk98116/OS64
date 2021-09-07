#include <tstdio.h>

//flags state
#define ZEROPAD	1		/* pad with zero */
#define SIGN	  2   /* unsigned/signed long */
#define PLUS    4	  /* show plus */
#define SPACE	  8   /* space if plus */
#define LEFT	  16  /* left justified */
#define SPECIAL	32  /* 0x */
#define SMALL	  64  /* use 'abcdef' instead of 'ABCDEF' */

#define do_div(n,base) ({ \
int __res; \
__asm__("divl %4":"=a" (n),"=d" (__res):"0" (n),"1" (0),"r" (base)); \
__res; })

static char * number(char * str, int num, int base, int size, int precision,int type){
	char c,sign,tmp[36];
	const char *digits="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i;

	if(type&SMALL) digits="0123456789abcdefghijklmnopqrstuvwxyz";
	if(type&LEFT) type &= ~ZEROPAD;
	if(base<2 || base>36)
		return 0;
	c = (type & ZEROPAD) ? '0' : ' ' ;
	if(type&SIGN && num<0) {
		sign='-';
		num = -num;
	}
	else
		sign=(type&PLUS) ? '+' : ((type&SPACE) ? ' ' : 0);
	if(sign) size--;
	if(type&SPECIAL){
		if (base==16) size -= 2;
		else if (base==8) size--;
	}
	i=0;
	if(num==0)
		tmp[i++]='0';
	else while (num!=0)
		tmp[i++]=digits[do_div(num,base)];
	if(i>precision) precision=i;
	size -= precision;
	if(!(type&(ZEROPAD+LEFT)))
		while(size-->0)
			*str++ = ' ';
	if(sign)
		*str++ = sign;
	if(type&SPECIAL){
		if(base==8)
			*str++ = '0';
		else if(base==16) {
			*str++ = '0';
			*str++ = digits[33];
		}
	}
	if(!(type&LEFT))
		while(size-->0)
			*str++ = c;
	while(i<precision--)
		*str++ = '0';
	while(i-->0)
		*str++ = tmp[i];
	while(size-->0)
		*str++ = ' ';
	return str;
}

//change the current fmt to get the number
static int getnum(const char** s){
    int ret = 0;
    while(isdigit(**s)){
        //attention here !!!!,not(*(*s++)),*s plus 1,not s plus 1
        ret = ret*10 +(*(*s)++) - '0';
    }
    return ret;
}

int print(const char* fmt,char* buf,va_list arg){
	int len;
	int i;
	char * str;
	char *s;
	int *ip;

	int flags;		/* flags to number() */

	int field_width;	/* width of output field */
	int precision;		/* min. # of digits for integers; max
				   number of chars for from string */

	for (str=buf ; *fmt ; ++fmt) {
		if (*fmt != '%') {
			*str++ = *fmt;
			continue;
		}
			
		/* process flags */
		flags = 0;
		repeat:
			++fmt;		/* this also skips first '%' */
			switch (*fmt) {
				case '-': flags |= LEFT; goto repeat;
				case '+': flags |= PLUS; goto repeat;
				case ' ': flags |= SPACE; goto repeat;
				case '#': flags |= SPECIAL; goto repeat;
				case '0': flags |= ZEROPAD; goto repeat;
				}
		
		/* get field width */
		field_width = -1;
		if (isdigit(*fmt))
			field_width = getnum(&fmt);
		else if (*fmt == '*') {
			/* it's the next argument */
			field_width = va_arg(arg, int);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/* get the precision */
		precision = -1;
		if (*fmt == '.') {
			++fmt;	
			if (isdigit(*fmt))
				precision = getnum(&fmt);
			else if (*fmt == '*') {
				/* it's the next argument */
				precision = va_arg(arg, int);
			}
			if (precision < 0)
				precision = 0;
		}

		switch (*fmt) {
		case 'c':
			if (!(flags & LEFT))
				while (--field_width > 0)
					*str++ = ' ';
			*str++ = (unsigned char) va_arg(arg, int);
			while (--field_width > 0)
				*str++ = ' ';
			break;

		case 's':
			s = va_arg(arg, char *);
			len = strlen(s);
			if (precision < 0)
				precision = len;
			else if (len > precision)
				len = precision;

			if (!(flags & LEFT))
				while (len < field_width--)
					*str++ = ' ';
			for (i = 0; i < len; ++i)
				*str++ = *s++;
			while (len < field_width--)
				*str++ = ' ';
			break;

		case 'o':
			str = number(str, va_arg(arg, unsigned long), 8,
				field_width, precision, flags);
			break;

		case 'p':
			if (field_width == -1) {
				field_width = 8;
				flags |= ZEROPAD;
			}
			str = number(str,
				(unsigned long) va_arg(arg, void *), 16,
				field_width, precision, flags);
			break;

		case 'x':
			flags |= SMALL;
		case 'X':
			str = number(str, va_arg(arg, unsigned long), 16,
				field_width, precision, flags);
			break;

		case 'd':
		case 'i':
			flags |= SIGN;
		case 'u':
			str = number(str, va_arg(arg, unsigned long), 10,
				field_width, precision, flags);
			break;
		case 'b':
			str = number(str, va_arg(arg, unsigned long), 2,
				field_width, precision, flags);
			break;

		case 'n':
			ip = va_arg(arg, int *);
			*ip = (str - buf);
			break;

		default:
			if (*fmt != '%')
				*str++ = '%';
			if (*fmt)
				*str++ = *fmt;
			else
				--fmt;
			break;
		}
	}
	*str = '\0';
	return str-buf;
}

void printk(const char* format,...){
    char buf[1024];
    va_list arg;
    va_start(arg,format);
    int len = print(format,buf,arg);
    va_end(arg);
    buf[len] = '\0';
    console_write(buf);
}

void printk_color(uint32_t back,uint32_t fore,const char* format,...){
    char buf[1024];
    va_list arg;
    va_start(arg,format);
    int len = print(format,buf,arg);
    va_end(arg);
    buf[len] = '\0';
    console_write_color(buf,back,fore);
}