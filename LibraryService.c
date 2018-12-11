#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

typedef struct client {
   char id[10];
   char password[20];
   char name[20];
   char address[100];
   char phonenumber[30];
   struct client * next;
}Client;

typedef struct book {
   long long isbn;
   char name[50];
   char publisher[50];
   char author[50];
   int booknum;
   char library[50];
   char yes_no;
   struct book * next;
}Book;

typedef struct borrow {
  char id[10];
  int booknum;
  time_t rent_day;
  time_t return_day;
  struct borrow * next;
}Borrow;

void PrintMenu(); // 초기 화면
void SignUp(); // 회원 가입
int Login(); // 로그인

void Client_Login(char *); // 회원 로그인
void My_Borrow_List(char *); // 내 대여 목록
void Edit(); // 개인 정보 수정
void pwdchange(Client *);
void adschange(Client *);
void pnbchange(Client *);
void DropOut(char *); // 회원 탈퇴
void DeleteInfo(Client *, char *);
int Check(char *);

void Admin_Login(); // 관리자 로그인
int Register(); // 도서 등록
void Delete(); // 도서 삭제
int NameDelete(); //도서삭제_도서제목으로 검색 후 삭제
int ISBNDelete(); //도서삭제-isbn으로 검색 후 삭제
void BorrowBook(); // 도서 대여
void Return(); // 도서 반납
void ClientPrintList(); // 회원 목록

void Search(); // 도서 검색
void PrintBookInfo(Book *);
void NameFind(); //책제목 검색
void PublisherFind(); //출판사 검색
void IsbnFind(); //isbn 검색
void AuthorFind(); //저자 검색
void BookPrintList(Book *); //전체 검색(출력)

void removeNode(Book *);
void removeEndNode(Book *);
void removeclientNode(Client *);
void removeclientEndNode(Client *);

int CountBookNum(Book *);
Book * BookRead();
Book * BookBubbleSort(Book *); // Book.txt isbn 순으로 정렬
Book * Bhead = NULL;

int CountClientNum(Client *);
Client * ClientBubbleSort(Client *);
Client * ClientRead();
Client * Chead = NULL;

Borrow * BorrowRead();
Borrow * BWhead = NULL;
int Rented();
int NameBorrow();
int ISBNBorrow();

int count = 1;
int count2 = 1;
char ID[10]; // 로그인 시 Client_Login가 전달받는 인자
char * day[] = {"일요일", "월요일", "화요일", "수요일", "목요일", "금요일", "토요일"};

int main()
{
   Chead = ClientRead();
   Bhead = BookRead();
   BWhead = BorrowRead();
   system("clear");

   while (1)
   {
      PrintMenu();
   }

   return 0;
}

void PrintMenu()
{
   char num;
   count = 1;

   system("clear");

   printf(">> 도서관 서비스 <<\n");
   printf("1.회원 가입\t2.로그인\t3.프로그램 종료\n\n");
   printf("번호를 선택하세요 : ");
   scanf("%s", &num);
   getchar();

   switch (num)
   {
   case '1':
      SignUp();
      break;
   case '2':
      Login();
      break;
   case '3':
      printf("\n");
      exit(1);
   default:
      printf("\n다시 입력해주세요.\n\n");
      sleep(2);
      break;
   }
}

void SignUp() // 회원 가입 함수
{
   FILE *ofp;
   Client info;

   ofp = fopen("client.txt", "a"); // client.txt 파일 끝에 이어쓰기, 파일이 없을 시 만들기

   printf("\n>> 회원 가입 <<\n");
   printf("학번, 비밀번호, 이름, 주소, 전화번호를 입력하세요.\n\n");

   printf("학번 : ");
   scanf("%[^\n]", info.id);
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

   fprintf(ofp, "%s|%s|%s|%s|%s\n", info.id, info.password, info.name, info.address, info.phonenumber);

   fclose(ofp);

   printf("\n회원가입이 되셨습니다.\n\n");
   sleep(2);
}

int Login() // 로그인 함수
{
   FILE * ofp;
   Client login;
   Client * head = NULL;
   Client * newNode = NULL;
   Client * tail = NULL;

   ofp = fopen("client.txt", "rt");

   printf("\n>> 로그인 <<\n");
   printf("학번 : ");
   scanf("%s", login.id);
   printf("비밀번호 : ");
   scanf("%s", login.password);

   while (!feof(ofp))
   {
      newNode = (Client *)malloc(sizeof(Client));
      fscanf(ofp, "%[^|]|%[^|]|%*[^\n]", newNode->id, newNode->password);
      fgetc(ofp);
      newNode->next = NULL;

      if (head == NULL)
      {
         head = newNode;
      }
      else
      {
         tail->next = newNode;
      }

      tail = newNode;

      if (strcmp(login.id, "admin") == 0) // 입력 받은 id가 admin 일 때
      {
         if (strcmp(login.password, newNode->password) == 0)
         {
            while (head->next != NULL)
            {
               free(head);
               head = head->next;
            }
            fclose(ofp);
            while (count)
            {
               Admin_Login();
            }
            return 0;
         }
      }

      if (strcmp(login.id, newNode->id) == 0) // 입력 받은 id가 newNode의 id와 일치할 때
      {
         if (strcmp(login.password, newNode->password) == 0)
         {

            strcpy(ID, newNode->id);

            while (head->next != NULL)
            {
               free(head);
               head = head->next;
            }
            fclose(ofp);
            while (count)
            {
               Client_Login(ID);
            }
            return 0;
         }
      }

      if (feof(ofp)) // 파일의 끝까지 찾았는데 존재하지 않은 id일 경우
      {
         printf("아이디 혹은 비밀번호가 잘못되었습니다.\n\n");
         return Login();
      }
   }
}

