#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void PrintMenu();
void SignUp();
int Login();
void Client_Login();
void Admin_Login();

union student_number
{
  int num;
  char s[10];
};

typedef struct client{
        union student_number id;
        char password[20];
        char name[20];
        char address[100];
        char phonenumber[30];
        struct client * next;
}Client;

int main()
{
  system("clear");
  // client.txt 정렬 함수 따로 만들어서 넣기

  while(1)
  {
    PrintMenu();
  }

  return 0;
}

void PrintMenu()
{
  char num;

  printf(">> 도서관 서비스 <<\n");
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
  // 회원가입에 추가해야할 사항
  // 1. client.txt 파일 학번 순으로 정렬
  // 2. client.txt 파일에 동일한 학번 발견 시 출력하고 다시 회원가입으로 돌아가기
  FILE *ofp; // ofp -> 데이터쓰기
  Client info;

  ofp = fopen("client.txt", "a"); // client.txt 파일 끝에 이어쓰기, 파일이 없을 시 만들기

  printf("\n>> 회원 가입 <<\n");
  printf("학번, 비밀번호, 이름, 주소, 전화번호를 입력하세요.\n\n");

  printf("학번 : ");
  scanf("%d", &(info.id.num));
  getchar();

  printf("비밀번호 : ");
  scanf("%[^\n]", info.password);
  getchar();

  printf("이름 : ");
  scanf("%[^\n]", info.name);
  getchar();

  printf("주소 : ");
  scanf("%[^\n]", info.address);
  getchar();

  printf("전화번호 : ");
  scanf("%[^\n]", info.phonenumber);
  getchar();

  fprintf(ofp, "%d|%s|%s|%s|%s\n", info.id.num, info.password, info.name, info.address, info.phonenumber);

  fclose(ofp);

  printf("\n회원가입이 되셨습니다.\n\n");
  sleep(2);
  system("clear");
}

int Login()
{
  FILE * ofp;
  Client login;
  Client * head = NULL;
  Client * newNode = NULL;
  Client * tail = NULL;

  char str1[10];
  char str2[10];

  ofp = fopen("client.txt", "rt");

  printf("\n>> 로그인 <<\n");
  printf("학번 : ");
  scanf("%s", str1);
  printf("비밀번호 : ");
  scanf("%s", login.password);

  newNode = (Client *)malloc(sizeof(Client));

  while(!feof(ofp))
  {
    fscanf(ofp, "%[^|]|%[^|]|%*[^\n]", str2, newNode->password);
    fgetc(ofp);
    newNode->next = NULL;

    if(strcmp(str2, "admin") == 0)
    {
      strcpy(newNode->id.s, str2);
    }
    else
    {
      newNode->id.num = atoi(str2);
    }

    if(head == NULL)
    {
      head = newNode;
    }
    else
    {
      tail->next = newNode;
    }

    tail = newNode;

    if(strcmp(str1, "admin") == 0)
    {
      strcpy(login.id.s, str1);
      if(strcmp(login.id.s, "admin") == 0)
      {
        if(strcmp(login.password, newNode->password) == 0)
        {
          Admin_Login();
          fclose(ofp);
          return 0;
        }
      }
      else
      {
        printf("아이디 혹은 비밀번호가 잘못되었습니다.\n\n");
        return Login();
      }
    }
    else
    {
      login.id.num = atoi(str1);
      if (login.id.num == newNode->id.num)
      {
        if(strcmp(login.password, newNode->password) == 0)
        {
          Client_Login();
          fclose(ofp);
          return 0;
        }
        else
        {
          printf("아이디 혹은 비밀번호가 잘못되었습니다.\n\n");
          return Login();
        }
      }
    }
    if(feof(ofp))
    {
      printf("아이디 혹은 비밀번호가 잘못되었습니다.\n\n");
      return Login();
    }
  }
}

void Client_Login()
{
  printf("client ok\n");
}
void Admin_Login()
{
  printf("admin ok\n");
}
