extern "C" {
#include <GL/glut.h>  
#include <GL/gl.h>	
#include <GL/glu.h>	
#include <unistd.h>  
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>

}

/* ASCII code for the escape key. */
#define ESCAPE 27

#define XMIN -1000.0f
#define XMAX 1000.0f
#define YMIN -1000.0f
#define YMAX 1000.0f

class Tank
{
public:
	float   x;
	float   y;
	float   angle;
    float   velocity;
	
public:

	// constructeur
	Tank() {
        x = y = angle = velocity = 0.f;
    }
   
    float getVelocity() {
      return velocity;
    }

    void setVelocity(float velocity) {
      this->velocity = velocity;
    }

    // met à jour la position du vaisseau (absolu)
    void setTranslation(float x, float y) {
      this->x = x;
      this->y = y;
    }

    // donne l'orientation du vaisseau
    void setOrientation(float angle) {
      this->angle = angle;
    }

    void accelerate() {    
        velocity += 10.f;
        if (velocity > 100.0f) velocity = 100.0f;
    }
    void decelerate() {
        velocity -= 10.f;
        if (velocity < -100.0f) velocity = -100.0f;
    }
	void turnRight() {
        angle -= 5.0f;
    }
	void turnLeft() {
        angle += 5.0f;
	}

  
    // met à jour le tank en prenant en compte la vitesse de celui-ci
    void update(double time) {
        if (velocity != 0.0f) {
            x += velocity*time*cos(angle*M_PI/180);        
            y += velocity*time*sin(angle*M_PI/180);

            if (x < XMIN) x = XMIN;
            if (x > XMAX) x = XMAX;
            if (y < YMIN) y = YMIN;
            if (y > YMAX) y = YMAX;
        }
    }
  
    void drawGL(bool local=true) {
        if(local) {
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(12.f, 0.f, -4.f);
            glRotatef(-angle, 0.0f, 0.0f, 1.0f);
            glTranslatef(-x, -y, 0.f);    
        }

        glPushMatrix();
        glTranslatef(x,y,0.0);
        glRotatef(angle, 0.f, 0.f, 1.0f);

        if (local) glColor3f(0.1f, 0.1f, 0.f);
        else glColor3f(0.1f, 0.2f, 0.f);

        // Note : cette facon de coder (en faisant des #include de code C)
        // n'est pas recommandee... (il vaudrait mieux definir des fonctions
        // dans les fichiers .c et les appeler :-)
        #include "barrel.c"
        #include "ltread.c"
        #include "rtread.c"
        glColor3f(0.5f, 0.5f, 0.f);
        #include "body.c"
        glColor3f(0.3f, 0.3f, 0.f);
        #include "turret.c"
        glPopMatrix();
    }
  
};

class World
{
private:
    Tank*       localTank;
    Tank*       tanks[100];
    Tank*       ghostTank;
    double      lastTime;

#define BUFFERLEN 256
    char        buf[BUFFERLEN];        // Tampon pour le message
    int         id = 4;
    int         sock, n;

    struct sockaddr *srcAdr;
    socklen_t   longueurAdr;
    struct sockaddr *adrDest;
	
public:

	World() {
        lastTime = -1;
        localTank = new Tank();
        ghostTank = new Tank();
        for(int i = 0; i < 100; i++)
            tanks[i] = NULL;

        struct addrinfo	hints, *res, *ressave;
        // Récupération de l'adresse de destination (@IP+port)
        bzero(&hints, sizeof(struct addrinfo));
        hints.ai_family = AF_UNSPEC; // le système choisira IPv4 ou IPv6
        hints.ai_socktype = SOCK_DGRAM; // on veut UDP
        // à la place de localhost on peut mettre le nom de la machine
        // ou son adresse IP (v4 ou v6)
        if ( (n = getaddrinfo("localhost", "8123", &hints, &res)) != 0) {
            printf("erreur getaddrinfo : %s\n", gai_strerror(n));
            exit(0);
        }
        ressave = res;

        do { // Construction d'un socket compatible avec cette adresse
            sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
            if (sock >= 0)
                break;		// ça a marché
        } while ( (res = res->ai_next) != NULL);

        // aucune adresse n'a fonctionné, errno est positionné par socket()
        if (res == NULL) {
            perror("erreur socket");
            exit(0);
        }

        // on alloue puis on recopie l'adresse IP + port de destination
        adrDest = (struct sockaddr*)malloc(res->ai_addrlen);
        memcpy(adrDest, res->ai_addr, res->ai_addrlen);
        longueurAdr = res->ai_addrlen;
        // on libère la mémoire allouée par getaddrinfo
        freeaddrinfo(ressave);

         // Contiendra l'@IP et le port de l'?metteur
        srcAdr = (struct sockaddr*) malloc(longueurAdr);
        bzero((char *)srcAdr, longueurAdr);  // init. ? 0
    }
    
