#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//void PrintMenu();
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
  system("clear");

  while(1)
  {
    PrintMenu();
  }

  return 0;
}

void PrintMenu()
{
  char num;

  printf(">>도서관 서비스<<\n");
  printf("1.회원 가입\t2.로그인\t3.프로그램 종료\n\n");
  printf("번호를 선택하세요 : ");
  scanf("%s", &num);
  getchar();

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
        printf("\n다시 입력해주세요.\n\n");
        sleep(2);
        break;
  }
}

void SignUp()
{
  FILE *ofp; // ofp -> 데이터쓰기

  ofp = fopen("client.txt", "a"); // client.txt 파일 끝에 이어쓰기, 파일이 없을 시 만들기

  client c1;

  printf("\n>> 회원 가입 <<\n");
  printf("학번, 비밀번호, 이름, 주소, 전화번호를 입력하세요.\n\n");

  printf("학번 : ");
  scanf("%d", &c1.student_number);
  getchar();

  printf("비밀번호 : ");
  scanf("%[^\n]", c1.password);
  getchar();

  printf("이름 : ");
  scanf("%[^\n]", c1.name);
  getchar();

  printf("주소 : ");
  scanf("%[^\n]", c1.address);
  getchar();

  printf("전화번호 : ");
  scanf("%[^\n]", c1.phonenumber);
  getchar();

  fprintf(ofp, "%d|%s|%s|%s|%s\n", c1.student_number, c1.password, c1.name, c1.address, c1.phonenumber);

  fclose(ofp);

  printf("\n회원가입이 되셨습니다.\n\n");
  sleep(2);
  system("clear");
}

void Login()
{
  // 학번으로 시작할 시에
}
