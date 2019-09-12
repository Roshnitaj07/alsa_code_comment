#include<stdio.h>
//#include<conio.h>
//-------------------------------------
typedef struct Example
{
  int num1;
  int num2;
}s;
//-------------------------------------
void accept(s *sptr,int n)
{
  int i;
  for(i=0;i<n;i++)
  {
  printf("\nEnter num1 : ");
  scanf("%d",&(sptr+i)->num1);
  printf("\nEnter num2 : ");
  scanf("%d",&(sptr+i)->num2);
  }
}
//-------------------------------------
void print(s *sptr,int n)
{
  int i;
  for(i=0;i<n;i++)
  {
  printf("\nNum1 : %d",(sptr+i)->num1);
  printf("\nNum2 : %d",(sptr+i)->num2);
  }
}
//-------------------------------------
void main()
{
    s s1[3];
int i;
//clrscr();
accept(s1,3);
print(s1,3);
//getch();
}
