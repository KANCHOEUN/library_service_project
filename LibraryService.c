#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void PrintMenu();
void SignUp();
int Login();
void Client_Login();
void My_Borrow_List();
void Edit();
void DropOut();

void Admin_Login();
void Register();
void Delete();
void Borrow();
void Return();
void List();

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

void Search();
void NameFind(Book *); //책제목 검색
void PublisherFind(Book *); //출판사 검색
void IsbnFind(Book *); //isbn 검색
void AuthorFind(Book *); //저자 검색
void BookPrintList(Book *); //전체 검색(출력)

Book * BookRead();
Book * swap(Book *, Book *);
Book * Bookbubblesort(Book *);

int count = 1;

int main()
{
	system("clear");
	// client.txt 정렬 함수 따로 만들어서 넣기

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

int Login()
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

		if (strcmp(login.id, "admin") == 0)
		{
			if (strcmp(login.password, newNode->password) == 0)
			{
				while (count)
				{
					Admin_Login();
				}
				fclose(ofp);
				return 0;
			}
		}
		else
		{
			printf("아이디 혹은 비밀번호가 잘못되었습니다.\n\n");
			return Login();
		}

		if (strcmp(login.id, newNode->id) == 0)
		{
			if (strcmp(login.password, newNode->password) == 0)
			{
				while (count)
				{
					Client_Login();
				}
				fclose(ofp);
				return 0;
			}
			else
			{
				printf("아이디 혹은 비밀번호가 잘못되었습니다.\n\n");
				return Login();
			}
		}
		if (feof(ofp))
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

	head = Bookbubblesort(head);

	return head; //Book.txt 정보 구조체 헤드 리턴
}

Book * swap(Book * prev, Book * subs) {
	prev->next = subs->next;
	subs->next = prev;
	return subs;
}

Book * Bookbubblesort(Book * head)
{
	Book *p, *q, *top;
	int changed = 1;

	top = (Book *)malloc(sizeof(Book));

	top->next = head;

	if (head != NULL && head->next != NULL) {
		while (changed) {
			changed = 0;
			q = top;
			p = top->next;
			while (p->next != NULL) {
				if (p->isbn > p->next->isbn) {
					q->next = swap(p, p->next);
					changed = 1;
				}
				q = p;
				if (p->next != NULL)
					p = p->next;
			}
		}
	}
	p = top->next;
	free(top);
	return p;
}

