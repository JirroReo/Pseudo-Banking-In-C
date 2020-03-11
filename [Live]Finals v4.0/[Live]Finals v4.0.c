/*Program: Banking System In C
  Programmer: Jirro Reoloso
  Course, Year & Section: BSCS 1-2
  */


#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <dir.h>
#include <limits.h>
#include <sys/time.h>
#include <ctype.h>

struct credentials{
    char accnum[10];
    char accname[50];
    char accpass[15];
    char accphone[15];
    char acchashedpass[15];

    /*float withdrawold;
    float towithdrawall;
    float withdrawnew;

	float depositold;
	float depositnew;
	float todepositall;*/

    float accbalance;
}user[50];
int z=0;
char adminpass[15] = "admin";
char tellerpass[15] = "teller";
char masterpass[15] = "jirro";
char uname[15];

char dirName[261];
char *prefix = "Transaction Records";


COORD coord = {0,0}; ///set the coordinate to 0, 0 (top-left corner of window);
void gotoxy(int x, int y){
    coord.X = x; coord.Y = y; /// X and Y coordinates
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//This will set the foreground color for printing in a console window.
void SetColor(int ForgC){
     WORD wColor;
     ///We will need this handle to get the current background attribute
     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;

     ///We use csbi for the wAttributes word.
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
        ///Mask out all but the background attribute, and add in the foreground color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}
void ClearConsoleToColors(int ForgC, int BackC){
     WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
     ///Get the handle to the current output buffer...
     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     ///This is used to reset the carat/cursor to the top left.
     COORD coord = {0, 0};
     ///A return value... indicating how many chars were written
     ///   not used but we need to capture this since it will be
     ///   written anyway (passing NULL causes an access violation).
     DWORD count;
     ///This is a structure containing all of the console info
     /// it is used here to find the size of the console.
     CONSOLE_SCREEN_BUFFER_INFO csbi;
     ///Here we will set the current color
     SetConsoleTextAttribute(hStdOut, wColor);
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          ///This fills the buffer with a given character (in this case 32=space).
          FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
          FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count );
          ///This will set our cursor position for the next print statement.
          SetConsoleCursorPosition(hStdOut, coord);
     }
     return;
}
void SetColorAndBackground(int ForgC, int BackC){
     WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
     return;
}
void drawRectangle(){
    int i, j;
    gotoxy(0,0);
    printf("%c",201);
    for(i = 1; i < 78; i++){
        gotoxy(i, 0);
        printf("%c",205);
    }
    gotoxy(78,0);
    printf("%c",187);
    for(i = 1; i < 25; i++){
        gotoxy(78, i);
        if(i == 6){
            printf("%c",185);
        }else{
            printf("%c",186);
        }
    }
    gotoxy(78, 25);
    printf("%c",188);
    for(i = 77; i > 0; i--){
        gotoxy(i,25);
        if(i == 35){
            printf("%c",202);
        }else{
            printf("%c",205);
        }
    }
    gotoxy(0,25);
    printf("%c",200);
    for(i = 24; i > 0; i--){
        gotoxy(0,i);
        if(i == 6){
            printf("%c",204);
        }else{
            printf("%c",186);
        }
    }

    for(i = 1; i < 78; i++){
        gotoxy(i,6);
        if(i == 35){
            printf("%c",203);
        }else{
            printf("%c",205);
        }
    }

    for(i = 7; i < 25; i++){
        gotoxy(35,i);
        printf("%c",186);
    }
}
void clearWindow(){
        int i,j;
    for(i = 37; i < 78; i++){
        for(j = 7; j < 25; j++){
            gotoxy(i,j);printf(" ");
        }
    }
    return;
}
void window(){
    drawRectangle();
    gotoxy(28,2);
    SetColor(3);
    printf("Finals Banking System");
    gotoxy(31,3);
    printf("Jirro, BSCS 1-2");
    gotoxy(33,4);
    printf("[LIVE] v2.2");
    gotoxy(25,24);
    SetColor(17);
}
void printheading(const char st[]){
    SetColorAndBackground(0,15);
    gotoxy(45,8);printf("Operation : %s",st);
    SetColorAndBackground(17,15);
}
char* cap(char string[]){
    int i;
    int x = strlen(string); // You want to get the length of the whole string.
    for (i=0;i<x;i++){
         if (i==0&&string[i]>='a'&&string[i]<='z'||isalpha(string[i]) && string[i-1] == ' '){
         // only first letters of a word.
             string[i]= toupper(string[i]);
         }
    }

    return string;
}


