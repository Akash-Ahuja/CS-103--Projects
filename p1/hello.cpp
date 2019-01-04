/*
 * CSc103 Project 1: (hello_world++)
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

  #include <string>
  using std::string;

  int main()

  {

   string x;
   string y;

   cout << "What is the name of your favorite person? \n";
   cin >> x;

   cout << "Name one of your close relatives: \n";
   cin >> y;

   cout << "Well, a fact for you. Do you know ";
   cout << x;
   cout <<" and ";
   cout << y;
   cout << " used to attend the same highschool? \n";


   return 0;

  }





