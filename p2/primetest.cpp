/*
 * CSc103 Project 2: prime numbers.
 * See readme.html for details.
 * Please list all references you made use of in order to complete the
 * assignment: your classmates, websites, etc.  Aside from the lecture notes
 * and the book, please list everything.  And remember- citing a source does
 * NOT mean it is okay to COPY THAT SOURCE.  What you submit here **MUST BE
 * YOUR OWN WORK**.
 * References:
 *
 */

   #include <iostream>
   using std::cin;
   using std::cout;
   using std::endl;
   using namespace std;
   #include <cmath>

   int main()

   {
       unsigned long a;



   while (cin >> a) {

      for (int x = 3; x > 3 ; x = x + 2) {
         if (a % x != 0) {
         cout << true << "\n";

         }
      }


     if( a == 0) {
      cout << false << "\n";
      }

      else if (a < 2) {
      cout << false << "\n";
      }

      else if (a == 2) {
      cout << true << "\n";
      }

      else if (a / a == 1 && a % 2 != 0) {
      cout << true << "\n";
      }

      else if (a % 2 == 0) {
      cout << false << "\n";
      }


    }

	return 0;
    }