void adduser(){
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
	int print = 37,x=0,isFound=0,i=0;
	char buf1[0x100], buf2[0x100];
	char admincheck[15];
	clearWindow();
    printheading("Add User");
    FILE *fp, *fp1, *fp2;
    fp = fopen("logs.txt","a+");
    SetColor(0);
    if(fp == NULL){
        MessageBox(0,"Error in Opening file\nMake sure your file is not write protected","Warning",0);

    }else{
        A:
        fflush(stdin);
        gotoxy(print+15,10);printf("     ");
        gotoxy(print+9,10);printf("Acc No   : ");gets(user[z].accnum);
       /* while(x<50){
        if(strcmpi(user[z].accnum,user[x].accnum) == 0){
            isFound = 1;
            break;
        }
        ++x;
        }
        if(isFound==1){
            gotoxy(print,12); printf("Acc Number Taken!");
            goto A;
        } */
		char deposits[20] = " - Deposits";
		char withdrawals[20] = " - Withdrawals";
        gotoxy(print,12); printf("                 ");
        gotoxy(print+9,12);printf("Name     : ");gets(user[z].accname);
        snprintf(buf1,sizeof(buf1),"%s/%s%s.txt",dirName,cap(user[z].accname),deposits);
        snprintf(buf2,sizeof(buf2),"%s/%s%s.txt",dirName,cap(user[z].accname),withdrawals);
        fp1 = fopen(buf1,"w+");
        fp2 = fopen(buf2,"w+");
        gotoxy(print+6,14);printf("Password    : ");//gets(user[z].accpass);
        for(i=0;i<15;++i){
    	user[z].accpass[i]=getch();
    	if(user[z].accpass[i]==13){
    		break;
		}
		printf("*");
	}
		user[z].accpass[i]='\0';
        strcpy(user[z].acchashedpass,user[z].accpass);
        strset(user[z].acchashedpass,42);
        //gotoxy(print+15+5,14);printf("%s",user[z].acchashedpass);
        gotoxy(print+5,16);printf("Phone Number : ");gets(user[z].accphone);
        gotoxy(print+5,18);printf("Balance      : ");scanf("%f",&user[z].accbalance);

		fprintf(fp1,"Withdrawal Record of: %s\n",user[z].accname);
		fprintf(fp1,"Account Created at %s-----------------\n",asctime(timeinfo));
		fprintf(fp1,"Operation\t\tOld Bal\t\tChange\t\tNew Bal");

		fprintf(fp2,"Deposit Record of: %s\n",user[z].accname);
		fprintf(fp2,"Account Created at %s-----------------\n",asctime(timeinfo));
		fprintf(fp2,"Operation\t\tOld Bal\t\tChange\t\tNew Bal");

        fprintf(fp,"\n%sAdd User: ",asctime(timeinfo));
        fprintf(fp,"\t%s",user[z].accnum);
        fprintf(fp,"\t%s",user[z].accname);
        fprintf(fp,"\t\t%s",user[z].acchashedpass);
        fprintf(fp,"\t%\t%s",user[z].accphone);
        fprintf(fp,"\t\t%0.2f",user[z].accbalance);

        gotoxy(40,22); printf("The record is successfully added");
        gotoxy(40,23); printf("     Press any to continue");
        getch();
        ++z;
    }
    SetColor(28);
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    return;
}
void searchuser(){
    A:
    clearWindow();
    printheading("Search User");
    SetColor(0);
    char uid[15];
    int isFound = 0,x=0;
    gotoxy(37,10);printf("Enter Acc Num to Search: ");fflush(stdin);
    gets(uid);
    FILE *fp;
    fp = fopen("record.txt","rb");
    while(x<50){
        if(strcmpi(uid,user[x].accnum) == 0){
            isFound = 1;
            break;
        }
        ++x;
    }
    if(isFound == 1){
        gotoxy(37,12);printf("The record is Found");
        gotoxy(37,14);printf("ID       : %s",user[x].accnum);
        gotoxy(37,16);printf("Name     : %s",user[x].accname);
        gotoxy(37,18);printf("Phone No : %s",user[x].accphone);
        gotoxy(37,20);printf("Balance  : %0.2f",user[x].accbalance);

    }else{
        gotoxy(37,12);printf("Sorry, No record found in the database!");
        getch();
        goto A;
    }
    gotoxy(40,22); printf("Operation successfully executed");
    gotoxy(40,23); printf("    Press any to continue");
    getch();
    SetColor(28);
    fclose(fp);
    return;
}
void deleteuser(){
    int x=0;
    char admincheck[15];
    clearWindow();
   /* printheading("Admin Check");
	C:
	gotoxy(37,10);printf("                                      ");
	gotoxy(37,10);printf("Enter administrator password: ");
	fflush(stdin);gets(admincheck);
	if(strcmp(admincheck,adminpass)!=0){
		gotoxy(37,14);printf("Wrong password!");
		goto C;
	}
*/
    clearWindow();
    printheading("Delete Record");
    SetColor(0);
    char uid[15],pass[15];
    int isFound = 0, print = 37,i=0;
    B:
    gotoxy(37,12);printf("                                      ");
    gotoxy(37,10);printf("Enter ID of Acc to Delete: ");fflush(stdin);
    gets(uid);
    while(x<50){
        if(strcmpi(uid,user[x].accnum)==0){
            A:
            gotoxy(37,14);printf("               ");
            gotoxy(37,12);printf("Enter password to proceed : "); fflush(stdin);

			//gets(pass);
			for(i=0;i<15;++i){
    			pass[i]=getch();
    			if(pass[i]==13){
    			break;
				}
				printf("*");
			}
				pass[i]='\0';

            if(strcmp(pass,user[x].accpass)==0){
            gotoxy(37+28,12);printf("%s", user[x].acchashedpass);
            strcpy(user[x].accnum,"");
            gotoxy(37,14);printf("Record deleted!");
            }else{
                gotoxy(37,14);printf("Wrong password!");
                goto A;
            }
            break;
        }
        else{
            gotoxy(37,12);printf("Account not found! Please try again :(");
            goto B;
            break;
        }
        ++x;
    }
    gotoxy(40,22); printf("Operation completely executed");
    gotoxy(40,23); printf("    Press any to continue");
    getch();
    SetColor(28);
    return;

}
void moduser(){
	char admincheck[15];
    clearWindow();
    /*printheading("Admin Check");
	C:
	gotoxy(37,10);printf("                                      ");
	gotoxy(37,10);printf("Enter administrator password: ");
	fflush(stdin);gets(admincheck);
	if(strcmp(admincheck,adminpass)!=0){
		gotoxy(37,14);printf("Wrong password!");
		goto C;
	}
    */
    clearWindow();
    printheading("Modify Record");
    SetColor(0);
    char uid[15], pass[15];
    int isFound = 0, print = 37, x=0,i=0;
    char oldaccnum[15];
    gotoxy(37,10);printf("Enter ID of account to modify: ");
    fflush(stdin);
    gets(uid);
    FILE *fp;
    fp = fopen("record.txt","a+");
    while(x<50){
        if(strcmpi(uid,user[x].accnum) == 0){
            A :
			gotoxy(37+28,12);printf("      ");
            gotoxy(37,12);printf("Enter password to proceed : "); fflush(stdin);

			//gets(pass);
            for(i=0;i<15;++i){
    			pass[i]=getch();
    			if(pass[i]==13){
    			break;
				}
				printf("*");
			}
				pass[i]='\0';

            if(strcmp(pass,user[x].accpass)==0){
            	gotoxy(print,14);printf("               "); //clear wrong pw message
           // strcpy(oldaccnum,user[x].accnum);
            	fflush(stdin);
        //gotoxy(print,12);printf("New Acc No       : ");gets(user[x].accnum);
        	gotoxy(print,14);printf("New Name         : ");gets(user[x].accname);
        	gotoxy(print,16);printf("New Password     : ");//gets(user[x].accpass);
        	for(i=0;i<15;++i){
    		user[z].accpass[i]=getch();
    			if(user[z].accpass[i]==13){
    			break;
				}
			printf("*");
			}
			user[z].accpass[i]='\0';
        	strcpy(user[x].acchashedpass,user[x].accpass);
        	strset(user[x].acchashedpass, 42);
        	gotoxy(print,18);printf("New Phone Number : ");gets(user[x].accphone);
        	//gotoxy(print,20);printf("New Balance      : ");scanf("%f",&user[x].accbalance);
        	fprintf(fp,"\n%s",user[x].accnum);
        	fprintf(fp,"\t\t\t%s",user[x].accname);
        	fprintf(fp,"\t\t\t%s",user[x].acchashedpass);
        	fprintf(fp,"\t\t\t%s",user[x].accphone);
        	fprintf(fp,"\t\t\t%0.2f",user[x].accbalance);
        	fprintf(fp,"\nPrevID: %s",oldaccnum);
        	isFound=1;
            }
            else{
                gotoxy(print,14);printf("Wrong Password!");
                goto A;
            }
        }
        ++x;
        if(!isFound){
        gotoxy(print, 12);printf("No Record Found");
    }
    gotoxy(40,22); printf("Operation successfully executed");
    gotoxy(40,23); printf("    Press any to continue");
    getch();
    fclose(fp);
    SetColor(28);
    return;

    }
}