Book * BookRead() {
   FILE * ifp;
   Book * head = NULL;
   Book * newNode = NULL;
   Book * tail = NULL;

   ifp = fopen("book.txt", "rt");

   while (!feof(ifp))
   {
      newNode = (Book *)malloc(sizeof(Book));
      fscanf(ifp, "%lld|%[^|]|%[^|]|%[^|]|%d|%[^|]|%[^\n]", &(newNode->isbn), newNode->name, newNode->publisher, newNode->author, &(newNode->booknum), newNode->library, &(newNode->yes_no));
      fgetc(ifp);
      newNode->next = NULL;

      if (head == NULL)
      {
         head = newNode;
      }
      else
      {
         tail->next = newNode;
      }

      tail = newNode;
   }

   fclose(ifp);
   tail->next = NULL;

   head = BookBubbleSort(head);

   FILE * sfp = fopen("book.txt", "wt");
   Book * tmp = head;

   while (tmp->next != NULL)
   {
      fprintf(sfp, "%lld|%s|%s|%s|%d|%s|%c\n", tmp->isbn, tmp->name, tmp->publisher, tmp->author, tmp->booknum, tmp->library, tmp->yes_no);
      tmp = tmp->next;
   }

   fclose(sfp);
   return head; //Book.txt 정보 구조체 헤드 리턴
}

int CountBookNum(Book * head)
{
   Book * tmp = head;
   int num = 0;
   while (tmp->next != NULL)
   {
      num++;
      tmp = tmp->next;
   }
   return num;
}

Book * BookBubbleSort(Book * head)
{
   Book * p = head;
   Book * prev = head;
   Book * tmp = NULL;
   int total = CountBookNum(head);

   for (int i = 0; i < total; i++)
   {
      for (int j = 0; j < total - 1; j++)
      {
         if (p->isbn >(p->next)->isbn)
         {
            if (p == head)
            {
               head = p->next;
               tmp = head->next;
               head->next = p;
               p->next = tmp;
               p = head;
               prev = head;
            }
            else
            {
               tmp = p->next;
               p->next = tmp->next;
               prev->next = tmp;
               tmp->next = p;
               p = tmp;
            }
         }
         prev = p;
         p = p->next;
      }
      p = head;
      prev = p;
   }
   return head;
}

int CountClientNum(Client * head)
{
   Client * tmp = head;
   int num = 0;
   while (tmp->next != NULL)
   {
      num++;
      tmp = tmp->next;
   }
   return num;
}

Client * ClientBubbleSort(Client * head)
{
   Client * p = head;
   Client * prev = head;
   Client * tmp = NULL;
   int total = CountClientNum(head);

   for (int i = 0; i < total; i++)
   {
      for (int j = 0; j < total - 1; j++)
      {
         if (strcmp(p->id, (p->next)->id) > 0)
         {
            if (p == head)
            {
               head = p->next;
               tmp = head->next;
               head->next = p;
               p->next = tmp;
               p = head;
               prev = head;
            }
            else
            {
               tmp = p->next;
               p->next = tmp->next;
               prev->next = tmp;
               tmp->next = p;
               p = tmp;
            }
         }
         prev = p;
         p = p->next;
      }
      p = head;
      prev = p;
   }
   return head;
}

Client * ClientRead()
{
   FILE * ifp;
   Client * head = NULL;
   Client * newNode = NULL;
   Client * tail = NULL;

   ifp = fopen("client.txt", "rt");

   while (!feof(ifp))
   {
      newNode = (Client *)malloc(sizeof(Client));
      fscanf(ifp, "%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]", newNode->id, newNode->password, newNode->name, newNode->address, newNode->phonenumber);
      fgetc(ifp);
      newNode->next = NULL;

      if (head == NULL)
      {
         head = newNode;
      }
      else
      {
         tail->next = newNode;
      }

      tail = newNode;
   }

   fclose(ifp);
   tail->next = NULL;

   head = ClientBubbleSort(head);

   FILE * sfp = fopen("client.txt", "wt");
   Client * tmp = head;

   while (tmp->next != NULL)
   {
      fprintf(sfp, "%s|%s|%s|%s|%s\n", tmp->id, tmp->password, tmp->name, tmp->address, tmp->phonenumber);
      tmp = tmp->next;
   }

   fclose(sfp);
   return head;
}

