#include<iostream>
using namespace std;

void towers(int, int, int, int);

int main()
{
    int num;

    cout<<"Enter the number of disks " << endl;
    cin>>num;
    cout<<"The sequence of moves involved in the Tower of Hanoi are :n" << endl;
    towers(num, 1, 3, 2);
    return 0;
}
void towers(int num, int start, int end, int intermediate)
{
    if (num == 1)
    {
        cout<<start<<" "<<end << endl;
        return;
    }
    towers(num - 1, start, intermediate, end);
    cout<<start<<" "<<end << endl;
    towers(num - 1, intermediate, end, start);
  }