#include<iostream>
using namespace std;
int main() {
    int d[10];
    int rec[10],s,s1,s2,s3,i;
    cout<<"Enter 4 bits of data one by one:\n";
    cin>>d[0];
    cin>>d[1];
    cin>>d[2];
    cin>>d[3];
    d[6]=d[0]^d[2]^d[3];
    d[5]=d[0]^d[1]^d[3];
    d[4]=d[0]^d[1]^d[2];
    cout<<"\nEncoded data is\n";
    for(i=0;i<7;i++)
        cout<<d[i];
    cout<<"\n\nEnter received data bits one by one\n";
    for(i=0;i<7;i++)
        cin>>rec[i];
    s1=rec[6]^rec[3]^rec[2]^rec[0];
    s2=rec[5]^rec[3]^rec[1]^rec[0];
    s3=rec[4]^rec[2]^rec[1]^rec[0];
    s=s3*4+s2*2+s1 ;
    if(s==0) {
        cout<<"\nNo error while transmission of data\n";
    }
    else {
        cout<<"\nError on position "<<s+1;
        cout<<"\nData sent : ";
        for(i=0;i<7;i++)
            cout<<d[i];
        cout<<"\nData received : ";
        for(i=0;i<7;i++)
            cout<<rec[i];
        cout<<"\nCorrect message is\n";
        if(rec[7-s+1]==0)
            rec[7-s+1]=1;
        else
             rec[7-s+1]=0;
        for (i=0;i<7;i++) {
            cout<<rec[i];
        }
    }
    return 0;
}