Borrow * BorrowRead()
{
   FILE * ifp;
   Borrow * head = NULL;
   Borrow * newNode = NULL;
   Borrow * tail = NULL;

   ifp = fopen("borrow.txt", "rt");

   while (!feof(ifp))
   {
      newNode = (Borrow *)malloc(sizeof(Borrow));
      fscanf(ifp, "%[^|]|%d|%u|%u\n", newNode->id, &(newNode->booknum), &(newNode->rent_day), &(newNode->return_day));
      newNode->next = NULL;

      if (head == NULL)
      {
         head = newNode;
      }
      else
      {
         tail->next = newNode;
      }

      tail = newNode;
   }
   tail->next = NULL;

   fclose(ifp);
   return head;
}

void Client_Login(char *id)
{
   system("clear");

   char choice;

   //printf("%s\n\n", id); // for testing -> erase it
   printf(">> 회원 메뉴 <<\n");
   printf("1. 도서 검색\t2. 내 대여 목록\n3. 개인정보 수정\t4. 회원 탈퇴\n5. 로그아웃\t6. 프로그램 종료\n\n");
   printf("번호를 선택하세요 : ");
   scanf("%s", &choice);
   getchar();

   switch (choice)
   {
   case '1':
      Search();
      break;
   case '2':
      My_Borrow_List(id);
      break;
   case '3':
      Edit();
      break;
   case '4':
      DropOut(id);
      break;
   case '5':
      count = 0;
      break;
   case '6':
      printf("\n");
      exit(1);
   default:
      printf("\n다시 입력해주세요.\n\n");
      sleep(2);
      break;
   }
}

void My_Borrow_List(char * id)
{
   Borrow * borrowhead = BorrowRead();
   Book * bookhead = BookRead();
   Borrow * tmp = borrowhead;
   Book * tmp2 = bookhead;
   struct tm * t1;

   printf("\n>>내 대여목록<<\n\n");

   while (tmp->next != NULL) // borrow.txt에서 내 학번있는지 반복
   {
      if (strstr(tmp->id, id) != NULL)
      {
         while (tmp2->next != NULL) // book.txt에서 내가 빌린 책 출력
         {
            if (tmp->booknum == tmp2->booknum)
            {
               printf("도서번호 : %d\n도서명 : %s\n", tmp2->booknum, tmp2->name);
               t1 = localtime(&(tmp->rent_day));
               printf("대여일자 : %d년 %d월 %d일 %s\n", (*t1).tm_year + 1900, (*t1).tm_mon + 1, (*t1).tm_mday, day[(*t1).tm_wday]);
               t1 = localtime(&(tmp->return_day));
               printf("반납일자 : %d년 %d월 %d일 %s\n\n", (*t1).tm_year + 1900, (*t1).tm_mon + 1, (*t1).tm_mday, day[(*t1).tm_wday]);
            }
            tmp2 = tmp2->next;
         }
         tmp2 = bookhead;
      }
      tmp = tmp->next;
   }

   sleep(2);
}

void pwdchange(Client * head)
{
   Client * tmp = head;
   char changepwd[20];
   printf("수정할 비밀번호를 입력하세요 : ");
   gets(changepwd);

   while (tmp->next != NULL)
   {
      if (strstr(tmp->id, ID) != NULL)
      {
         strncpy(tmp->password, changepwd, strlen(changepwd) + 1);
         printf("\n수정완료\n\n");
         break;
      }
      tmp = tmp->next;
   }

   FILE * ofp;
   ofp = fopen("client.txt", "w");
   tmp = head;

   while (tmp->next != NULL)
   {
      fprintf(ofp, "%s|%s|%s|%s|%s\n", tmp->id, tmp->password, tmp->name, tmp->address, tmp->phonenumber);
      tmp = tmp->next;
   }
   free(tmp);
   fclose(ofp);
}

void adschange(Client * head)
{
   Client * tmp = head;
   char changeads[100];
   printf("수정할 주소를 입력하세요 : ");
   gets(changeads);

   while (tmp->next != NULL)
   {
      if (strstr(tmp->id, ID) != NULL)
      {
         strncpy(tmp->address, changeads, strlen(changeads) + 1);
         printf("\n수정완료\n\n");
         break;
      }
      tmp = tmp->next;
   }

   FILE * ofp;
   ofp = fopen("client.txt", "w");
   tmp = head;

   while (tmp->next != NULL)
   {
      fprintf(ofp, "%s|%s|%s|%s|%s\n", tmp->id, tmp->password, tmp->name, tmp->address, tmp->phonenumber);
      tmp = tmp->next;
   }

   fclose(ofp);
}

void pnbchange(Client * head)
{
   Client * tmp = head;
   char changepnb[30];
   printf("수정할 전화번호를 입력하세요 : ");
   gets(changepnb);

   while (tmp->next != NULL)
   {
      if (strstr(tmp->id, ID) != NULL)
      {
         strncpy(tmp->phonenumber, changepnb, strlen(changepnb) + 1);
         printf("\n수정완료\n\n");
         break;
      }
      tmp = tmp->next;
   }

   FILE * ofp;
   ofp = fopen("client.txt", "w");
   tmp = head;

   while (tmp->next != NULL)
   {
      fprintf(ofp, "%s|%s|%s|%s|%s\n", tmp->id, tmp->password, tmp->name, tmp->address, tmp->phonenumber);
      tmp = tmp->next;
   }

   fclose(ofp);
}

