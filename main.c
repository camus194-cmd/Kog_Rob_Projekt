#include <stdio.h>

int main() {

	int age;
	int sentinela = 0 ;

   	printf("Hello World\n");

	while(!sentinela){
	printf("Write ur age : \n");
	scanf("%d",&age);
		if (age > 18 ){
			 printf("You can drink alcohol\n");
				sentinela = 1;
			}
		else { 
			printf("You cannot drink alhocol , go home \n");
			
	}
	
} 
    
		return 0;
}
