#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <iomanip>
#include <time.h>
using namespace std;
void Probability(int s1, int s2, int N, int* winner, int* loser)
{
  double r1,r2;
  r1=double(rand())/RAND_MAX;
  r2=double(rand())/RAND_MAX;
  if (r1>=r2)
  {
    *winner=s1;
    *loser = s2;
  }
  else
  {
    *winner = s2;
    *loser = s1;
  }
}
double delta_m(int s1, int s2, double *Money, double Money_avg)
{
  double r;
  double delta;
  r=double(rand())/RAND_MAX;
  delta=r*Money_avg;
//  delta = 1.;
  return delta;
}
void Distribution(double *Money, int *Portion, double Width, int N)
{
  int i,n;
  for (i=1;i<=N;i++)
  {
    n=int(ceil(Money[i]/Width));
    Portion[n]=Portion[n]+1;
  }
}
main() 
{
  int i,j,k,p,N,step,Number_bin,Counter,tag,p_max;
  int s1,s2,winner,loser,Transaction;
  double Debt,Money_t,Money_avg,Width,delta_money,Entropy;
  double random;
  FILE *fp1,*fp2,*fp3;
  srand (time(NULL));
//Read Input data:
  fp1=fopen("Input.dat","r+");
  fscanf(fp1,"Money_t=%lf N=%i step=%i Debt=%lf Width=%lf tag=%i p_max=%i",&Money_t,&N,&step,&Debt,&Width,&tag,&p_max); 
  fclose(fp1);
  fp2 = fopen("Ditribution.dat","w+"); 
  fp3 = fopen("Entropy.dat","w+"); 
//Set Parameters:
  Money_avg = Money_t/N;
  delta_money = 0.;
  Counter = 0;
  Entropy = 0.;
//Set Money Array:
  double Money[N+1];
//Initialize Agents Money:
  for (i=1;i<=N;i++)
  {
    Money[i] = Money_avg;
  }
//Set Money Distribution Array:
  Number_bin=int(ceil(Money_t/Width));
  int Portion[Number_bin+1];
//Initialize Distribution Array:
  for (i=1;i<=Number_bin;i++)
  {
    Portion[i] = 0;
  }
//Check Parameters Values:
  printf(" Money_t=%lf and N=%i and step=%i and Debt=%lf\n",Money_t,N,step,Debt);
//Print Header of Output File:
  for (i=1;i<=200;i++)
  {
    j = i*Width;
    fprintf(fp2,"%d\t",j);
  }
  fprintf(fp2,"\n");
  i=1;
//Main Loop for Equilibria:
  for (p=1;p<=p_max;p++)
  {
    for (i=1;i<=step;i++)
    {
//Main loop for Identifying Agents:
      do
      {
        do
        {
          random=double(rand())/RAND_MAX;
          s1=int(ceil(random*N));
          random=double(rand())/RAND_MAX;
          s2=int(ceil(random*N));    
        }while(s1==0 || s2==0);
        if (Money[s1] >=Debt && Money[s2]>=Debt)
        {
          Probability(s1,s2,N,&winner,&loser);
          delta_money = delta_m(s1,s2,Money,Money_avg);
          if (Money[loser]<delta_money)
          {
            Transaction = 0;
          }
          else
          {
            Transaction = 1;
          }        
        }
        else
        {
          Transaction=0;
        }
      }while(Transaction==0);
//Update Money Due to Transaction:
      Money[winner] = Money[winner]+delta_money;
      Money[loser] = Money[loser]-delta_money;
      Counter = Counter+1;
      if (Counter==tag)
      {
        Distribution(Money,Portion,Width,N);
        for (j=1;j<=200;j++)
        {
          if (Portion[j]!=0)
          {
            Entropy = Entropy+(double(Portion[j])/N)*log(double(Portion[j])/N);
          }
          fprintf(fp2,"%d\t",Portion[j]);        
        }        
        fprintf(fp2,"\n");
        fprintf(fp3,"%i %lf\n",i+(p-1)*step,-Entropy);        
        for (k=1;k<=Number_bin;k++)
        {
          Portion[k] = 0;
        }
        printf("step=%i and N=%i and Entropy=%lf\n",i+(p-1)*step,N,-Entropy);
        Counter = 0;  
        Entropy = 0.;      
      }
    }
  }
  Distribution(Money,Portion,Width,N);
  for (i=1;i<=200;i++)
  {
    fprintf(fp2,"%d\t",Portion[i]);
  }
  fprintf(fp2,"\n");
  fclose(fp2);
  printf("End\n");
  return 0;
}


