#include <cstdio>
#include <getopt.h> // to parse long arguments.
#include <string>
using std::string;
#include <iostream>
using std::cin;
using std::cout;

static const char* usage =
"Usage: %s [OPTIONS]...\n"
"Limited clone of uniq.  Supported options:\n\n"
"   -c,--count         prefix lines by their counts.\n"
"   -d,--repeated      only print duplicate lines.\n"
"   -u,--unique        only print lines that are unique.\n"
"   --help             show this message and exit.\n";

int main(int argc, char *argv[]) {
	// define long options
	static int showcount=0, dupsonly=0, uniqonly=0;
	static struct option long_opts[] = {
		{"count",         no_argument, 0, 'c'},
		{"repeated",      no_argument, 0, 'd'},
		{"unique",        no_argument, 0, 'u'},
		{"help",          no_argument, 0, 'h'},
		{0,0,0,0}
	};
	// process options:
	char c;
	int opt_index = 0;
	while ((c = getopt_long(argc, argv, "cduh", long_opts, &opt_index)) != -1) {
		switch (c) {
			case 'c':
				showcount = 1;
				break;
			case 'd':
				dupsonly = 1;
				break;
			case 'u':
				uniqonly = 1;
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

	return 0;
}
