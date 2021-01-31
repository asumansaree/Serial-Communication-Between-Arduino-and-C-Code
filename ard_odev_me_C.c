#include <stdio.h>
#include <string.h> 

//for exit function:
#include <stdlib.h>

//to use clock funtion in time library we have to add:
#include <time.h>

//for read,close,write functions we have to add:
#include <unistd.h>  

//for open function we have to add:
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 

//for mygetch function
#include <termios.h>
#include <unistd.h> 

//C couldn't catch data send by arduino,so we have to add some wait time to our C code.
//Arduino has it's own delay function but in C,we should add that.
void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds); 
} 

//I will use this function to decide whether user wants to continue button press or not
char mypause ( void );

void menu(void) //Required menu function which run until user enters 0
{
  printf("(1) TURN ON LED ON ARDUINO\n"
  "(2) TURN OFF LED ON ARDUINO\n"
  "(3) FLASH ARDUINO LED 3 TIMES\n"
  "(4) SEND A NUMBER TO ARDUINO TO COMPUTE SQUARE BY ARDUINO\n"
  "(5) Button press counter\n"
  "(0) EXIT\nPLEASE SELECT:");
}
int main()
{
  printf("## WELCOME TO GTU ARDUINO LAB        ##\n"
  "## STUDENT NAME: Asuman Sare ERGUT   ##\n"
  "## PLEASE SELECT FROM THE FOLLOWING  ##\n"
  "## MENU :                            ##\n");
  //The way of opening a port in Linux OS
  //int open(const char *pathname, int flags);
  //The argument flags must include one of the following access modes: O_RDONLY, O_WRONLY, or O_RDWR.
  //These request opening the file read-only, write-only, or read/write, respectively.
  //Here we will use O_RDWR to both read and write
  int current_port = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NONBLOCK );    
  if(current_port == -1)
  {
    printf("Failed to open the port\n");
    exit(1);
  }

  char choice[10]; //In arduino ide,I receive values as string so I should again take values to send arduino as string
  menu();
  scanf("%s",choice);
  while(strcmp(choice,"0") != 0)  //This loop will run until 0 enter
  { 
    delay(100);
    int current_port = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NONBLOCK ); 
    delay(100);
    if(strcmp(choice,"1") == 0)   //To open led
    {
      //To send a value to arduino we should use write function
      //Usage of write function in Linux OS
      //ssize_t write(int fd, const void *buf, size_t count);
      write(current_port,"1",1);
      close(current_port);
    }
    else if(strcmp(choice,"2") == 0)    //To close led
    {
      write(current_port,"2",1);                                              
      close(current_port);
    }
    else if(strcmp(choice,"3") == 0)
    {
      write(current_port,"3",1);    //To turn on led 3 times 
      close(current_port);
    }
    else if(strcmp(choice,"4") == 0)    //To calculate square of number received by user to send arduino
    {
      write(current_port,"4",1);
      char number[10];
      char numcpy[10];    //I create a copy of number array we receive from user to check whether sending value to arduino is succesfull or not
      char string[10];
      printf("Enter a number:\n");
      scanf("%s",number);
      int len = strlen(number);

      for(int i=0; i<len; i++)
        numcpy[i] = number[i];
      numcpy[len] = '\0';

      //printf("sending: %s\n",numcpy);   //Just for control
      write(current_port,numcpy,len);
      delay(10000);
      
      int n = read(current_port,string,10);
      string[n] = '\0';
      //printf("received string: %s\nreturn n value from read: %d\n",string,n);   //Just for control

      delay(50);
      int square = atoi(string);    //Arduino is sending us string so we can convert it to integer via atoi function
      printf("Square of number is:%d\n",square); 
      close(current_port);    //To prevent errors,everytime user enters 4,I closed and open port again and again
    }
    else if(strcmp(choice,"5") == 0)
    {
      write(current_port,"5",1); 
      delay(100);

      char array[20];
      int count = 0;
      char continue_status = 'c';
      int n=0;
      while(n == 0 && continue_status != 'q')
      {
        n = read(current_port,array,20);
        delay(1000);
        if(n > 0)
          count++;
        printf("%d times pushed\n",count);
        continue_status = mypause();
        delay(2000);
        n =0;
      }
      if(continue_status == 'q')
      {
          write(current_port,"q",1);    //Arduino will stop sending data if it gets any value.Because Serial.available will be different than 0
          close(current_port);    //Close the port only if user enters "q" (means quit),else continue.
      }
    }
    else
    {
      printf("Invalid\n");    //If user makes a choice invalid,invalid word will print.
    }
    menu();   //After every if-else statement ends,menu function will execute again and again.Until user enters 0,of course.
    scanf("%s",choice);
  }
  printf("Good Bye!!!\n");    //This is time to say good bye!Thank you.
  return 0;
}


//This function helps us wait until user enters a value.
//And according to that value we will decide button execution will continue or not.
char mypause ( void ) 
{ 
  printf ( "Do you want to quit?Press enter to continue and q to quit\n" );
  fflush(stdin);
  char x = getchar();
  return x;
}