void Edit()
{
   char choice;
   FILE * ifp;
   Client * head = NULL;
   Client * newNode = NULL;
   Client * tail = NULL;

   ifp = fopen("client.txt", "rt");

   while (!feof(ifp))
   {
      newNode = (Client *)malloc(sizeof(Client));
      fscanf(ifp, "%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]", newNode->id, newNode->password, newNode->name, newNode->address, newNode->phonenumber);
      fgetc(ifp);
      newNode->next = NULL;

      if (head == NULL)
      {
         head = newNode;
      }
      else
      {
         tail->next = newNode;
      }

      tail = newNode;
   }

   fclose(ifp);
   tail->next = NULL;

   head = ClientBubbleSort(head);
   printf("어떤 정보를 변경하시겠습니까?\n");
   printf("1. 비밀번호\t2. 주소\n3. 전화번호\n");
   scanf("%s", &choice);
   getchar();

   switch (choice)
   {
   case'1':
      pwdchange(head);
      break;

   case '2':
      adschange(head);
      break;

   case '3':
      pnbchange(head);
      break;
   }

   free(head);

   sleep(2);
}

int check(char * id)
{
   Borrow * head = BorrowRead();
   int i = 0;
   Borrow * tmp = head;

   while (tmp->next != NULL)
   {
      if (strstr(tmp->id, id) != NULL)
      {
         i = 1;
         break;
      }
      tmp = tmp->next;
   }
   return i;
}

void DeleteInfo(Client * head, char * id)
{
   Client * tmp2 = NULL;
   tmp2 = (Client *)malloc(sizeof(Client));
   tmp2->next = head;

   int i = 0;
   FILE * ofp;
   ofp = fopen("client.txt", "w");

   while (tmp2->next != NULL)
   {
      if (strstr(tmp2->next->id, id) != NULL) //입력한 학번과 해당 학번이 같다면
      {
         if (tmp2->next->next == NULL)
         {
            removeclientEndNode(tmp2);
            printf("탈퇴가 완료되었습니다.\n");
            fprintf(ofp, "%s|%s|%s|%s|%s\n", tmp2->id, tmp2->password, tmp2->name, tmp2->address, tmp2->phonenumber);
            break;
         }
         else
         {
            removeclientNode(tmp2);
            printf("탈퇴가 완료되었습니다.\n");
         }
      }
      if (i != 0)
      {
         fprintf(ofp, "%s|%s|%s|%s|%s\n", tmp2->id, tmp2->password, tmp2->name, tmp2->address, tmp2->phonenumber);
      }
      tmp2 = tmp2->next;
      i += 1;
   }
   fclose(ofp);
}

void DropOut(char * id)
{
   Client * head = ClientRead();

   int checkpoint = check(id);

   char choice;

   printf("탈퇴하시겠습니까? : ");
   scanf("%c", &choice);

   if (choice == 'Y')
   {
      if (checkpoint == 0)
      {
         DeleteInfo(head, id);
         sleep(2);
      }
      if (checkpoint != 0)
      {
         printf("\n대여한 도서가 있어 탈퇴를 할 수 없습니다.\n");
         sleep(2);
      }
   }
   else if (choice == 'N')
   {
      printf("탈퇴를 취소하였습니다.\n");
      sleep(2);
   }
}

void Admin_Login()
{
   system("clear");
   char choice;

   printf(">> 관리자 메뉴 <<\n");
   printf("1. 도서 등록\t2. 도서 삭제\n3. 도서 대여\t4. 도서 반납\n5. 도서 검색\t6. 회원 목록\n7. 로그아웃\t8. 프로그램 종료\n\n");
   printf("번호를 선택하세요 : ");
   scanf("%s", &choice);
   getchar();

   switch (choice)
   {
   case '1':
      Register();
      break;
   case '2':
      Delete();
      break;
   case '3':
      BorrowBook();
      break;
   case '4':
      Return();
      break;
   case '5':
      Search();
      break;
   case '6':
      Chead = ClientRead();
      ClientPrintList(Chead);
      break;
   case '7':
      count = 0;
      break;
   case '8':
      printf("\n");
      exit(1);
   default:
      printf("\n다시 입력해주세요.\n\n");
      sleep(2);
      break;
   }
}

