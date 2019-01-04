/*a non recursive approach to the towers of hanaoi*/
#include<iostream>
using namespace std;

void Towers(int, int, int, int);

int main()
{
  int n;
  cout << "how many disks:" << flush;
  cin >> n;

  Towers(n, 1, 2, 3);
  return 0;
}

void Towers(int n, int start, int intermediate, int end)
{
  if(n==1)
  {
    cout << "disk " << n << " is moving from " << start << " to " << end << endl;
    return;
  }
  else
  {
    Towers(n-1,start, end, intermediate);
    cout  << "disk " << n << " IS MOVING FROM "<< start << " TO " << end << endl;
    Towers(n-1, intermediate, start, end);
    }
  }