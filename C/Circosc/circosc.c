#include<stdio.h> 
#include<math.h> 

#include<GL/glut.h>

#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#define	WIDTH	1920
#define	HEIGHT	1200

#define BFRAMES	3000
#define NBLOCKS	10

// global declaration 
float x, y; 

int refreshMillis;      // Refresh period in milliseconds
float freq;

float f_do;
float f_re;
float f_mi;
float f_fa;
float f_sol;
float f_la;
float f_si;

float tempi;
float resy;

bool pause1;

RtAudio adac;
unsigned int bufferFrames = BFRAMES, fs, oDevice = 0, iDevice = 0, iOffset = 0, oOffset = 0;
float bufferBytes;
float *mybuffer,*data;

float coda[NBLOCKS][BFRAMES];
int depo,prel;
float buffer[2*BFRAMES];
int indice;

bool floop;

float xbegin;


void init_coda(void)
{
	prel=0;
	depo=0;
	memset(coda,0,NBLOCKS*BFRAMES*sizeof(float));
}

bool empty(void)
{
	return(prel==depo);
}

bool full(void)
{
	int gap;

	gap=depo-prel;

	return(gap==-1 || gap==NBLOCKS-1);
}

void incprel(void)
{
	int appo;

	appo=prel;
	(++appo)%=NBLOCKS;
	prel=appo;
}

void incdepo(void)
{
	int appo;

	appo=depo;
	(++appo)%=NBLOCKS;
	depo=appo;
}

bool deposita(float *blocco)
{
	if (full())
		return(false);
	memcpy(coda[depo],blocco,BFRAMES*sizeof(float));
	incdepo();
	return(true);
}

bool preleva(float *blocco)
{
	if (empty())
		return(false);
	memcpy(blocco,coda[prel],BFRAMES*sizeof(float));
	incprel();
	return(true);
}

int input(void *outputBuffer, void *inputBuffer, unsigned int /*nBufferFrames*/,
          double /*streamTime*/, RtAudioStreamStatus status, void *data)
{
    if (status)
        printf("Stream over/underflow detected.\n");

    if (*(float *)data!=BFRAMES*sizeof(float))
    {
	    printf("Not enough bytes to read!\n");
	    return(0);
    }

    mybuffer = (float *)inputBuffer;

    deposita(mybuffer);

    return 0;
}

