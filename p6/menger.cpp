#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <getopt.h>
#include <cstdlib>   /* for atexit() */
#include <cmath>     /* sin(x),cos(x) */
#include <time.h>    /* for time() */
#include <algorithm>
using std::min;
using std::max;
using std::swap;
#include <deque>
using std::deque;

static const char* usage =
"Usage: %s [OPTIONS...]\n\n"
"Make cool configurations of pixels on your screen.  Options:\n"
"  -d,--depth [NUM]  recurse NUM times (default=3, 4 melts your computer)\n"
"  -m,--mode  [NUM]  0<=NUM<16 describes the subset of subcubes to draw.\n"
"  -r,--random       draw a random (likely asymmetric) modulation.\n"
"  -F,--full         show window fullscreen\n";

#define USELIGHT 1

/* initial size; will be set to screen size after window creation. */
int WWIDTH = 800;
int WHEIGHT = 600;
int fsflag = 0;
int fcount = 0; /* frame counter */

bool init(); /* setup SDL + openGL */
bool initGL();
void render();
void close(); /* free SDL resources */
SDL_Window* gWindow = 0; /* main window */
SDL_GLContext gContext; /* openGL context */

/* we parameterize as follows:
 * (cx,cy,cz) == center of cube
 * each edge will be of length 2r.
 * Initial cube: */
const double cx = 0;
const double cy = 0;
const double cz = -1.7;
const double r = 0.5;

double faces[6][4][3]; /* used for drawing; you can ignore it. */

/* store coordinates of center, along with the "radius".
 * see the readme for details. */
struct cube {
	double x,y,z,r;
	cube(double x0, double y0, double z0, double r0) :
		x(x0),y(y0),z(z0),r(r0) {};
	cube():x(0),y(0),z(-1.7),r(0.5) {};
};

deque<cube> M(1,cube(cx,cy,cz,r));
/* NOTE: M is where the cubes live on which you need to apply the
 * fractalization process.  Store the result back into M. */

/* this function is for drawing; you can ignore it. */
void setFaces(const cube& C)
{
	/* NOTE: it is important to get the vertexes in an order
	 * which avoids a diagonal across one of the faces. */
	double cx = C.x;
	double cy = C.y;
	double cz = C.z;
	double r = C.r;

	/* back face */
	faces[0][0][0] = cx + r;
	faces[0][0][1] = cy + r;
	faces[0][0][2] = cz - r;
	faces[0][1][0] = cx + r;
	faces[0][1][1] = cy - r;
	faces[0][1][2] = cz - r;
	faces[0][2][0] = cx - r;
	faces[0][2][1] = cy - r;
	faces[0][2][2] = cz - r;
	faces[0][3][0] = cx - r;
	faces[0][3][1] = cy + r;
	faces[0][3][2] = cz - r;
	/* left face */
	faces[1][0][0] = cx - r;
	faces[1][0][1] = cy + r;
	faces[1][0][2] = cz + r;
	faces[1][1][0] = cx - r;
	faces[1][1][1] = cy + r;
	faces[1][1][2] = cz - r;
	faces[1][2][0] = cx - r;
	faces[1][2][1] = cy - r;
	faces[1][2][2] = cz - r;
	faces[1][3][0] = cx - r;
	faces[1][3][1] = cy - r;
	faces[1][3][2] = cz + r;
	/* right face */
	faces[2][0][0] = cx + r;
	faces[2][0][1] = cy + r;
	faces[2][0][2] = cz + r;
	faces[2][1][0] = cx + r;
	faces[2][1][1] = cy + r;
	faces[2][1][2] = cz - r;
	faces[2][2][0] = cx + r;
	faces[2][2][1] = cy - r;
	faces[2][2][2] = cz - r;
	faces[2][3][0] = cx + r;
	faces[2][3][1] = cy - r;
	faces[2][3][2] = cz + r;
	/* top face */
	faces[3][0][0] = cx + r;
	faces[3][0][1] = cy + r;
	faces[3][0][2] = cz + r;
	faces[3][1][0] = cx + r;
	faces[3][1][1] = cy + r;
	faces[3][1][2] = cz - r;
	faces[3][2][0] = cx - r;
	faces[3][2][1] = cy + r;
	faces[3][2][2] = cz - r;
	faces[3][3][0] = cx - r;
	faces[3][3][1] = cy + r;
	faces[3][3][2] = cz + r;
	/* bottom face */
	faces[4][0][0] = cx + r;
	faces[4][0][1] = cy - r;
	faces[4][0][2] = cz + r;
	faces[4][1][0] = cx + r;
	faces[4][1][1] = cy - r;
	faces[4][1][2] = cz - r;
	faces[4][2][0] = cx - r;
	faces[4][2][1] = cy - r;
	faces[4][2][2] = cz - r;
	faces[4][3][0] = cx - r;
	faces[4][3][1] = cy - r;
	faces[4][3][2] = cz + r;
	/* front face */
	faces[5][0][0] = cx + r;
	faces[5][0][1] = cy + r;
	faces[5][0][2] = cz + r;
	faces[5][1][0] = cx + r;
	faces[5][1][1] = cy - r;
	faces[5][1][2] = cz + r;
	faces[5][2][0] = cx - r;
	faces[5][2][1] = cy - r;
	faces[5][2][2] = cz + r;
	faces[5][3][0] = cx - r;
	faces[5][3][1] = cy + r;
	faces[5][3][2] = cz + r;
}