    Tank& getLocalTank() {
        return *localTank;
    }

    Tank& getGhostTank() {
        return *ghostTank;
    }
  
    // met à jour le monde
    void update()
    {
        char machine[NI_MAXHOST];
        char service[NI_MAXSERV];

        //while(1)
        {
            sprintf(buf, "D %d %f %f %f %f", id, localTank->x, localTank->y, localTank->angle, localTank->velocity);  // On écrit le message dans le tampon
            // Envoie le message
            int dist_2 = (getLocalTank().x - getGhostTank().x)*(getLocalTank().x - getGhostTank().x) + (getLocalTank().y - getGhostTank().y)*(getLocalTank().y - getGhostTank().y);
            if(dist_2 > 25)
            {
                if (sendto(sock, buf, strlen(buf) + 1, 0, adrDest, longueurAdr) <0) {
                      perror("sendto");
                      exit(0);
                }
                getGhostTank().x = getLocalTank().x;
                getGhostTank().y = getLocalTank().y;
                getGhostTank().angle = getLocalTank().angle;
                getGhostTank().velocity = getLocalTank().velocity;
            }

            // Exemple avec un seul socket :
            fd_set fdset;
            struct timeval date;
            // attendre au max pendant
            // 50 ms = 50000 us
            date.tv_sec = 0;
            date.tv_usec = 50000;
            FD_ZERO(&fdset);
            FD_SET(sock, &fdset);

            while(date.tv_usec > 0)
            {
                // un message est-il arrivé ?
                if (select(sock+1, &fdset, NULL, NULL, &date) > 0)
                {
                    if (recvfrom(sock, buf, sizeof(buf), 0, srcAdr, &longueurAdr) < 0) {
                        perror("recvfrom");
                        exit(0);
                    }


                    // on essaie de récupérer l'adresse IP et le port de l'émetteur
                    if((n = getnameinfo(srcAdr, longueurAdr, machine, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV)) == 0)
                    {
                        printf("recu : %s depuis %s:%s\n",buf, machine, service);

                        int id_;
                        float x_, y_, angle_, velocity_;
                        if(sscanf(buf, "D %d %f %f %f %f", &id_, &x_, &y_, &angle_, &velocity_) == 5);
                        {
                            if(tanks[id_] == NULL)
                                tanks[id_] = new Tank();

                            tanks[id_]->setTranslation(x_, y_);
                            tanks[id_]->setOrientation(angle_);

                        }
                    }
                    else {
                        printf("recu : %s\n",buf);
                        printf("erreur getnameinfo : %s\n", gai_strerror(n));
                    }
                }
            }
        }

        struct timeval date;
        gettimeofday(&date, NULL);
        double  time = date.tv_sec + date.tv_usec / 1000000.0;
        
        if (lastTime != -1) {
            localTank->update(time - lastTime);
            ghostTank->update(time- lastTime);
            for(int i = 0; i < 100; i++)
            {
                if(tanks[i] == NULL)
                {
                    tanks[i] = new Tank();
                }
                tanks[i]->update(time - lastTime);
            }
        }
        lastTime = time;
    }

        // Dessine un sol
        void drawFloor()
        {
            glBegin(GL_QUADS);
                glColor3f(0.3f, 0.7f, 0.3f);
                glVertex3f(-1000.0f,  1000.0f, 0.0f);
                glVertex3f( 1000.0f,  1000.0f, 0.0f);
                glVertex3f( 1000.0f, -1000.0f, 0.0f);
                glVertex3f(-1000.0f, -1000.0f, 0.0f);
            glEnd();
        }
 
        // Dessine le décors : des pyramides
        void drawScenery()
        {
          for(int i = 0; i < 10; i++) {
            for(int j = 0; j < 10; j++) {
              glPushMatrix();
              glTranslatef(i*200.f - 900.f, j*200.f - 1000.f, 0.f);
              glScalef(10.f, 10.f, 10.f);
              glColor3f(((float)i) / 10.0f, ((float)j) / 10.0f, ((float)i+j) / 20.0f);
              glBegin(GL_POLYGON);
                glVertex3f(0.0f, 0.0f, 1.0f);        //Top Of Triangle (Front)
                glVertex3f(-1.0f,1.0f, -1.0f);      //Left Of Triangle (Front)
                glVertex3f(1.0f, 1.0f, -1.0f);       //Right Of Triangle (Front)

                glVertex3f(0.0f, 0.0f, 1.0f);        //Top Of Triangle (Right)
                glVertex3f(1.0f, 1.0f, -1.0f);       //Left Of Triangle (Right)
                glVertex3f(1.0f, -1.0f, -1.0f);      //Right Of Triangle (Right)

                glVertex3f(0.0f, 0.0f, 1.0f);        //Top Of Triangle (Back)
                glVertex3f(1.0f, -1.0f, -1.0f);      //Left Of Triangle (Back)
                glVertex3f(-1.0f, -1.0f, -1.0f);     //Right Of Triangle (Back)

                glVertex3f(0.0f, 0.0f, 1.0f);        //Top Of Triangle (Left)
                glVertex3f(-1.0f, -1.0f, -1.0f);     //Left Of Triangle (Left)
                glVertex3f(-1.0f, 1.0f, -1.0f);      //Right Of Triangle (Left)
              glEnd();
              glPopMatrix();
            }
          }
        }  
  
  
    // dessine le monde
    void drawGL() {
        localTank->drawGL(true);
        drawFloor();
        drawScenery();
        for(int i=0; i < 100; i++)
        {
            if(tanks[i] == NULL)
            {
                tanks[i] = new Tank();
            }
            tanks[i]->drawGL(false);
        }
    }
  
};


