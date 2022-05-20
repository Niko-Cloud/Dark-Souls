#include "head.h"

//lantai dasar
void lantai(){
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
    glVertex3f(-20, 0, 300);
    glVertex3f(-20, 0, -300);
    glVertex3f(320, 0, -300);
    glVertex3f(320, 0, 300);
    glEnd();
}

void trotoar(float x){
	glPushMatrix();
	glColor3f(0.3, 0.3, 0.3);
	glTranslatef(0.0, 0.1,0.0);
    glBegin(GL_QUADS);
    glVertex3f(-x, 0, 20);
    glVertex3f(-x, 0, -20);
    glVertex3f(x, 0, -20);
    glVertex3f(x, 0, 20);
    glEnd();
	glPopMatrix();

    //strip pembatas jalan
    int i;
    glPushMatrix();
    glColor3f(1, 0.5, 0);
    for(i = -x+10; i < x-50; i++){
        glBegin(GL_QUADS);
        glVertex3f(i, 0.2, 0.5);
        glVertex3f(i, 0.2, -0.5);
        glVertex3f(i+40, 0.2, -0.5);
        glVertex3f(i+40, 0.2, 0.5);
        glEnd();
    }
    glPopMatrix();
}

void Jalan()
{
    // Fungsi untuk membuat grid di "lantai"
    double i;
    const float Z_MIN = -50, Z_MAX = 50;
    const float X_MIN = -50, X_MAX = 50;
    const float gap = 10.0;


    //Push Pertama untuk lantai dasar dll agak kebawah
    glPushMatrix();
    glTranslatef(0.0, -0.5,0.0);

    //lantai dasar
    lantai();

	//Push jalan 6
	glPushMatrix();
	//rotasi ke kanan
	glRotated(90, 0, 1, 0);
	//Trotoar dan strip jalan
	trotoar(280);

    glPopMatrix();

    //push jalan 7
    glPushMatrix();
    glTranslatef(170.0, 0.0, -300.0);
    trotoar(190);
    //pop jalan 7
    glPopMatrix();

	//push jalan 8
    glPushMatrix();
    glTranslatef(170.0, 0.0, 300.0);
    trotoar(190);
    //pop jalan 8
    glPopMatrix();

	 //Push jalan 9
	glPushMatrix();
	//rotasi ke kanan
	glRotated(90, 0, 1, 0);
	glTranslatef(0.0, 0.0, 340.0);
	//Trotoar dan strip jalan
	trotoar(280);
	//pop jalan 9
	glPopMatrix();

	//pop end
	glPopMatrix();

}
