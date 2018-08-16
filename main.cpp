#include "camera.hpp"
#include "draw.hpp"
#include "gui.hpp"
#include "lighting.hpp"
#include "text.hpp"

#include "mass.hpp"
#include "system.hpp"
#include "spring.hpp"


#include <cmath>

using namespace std;

Window window;
Camera camera;
Lighting lighting;
Text text;
MassSpringSystem mss;


//PARAMETERS    
int OPTION = 2; // 0 = single spring | 1 = normal cloth | 2 = Flag in wind
int CHOICE = 1; // Integrators:  0 = Symplectic Euler | 1 = Midpoint Method | 3 = RK4
int SIZE = 10;
float MASS = 0.01;
float STRUCT_CONS = 10.0;
float SHEAR_CONS = 20.0;
float FLEXION_CONS = 1.0;
float STRUCT_LEN = 0.05;
float SHEAR_LEN = 0.1;
float FLEXION_LEN = 0.1;
vec3 GRAVITY = vec3(0.0, -9.81, 1.0);
float dt = 0.001;
float t = 0;
bool paused = false;
vec3 p0, p1;

void drawStuff() {
    
    // setColor(vec3(0.8,0.2,0.2));
    // drawArrow(vec3(0,0,0), vec3(1,0,0), 0.05);
    // setColor(vec3(0.2,0.6,0.2));
    // drawArrow(vec3(0,0,0), vec3(0,1,0), 0.05);
    // setColor(vec3(0.2,0.2,0.8));
    // drawArrow(vec3(0,0,0), vec3(0,0,1), 0.05);
    // setPointSize(10);
    // setColor(vec3(0.2,0.2,0.2));
    
    if(OPTION==0)
    {
        setPointSize(10);
        drawPoint(mss.masses.at(0)->position);
        drawPoint(mss.masses.at(1)->position);
        drawLine(mss.masses.at(0)->position,mss.masses.at(1)->position);
    }
    else if (OPTION==1)
    {
        setPointSize(10);
        drawPoint(p0);
        drawPoint(p1);
        setColor(vec3(0.8,0.2,0.2));
        for (int i=1; i<SIZE; i++) 
        {
            for (int j = 1; j < SIZE; ++j)
            {
                drawQuad(mss.masses.at(SIZE*i+j)->position,mss.masses.at(SIZE*i+j-1)->position,mss.masses.at(SIZE*(i-1)+j-1)->position,mss.masses.at(SIZE*(i-1)+j)->position);
            }
        }
    }
    else
    {
        setColor(vec3(0.0,0.0,0.0));
        setLineWidth(4);
        drawLine(vec3(0.0,0.0,0.0),vec3(0,-SIZE*STRUCT_LEN*4,0));
        setColor(vec3(0.2,0.2,0.8));
        for (int i=1; i<SIZE; i++) 
        {
            for (int j = 1; j < SIZE; ++j)
            {
                drawQuad(mss.masses.at(SIZE*i+j)->position,mss.masses.at(SIZE*i+j-1)->position,mss.masses.at(SIZE*(i-1)+j-1)->position,mss.masses.at(SIZE*(i-1)+j)->position);
            }
        }
    }
}

void drawWorld() 
{
    camera.apply(window);
    lighting.apply();
    clear(vec3(0.9,0.9,0.9));
    // setColor(vec3(0.7,0.7,0.7));
    // drawQuad(vec3(-3,0,-3), vec3(-3,0,3), vec3(3,0,3), vec3(3,0,-3));
    drawStuff();
    setColor(vec3(0,0,0));
    text.draw("WASD and LShift/LCtrl to move camera", -0.9, 0.90);
    text.draw("Mouse to rotate view", -0.9, 0.85);
    text.draw("Space to play/pause animation", -0.9, 0.80);
}

void update(float dt) 
{
    t += dt;
    mss.update(dt,CHOICE,GRAVITY);
}

void keyPressed(int key) 
{
    // See http://www.glfw.org/docs/latest/group__keys.html for key codes
    if (key == GLFW_KEY_SPACE)
        paused = !paused;
    if (key == GLFW_KEY_ESCAPE)
        exit(0);
}

