#include <bits/stdc++.h>
using namespace std;

int main(){

    int i=1;
    int& p=i;
    int* ptr=&i;
    cout<<"sizeof(p):"<<sizeof(p)<<endl;
    cout<<"sizeof(trp):"<<sizeof(ptr)<<endl;
    return 0;
}
