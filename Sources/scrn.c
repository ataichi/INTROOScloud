/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Screen output functions for Console I/O
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>
/* These define our textpointer, our background and foreground
*  colors (attributes), and x and y cursor coordinates */
unsigned short *textmemptr;
int attrib = 0x0F;
int csr_x = 0, csr_y = 0;

/*char *store = 0;
int index = 0;*/

//marquee marqueeArr[12];


/* Scrolls the screen */
void scroll(void)
{
    unsigned blank, temp;
    /* A blank is defined as a space... we need to give it
    *  backcolor too */
    blank = 0x20 | (attrib << 8);
    /* Row 25 is the end, this means we need to scroll up */
    int i =0;
    
    if(csr_y >= 25)
    {
        /* Move the current text chunk that makes up the screen
        *  back in the buffer by a line */
		for(i=0;i<12;i++){
	    if(marqueeArr[i].flag == 1){ //if x is at the last cell, 
		//then send the specific marqueeArr array to '\0'
			if(marqueeArr[i].y == 0){		
					memset(marqueeArr[i].data, '\0', 80);
					marqueeArr[i].x = 0;
					marqueeArr[i].y = 0;
					marqueeArr[i].flag = 0;
			}
			else{
				--marqueeArr[i].y;
			}
		}
	}
	
        temp = csr_y - 25 + 1;
        memcpy (textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
        memsetw (textmemptr + (25 - temp) * 80, blank, 80);
        csr_y = 25 - 1;
    }
}

/* Updates the hardware cursor: the little blinking line
*  on the screen under the last character pressed! */
void move_csr(void)
{
    unsigned temp;
    /* The equation for finding the index in a linear
    *  chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    temp = csr_y * 80 + csr_x;
    /* This sends a command to indicies 14 and 15 in the
    *  CRT Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where the hardware cursor is to be 'blinking'. To
    *  learn more, you should look up some VGA specific
    *  programming documents. A great start to graphics:
    *  http://www.brackeen.com/home/vga */
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

/* Clears the screen */
void cls()
{
    unsigned blank;
    int i;
    /* Again, we need the 'short' that will be used to
    *  represent a space with color */
    blank = 0x20 | (attrib << 8);

    /* Sets the entire screen to spaces in our current
    *  color */
    for(i = 0; i < 25; i++)
        memsetw (textmemptr + i * 80, blank, 80);

	for(i=0; i<12; i++){
		memset(marqueeArr[i].data, '\0', 80);
		marqueeArr[i].x = 0;
		marqueeArr[i].y = 0;
		marqueeArr[i].flag = 0;
	}

    /* Update out virtual cursor, and then move the
    *  hardware cursor */
    csr_x = 0;
    csr_y = 0;
    move_csr();
}

/* Deletes a character */

void delch(unsigned short *location){
	unsigned blank;
    blank = 0x20 | (attrib << 8);

    *location = blank;
    store[--index] = '\0';
}

/* Puts a single character on the screen */
void putch(unsigned char c)
{
    unsigned short *where;
    unsigned att = attrib << 8;

    //char *store;
    //int i;

    /* Handle a backspace, by moving the cursor back one space */
    if(c == 0x08)
    {
        if(csr_x != 0 && csr_x != 10) {
			
			where = textmemptr + (csr_y * 80 + csr_x-1);
			delch(where);
			csr_x--;
        }
    }

    /* Handles a tab by incrementing the cursor's x, but only
    *  to a point that will make it divisible by 8 */
    else if(c == 0x09)
    {
        csr_x = (csr_x + 8) & ~(8 - 1);
    }

    /* Handles a 'Carriage Return', which simply brings the
    *  cursor back to the margin */

    else if(c == '\r')
    {
        csr_x = 0;
    }

    /* We handle our newlines the way DOS and the BIOS do: we
    *  treat it as if a 'CR' was also there, so we bring the
    *  cursor to the margin and we increment the 'y' value */
    else if(c == '\n')
    {
        csr_x = 0;
        csr_y++;
	}

    /* Any character greater than and including a space, is a
    *  printable character. The equation for finding the index
    *  in a linear chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    else if(c >= ' ')
    {
        where = textmemptr + (csr_y * 80 + csr_x);
        *where = c | att;	/* Character AND attributes: color */
        csr_x++;
    }

    /* If the cursor has reached the edge of the screen's width, we
    *  insert a new line in there */
    if(csr_x >= 80)
    {
        csr_x = 0;
        csr_y++;
    }

    /* Scroll the screen if needed, and finally move the cursor */
    scroll();
    move_csr();
}

/* Uses the above routine to output a string... */
void puts(char *text)
{
    int i=0;
    
    for (i = 0; i < strlen(text); i++)
    {
        putch(text[i]);
    }
    
}

/* Sets the forecolor and backcolor that we will use */
void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

void fmarquee(void){
	
	int i;
	int tempx;
	int tempy;
	unsigned short *last;
	unsigned blank;
	blank = 0x20 | (0x0F << 8);
	
	tempx = csr_x;
	tempy = csr_y;
	//last = textmemptr + (csr_y * 80 + 79);
    //puts(marqueeArr[0].data)
	for(i=0; i<12; i++){
		
		if(marqueeArr[i].flag != 0){
			++marqueeArr[i].x;
			csr_x = marqueeArr[i].x;
			csr_y = marqueeArr[i].y;
			last = textmemptr + (csr_y * 80 + 79); //last cell of the line per row of the screen
			
			if(marqueeArr[i].x == 80){ //if x is at the last cell, then send the specific marqueeArr 
			//array to '\0'
					memset(marqueeArr[i].data, '\0', 80);
					marqueeArr[i].x = 0;
					marqueeArr[i].y = 0;
					marqueeArr[i].flag = 0;
				}
			
			if(*last != blank || marqueeArr[i].data[strlen(marqueeArr[i].data)-1] == ' '){ //if last cell of the row has character, then '\0' ung dulo ng character
																							//if may 'blank', then check the index 0 if may character pa
				marqueeArr[i].data[strlen(marqueeArr[i].data)-1] = '\0'; //
			}
				
			memsetw (textmemptr + (csr_y) * 80, blank, 80); //clear the line
			puts(marqueeArr[i].data);
		}
	}
	
	csr_x = tempx;
	csr_y = tempy;
	move_csr();
}

/* Sets our text-mode VGA pointer, then clears the screen for us */
void init_video(void)
{
	int i;
	
    textmemptr = (unsigned short *)0xB8000;
    cls();
    
    //initialize marquee
    for(i=0; i<12; i++){
		memset(marqueeArr[i].data, '\0', 80);
		marqueeArr[i].x = 0;
		marqueeArr[i].y = 0;
		marqueeArr[i].flag = 0;
	}
	
    puts("TEAM APOL>");
}





