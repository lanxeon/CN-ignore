#include <iostream>
#include <cstring>
using namespace std;

char g[]="10001000000100001";
int gl=strlen(g);
char d[30], md[30];
int dl,mdl;
int i,j,k;
char cs[20], rem[20];
int flag=0;

void xo()
{
  for (int i=0;i<gl;i++)
    rem[i]=((cs[i]==g[i])?'0':'1');
}
void crc(){
//adding modified data to check sum array
    for(j=0;j<gl;j++)
    cs[j]=md[j];
    cout<<endl;
    
  for(i=0;i<dl;i++)
  {
    if(cs[0]=='1')
    xo();
    else
    {
      for(k=0;k<gl;k++)
      rem[k]=cs[k];
    }

    for(k=0;k<gl-1;k++)
      cs[k]=rem[k+1];
      
      cs[k]=md[gl+i];
  }
cout<<"final remainder is "<<endl;
cout<<rem;
}


int main()
{
cout<<"Enter data: " <<endl;
cin>>d;
dl=strlen(d);

for (i=0;i<dl;i++)
  md[i]=d[i];
  
cout<<endl;

for (i=dl;i<dl+gl-1;i++)
  md[i]='0';

mdl=strlen(md);
cout<<"The modified data is: "<<endl<<md;

  
cout<<endl<<endl; 
crc();
for (i=dl,j=1;i<dl+gl-1;i++,j++)
  md[i]=rem[j];
  
mdl=strlen(md);
 cout<<endl<<"codeword sent"<<endl;
  cout<<md;
  cout<<endl<<endl;
  cout<<"enter the code word received : ";
  cin>>md;
  mdl=strlen(md);
  cout<<mdl<<endl;
  cout<<md;
  
crc();
  for (i=1;i<gl;i++)
  {
    if(rem[i]=='1'){
      flag=1;
      break;
    }
  }
  if(flag)
  {
    cout<<"\nerror detected!"<<endl;
  }
  else
  cout<<"\nno error!"<<endl;
}