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

float dt = 1/60.;
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
    // drawPoint(p0);
    // drawPoint(p1);

	// drawPoint(p0);
 //    drawPoint(p1);	

    mss.draw();
    

}

void drawWorld() {
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

void update(float dt) {
    t += dt;
    // p1 = CalculateMidpoint(t);
    // p1 = CalculateRK4(t);
    mss.update(dt);
}

void keyPressed(int key) {
    // See http://www.glfw.org/docs/latest/group__keys.html for key codes
    if (key == GLFW_KEY_SPACE)
        paused = !paused;
    if (key == GLFW_KEY_ESCAPE)
        exit(0);

}

int main(int argc, char **argv) {
    window.create("Animation", 1024, 768);
    window.onKeyPress(keyPressed);
    camera.lookAt(vec3(1,1.5,5), vec3(0,0.5,0));
    lighting.createDefault();
    text.initialize();

    // mss.addMass(0,0,0,0); 
    // mss.addMass(1,1.5,0,0); 
    // mss.addSpring(1,1,mss.masses.at(0),mss.masses.at(1));
    int SIZE = 10;
    for(int i = 0 ; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            if(i == 0)
                mss.addMass(0,i/10.0-SIZE/2,j/10-SIZE/2.0,0);
            else
                mss.addMass(1,i/10.0-SIZE/2,j/10.0-SIZE/2,0);
            //Structural Springs
            if(j>0)
                mss.addSpring(1,0.15,mss.masses.at(i*SIZE+j-1),mss.masses.at(i*SIZE+j));
            if(i>0)
                mss.addSpring(1,0.15,mss.masses.at(i*SIZE+j),mss.masses.at((i-1)*SIZE+j));
            //Shear Springs
            if(i>0)
            {
                if(j<SIZE-1)
                    mss.addSpring(1,0.2,mss.masses.at(i*SIZE+j),mss.masses.at((i-1)*SIZE+j+1));
                if(j>0)
                    mss.addSpring(1,0.2,mss.masses.at(i*SIZE+j),mss.masses.at((i-1)*SIZE+j-1));
            }
            //Flexion Springs
            
            if(j>1)
                mss.addSpring(1,0.25,mss.masses.at(i*SIZE+j-2),mss.masses.at(i*SIZE+j));
            if(i>1)
                mss.addSpring(1,0.25,mss.masses.at(i*SIZE+j),mss.masses.at((i-2)*SIZE+j));
        }

    }


    while (!window.shouldClose()) {
        camera.processInput(window);
        if (!paused)
            update(dt);
        window.prepareDisplay();
        drawWorld();
        window.updateDisplay();
        window.waitForNextFrame(dt);
    }
}
