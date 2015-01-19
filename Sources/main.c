/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Main.c: C code entry.
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>
unsigned int j = 0;
unsigned int com = 0;

void *memcpy(void *dest, const void *src, size_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void *memset(void *dest, char val, size_t count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

size_t strlen(char *str)
{
    size_t retval;
    for(retval = 0; *str != '\0'; str++) retval++;
    return retval;
}

size_t strcmp(char *str1, char *str2){
	int i = 0;
	int bool = 0;
	
	if(strlen(str1) == strlen(str2)){
		while(i < strlen(str1)){
			if(str1[i] == str2[i]){
				bool = 1;
				i++;
			}else{
				return 0;
			}
		}
		
		return bool;
	}else{
		return 0;
	}
		
}

unsigned char inportb (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}


int main()
{

    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    init_video();
    timer_install();
    keyboard_install();
    __asm__ __volatile__ ("sti");
	
    //puts("TEAM APOL>");
    //check("cls");
    

//    i = 10 / 0;
//    putch(i);

    for (;;);
	return 0;
}