void genreportu(){
    system("cls");
    //printheading("All Users");
    int x,y;
    char ch;
    FILE *fp;
    fp = fopen("allusers.txt","w+");
    fputs("\t\t\t-------------------\n\t\t\tBanko De Jiro\n\t\t\tAll Users\n\t\t\t-------------------",fp);
    fputs("\nNo.\tName\t\tPass\t\tPhone Number\t\tBalance",fp);
    for(x=0;x<50;x++){
            if(user[x].accbalance!=0){
        fprintf(fp,"\n%s",user[x].accnum);
        fprintf(fp,"\t%s",user[x].accname);
        fprintf(fp,"\t\t%s",user[x].acchashedpass);
        fprintf(fp,"\t%\t%s",user[x].accphone);
        fprintf(fp,"\t\t%0.2f",user[x].accbalance);
        }
    }
    fclose(fp);

    /*fp = fopen("withdrawals.txt","a+");
    fputs("\t\t\t-------------------\n\t\t\tAccounts\n\t\t\t-------------------",fp);
    for(x=0;x<50;x++){
            if(user[x].accbalance!=0){
        fprintf(fp,"\n%s",user[x].accnum);
        fprintf(fp,"\t%s",user[x].accname);
        fprintf(fp,"\t\t%s",user[x].acchashedpass);
        fprintf(fp,"\t%\t%s",user[x].accphone);
        fprintf(fp,"\t\t%0.2f",user[x].accbalance);
        }
    }
    //fputs("\nNo.\tName\t\tOld Balance\t\tNew Balance",fp);
    fclose(fp);

    fp = fopen("deposits.txt","a+");
    fputs("\t\t\t-------------------\n\t\t\tAccounts\n\t\t\t-------------------",fp);
    for(x=0;x<50;x++){
            if(user[x].accbalance!=0){
        fprintf(fp,"\n%s",user[x].accnum);
        fprintf(fp,"\t%s",user[x].accname);
        fprintf(fp,"\t\t%s",user[x].acchashedpass);
        fprintf(fp,"\t%\t%s",user[x].accphone);
        fprintf(fp,"\t\t%0.2f",user[x].accbalance);
        }
    }
    fclose(fp); */

    fp = fopen("allusers.txt","r");
    SetColor(0);
    fseek(fp, 0, SEEK_SET);
    while((ch = fgetc(fp)) != EOF){
    printf("%c", ch);
	}
    gotoxy(45,23); printf("Operation executed");
    gotoxy(45,24); printf("Press any to continue");
    fclose(fp);
    //gotoxy(40,15); printf("Text file has been opened.");
    //system("notepad record.txt");
    getch();
    ClearConsoleToColors(17,15);
    window();
}
void genreportwadmin(){
	system("cls");
    //printheading("All Users");
    int x,y;
    char ch;
    FILE *fp;
    //char buf[0x100];
   // snprintf(buf,sizeof(buf),"%s.txt",user[z].accname);
   // fp = fopen(buf,"r");
   fp = fopen("withdrawals.txt","r");
    SetColor(0);
    fseek(fp, 0, SEEK_SET);
    while((ch = fgetc(fp)) != EOF){
    printf("%c", ch);
	}
    gotoxy(45,23); printf("Operation executed");
    gotoxy(45,24); printf("Press any to continue");
    fclose(fp);
    //gotoxy(40,15); printf("Text file has been opened.");
    //system("notepad record.txt");
    getch();
    ClearConsoleToColors(17,15);
    window();
    clearWindow();

}
void genreportdadmin(){
	system("cls");
    //printheading("All Users");
    int x,y;
    char ch;
    FILE *fp;
    //char buf[0x100];
   // snprintf(buf,sizeof(buf),"%s.txt",user[z].accname);
   // fp = fopen(buf,"r");
   fp = fopen("deposits.txt","r");
    SetColor(0);
    fseek(fp, 0, SEEK_SET);
    while((ch = fgetc(fp)) != EOF){
    printf("%c", ch);
	}
    gotoxy(45,23); printf("Operation executed");
    gotoxy(45,24); printf("Press any to continue");
    fclose(fp);
    //gotoxy(40,15); printf("Text file has been opened.");
    //system("notepad record.txt");
    getch();
    ClearConsoleToColors(17,15);
    window();
    clearWindow();

}
void genreportw(){
	char namename[20],uid[15];
	int isFound=0;
	int x=0,y;
	clearWindow();
	A:
	gotoxy(37+9+13,10);printf("       ");
	gotoxy(37+9,10);printf("Acc Num   : ");fflush(stdin);gets(uid);

    //printheading("All Users");
    while(x<50){
        	if(strcmpi(uid,user[x].accnum) == 0){
            isFound = 1;
            break;
       		 }
    ++x;
    }
    if(isFound == 1){
    			system("cls");
				char ch;
   				FILE *fp;
    			char buf[0x100];
    			char withdraw[20] =" - Withdrawals";
    			snprintf(buf,sizeof(buf),"%s/%s%s.txt",dirName,cap(user[x].accname),withdraw);
    			fp = fopen(buf,"r");
    			//fp = fopen("withdrawals.txt","r");
    			SetColor(0);
    			fseek(fp, 0, SEEK_SET);
    				while((ch = fgetc(fp)) != EOF){
    					printf("%c", ch);
					}
    			gotoxy(45,23); printf("Operation executed");
    			gotoxy(45,24); printf("Press any to continue");
    			fclose(fp);
    			//gotoxy(40,15); printf("Text file has been opened.");
    			//system("notepad record.txt");
   				 getch();
   				 ClearConsoleToColors(17,15);
  			  	window();
   				clearWindow();
			}else{
        	gotoxy(37,12);printf("Sorry, No record found in the database!");
       		getch();
        	goto A;
    }
}
void genreportd(){
	char namename[20],uid[15];
	int isFound=0;
	int x=0,y;
	clearWindow();
	A:
	gotoxy(37+9+13,10);printf("       ");
	gotoxy(37+9,10);printf("Acc Num   : ");fflush(stdin);gets(uid);

    //printheading("All Users");
     while(x<50){
        if(strcmpi(uid,user[x].accnum) == 0){
            isFound = 1;
            break;
        }
        ++x;
    }
    if(isFound == 1){
    			system("cls");
    			//printheading("All Users");
   				 //int x,y;
   				 char ch;
   	 			FILE *fp;
			    char buf[0x100];
			    char deposit[20]=" - Deposits";
			    snprintf(buf,sizeof(buf),"%s/%s%s.txt",dirName,cap(user[x].accname),deposit);
			    fp = fopen(buf,"r");
			    //fp = fopen("deposits.txt","r");
			    SetColor(0);
			    fseek(fp, 0, SEEK_SET);
				    while((ch = fgetc(fp)) != EOF){
				    printf("%c", ch);
					}
			    gotoxy(45,23); printf("Operation executed");
			    gotoxy(45,24); printf("Press any to continue");
			    fclose(fp);
			    //gotoxy(40,15); printf("Text file has been opened.");
			    //system("notepad record.txt");
			    getch();
			    ClearConsoleToColors(17,15);
			    window();
				}else{
			        gotoxy(37,12);printf("Sorry, No record found in the database!");
			       	getch();
			        goto A;
		    	}
}
void genreport(){
    clearWindow();
    SetColor(0);
    printheading("Generate Report");
    char choice;
    gotoxy(37,10);printf("Select One: ");
    gotoxy(37,14);printf("1. Generate Report of All Users");
    gotoxy(37,16);printf("2. Generate Report of All Withdrawals");
    gotoxy(37,18);printf("3. Generate Report of All Deposits");
    gotoxy(37,20);printf("Enter your choice: ");fflush(stdin);
    A:
    gotoxy(58,20);printf("   ");
    gotoxy(58,20);scanf(" %c",&choice);
    gotoxy(37,22);printf("                           ");

    switch(choice){
        case '1':
            genreportu();
            break;
        case '2':
        	if(strcmp(uname,"admin")==0){
        	genreportwadmin();
			}else{
			genreportw();
			}
            break;
        case '3':
        	if(strcmp(uname,"admin")==0){
        	genreportdadmin();
			}else{
            genreportd();
        	}
            break;
        default:
            gotoxy(37,22);printf("Error! Please try again! :(");
            goto A;
            break;
    }
}