int main(int argc, char **argv) 
{
    window.create("Animation", 1024, 768);
    window.onKeyPress(keyPressed);
    camera.lookAt(vec3(1,1.5,5), vec3(0,0,0));
    lighting.createDefault();
    text.initialize();

    if (OPTION==0)
    {
        GRAVITY = vec3(0,0,0);
        mss.addMass(0,0,0,0); 
        mss.addMass(1,1.5,0,0); 
        mss.addSpring(1,1,mss.masses.at(0),mss.masses.at(1)); 
    }
    
    if(OPTION==1) // normal cloth
    {
        p0 = vec3(0,0,0);
        p1 = vec3((SIZE-1)/10.0,0,0);

        for(int i = 0 ; i < SIZE; ++i)
        {
            for (int j = 0; j < SIZE; ++j)
            {
                if(i == 0 && (j == 0 || j == SIZE-1))
                    mss.addMass(0,j/10.0,-i/10.0,0);
                else
                    mss.addMass(MASS,j/10.0,0,-i/10.0);
                //Structural Springs
                if(j>0)
                    mss.addSpring(STRUCT_CONS,STRUCT_LEN,mss.masses.at(i*SIZE+j-1),mss.masses.at(i*SIZE+j));
                if(i>0)
                    mss.addSpring(STRUCT_CONS,STRUCT_LEN,mss.masses.at(i*SIZE+j),mss.masses.at((i-1)*SIZE+j));
                //Shear Springs
                if(i>0)
                {
                    if(j<SIZE-1)
                        mss.addSpring(SHEAR_CONS,SHEAR_LEN,mss.masses.at(i*SIZE+j),mss.masses.at((i-1)*SIZE+j+1));
                    if(j>0)
                        mss.addSpring(SHEAR_CONS,SHEAR_LEN,mss.masses.at(i*SIZE+j),mss.masses.at((i-1)*SIZE+j-1));
                }
                //Flexion Springs
                if(j>1)
                    mss.addSpring(FLEXION_CONS,FLEXION_LEN,mss.masses.at(i*SIZE+j-2),mss.masses.at(i*SIZE+j));
                if(i>1)
                    mss.addSpring(FLEXION_CONS,FLEXION_LEN,mss.masses.at(i*SIZE+j),mss.masses.at((i-2)*SIZE+j));
            }
        }
    }
    else if (OPTION==2) // flag in wind
    {
        for(int i = 0 ; i < SIZE; ++i)
        {
            for (int j = 0; j < SIZE; ++j)
            {
                if(j==0)
                    mss.addMass(0,j/10.0,-i/10.0,0);
                else
                    mss.addMass(MASS,j/10.0,-i/10.0,0);
                //Structural Springs
                if(j>0)
                    mss.addSpring(STRUCT_CONS,STRUCT_LEN,mss.masses.at(i*SIZE+j-1),mss.masses.at(i*SIZE+j));
                if(i>0)
                    mss.addSpring(STRUCT_CONS,STRUCT_LEN,mss.masses.at(i*SIZE+j),mss.masses.at((i-1)*SIZE+j));
                //Shear Springs
                if(i>0)
                {
                    if(j<SIZE-1)
                        mss.addSpring(SHEAR_CONS,SHEAR_LEN,mss.masses.at(i*SIZE+j),mss.masses.at((i-1)*SIZE+j+1));
                    if(j>0)
                        mss.addSpring(SHEAR_CONS,SHEAR_LEN,mss.masses.at(i*SIZE+j),mss.masses.at((i-1)*SIZE+j-1));
                }
                //Flexion Springs
                if(j>1)
                    mss.addSpring(FLEXION_CONS,FLEXION_LEN,mss.masses.at(i*SIZE+j-2),mss.masses.at(i*SIZE+j));
                if(i>1)
                    mss.addSpring(FLEXION_CONS,FLEXION_LEN,mss.masses.at(i*SIZE+j),mss.masses.at((i-2)*SIZE+j));
            }
        }
    }

    while (!window.shouldClose()) 
    {
        camera.processInput(window);
        if (!paused)
            update(dt);
        window.prepareDisplay();
        drawWorld();
        window.updateDisplay();
        window.waitForNextFrame(dt);
    }
}