/* SDL initialization code.  Safe to ignore. */
bool init()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL_Init failed.  Error: %s\n", SDL_GetError());
		return false;
	}
	if(!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1")) {
		printf("Warning: vsync hint didn't work.\n");
	}
	/* set depth buffer size */
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
	/* create main window */
	gWindow = SDL_CreateWindow("Is this...recursion?",
								SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED,
								WWIDTH, WHEIGHT,
								SDL_WINDOW_OPENGL|
								SDL_WINDOW_SHOWN|
								fsflag);
								// FULLSCREEN_FLAG);
	if(!gWindow) {
		printf("Failed to create main window. SDL Error: %s\n", SDL_GetError());
		return false;
	}
	/* set width and height */
	SDL_GetWindowSize(gWindow, &WWIDTH, &WHEIGHT);
	gContext = SDL_GL_CreateContext(gWindow);
	if (!gContext) {
		printf("openGL context fail x_x error: %s\n",SDL_GetError());
		return false;
	}
	/* setup frame-sync'd drawing with openGL: */
	if(SDL_GL_SetSwapInterval(1) < 0) {
		printf("openGL vsync fail x_x error: %s\n",SDL_GetError());
		// return false;
	}
	if(!initGL()) {
		printf("openGL init fail x_x error: %s\n",SDL_GetError());
		return false;
	}

	/* XXX setup polygon VBO's here if needed. */
	return true;
}

/* OpenGL initialization code.  Safe to ignore. */
bool initGL()
{
	GLenum error = GL_NO_ERROR;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glViewport(0, 0, WWIDTH, WHEIGHT); /* use entire window */
	glMatrixMode(GL_PROJECTION);
	float aspect = (float)WWIDTH / WHEIGHT;
	glFrustum(-aspect, aspect, -1, 1, 0.7, 5); /* fix aspect ratio */

	glClearColor(0,0,0,1);

	error = glGetError();
	if(error != GL_NO_ERROR) {
		printf("initGL failed.\n");
		return false;
	}
	/* reset mode to modelview for subsequent transformations. */
	glMatrixMode(GL_MODELVIEW);
	/* NOTE: in order to make the depth buffer to work, you must:
	 * 1. enable GL_DEPTH_TEST
	 * 2. make sure range and function make sense
	 * 3. call glClear with GL_DEPTH_BUFFER_BIT set
	 * 4. (?) set depth buffer size via SDL (?)
	 * */
	glEnable(GL_DEPTH_TEST);
	glDepthRange(0,1);
	glDepthFunc(GL_LESS);
	/* Links for those interested:
	 * https://learnopengl.com/Getting-started/Coordinate-Systems
	 * https://www.opengl.org/archives/resources/faq/technical/depthbuffer.htm
	 * https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideoopengl.html
	 * https://learnopengl.com/Advanced-OpenGL/Depth-testing
	 * */
	/* try to setup lighting: */
	#if USELIGHT
	float m_spec[]  = {0.5,0.5,0.5,1};
	float m_shiny[] = {50};
	// float l_pos[] = {1,1,1,0};
	float l_pos[] = {cx,cy,cz,1}; /* light at center */
	float l_amb[] = {0.2,0.2,0.2,1};
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT,GL_SPECULAR,m_spec);
	glMaterialfv(GL_FRONT,GL_SHININESS,m_shiny);
	glLightfv(GL_LIGHT0,GL_POSITION,l_pos);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	/* set ambient light: */
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,l_amb);
	/* Links for those interested:
	 * http://www.glprogramming.com/red/chapter05.html
	 * https://www.cse.msu.edu/~cse872/tutorial3.html
	 * https://www.khronos.org/opengl/wiki/How_lighting_works
	 * */
	#endif
	return true;
}