// map a value proportionally
float map(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

bool MicrophoneInput(void)
{
    unsigned int channels;

    if (adac.getDeviceCount() < 1)
    {
        std::cout << "\nNo audio devices found!\n";
        exit(1);
    }

    channels = 1;
    fs = 44100;

    refreshMillis=1000.0*f_la/fs;
    refreshMillis=0;

    data = (float *)calloc(channels, sizeof(float));
    mybuffer = (float *)calloc(channels, sizeof(float));

    // Let RtAudio print messages to stderr.
    adac.showWarnings(true);

    // Set our stream parameters for input.
    bufferFrames = BFRAMES;				// numero di floats per canale
    RtAudio::StreamParameters iParams;
    iParams.deviceId = iDevice;
    iParams.nChannels = channels;
    iParams.firstChannel = iOffset;
    iParams.deviceId = adac.getDefaultInputDevice();

    RtAudio::StreamOptions options;
    options.flags = RTAUDIO_HOG_DEVICE;
    options.flags = RTAUDIO_NONINTERLEAVED;

    try
    {
        adac.openStream(NULL, &iParams, RTAUDIO_FLOAT32, fs, &bufferFrames, &input, (void *)&bufferBytes, &options);
        std::cout << "\nStream latency = " << adac.getStreamLatency() << std::endl;

        bufferBytes = bufferFrames * channels * sizeof(float);

        // Start the stream
        adac.startStream();
        std::cout << "\nRunning Audio In ...\n";
    }
    catch (RtAudioError &e)
    {
        e.printMessage();
        return false;
    }

    init_coda();
    while (preleva(&buffer[0])==false)
	sleep(1);

    return true;
}


// Initialization function 
void myInit (void) 
{ 
	// Reset background color with black (since all three argument is 0.0) 
	glClearColor(0.0, 0.0, 0.0, 1.0); 
	
	// Set picture color to green (in RGB model) 
	// as only argument corresponding to G (Green) is 1.0 and rest are 0.0 
	glColor3f(0.0, 1.0, 0.0); 
	
	// Set width of point to one unit 
	glPointSize(1.0); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	
	// Set window size in X- and Y- direction 
	gluOrtho2D(-WIDTH/2, WIDTH/2, -HEIGHT/2, HEIGHT/2); 

	f_do=264;
	f_re=9.0/8*f_do;
	f_mi=5.0/4*f_do;
	f_fa=4.0/3*f_do;
	f_sol=3.0/2*f_do;
	f_la=5.0/3*f_do;
	f_si=15.0/8*f_do;

	freq=f_la;

	pause1=false;

	indice=BFRAMES/2;

	floop=true;

	xbegin=-WIDTH/2;

	tempi=20;

	resy=0.1;
} 


// Function to display animation 
void display (void) 
{ 
	float r;
	float lmin;
	float lmax;

	float val;

	float fr;

	float step;

	int counter;

	float x1;

	if (pause1==true && floop==true)
		return;

	counter=0;

	step=(float)WIDTH*tempi/fs;

	r=300;

	lmin=-r/4;
	lmax=r/4;

	if (lmax>200)
	{
		lmin=-200;
		lmax=200;
	}

	fr=freq/pow(2,7);
	fr=1;

#if 0
	for (int pot=0;pot<10;pot++)
	{
		float appo;
		appo=freq/pow(2,pot);
		if (appo!=floor(appo))
			break;
	}
	fr=freq/pow(2,pot-1);
#endif
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1);

	if (floop)
	{
		glClear(GL_COLOR_BUFFER_BIT); 

		x=xbegin;
		floop=false;
	}

	glBegin(GL_POINTS); 

	while (counter<BFRAMES)
	{
		int npass=10;
		int gap;

		if (indice==0 || indice==BFRAMES)
		{
			if (preleva(&buffer[indice])==false)
			{
//			printf("Coda vuota!\n");
				return;
			}
		}

		val=0;
		for (int j = 0; j<npass ; j++)
		{
			gap=indice-j;
			if (gap>=0)
				val+=buffer[gap];
			else
				val+=buffer[2*BFRAMES+gap];
		}

		val/=npass;

		x += step;

		if (x>WIDTH/2)
		{
			x-=WIDTH;
			floop=true;
		}

#if 1
		lmin=-r/4;
		lmax=r/4;

//		x1 = r * cos(2*M_PI*fr*(x+WIDTH/2)/WIDTH); 
//		y  = r * sin(2*M_PI*fr*(x+WIDTH/2)/WIDTH); 

//		glVertex2f(x1, y);

		x1 = (r + map(val,-1.0*resy,1.0*resy,lmin,lmax)) * cos(2*M_PI*fr*(x+WIDTH/2)/WIDTH); 
		y  = (r + map(val,-1.0*resy,1.0*resy,lmin,lmax)) * sin(2*M_PI*fr*(x+WIDTH/2)/WIDTH); 

		glVertex2f(x1, y);
#else

		lmin=-HEIGHT/2;
		lmax=HEIGHT/2;

		y = map(val,-1.0*resy,1.0*resy,lmin,lmax);

		glVertex2f(x, y);
#endif

		counter++;
		indice+=1;
		indice%=(2*BFRAMES);

		if (floop==true)
		{
			xbegin=x+step;
			break;
		}
	}
		
	glEnd(); 
	glFlush(); 
} 

/* Called back when the timer expired */
void Timer(int value) {
   glutPostRedisplay();    // Post a paint request to activate display()
   glutTimerFunc(refreshMillis, Timer, 0); // subsequent timer call at milliseconds
}

/* Callback handler for normal-key event */
void keyboard(unsigned char key, int x, int y) {
   switch (key) {
      case 27:     // ESC key
         exit(0);
         break;
      case 'p':		pause1=!pause1;
			break;
      case '1':		freq=f_do;
			break;
      case '2':		freq=f_re;
			break;
      case '3':		freq=f_mi;
			break;
      case '4':		freq=f_fa;
			break;
      case '5':		freq=f_sol;
			break;
      case '6':		freq=f_la;
			break;
      case '7':		freq=f_si;
			break;

      case 't':		tempi+=0.01;
			printf("tempi=%f\n",tempi);
			break;

      case 'y':		tempi+=-0.01;
			if (tempi<0.01)
				tempi=0.01;
			printf("tempi=%f\n",tempi);
			break;

      case 'u':		resy-=0.01;
			if (resy<0.01)
				resy=0.01;
			break;

      case 'i':		resy+=0.01;
			if (resy>1)
				resy=1;
			break;
   }
}

int main (int argc, char** argv) 
{ 
	glutInit(&argc, argv); 
	
	// Display mode which is of RGB (Red Green Blue) type 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
	
	// Declares window size 
	glutInitWindowSize(WIDTH, HEIGHT); 
	
	// Declares window position which is (0, 0) 
	// means lower left corner will indicate position (0, 0) 
	glutInitWindowPosition(0, 0); 

	// Name to window 
	glutCreateWindow("Revolution"); 

	// full screen
//	glutFullScreen();

	// Call to myInit() 
	myInit(); 

	if (!MicrophoneInput())
	{
		printf("Failed to initialize audio!\n");
		exit(0);
	}

	glutDisplayFunc(display); 
	glutKeyboardFunc(keyboard);   // Register callback handler for special-key event
	glutTimerFunc(1000, Timer, 0);   
	glutMainLoop(); 
} 