int Register()
{
   Book * head = BookRead();
   int bookcount = CountBookNum(head) + 1000001;
   char select;
   Book * NewBook;
   NewBook = (Book *)malloc(sizeof(Book));
   NewBook->next = NULL;
   FILE * ofp;
   ofp = fopen("book.txt", "a");

   printf(">>도서 등록<<\n");
   printf("도서명 : ");
   scanf("%s", NewBook->name);
   getchar();
   printf("출판사 : ");
   scanf("%s", NewBook->publisher);
   getchar();
   printf("저자명 : ");
   scanf("%s", NewBook->author);
   getchar();
   printf("ISBN : ");
   scanf("%lld", &(NewBook->isbn));
   getchar();
   printf("소장처 : ");
   scanf("%s", NewBook->library);
   getchar();

   printf("\n자동입력 사항\n");

   printf("대여가능 여부 : Y\n");
   NewBook->yes_no = 'Y';
   printf("도서 번호 : %d\n", bookcount);
   NewBook->booknum = bookcount;

   printf("등록하시겠습니까? : \n");
   scanf("%c", &(select));

   if (select == 'Y') {
      fprintf(ofp, "%lld|%s|%s|%s|%d|%s|%c\n", NewBook->isbn, NewBook->name, NewBook->publisher, NewBook->author, NewBook->booknum, NewBook->library, NewBook->yes_no);
   }
   else {
      printf("이전메뉴로 돌아갑니다.");
      return 0;
   }

   fclose(ofp);
   free(NewBook);
   printf("도서등록이 완료되었습니다.");
   return 0;
}

void removeNode(Book * node)
{
   Book *removeNode = node->next;
   node->next = removeNode->next;

   free(removeNode);
}

void removeEndNode(Book * node)
{
   Book * removeNode = node->next;
   node->next = NULL;

   free(removeNode);
}

void removeclientNode(Client * node)
{
   Client * removeNode = node->next;
   node->next = removeNode->next;

   free(removeNode);
}

void removeclientEndNode(Client * node)
{
   Client * removeNode = node->next;
   node->next = NULL;

   free(removeNode);
}

int NameDelete()
{
   char search[50];
   Book * tmp = (Book *)malloc(sizeof(Book));
   tmp->next = Bhead;
   char * ptr = NULL;

   printf("\n도서명을 입력하세요 : ");
   gets(search);

   printf("\n\n>>검색 결과<<\n");

   tmp = tmp->next;

   while (tmp->next != NULL)
   {
      if (strstr(tmp->name, search) != NULL) // 책이름 찾으면
      {
         ptr = strstr(tmp->name, search);
         printf("도서번호 : ");
         while (1)
         {
            printf("%d(삭제 가능 여부 : %c) ", tmp->booknum, tmp->yes_no);
            if (tmp->isbn != (tmp->next)->isbn)
            {
               printf("\n");
               PrintBookInfo(tmp);
               break;
            }
            tmp = tmp->next;
         }
      }
      tmp = tmp->next;
   }

   if (ptr == NULL)
   {
      printf("존재하지 않는 도서입니다.\n");
      printf("다시 입력해주세요.\n\n");

      return count2;
   }

   int search2;
   Book * tmp2 = NULL; // tmp2는 삭제를 위하여 삭제하기위한 노드의 이전노드가 된다.
   tmp2 = (Book *)malloc(sizeof(Book));
   tmp2->next = Bhead;
   int ptr2 = 0;
   char choice;
   int i = 0;

   printf("삭제할 도서의 번호를 입력하세요 : ");
   scanf("%d", &search2);

   FILE * ofp;
   ofp = fopen("book.txt", "w");

   while (tmp2->next != NULL)
   {
      if ((tmp2->next->booknum == search2) != 0) //입력한 도서번호와 삭제할 도서번호가 같다면
      {
         ptr2 = (tmp2->next->booknum == search2);
         if (tmp2->next->yes_no == 'Y') {
            if (tmp2->next->next != NULL) { //처음, 중간 노드 삭제할 경우
               removeNode(tmp2);
               printf("삭제가 완료되었습니다.\n");
            }
            else { //마지막 노드를 삭제할 경우
               removeEndNode(tmp2);
               fprintf(ofp, "%lld|%s|%s|%s|%d|%s|%c\n", tmp2->isbn, tmp2->name, tmp2->publisher, tmp2->author, tmp2->booknum, tmp2->library, tmp2->yes_no);
               printf("삭제가 완료되었습니다.\n");
               break;
            }
         }
         else if (tmp2->next->yes_no == 'N') {
            printf("이 도서는 삭제할 수 없습니다.\n");
         }
      }
      if (i != 0) {
         fprintf(ofp, "%lld|%s|%s|%s|%d|%s|%c\n", tmp2->isbn, tmp2->name, tmp2->publisher, tmp2->author, tmp2->booknum, tmp2->library, tmp2->yes_no);
      }
      i += 1;
      tmp2 = tmp2->next;
   }
   fclose(ofp);

   if (ptr2 == 0)
   {
      printf("존재하지 않는 도서입니다.\n\n");
   }

   free(tmp2);

   count2 = 0;

   return 0;
}