void exitfunc(){
    FILE *fp;
    fp = fopen("record.txt","a+");
    fputs("\n\n",fp);
    fclose(fp);
    exit(0);

}
void changepass(){
    clearWindow();
    printheading("Change Password");
    SetColor(0);
    char uid[15];
    char pass[15];
    char temppass[15];
    int isFound = 0,x=0;
    gotoxy(37,10);printf("Enter Acc Num: ");fflush(stdin);
    gets(uid);
    FILE *fp;
    fp = fopen("record.txt","rb");
    while(x<50){
        if(strcmpi(uid,user[x].accnum) == 0){
            gotoxy(37,12);printf("The record is Found");
            gotoxy(37,14);printf("Name       : %s",user[x].accname);
            A:
            gotoxy(37,20);printf("               ");
            gotoxy(37,16);printf("                                                ");
            gotoxy(37,16);printf("Enter old password : "); fflush(stdin);
            gets(pass);
            strcpy(temppass,pass);
            strset(temppass,42);
            gotoxy(58,16);printf("%s",temppass);
            if(strcmp(pass,user[x].accpass)==0){
                gotoxy(37,18);printf("Enter new password : "); fflush(stdin);
                gets(user[x].accpass);
                strcpy(user[x].acchashedpass,user[x].accpass);
                strset(user[x].acchashedpass,42);
                gotoxy(58,18);printf("%s",user[x].acchashedpass);
            }
            else{
                gotoxy(37,20);printf("Wrong password!");
                goto A;
            }
            break;
        }
        else{
        gotoxy(37,12);printf("Sorry, No record found in the database :(");
    }
        ++x;

    }
    gotoxy(40,22); printf("Operation successfully executed");
    gotoxy(40,23); printf("Navigate back in the main window");
    SetColor(28);
}

