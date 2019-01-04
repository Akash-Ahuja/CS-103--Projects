/*
 * CSc103 Project 7: Towers of Hanoi
 * See readme.html for details.
 * Please list all references you made use of in order to complete the
 * assignment: your classmates, websites, etc.  Aside from the lecture notes
 * and the book, please list everything.  And remember- citing a source does
 * NOT mean it is okay to COPY THAT SOURCE.  What you submit here **MUST BE
 * YOUR OWN WORK**.
 * References:

 *In order to make towers of hanoi i used different coins as a model for towers, to check the algorithm and finally see how the number of steps are required as number of disks increase, major time i spent was actually understanding the game itself
 sources certainly were some youtube videos to understand the geptopt and argc, char *argv[] and learning history of the game
 *
 *
 * Finally, please indicate approximately how many hours you spent on this:
 * #hours: About 12 hours
 */

// TODO: write the program.
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <getopt.h> // to parse long arguments.
#include <cstdlib> // for atoi function

void Towers(int, int, int, int); /*prototype*/

/* Here's a skeleton main function for processing the arguments. */
int main(int argc, char *argv[]) {
	// define long options
	static struct option long_opts[] = {
		{"start",        required_argument, 0, 's'},
		{"end",          required_argument, 0, 'e'},
		{"num-disks",    required_argument, 0, 'n'},
		{0,0,0,0} // this denotes the end of our options.
	};
	// now process the options:
	int intermediate = 2; /*just fixing the value of intermediate to 2*/

	char c; // to hold the option
	int opt_index = 0;
	short n=0,start=0,end=0; /* to store inputs to the towers function. */
	while ((c = getopt_long(argc, argv, "s:e:n:", long_opts, &opt_index)) != -1) {
		switch (c) {
			case 's': // process option s
				start = atoi(optarg);
				break;
			case 'e': // process option e
				end = atoi(optarg);
				break;
			case 'n': // process option n
				n = atoi(optarg);
				break;
			case '?': // this will catch unknown options.
				// here is where you would yell at the user.
				// although, getopt will already print an error message.
				return 1;
		}
	}

	Towers(n, start, intermediate, end); /*calling the function*/


	return 0;
}

/*FUNCTION*/
void Towers(int n, int start, int intermediate, int end)
{
  if(n==1)
  {
    cout << start << "	" << end << endl;
    return;
  }
  else
  {
    Towers(n-1,start, end, intermediate);
    cout << start << "	" << end << endl;
    Towers(n-1, intermediate, start, end);
    }
  }