int ISBNDelete()
{
   long long search;
   Book * tmp = (Book *)malloc(sizeof(Book));
   tmp->next = Bhead;
   int ptr = 0;

   printf("\nISBN을 입력하세요 : ");
   scanf("%lld", &search);

   printf("\n\n>>검색 결과<<\n");

   tmp = tmp->next;

   while (tmp->next != NULL)
   {
      if ((tmp->isbn == search) != 0) // 책이름 찾으면
      {
         ptr = (tmp->isbn == search);
         printf("도서번호 : ");
         while (1)
         {
            printf("%d(삭제 가능 여부 : %c) ", tmp->booknum, tmp->yes_no);
            if (tmp->isbn != (tmp->next)->isbn)
            {
               printf("\n");
               PrintBookInfo(tmp);
               break;
            }
            tmp = tmp->next;
         }
      }
      tmp = tmp->next;
   }

   if (ptr == 0)
   {
      printf("존재하지 않는 도서입니다.\n");
      printf("다시 입력해주세요.\n\n");

      return 0;
   }

   int search2;
   Book * tmp2 = NULL; // tmp2는 삭제를 위하여 삭제하기위한 노드의 이전노드가 된다.
   tmp2 = (Book *)malloc(sizeof(Book));
   tmp2->next = Bhead;
   int ptr2 = 0;
   char choice;
   int i = 0;

   printf("삭제할 도서의 번호를 입력하세요 : ");
   scanf("%d", &search2);

   FILE * ofp;
   ofp = fopen("book.txt", "w");

   while (tmp2->next != NULL)
   {
      if ((tmp2->next->booknum == search2) != 0) //입력한 도서번호와 삭제할 도서번호가 같다면
      {
         ptr2 = (tmp2->next->booknum == search2);
         if (tmp2->next->yes_no == 'Y') {
            if (tmp2->next->next != NULL) { //처음, 중간 노드 삭제할 경우
               removeNode(tmp2);
               printf("삭제가 완료되었습니다.\n");
            }
            else { //마지막 노드를 삭제할 경우
               removeEndNode(tmp2);
               fprintf(ofp, "%lld|%s|%s|%s|%d|%s|%c\n", tmp2->isbn, tmp2->name, tmp2->publisher, tmp2->author, tmp2->booknum, tmp2->library, tmp2->yes_no);
               printf("삭제가 완료되었습니다.\n");
               break;
            }
         }
         else if (tmp2->next->yes_no == 'N') {
            printf("이 도서는 삭제할 수 없습니다.\n");
         }
      }
      if (i != 0) {
         fprintf(ofp, "%lld|%s|%s|%s|%d|%s|%c\n", tmp2->isbn, tmp2->name, tmp2->publisher, tmp2->author, tmp2->booknum, tmp2->library, tmp2->yes_no);
      }
      i += 1;
      tmp2 = tmp2->next;
   }
   fclose(ofp);

   if (ptr2 == 0)
   {
      printf("존재하지 않는 도서입니다.\n\n");
   }

   free(tmp2);

   count2 = 0;

   return 0;
}

void Delete() // 도서 삭제
{
   Bhead = BookRead();
   char choice;
   int deletenum;

   printf(">>도서 삭제<<\n");
   printf("1.도서명 검색    2.ISBN 검색\n\n");
   printf("검색 번호를 입력하세요 : ");
   scanf("%s", &choice);
   getchar();

   switch (choice)
   {
   case '1':
      while (count2)
      {
         NameDelete();
      }
      break;

   case '2':
      while (count2)
      {
         ISBNDelete();
      }
      break;
   default:
      printf("다시 입력해주세요.\n");
      break;
   }
}

int Rented()
{
	char * data = (char *)malloc(sizeof(char) * 20);
	char * id = (char *)malloc(sizeof(char) * 10);
  char * answer = (char *)malloc(sizeof(char));
	int search;
	int ptr2 = 0;

	Book * tmp = (Book *)malloc(sizeof(Book));
	tmp->next = Bhead;
	Client * tmp2 = Chead;

	time_t now = time(NULL);
  struct tm * t = localtime(&now);

	FILE * cfp = fopen("client.txt", "r");

  printf("학번을 입력하세요 : ");
	gets(id);

	while(!feof(cfp))
	{
		fscanf(cfp, "%[^|]%*[^\n]\n", data);
		if(strcmp(data, id) == 0)
		{
			break;
		}
		if(feof(cfp))
		{
			printf("\n학번이 존재하지 않습니다.\n\n");
			free(data);
			fclose(cfp);
			return 0;
		}
	}

  printf("\n도서번호를 입력하세요 : ");
  scanf("%d", &search);
	getchar();

  printf("\n이 도서를 대여합니까? (Y / N) ");
  scanf("%s", &answer);
  getchar();

  if(*answer == 'N')
  {
		free(answer);
    return 0;
  }

	FILE * bkfp = fopen("book.txt", "w");
	FILE * bofp = fopen("borrow.txt", "a");

  while(tmp->next != NULL)
  {
    if((tmp->next)->booknum == search)
    {
      ptr2 = ((tmp->next)->booknum == search);
      if((tmp->next)->yes_no == 'Y')
      {
        (tmp->next)->yes_no = 'N';
				if( (*t).tm_wday == 5 )
				{
					fprintf(bofp, "%s|%d|%u|%u\n", id, search, now, now + (24 * 60 * 60 * 31));
				}
				else
				{
					fprintf(bofp, "%s|%d|%u|%u\n", id, search, now, now + (24 * 60 * 60 * 30));
				}
        printf("\n\n도서가 대여되었습니다.\n\n");
      }
      else
      {
        printf("\n\n이 도서는 대여할 수 없습니다.\n\n");
      }
    }
    tmp = tmp->next;
  }

  if(ptr2 == 0)
  {
    printf("\n\n존재하지 않는 도서입니다.\n\n");
  }

	tmp = Bhead;

	while(tmp->next != NULL)
	{
		fprintf(bkfp, "%lld|%s|%s|%s|%d|%s|%c\n", tmp->isbn, tmp->name, tmp->publisher, tmp->author, tmp->booknum, tmp->library, tmp->yes_no);
		tmp = tmp->next;
	}

	free(id);
	free(tmp);
  fclose(bkfp);
  fclose(bofp);

  return 0;
}