void withdraw(){
    //int x=0, isFound=0;
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    clearWindow();
    printheading("Withdraw");
    SetColor(0);
    char uid[15];
    char pass[15];
    char temppass[15];
    float towithdraw = 0;
    int isFound = 0,x=0,i=0;
    B:
    gotoxy(37+25,10);printf("     ");
    gotoxy(37,10);printf("Enter acc num: ");fflush(stdin);
    gets(uid);

    while(x<50){
        if(strcmpi(user[x].accnum,uid)==0)
            break;
        ++x;
    }

    FILE *fp, *fp1, *fp2;
    fp = fopen("withdrawals.txt","a");
    fp1 = fopen("logs.txt","a");
    char buf[0x100];
    char withdraw[20] = " - Withdrawals";

   // fputs("\nNo.\t\t\tName\t\t\tOld Balance\t\t\tNew Balance",fp);
    while(x<50){
            if(strcmpi(uid,user[x].accnum) == 0){
            	isFound=1;
            	break;
            }
            ++x;
        }

        snprintf(buf,sizeof(buf),"%s/%s%s.txt",dirName,cap(user[x].accname),withdraw);
        fp2 = fopen(buf,"a");

        if(isFound==1){
            gotoxy(37,12);printf("The record is found");

            fprintf(fp,"\n%s",user[x].accnum);
            fprintf(fp,"\t\t%s",user[x].accname);
            fprintf(fp,"\t\t\t%0.2f",user[x].accbalance);

            fprintf(fp1,"\n%sWithdrawal:",asctime(timeinfo));
            fprintf(fp1,"\t%s",user[x].accnum);
            fprintf(fp1,"\t\t%s",user[x].accname);
            fprintf(fp1,"\t\t\t%0.2f",user[x].accbalance);

            A:
            gotoxy(37,14);printf("Enter password to proceed : "); fflush(stdin);

			//gets(pass);
			for(i=0;i<15;++i){
    			pass[i]=getch();
    			if(pass[i]==13){
    			break;
				}
				printf("*");
			}
				pass[i]='\0';

            if(strcmp(pass,user[x].accpass)==0){
            gotoxy(37+28,14);printf("       ");
            gotoxy(37,16);printf("                  ");
            strcpy(temppass,pass);
            strset(temppass,42);
            gotoxy(37+28,14);printf("%s",temppass);
            fprintf(fp2,"\nWithdraw:\t\t%0.2f",user[x].accbalance);
            gotoxy(37,16);printf("Current Balance: %0.2f",user[x].accbalance);
            C:
            gotoxy(37,18);printf("Enter amount to withdraw: "); fflush(stdin);
            scanf("%f",&towithdraw);
                if(towithdraw<user[x].accbalance){
                //strcat(user[x].withdrawals,"Time\tOld Balance\tWithdrawn\tNew Balance");
                gotoxy(37,21);printf("                ");
                //user[x].withdrawold=user[x].accbalance;
               // user[x].towithdrawall=towithdraw;
                gotoxy(37,19);printf("Old Balance: %0.2f",user[x].accbalance);
                //fprintf(fp2,"%0.2f",user[x].accbalance);
                fprintf(fp2,"\t\t-%0.2f",towithdraw);
                user[x].accbalance-=towithdraw;
                gotoxy(37,20);printf("New Balance: %0.2f",user[x].accbalance);
                fprintf(fp2,"\t\t%0.2f",user[x].accbalance);
               // user[x].withdrawnew=user[x].accbalance;
                //strcat(user[x].withdrawals,"\t%0.2f",user[x].accbalance);
                fprintf(fp,"\t\t%0.2f\t\t\t%0.2f",towithdraw,user[x].accbalance);
                fprintf(fp1,"\t\t%0.2f\t\t\t%0.2f",towithdraw,user[x].accbalance);
                }
                else{
                    gotoxy(37,21);printf("Invalid amount!");
                    goto C;
                }
            }else{
            gotoxy(37,14);printf("Password Incorrect");
            goto A;
            }
        }else{
            gotoxy(37,12);printf("Sorry, No record found in the database!");
            goto B;
        }


    gotoxy(40,22); printf("Operation successfully executed");
    gotoxy(40,23); printf("Navigate back in the main window");
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    getch();
    SetColor(28);
}
void deposit(){
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    clearWindow();
    printheading("Deposit");
    SetColor(0);
    char uid[15];
    char pass[15];
    char temppass[15];
    float todeposit = 0;
    int isFound = 0,x=0;
    B:
    gotoxy(37+25,10);printf("     ");
    gotoxy(37,10);printf("Enter Acc Num to Search: ");fflush(stdin);
    gets(uid);
    FILE *fp, *fp1, *fp2;
    fp = fopen("deposits.txt","a");
    fp1 = fopen("logs.txt","a");
    char buf[0x100];
    char deposit[20]= " - Deposits";

   // fputs("\nNo.\t\t\tName\t\t\tOld Balance\t\t\tNew Balance",fp);
    while(x<50){
    		if(strcmpi(uid,user[x].accnum) == 0){
            gotoxy(37,12);printf("The record is Found");
            isFound=1;
            break;
        }
        ++x;
    }

    snprintf(buf,sizeof(buf),"%s/%s%s.txt",dirName,cap(user[x].accname),deposit);
    fp2 = fopen(buf,"a");

    if(isFound==1){
            fprintf(fp,"\n%s",user[x].accnum);
            fprintf(fp,"\t\t%s",user[x].accname);
            fprintf(fp,"\t\t\t%0.2f",user[x].accbalance);

            fprintf(fp1,"\n%sDeposit:",asctime(timeinfo));
            fprintf(fp1,"\t%s",user[x].accnum);
            fprintf(fp1,"\t\t%s",user[x].accname);
            fprintf(fp1,"\t\t\t%0.2f",user[x].accbalance);

            A:
            gotoxy(37+28,16);printf("       ");
            C:
            gotoxy(37,17);printf("Enter amount to deposit: "); fflush(stdin);
            scanf("%f",&todeposit);
            gotoxy(37,18);printf("               ");
                if(todeposit>=0){
           // strcat(user[x].deposits,"Time\tOld Balance\tDeposited\tNew Balance");
           //	user[x].depositold=user[x].accbalance;
            //user[x].todepositall=todeposit;
            gotoxy(37,19);printf("Old Balance: %0.2f",user[x].accbalance);
            fprintf(fp2,"\nDeposit:\t\t%0.2f",user[x].accbalance);
            fprintf(fp2,"\t\t+%0.2f",todeposit);
            //strcat(user[x].deposits,"\n%s%0.2f%0.2f",asctime(timeinfo),user[x].accbalance,todeposit);
            user[x].accbalance+=todeposit;
            gotoxy(37,20);printf("New Balance: %0.2f",user[x].accbalance);
            fprintf(fp2,"\t\t%0.2f",user[x].accbalance);
          //  user[x].depositnew=user[x].accbalance;
           // strcat(user[x].deposits,"\t%0.2f",user[x].accbalance);
            fprintf(fp,"\t\t%0.2f\t\t\t%0.2f",todeposit,user[x].accbalance);
            fprintf(fp1,"\t\t%0.2f\t\t\t%0.2f",todeposit,user[x].accbalance);
                }else{
                gotoxy(37,18);printf("Invalid amount!");
                goto C;
                }
            }else{
            gotoxy(37,12);printf("Sorry, No record found in the database!");
            goto B;
			}

    gotoxy(40,22); printf("Operation successfully executed");
    gotoxy(40,23); printf("Navigate back in the main window");
    getch();
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    SetColor(28);
}
void withdepo(){
    clearWindow();
    SetColor(0);
    printheading("Transactions");
    char choice;
    SetColor(0);
    gotoxy(37,10);printf("Select One: ");
    gotoxy(37,14);printf("1. Withdraw");
    gotoxy(37,16);printf("2. Deposit");
    //gotoxy(37,18);printf("3. Generate Report of All Deposits");
    gotoxy(37,20);printf("Enter your choice: ");fflush(stdin);
    gotoxy(58,20);scanf(" %c",&choice);
    switch(choice){
        case '1':
            withdraw();
            break;
        case '2':
            deposit();
            break;
        //case '3':
            //genreportd();
            //break;
        default:
            gotoxy(37,20);printf("Error! Please try again! :(");
            break;
    }
}

