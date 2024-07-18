#include<stdarg.h>

extern int puts(const char*);
extern int putchar(int);

char *convert(unsigned int num, int base)
{
    static char Representation[]= "0123456789ABCDEF";
    static char buffer[50];
    char *ptr;

    ptr = &buffer[49];
    *ptr = '\0';

    do
    {
        *--ptr = Representation[num%base];
        num /= base;
    }while(num != 0);

    return(ptr);
}

void __printf(char* format,...)
{
    unsigned int ui;
    int i;
    char* s;
    char c;

    //Module 1: Initializing __printf's arguments
    va_list arg;
    va_start(arg, format);

    for(char* traverse = format; *traverse != '\0'; traverse++)
    {
        while(*traverse != '%')
        {
            if(*traverse == '\0') return;
            putchar(*traverse);
            traverse++;
        }

        traverse++;

        //Module 2: Fetching and executing arguments
        switch(*traverse)
        {
            case 'c' : {
                c = va_arg(arg,int);     //Fetch char argument
                putchar(c);
                break;
            }

            case 'd' : {
                i = va_arg(arg, int);         //Fetch Decimal/Integer argument
                if(i < 0) {
                    i = -i;
                    putchar('-');
                }
                puts(convert(i, 10));
                break;
            }

            case 'o': {
                ui = va_arg(arg, unsigned int); //Fetch Octal representation
                puts(convert(ui,8));
                break;
            }

            case 's': {
                s = va_arg(arg,char *);       //Fetch string
                puts(s);
                break;
            }

            case 'x': {
                i = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
                puts(convert(i,16));
                break;
            }
        }
    }

    //Module 3: Closing argument list to necessary clean-up
    va_end(arg);
}