int NameBorrow()
{
  char search[50];
  Book * tmp = (Book *)malloc(sizeof(Book));
	tmp->next = Bhead;
  char * ptr = NULL;

  printf("\n도서명을 입력하세요 : ");
  gets(search);

  printf("\n\n>>검색 결과<<\n\n");

  tmp = tmp->next;

  while (tmp->next != NULL)
  {
    if (strstr(tmp->name, search) != NULL) // 책이름 찾으면
    {
       ptr = strstr(tmp->name, search);
       printf("도서번호 : ");
       while (1)
       {
          printf("%d(대여 가능 여부 : %c) ", tmp->booknum, tmp->yes_no);
          if (tmp->isbn != (tmp->next)->isbn)
          {
             printf("\n");
             PrintBookInfo(tmp);
             break;
          }
          tmp = tmp->next;
       }
    }
    tmp = tmp->next;
  }

  if (ptr == NULL)
  {
    printf("존재하지 않는 도서입니다.\n\n");
    printf("이전 메뉴로 돌아갑니다.");
    return 0;
  }

  Rented();
	return 0;
}

 int ISBNBorrow() // ISBN ㄱ
 {
    long long search;
    Book * tmp = (Book*)malloc(sizeof(Book));
		tmp->next = Bhead;
    int ptr = 0;

    printf("\nISBN을 입력하세요 : ");
    scanf("%lld", &search);
		getchar();

    printf("\n\n>>검색 결과<<\n");

    tmp = tmp->next;

    while (tmp->next != NULL)
    {
       if ((tmp->isbn == search) != 0) // 책이름 찾으면
       {
          ptr = (tmp->isbn == search);
          printf("도서번호 : ");
          while (1)
          {
             printf("%d(대여 가능 여부 : %c) ", tmp->booknum, tmp->yes_no);
             if (tmp->isbn != (tmp->next)->isbn)
             {
                printf("\n");
								PrintBookInfo(tmp);
                break;
             }
             tmp = tmp->next;
          }
       }
       tmp = tmp->next;
    }

    if (ptr == 0)
    {
       printf("존재하지 않는 도서입니다.\n");
       printf("다시 입력해주세요.\n\n");
       return 0;
    }

    Rented();
		return 0;
}

void BorrowBook()
{
   Bhead = BookRead();
   Chead = ClientRead();
   char choice;

   printf("\n\n>> 도서 대여 <<\n\n");
   printf("1. 도서명 검색\t2. ISBN 검색\n\n");

   printf("검색 번호를 입력하세요 : ");
   scanf("%s", &choice);
   getchar();

   switch(choice)
   {
     case '1' :
        NameBorrow();
        break;
     case '2' :
        ISBNBorrow();
        break;
   }
}

void Return()
{
   printf("Return\n\n");
   sleep(2);
}

void ClientPrintList(Client * head)
{
   system("clear");

   int i = 1;
   Client * tmp = head;
   printf("\n");
   while (tmp->next != NULL)
   {
      printf("%d. %s|%s|%s|%s|%s\n", i++, tmp->id, tmp->password, tmp->name, tmp->address, tmp->phonenumber);
      tmp = tmp->next;
   }
   sleep(10);
}

void Search()
{
   Bhead = BookRead();
   char choice;

   while (1)
   {
      printf("\n>> 도서 검색 <<\n");
      printf("1. 도서명 검색\t2. 출판사 검색\n3. ISBN 검색\t4. 저자명 검색\n5.전체 검색\t6. 이전 메뉴\n\n");

      printf("번호를 입력하세요 : ");
      scanf("%s", &choice);
      getchar();

      switch (choice)
      {
      case '1':
         NameFind();
         continue;
      case '2':
         PublisherFind();
         continue;
      case '3':
         IsbnFind();
         continue;
      case '4':
         AuthorFind();
         continue;
      case '5':
         BookPrintList(Bhead);
         continue;
      case '6':
         break;
      default:
         printf("\n다시 입력해주세요.\n\n");
         sleep(2);
         continue;
      }
      break;
   }
}

void PrintBookInfo(Book * tmp)
{
   printf("도서명 : %s\n", tmp->name);
   printf("출판사 : %s\n", tmp->publisher);
   printf("저자명 : %s\n", tmp->author);
   printf("ISBN : %lld\n", tmp->isbn);
   printf("소장처 : %s\n\n", tmp->library);
}

