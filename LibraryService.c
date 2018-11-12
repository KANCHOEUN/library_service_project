#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void PrintMenu();
void SignUp();
void Login();

int main()
{
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
  scanf("%d", &num);

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

}

void Login()
{

}