/////////////// Le Main et les fonctions callback de la GLUT /////////////////

World *TheWorld = NULL;



/* The number of our GLUT window */
int window; 

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)	        /* We call this right after our OpenGL window is created. */
{
  glClearColor(0.2f, 0.2f, 0.7f, 0.0f);		/* This Will Clear The Background Color To Black */
  glClearDepth(1.0);				/* Enables Clearing Of The Depth Buffer */
  glDepthFunc(GL_LESS);				/* The Type Of Depth Test To Do */
  glEnable(GL_DEPTH_TEST);			/* Enables Depth Testing */
  glShadeModel(GL_SMOOTH);			/* Enables Smooth Color Shading */

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();				/* Reset The Projection Matrix */

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.001f,10000.0f);	/* Calculate The Aspect Ratio Of The
  Window*/

  glMatrixMode(GL_MODELVIEW);
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
  if (Height==0)				/* Prevent A Divide By Zero If The Window Is Too Small */
    Height=1;

  glViewport(0, 0, Width, Height);		/* Reset The Current Viewport And Perspective Transformation */

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.001f,10000.0f);
  glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void DrawGLScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		/* Clear The Screen And The Depth Buffer */
  glLoadIdentity();				/* Reset The View */

  TheWorld->drawGL();

  /* swap buffers to display, since we're double buffered.*/
  glutSwapBuffers();
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y) 
{
    /* If escape is pressed, kill everything. */
    if (key == ' ') TheWorld->getLocalTank().setVelocity(0.0f);
    else if (key == ESCAPE) 
    { 
	/* shut down our window */
	glutDestroyWindow(window); 
	
	/* exit the program...normal termination. */
	exit(0);                   
    }
}

void specialFunc(int key, int x, int y)  
{  
	switch (key) {	
		case GLUT_KEY_UP: // accelerate
            TheWorld->getLocalTank().accelerate();
			break;
		case GLUT_KEY_DOWN: // decelerate
            TheWorld->getLocalTank().decelerate();
			break;
		case GLUT_KEY_RIGHT: // turn right
            TheWorld->getLocalTank().turnRight();
			break;
		case GLUT_KEY_LEFT: // turn left
            TheWorld->getLocalTank().turnLeft();
			break;
		default: // do nothing
			break;
	}
}


void simulation() {
    TheWorld->update();
    glutPostRedisplay();
}


int main(int argc, char **argv) 
{  
    TheWorld = new World();

  
  /* Initialize GLUT state - glut will take any command line arguments that pertain to it or 
     X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */  
  glutInit(&argc, argv);  

  /* Select type of Display mode:   
     Double buffer 
     RGBA color
     Depth buffer */  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);  

  /* get a 640 x 480 window */
  glutInitWindowSize(640, 480);  

  /* the window starts at the upper left corner of the screen */
  glutInitWindowPosition(0, 0);  

  /* Open a window */  
  window = glutCreateWindow("Tank");  

  /* Register the function to do all our OpenGL drawing. */
  glutDisplayFunc(&DrawGLScene);  

  /* Go fullscreen.  This is the soonest we could possibly go fullscreen. */
  /* glutFullScreen(); */

  /* Even if there are no events, redraw our gl scene. */
  /* glutIdleFunc(&DrawGLScene); */

  /* Register the function called when our window is resized. */
  glutReshapeFunc(&ReSizeGLScene);

  /* Register the function called when the keyboard is pressed. */
  glutKeyboardFunc(&keyPressed);
  glutSpecialFunc(specialFunc);
  
  glutIdleFunc(simulation);

  /* Initialize our window. */
  InitGL(640, 480);
  
  /* Start Event Processing Engine */  
  glutMainLoop();  

  return 1;
}
