#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>

int getch();

void PrintMenu();
void SignUp();
void Login();

typedef struct client{
        int student_number;
        char password[20];
        char name[10];
        char address[30];
        char phonenumber[15];
} client;

int main()
{
  while(1)
  {
    PrintMenu();
  }

  return 0;
}

int getch(void){
    int ch;
    struct termios buf, save;
    tcgetattr(0,&save);
    buf = save;
    buf.c_lflag &= ~(ICANON|ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;
    tcsetattr(0, TCSAFLUSH, &buf);
    ch = getchar();
    tcsetattr(0, TCSAFLUSH, &save);
    return ch;
}

void PrintMenu()
{
  char num;

  system("clear");

  printf(">>도서관 서비스<<\n");
  printf("1.회원 가입\t2.로그인\t3.프로그램 종료\n\n");
  printf("번호를 선택하세요 : ");
  scanf("%s", &num);
  getch();

  switch(num)
  {
    case '1' :
        SignUp();
        break;
    case '2' :
        Login();
        break;
    case '3' :
        printf("\n");
        exit(1);
    default :
        printf("다시 입력해주세요.\n\n");
        sleep(2);
        break;
  }
}

void SignUp()
{
        system("clear");

        FILE *ofp; // ofp -> 데이터쓰기

        ofp = fopen("client.txt", "a"); // client.txt 파일에 덮어쓰기, 파일이 없을 >시 만들기

        client c1;

        printf(">> 회원 가입 <<\n\n");

        printf("학번 : ");
        scanf("%d", &c1.student_number);
        getch();

        printf("비밀번호 : ");
        scanf("%[^\n]", c1.password);
        getch();

        printf("이름 : ");
        scanf("%[^\n]", c1.name);
        getch();

        printf("주소 : ");
        scanf("%[^\n]", c1.address);
        getch();

        printf("전화번호 : ");
        scanf("%[^\n]", c1.phonenumber);
        getch();

        fprintf(ofp, "%d|%s|%s|%s|%s\n", c1.student_number, c1.password, c1.name, c1.address, c1.phonenumber);

        fclose(ofp);
}

void Login()
{

}
