#include <cstdio>
#include <getopt.h> // to parse long arguments.
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <set>
using std::set;
using std::multiset;
#include <strings.h>
#include <algorithm>
using std::swap;
using std::sort;
#include <fstream>
using namespace std;

static const char* usage =
"Usage: %s [OPTIONS]...\n"
"Limited clone of sort.  Supported options:\n\n"
"   -r,--reverse        Sort descending.\n"
"   -f,--ignore-case    Ignore case.\n"
"   -u,--unique         Don't output duplicate lines.\n"
"   --help              Show this message and exit.\n";

struct igncaseComp {
	bool operator()(const string& s1, const string& s2) {
		return (strcasecmp(s1.c_str(),s2.c_str()) < 0);
	}
};
/* NOTE: set<string,igncaseComp> S; would declare a set S which
 * does its sorting in a case-insensitive way! */

void rundescending(vector<string> &values) //function for reverse
{
	sort(values.begin(), values.end());
	for (size_t i=0; i < values.size()/2; i++)
	{
		swap(values[i], values[values.size()-1-i]);
	}
}

void nodups(vector<string> &values) //function for unique
{
	set<string> s;
	vector<string> k;
	for (vector<string>::iterator i = values.begin(); i != values.end(); i++)
	{
		s.insert(*i);
	}
	for (set<string>::iterator i = s.begin(); i != s.end(); i++)
	{
		k.push_back(*i);
	}
	values = k;
}

void nosens(vector<string> &values) //function for ignorecase
{
	sort(values.begin(), values.end(), igncaseComp());
}

/* The issue I had is resolved. However, there are still 4 errors remaining when I do the random tests
Try: echo "this is a test" | ./sort
*/

int main(int argc, char *argv[]) {
	// define long options
	static int descending=0, ignorecase=0, unique=0;
	static struct option long_opts[] = {
		{"reverse",       no_argument,   0, 'r'},
		{"ignore-case",   no_argument,   0, 'f'},
		{"unique",        no_argument,   0, 'u'},
		{"help",          no_argument,   0, 'h'},
		{0,0,0,0}
	};
	// process options:
	char c;
	int opt_index = 0;
	while ((c = getopt_long(argc, argv, "rfuh", long_opts, &opt_index)) != -1) {
		switch (c) {
			case 'r':
				descending = 1;
				break;
			case 'f':
				ignorecase = 1;
				break;
			case 'u':
				unique = 1;
				break;
			case 'h':
				printf(usage,argv[0]);
				return 0;
			case '?':
				printf(usage,argv[0]);
				return 1;
		}
	}

	/* TODO: write me... */

	#if 1
	vector<string> values;
	string x;
	while (getline(cin,x))
	{
		values.push_back(x);
	}
	cin.clear();
	#else
	string line;
	vector<string> values;
	ifstream f ("file");
	while (getline(f,line))
	{
		values.push_back(line);
	}
	#endif
	if (descending)
	{
		rundescending(values);
		if (ignorecase)
		{
			nosens(values);
			if (unique)
			{
				nodups(values);
			}
		}
		else if (unique)
		{
			nodups(values);
			if (ignorecase)
			{
				nosens(values);
			}
		}
	}

	else if (ignorecase)
	{
		nosens(values);
		if (descending)
		{
			rundescending(values);
			if (unique)
			{
				nodups(values);
			}
		}
		else if (unique)
		{
			nodups(values);
			if (descending)
			{
				rundescending(values);
			}
		}
	}

	else if (unique)
	{
		nodups (values);
		if (descending)
		{
			rundescending (values);
			if (ignorecase)
			{
				nosens (values);
			}
		}

		else if (ignorecase)
		{
			nosens (values);
			if (descending)
			{
				rundescending (values);
			}
		}
	}

	else
	{
		sort(values.begin(), values.end());
	}

	for (vector<string>::iterator i = values.begin(); i != values.end(); i++)
	{
		cout << *i << "\n";
	}

	return 0;
}