#include "randomobject.h"
#include "common.h"

void Objek::setX(float x)
{
    bx = x;
}

void Objek::setY(float y)
{
    by = y;
}

void Objek::setZ(float z)
{
    bz = z;
}

void Objek::setSize(float s)
{
    bsize = s;
}

void Objek::setHitValue(int v){
    bhitval = v;
}

float Objek::getX()
{
    return bx;
}

float Objek::getY()
{
    return by;
}

float Objek::getZ()
{
    return bz;
}

float Objek::getSize()
{
    return bsize;
}

int Objek::getHitValue()
{
    return bhitval;
}

Sampah::Sampah(float x, float y, float z, float s){
	setX(x);
    setY(y);
    setZ(z);
    setSize(s);
    setHitValue(0);
    GLUquadricObj *quadratic;
    GLUquadricObj *quadric;
    quadratic = gluNewQuadric();
    quadric = gluNewQuadric();

    glPushMatrix();
        glTranslatef(getX(),getY(), getZ());
        glScalef(s,s,s);
        //batang
    	GLUquadricObj *pObj;
    	pObj = gluNewQuadric();
    	gluQuadricNormals(pObj, GLU_SMOOTH);
    	glPushMatrix();
    		glColor3ub(10, 60, 50);
    		glRotated(270, 1, 0, 0);
    		glTranslatef(0,0,0);
    		gluCylinder(pObj, 1, 1, 1, 50, 5);
    	glPopMatrix();
	glPopMatrix();
	glFlush();
}

Apartment::Apartment(float x, float y, float z, float s, float cr, float cg, float cb, int stackbangunan){
    setX(x);
    setY(y);
    setZ(z);
    setSize(s);
    setHitValue(0);

    for (int i=0;i<jumlahTumpuk;i++){
        glPushMatrix();
        glTranslatef(getX(),getY()+getSize()*i, getZ());
            glPushMatrix();
                glColor3f(cr, cg, cb);
                glutSolidCube(getSize());
            glPopMatrix();
        glPopMatrix();
    }
}

//pohon
Pohon::Pohon(float x, float y, float z, float s){
	setX(x);
    setY(y);
    setZ(z);
    setSize(s);
    setHitValue(0);
    GLUquadricObj *quadratic;
    GLUquadricObj *quadric;
    quadratic = gluNewQuadric();
    quadric = gluNewQuadric();

    glPushMatrix();
        glTranslatef(getX(),getY(), getZ());
        glScalef(s,s,s);
        //batang
    	GLUquadricObj *pObj;
    	pObj = gluNewQuadric();
    	gluQuadricNormals(pObj, GLU_SMOOTH);
    	glPushMatrix();
    		glColor3ub(105, 49, 0);
    		glRotated(270, 1, 0, 0);
    		glTranslatef(0,0,0);
    		gluCylinder(pObj, 1, 0.5, 15, 10, 10);
		glPopMatrix();
        //daun 1
        glPushMatrix();
            glColor3ub(18,118,13);
            glScaled(9, 9, 9);
            glRotated(90, 0, 1, 0);
            glTranslatef(0,1.4,0);
            glutSolidCube(1);
        glPopMatrix();
	glPopMatrix();
	glFlush();
}
}

Billboard::Billboard(float x, float y, float z, float s){
	setX(x);
    setY(y);
    setZ(z);
    setSize(s);
    setHitValue(0);

    glPushMatrix();
        glTranslatef(getX(),getY(), getZ());
        glScalef(s,s,s);
        //Tiang
    	glPushMatrix();
    		glColor3ub(98, 98, 98);
    		glScaled(1, 9, 1);
    		glRotated(270, 1, 0, 0);
    		glTranslatef(0,0,0);
    		glutSolidCube(1);
		glPopMatrix();
        //Papan Billboard
        glPushMatrix();
            glColor3ub(0,0,0);
            glScaled(1, 6, 10);
            glRotated(-90, 1, 0, 0);
            glTranslatef(0,0,1.2);
            glutSolidCube(1);
        glPopMatrix();
	glPopMatrix();
	glFlush();
}