/* construct rectangles for the faces of all the cubes.  safe to ignore. */
void drawFaces()
{
	/* XXX with GL_QUADS, you have to be careful with the order
	 * in which vertexes are specified.  The nice combinatorial
	 * looping will give you strange results. */
#if !USELIGHT
	float colors[18] = {
		0.5,0,1,
		1,0,0,
		0,1,0,
		1,1,0,
		0,0,1,
		0.8,0.35,0.1
	};
#else
	float diffc[24] = {
		0.5,0,1,1,
		1,0,0,1,
		0,1,0,1,
		1,1,0,1,
		0,0,1,1,
		0.8,0.35,0.1,1
	};
	/* NOTE: this has to agree with setFaces() */
	float normals[18] = {
		0,0,-1, /* back */
		-1,0,0, /* left */
		1,0,0,  /* right */
		0,1,0,  /* top */
		0,-1,0, /* bottom */
		0,0,1   /* front */
	};
#endif
	glBegin(GL_QUADS);
	for (size_t l = 0; l < M.size(); l++) {
		setFaces(M[l]);
		for (size_t i = 0; i < 6; i++) {
			/* NOTE: glColor does nothing with lighting.
			 * Instead we have to set material properties. */
#if !USELIGHT
			glColor3fv(colors+i*3);
#else
			glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,diffc+i*4);
			glNormal3fv(normals+i*3);
#endif
			for (size_t j = 0; j < 4; j++) {
				glVertex3d(faces[i][j][0],
						   faces[i][j][1],
						   faces[i][j][2]);
			}
		}
	}
	glEnd();
}

/* rotate scene, draw object. */
void render()
{
	/* clear contents (uses clearcolor above) */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	// float scale = 1 + 0.3*sin(fcount/50.0);
	// glScalef(scale,scale,scale);
	float zdir = 0.7*sin(fcount/50.0);
	float angle = fcount % 360;
	glTranslatef(cx,cy,cz);
	glRotatef(angle,1,1,zdir);
	glTranslatef(-cx,-cy,-cz);
	drawFaces();
	glPopMatrix();
	return;
}

void close()
{
	SDL_DestroyWindow(gWindow);
	gWindow = 0;
	SDL_Quit();
}

int main(int argc, char *argv[])
{
	static struct option long_opts[] = {
		{"full",    no_argument,       0, 'F'},
		{"depth",   required_argument, 0, 'd'},
		{"mode",    required_argument, 0, 'm'},
		{"random",  no_argument,       0, 'r'},
		{"help",    no_argument,       0, 'h'},
		{0,0,0,0}
	};
	char c;
	int opt_index = 0;
	int d = 3;
	int m = 12;
	bool use_rand = false;
	while ((c = getopt_long(argc, argv, "hFd:m:r", long_opts, &opt_index)) != -1) {
		switch (c) {
			case 'h':
				printf(usage,argv[0]);
				return 0;
			case 'F':
				// fsflag = SDL_WINDOW_FULLSCREEN; /* this one is strange */
				fsflag = SDL_WINDOW_FULLSCREEN_DESKTOP;
				break;
			case 'd':
				d = atoi(optarg);
				break;
			case 'm':
				m = atoi(optarg);
				break;
			case 'r':
				use_rand = true;
				srand(time(0));
				m = rand() % (1<<27);
				fprintf(stderr, "rendering random mode (%i)\n",m);
				break;
			case '?':
				printf(usage,argv[0]);
				return 1;
		}
	}

	/* TODO: apply the transformation to the global deque M of cubes. */

	printf("num cubes = %lu\n",M.size());
	/* start SDL; create window; setup openGL + polygons */
	if(!init()) {
		printf("init() failed from main().\n");
		return 1;
	}
	atexit(close);
	/* set this to exit main loop. */
	bool quit = false;
	SDL_Event e;
	/* main loop: */
	int run = 1;
	while(!quit) {
		/* handle events */
		while(SDL_PollEvent(&e) != 0) {
			/* meta-q in i3, for example: */
			if(e.type == SDL_QUIT) {
				quit = true;
			} else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_SPACE:
						run = 1-run;
						break;
					case SDLK_ESCAPE:
					case SDLK_q:
						quit = true;
						break;
					default:
						break;
				}
			}
		}
		render();

		SDL_GL_SwapWindow(gWindow); /* actually draw screen */
		fcount += run;
	}

	printf("total frames rendered: %i\n",fcount);
	return 0;
}