void mainmenu(){
    int choice;
    SetColor(3);
    int x = 2;
    ClearConsoleToColors(17,15);
    window();
    while(choice!=8){
        SetColor(3);
        gotoxy(x+19,24);printf("    "); //line covers previous input with whitespace
        gotoxy(x,8);printf("1. Add User");
        gotoxy(x,10);printf("2. Search For A User");
        gotoxy(x,12);printf("3. Modify A User's Record");
        gotoxy(x,14);printf("4. Delete A User's Record");
        gotoxy(x,16);printf("5. Change password");
        gotoxy(x,18);printf("6. Transactions");
        gotoxy(x,20);printf("7. Generate Reports");
        gotoxy(x,22);printf("8. Back");
        gotoxy(x,24);printf("Enter your choice: ");
        scanf("%d",&choice);
        gotoxy(x+49,15);printf("              ");
        switch(choice){
            case 1:
                adduser();
                break;
            case 2:
               searchuser();
                break;
            case 3:
               moduser();
                break;
            case 4:
                deleteuser();
                break;
            case 5:
                changepass();
                break;
            case 6:
                withdepo();
                break;
            case 7:
                genreport();
                break;
           // case 8:
                //exitfunc();
           //     exit(1);
           //     break;
            case 9:
            	system("notepad logs.txt");
            	break;
            default:
                gotoxy(x+49,15);printf("Invalid Input!");
                gotoxy(x+53,16);printf("Error!");
                break;
        }

   }

}
void adminmenu(){
    int choice=0;
    int x = 2;
    //system("cls");
    ClearConsoleToColors(17,15);
    window();
    while(choice!=6){
        SetColor(3);
        clearWindow();
        //SetColor(0);
        gotoxy(x+19,24);printf("    "); //line covers previous input with whitespace
        gotoxy(x,8);printf("        ADMIN MENU");
        gotoxy(x,10);printf("1. Add User");
        gotoxy(x,12);printf("2. Search For A User");
        gotoxy(x,14);printf("3. Modify A User's Record");
        gotoxy(x,16);printf("4. Delete A User's Record");
        //gotoxy(x,16);printf("5. Change password");
        //gotoxy(x,18);printf("6. Transactions");
        gotoxy(x,18);printf("5. Generate Reports");
        gotoxy(x,20);printf("6. Login Screen");
        gotoxy(x,24);printf("Enter your choice: ");
        fflush(stdin);scanf("%d",&choice);
        gotoxy(x+7,22);printf("              "); //clears invalid input
        switch(choice){
            case 1:
                adduser();
                break;
            case 2:
               searchuser();
                break;
            case 3:
               moduser();
                break;
            case 4:
                deleteuser();
                break;
            case 5:
                genreport();
               // changepass();
                break;
         //   case 6:
               // withdepo();
          //      break;
          //  case 7:
          //      genreport();
          //      break;
          //  case 8:
                //exitfunc();
          //      exit(1);
          //      break;
            case 0:
            	system("notepad logs.txt");
            	break;
            default:
                gotoxy(x+7,22);printf("Invalid Input!");
              //  gotoxy(x+53,16);printf("Error!");
                break;
        }

   }
}
void tellermenu(){
    int choice=0;
    int x = 2;
    //system("cls");
    ClearConsoleToColors(17,15);
    window();
    while(choice!=5){
        SetColor(3);
        clearWindow();
        //SetColor(0);
        gotoxy(x+19,24);printf("    "); //line covers previous input with whitespace
        gotoxy(x,8);printf("       TELLER MENU");
        gotoxy(x,10);printf("1. Search For A User");
        gotoxy(x,12);printf("2. Withdraw");
        gotoxy(x,14);printf("3. Deposit");
        gotoxy(x,16);printf("4. Generate Reports");
        //gotoxy(x,16);printf("5. Change password");
        //gotoxy(x,18);printf("6. Transactions");
        gotoxy(x,18);printf("5. Login Screen");
        //gotoxy(x,20);printf("6. Back");
        gotoxy(x,24);printf("Enter your choice: ");
        fflush(stdin);scanf("%d",&choice);
        gotoxy(x+7,22);printf("              "); //clears invalid input
        switch(choice){
            case 1:
                searchuser();
                break;
            case 2:
                withdraw();
                break;
            case 3:
                deposit();
                break;
            case 4:
                genreport();
                break;

         //   case 6:
               // withdepo();
          //      break;
          //  case 7:
          //      genreport();
          //      break;
          //  case 8:
                //exitfunc();
          //      exit(1);
          //      break;
            case 0:
            	system("notepad logs.txt");
            	break;
            default:
                gotoxy(x+7,22);printf("Invalid Input!");
              //  gotoxy(x+53,16);printf("Error!");
                break;
        }

   }
}