void NameFind()
{
   char search[50];
   int booknum;
   int yes;
   Book * tmp = Bhead;
   char * ptr = NULL;

   printf("\n도서명을 입력하세요 : ");
   gets(search);

   while (tmp->next != NULL)
   {
      booknum = 0;
      yes = 0;
      if (strstr(tmp->name, search) != NULL)
      {
         booknum++;

         ptr = strstr(tmp->name, search);
         printf("\n>> 검색 결과 <<\n");
         PrintBookInfo(tmp);
         while (1)
         {
            if (tmp->yes_no == 'Y') {
               yes++;
            }

            if (tmp->isbn == tmp->next->isbn)
            {
               booknum++;
               tmp = tmp->next;
               continue;
            }

            break;
         }
         if (yes > 0)
         {
            printf("대여가능 여부 : Y (%d/%d)\n\n", yes, booknum);
         }
         else
         {
            printf("대여가능 여부 : N (%d/%d)\n\n", yes, booknum);
         }
      }
      tmp = tmp->next;
   }

   if (ptr == NULL)
   {
      printf("존재하지 않는 도서입니다.\n\n");
   }
}

void PublisherFind()
{
   char search[50];
   int booknum;
   int yes;
   Book * tmp = Bhead;
   char * ptr = NULL;

   printf("\n출판사를 입력하세요 : ");
   gets(search);

   while (tmp->next != NULL)
   {
      booknum = 0;
      yes = 0;
      if (strstr(tmp->publisher, search) != NULL)
      {
         booknum++;
         ptr = strstr(tmp->publisher, search);
         printf("\n>> 검색 결과 <<\n");
         PrintBookInfo(tmp);

         while (1)
         {
            if (tmp->yes_no == 'Y')
            {
               yes++;
            }
            if (tmp->isbn == (tmp->next)->isbn)
            {
               booknum++;
               tmp = tmp->next;
               continue;
            }
            break;
         }

         if (yes > 0)
         {
            printf("대여가능 여부 : Y (%d/%d)\n\n", yes, booknum);
         }
         else
         {
            printf("대여가능 여부 : N (%d/%d)\n\n", yes, booknum);
         }
      }
      tmp = tmp->next;
   }
   if (ptr == NULL)
   {
      printf("존재하지 않는 도서입니다.\n\n");
   }
}

void IsbnFind() // long long 형으로 바꾸자
{
   long long search;
   int booknum;
   int yes;
   Book * tmp = Bhead;
   long long * ptr = NULL;

   printf("\nISBN을 입력하세요 : ");
   scanf("%lld", &search);
   getchar();

   while (tmp->next != NULL)
   {
      booknum = 0;
      yes = 0;
      if (tmp->isbn == search)
      {
         booknum++;
         ptr = &(tmp->isbn);
         printf("\n>> 검색 결과 <<\n");
         PrintBookInfo(tmp);

         while (1)
         {
            if (tmp->yes_no == 'Y')
            {
               yes++;
            }
            if (tmp->isbn == (tmp->next)->isbn)
            {
               booknum++;
               tmp = tmp->next;
               continue;
            }
            break;
         }

         if (yes > 0)
         {
            printf("대여가능 여부 : Y (%d/%d)\n\n", yes, booknum);
         }
         else
         {
            printf("대여가능 여부 : N (%d/%d)\n\n", yes, booknum);
         }
      }
      tmp = tmp->next;
   }
   if (ptr == NULL)
   {
      printf("존재하지 않는 도서입니다.\n\n");
   }
}

void AuthorFind()
{
   char search[50];
   int booknum;
   int yes;
   Book * tmp = Bhead;
   char * ptr = NULL;

   printf("\n저자명을 입력하세요 : ");
   gets(search);

   while (tmp->next != NULL)
   {
      booknum = 0;
      yes = 0;
      if (strstr(tmp->author, search) != NULL)
      {
         booknum++;
         ptr = strstr(tmp->author, search);
         printf("\n>> 검색 결과 <<\n");
         PrintBookInfo(tmp);

         while (1)
         {
            if (tmp->yes_no == 'Y')
            {
               yes++;
            }
            if (tmp->isbn == (tmp->next)->isbn)
            {
               booknum++;
               tmp = tmp->next;
               continue;
            }
            break;
         }

         if (yes > 0)
         {
            printf("대여가능 여부 : Y (%d/%d)\n\n", yes, booknum);
         }
         else
         {
            printf("대여가능 여부 : N (%d/%d)\n\n", yes, booknum);
         }
      }
      tmp = tmp->next;
   }

   if (ptr == NULL)
   {
      printf("존재하지 않는 도서입니다.\n\n");
   }
}

void BookPrintList(Book * head)
{
   int i = 1; // 확인용 나중에 지워야 함
   Book * tmp = head;
   printf("\n");
   while (tmp->next != NULL)
   {
      printf("%d. %lld|%s|%s|%s|%d|%s|%c\n", i++, tmp->isbn, tmp->name, tmp->publisher, tmp->author, tmp->booknum, tmp->library, tmp->yes_no);
      tmp = tmp->next;
   }
}