void Client_Login()
{
	system("clear");

	char choice;

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
		My_Borrow_List();
		break;
	case '3':
		Edit();
		break;
	case '4':
		DropOut();
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

void My_Borrow_List()
{
	printf("My_Borrow_List\n\n");
	sleep(2);
}

void Edit()
{
	printf("Edit\n\n");
	sleep(2);
}

void DropOut()
{
	printf("DropOut\n\n");
	sleep(2);
}

void Admin_Login()
{
	system("clear");

	char choice;

	printf(">> 관리자 메뉴 <<\n");
	printf("1. 도서 등록\t2. 도서 삭제\n3. 도서 대여\t4. 도서 반납\n5. 도서 검색\t6. 회원 목록\n7. 로그아웃\t8.프로그램 종료\n\n");
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
		Borrow();
		break;
	case '4':
		Return();
		break;
	case '5':
		Search();
		break;
	case '6':
		List();
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

void Register()
{
	printf("Register\n\n");
	sleep(2);
}

void Delete()
{
	printf("Delete\n\n");
	sleep(2);
}

void Borrow()
{
	printf("Borrow\n\n");
	sleep(2);
}

void Return()
{
	printf("Return\n\n");
	sleep(2);
}

void List()
{
	printf("List\n\n");
	sleep(2);
}

void Search()
{
	 Book * head;
	 head = BookRead();

   char choice;

   while(1)
   {
     printf(">> 도서 검색 <<\n");
     printf("1. 도서명 검색\t2. 출판사 검색\n3. ISBN 검색\t4. 저자명 검색\n5.전체 검색\t6. 이전 메뉴\n\n");

     printf("번호를 입력하세요 : ");
     scanf("%s", &choice);
     getchar();

     switch(choice)
     {
       case 1 :
          NameFind(head);
          continue;
       case 2 :
          PublisherFind(head);
          continue;
       case 3 :
          IsbnFind(head);
          continue;
       case 4 :
          AuthorFind(head);
          continue;
       case 5 :
          BookPrintList(head);
          continue;
       case 6 :
          break;
       default :
          printf("\n다시 입력해주세요.\n\n");
          sleep(2);
          continue;
     }
     break;
   }
}

void NameFind(Book * head) {
   char search[50];
   // int num = 0;
   Book * tmp = head;
   printf("도서명을 입력하세요 : ");
   gets(search);
   while (1) {
      if (strstr(tmp->name, search) != NULL) {
         //printf("%lld|%s|%s|%s|%d|%s|%c\n", tmp->isbn, tmp->name, tmp->publisher, tmp->author, tmp->booknum, tmp->library, tmp->yes_no);
         printf(">> 검색 결과 <<\n");
         printf("도서명 : %s\n", tmp->name);
         printf("출판사 : %s\n", tmp->publisher);
         printf("저자명 : %s\n", tmp->author);
         printf("ISBN : %lld\n", tmp->isbn);
         printf("소장처 : %s\n\n", tmp->library);
         // 여기다가 대여 가능 목록 쓸 것
         break;
      }

      if (tmp->next == NULL)
      {
        printf("존재하지 않는 도서입니다.\n\n");
        break;
      }
      else
      {
        tmp = tmp -> next;
      }
   }
}

void PublisherFind(Book * head)
{
	char search[50];
	// int num = 0;
	Book * tmp = head;
	printf("출판사를 입력하세요 : ");
	gets(search);
	while (1) {
		 if (strstr(tmp->publisher, search) != NULL) {
				//printf("%lld|%s|%s|%s|%d|%s|%c\n", tmp->isbn, tmp->name, tmp->publisher, tmp->author, tmp->booknum, tmp->library, tmp->yes_no);
				printf(">> 검색 결과 <<\n");
				printf("도서명 : %s\n", tmp->name);
				printf("출판사 : %s\n", tmp->publisher);
				printf("저자명 : %s\n", tmp->author);
				printf("ISBN : %lld\n", tmp->isbn);
				printf("소장처 : %s\n\n", tmp->library);
				// 여기다가 대여 가능 목록 쓸 것
				break;
		 }

		 if (tmp->next == NULL)
		 {
			 printf("존재하지 않는 도서입니다.\n\n");
			 break;
		 }
		 else
		 {
			 tmp = tmp -> next;
		 }
	}
}

void IsbnFind(Book * head)
{
	long long search;
	// int num = 0;
	Book * tmp = head;
	printf("ISBN을 입력하세요 : ");
	scanf("%lld", &search);
	getchar();
	printf("\n");
	while (1) {
		 if (tmp->isbn == search) {
				//printf("%lld|%s|%s|%s|%d|%s|%c\n", tmp->isbn, tmp->name, tmp->publisher, tmp->author, tmp->booknum, tmp->library, tmp->yes_no);
				printf(">> 검색 결과 <<\n");
				printf("도서명 : %s\n", tmp->name);
				printf("출판사 : %s\n", tmp->publisher);
				printf("저자명 : %s\n", tmp->author);
				printf("ISBN : %lld\n", tmp->isbn);
				printf("소장처 : %s\n\n", tmp->library);
				// 여기다가 대여 가능 목록 쓸 것
				break;
		 }

		 if (tmp->next == NULL)
		 {
			 printf("존재하지 않는 도서입니다.\n\n");
			 break;
		 }
		 else
		 {
			 tmp = tmp -> next;
		 }
	}
}

void AuthorFind(Book * head)
{
	char search[50];
	// int num = 0;
	Book * tmp = head;
	printf("저자명을 입력하세요 : ");
	gets(search);
	while (1) {
		 if (strstr(tmp->author, search) != NULL) {
				//printf("%lld|%s|%s|%s|%d|%s|%c\n", tmp->isbn, tmp->name, tmp->publisher, tmp->author, tmp->booknum, tmp->library, tmp->yes_no);
				printf(">> 검색 결과 <<\n");
				printf("도서명 : %s\n", tmp->name);
				printf("출판사 : %s\n", tmp->publisher);
				printf("저자명 : %s\n", tmp->author);
				printf("ISBN : %lld\n", tmp->isbn);
				printf("소장처 : %s\n\n", tmp->library);
				// 여기다가 대여 가능 목록 쓸 것
				break;
		 }

		 if (tmp->next == NULL)
		 {
			 printf("존재하지 않는 도서입니다.\n\n");
			 break;
		 }
		 else
		 {
			 tmp = tmp -> next;
		 }
	}
}

void BookPrintList(Book * head)
{
	int i = 1; // 확인용 나중에 지워야 함
	Book * tmp = head;
	printf("\n");
	while(tmp != NULL)
	{
		printf("%d. %lld|%s|%s|%s|%d|%s|%c\n", i++, tmp->isbn, tmp->name, tmp->publisher, tmp->author, tmp->booknum, tmp->library, tmp->yes_no);
		tmp = tmp->next;
	}
}