int main(){
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    char pass[15]="";
    char* dirname="Transaction Records";
    pass[0] = '\0';
    int i=0;
    mkdir(dirname);
    snprintf(dirName,sizeof(dirName),"%s", prefix);
    ClearConsoleToColors(17,15);
    FILE *fp;
    fp = fopen("allusers.txt","w");
    fputs("\t\t\t-------------------\n\t\t\tBanko De Jiro\n\t\t\tAll Users\n\t\t\t-------------------",fp);
    fputs("\nNo.\tName\t\tPass\t\tPhone Number\t\tBalance",fp);
    fclose(fp);
    FILE *fp1;
    fp1 = fopen("withdrawals.txt","w");
    fputs("\t\t\t-------------------\n\t\t\tBanko De Jiro\n\t\t\tWithdrawals\n\t\t\t-------------------",fp1);
    fputs("\nNo.\tName\t\tOld Balance\t\tWithdrawn\t\tNew Balance",fp1);
    fclose(fp1);
    FILE *fp2;
    fp2 = fopen("deposits.txt","w");
    fputs("\t\t\t-------------------\n\t\t\tBanko De Jiro\n\t\t\tDeposits\n\t\t\t-------------------",fp2);
    fputs("\nNo.\tName\t\tOld Balance\t\tDeposited\t\tNew Balance",fp2);
    fclose(fp2);
    FILE *fp3;
    fp3 = fopen("logs.txt","w");
    fputs("\t\t--Logs--",fp3);
    fclose(fp);
    SetConsoleTitle("[Finals] Banking System");

    while(1){
    ClearConsoleToColors(17,15);
    window();
    //clearWindow();
    SetColor(0);

    gotoxy(48+2,8+2);printf(" dP""b8.  dP\"Y8 ");
    gotoxy(48+2,9+2);printf("dP  `\" `Ybo.\" ");
    gotoxy(48+2,10+2);printf("Yb      o.`Y8b ");
    gotoxy(48+2,11+2);printf(" YboodP 8bodP' ");

    gotoxy(49+1,12+3);printf("  .d    oP\"Yb. ");
    gotoxy(49+1,13+3);printf(".d88 ___\"' dP' ");
    gotoxy(49+1,14+3);printf("  88 \"\"\"  dP'  ");
    gotoxy(49+1,15+3);printf("  88    .d8888 ");


    gotoxy(52,20);printf(" Good Day! ");
    gotoxy(45,21);printf("%s",asctime(timeinfo));
    gotoxy(15,9);printf("LOGIN");
    A:
    gotoxy(7,14);printf("Username:           ");
    gotoxy(7,18);printf("Password:           ");
    gotoxy(17,14);scanf("%s",&uname);
    if(strcmp(strlwr(uname),"exit")==0){
    		gotoxy(25,25);
			exit(1);
	}else if(strcmp(strlwr(uname),"admin")!=0&&strcmp(strlwr(uname),"teller")!=0&&strcmp(strlwr(uname),"jirro")!=0){
        gotoxy(10,20);printf("No account found!");
        goto A;
    }
        gotoxy(10,20);printf("                 "); //clears no account found

    B:
    gotoxy(7,18);printf("Password:           ");
    gotoxy(17,18);
    for(i=0;i<15;++i){
    	pass[i]=getch();
    	printf("*");
    	if(pass[i]==13){
    		break;
		}
	}
		pass[i]='\0';
    //scanf("%s",&pass);
		if(strcmp(strlwr(uname),"admin")==0&&strcmp(pass,adminpass)==0){
            adminmenu();
        }else if(strcmp(strlwr(uname),"teller")==0&&strcmp(pass,tellerpass)==0){
            tellermenu();
        }else if(strcmp(strlwr(uname),"jirro")==0&&strcmp(pass,masterpass)==0){
            mainmenu();
        }else{
            gotoxy(10,20); printf("Wrong password!");
            goto B;
        }
            gotoxy(10,20); printf("               "); //clears invalid password
    }
    //use_pass_field();
    //mainmenu();
    return 0;
}

