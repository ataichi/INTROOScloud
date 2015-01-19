#include <system.h>

char *tempch;


void parse(char *in){
	int i=0;
	
	if(in[0] != '\0'){
			
		while(in[i] != '\0' && in[i] != ' '){
			tempch[i] = in[i];
			i++;
		}
		
		tempch[i]='\0';
		
		if(strcmp(tempch, "cls")){
			cls();
			
		}else if(strcmp(tempch, "say")){
			i++;
			while(in[i] != '\0'){		
				putch(in[i]);
				i++;
			}
			putch('\n');
			
		}else if(strcmp(tempch, "marquee")){
			int k = 0;
			int j = 0;	
	
			i++;
			while(marqueeArr[k].flag != 0){ //checking the struct flag
				k++;
			}
			
			while(in[i] != '\0'){ 
				marqueeArr[k].data[j] = in[i];
				i++;
				j++;
			}
			marqueeArr[k].x = 0; 
			marqueeArr[k].y = csr_y;
			marqueeArr[k].flag = 1;
			//puts(marqueeArr[k].data);
			
			putch('\n'); //to display newline team Apol
			
			
		}else{
			puts("command not found");
			putch('\n');
			
		}
}
	
	puts("TEAM APOL>");
	
	//i = 0;
	
}
