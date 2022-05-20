//TUBES GKV

#include "head.h"
#include "truk.h"
#include "map.h"
#include "bangunan.h"

float angle = 8.0;
float deltaAngle = 0.0;
float ratio;
float deltaMove = 0,h,w; //for debug cam
int bitmapHeight=12;

//debug and dev
int debugCamera = 0;

//global var buat game mechanics
float speedX = 0.0, speedZ = 0.0; //pergerakan truk
float putaranTruk = 90.0; //jgn di set negatif
int gasDitekan = 0;
int setirDitekan = 0; //-1 kiri 1 kanan 0 gak ditekan
float akselerasiDefault = 0.05;
float akselerasi = 0.0;
float dekselerasi = 0.05;
float cx=0.0, cy=0.0, cz=0.0;
float x=22.5f,y=10.0f,z=22.5f; // posisi awal kamera
float angleCam = 90.0; //jgn di set negatif
float lx=0.0f,ly=0.0f,lz=-1.0f;
float tx=0.0, ty=0.0, tz=0.0; //posisi truk (jgn diubah)
extern float cpDepanX, cpDepanZ, cpBelakangX, cpBelakangZ;//shared global variable collision point

//renders building
int buildings = 1;

void Reshape(int w1, int h1){
    // Fungsi reshape
    if(h1 == 0) h1 = 1;
    w = w1;
    h = h1;
    ratio = 1.0f * w / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45,ratio,0.1,500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(tx, y, tz, x + lx,y + ly,z + lz, 0.0f,1.0f,0.0f);
}

void moveCamFlat(float i){
    //kamera
    glLoadIdentity();
    i = i - 90;
    gluLookAt(tx+x*sin(i*M_PI/180), y, tz+z*cos(i*M_PI/180), tx, ty+y/2, tz, 0.0f,1.0f,0.0f);
}


void orientMe(float ang){
    //kamera debug
    // Fungsi ini untuk memutar arah kamera (tengok kiri/kanan)
    lx = sin(ang);
    lz = -cos(ang);
    glLoadIdentity();
    gluLookAt(cx, y, cz, cx + lx,y + ly,cz + lz, 0.0f,1.0f,0.0f);
}

void moveMeFlat(float i){
    //kamera debug
    // Fungsi ini untuk maju mundur kamera
    cx = cx + i*(lx)*0.1;
    cz = cz + i*(lz)*0.1;
    glLoadIdentity();
    gluLookAt(cx, y, cz, cx + lx,y + ly,cz + lz, 0.0f,1.0f,0.0f);
}

void moveTruk(float putaran, float deltaX){
    float deltaMundur = float(abs(180-putaran));
    tx = tx + (deltaX)*0.1*(90.0-deltaMundur)/-90;
    tz = tz + (deltaX)*0.1*-sin(putaran*M_PI/180)*(1-abs((90.0-deltaMundur)/-90));
}

int CollisionRule(Objek objek, int *existance) {
    float oMinX, oMaxX, oMinZ, oMaxZ; //objek min x, objek max x, dll.
    //jarak dari kaca depan truk ke koordinat ditengah(0,0,0) itu 10.3

    oMinX = objek.getX() - (objek.getSize()/2) - 1.6; //lebar truk 3.2 (kiri 1,6, kanan 1,6);
    oMaxX = objek.getX() + (objek.getSize()/2) + 1.6;
    oMinZ = objek.getZ() - (objek.getSize()/2) - 1.6;
    oMaxZ = objek.getZ() + (objek.getSize()/2) + 1.6;

    if(cpDepanX >= oMinX && cpDepanX <= oMaxX && cpDepanZ >= oMinZ && cpDepanZ <= oMaxZ){
        if (!objek.getHitValue()){
            if (speedX>0){
                speedX = 0.0;
            }
        }
    }

    if(cpBelakangX >= oMinX && cpBelakangX <= oMaxX && cpBelakangZ >= oMinZ && cpBelakangZ <= oMaxZ){
         if (!objek.getHitValue()){
            if (speedX<0){
                speedX = 0.0;
            }
        }
    }
    return 0;
}

void restorePerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClearColor(0.53, 0.77, 0.87, 0);
    if (!debugCamera){
        //pergerakan truk
        if (speedX){
            moveTruk(putaranTruk, speedX*2);
        }
        if (!gasDitekan){
            if (speedX>0.1){
                deltaMove -= dekselerasi;
                speedX -= dekselerasi;
            }else if (speedX<-0.1){
                deltaMove += dekselerasi;
                speedX += dekselerasi;
            }else{
                deltaMove = 0;
                speedX = 0;
            }
        }else{
            if (speedX < 2.5 && speedX > -2){
                speedX += akselerasi;
            }
            if (setirDitekan){
                putaranTruk -= setirDitekan/abs(setirDitekan);
                if (putaranTruk<0){
                    putaranTruk = 360 + putaranTruk;
                    if (angleCam<360){
                        angleCam += 360;
                    }
                }else if (putaranTruk>360){
                    putaranTruk -= 360;
                    if (angleCam>0){
                        angleCam -= 360;
                    }
                }
            }
        }

        //gerakan camera
        if (abs(angleCam-putaranTruk)<1){
            angleCam = putaranTruk;
        }else if(putaranTruk>0 && putaranTruk<360){
            if (angleCam<putaranTruk){
                angleCam+=0.75;
            }else{
                angleCam-=0.75;
            }
        }
        moveCamFlat(angleCam);
    }else{
        if (deltaMove){
            moveMeFlat(deltaMove*2);
        }
        if (deltaAngle) {
            angle += deltaAngle;
            orientMe(angle*0.2);
        }
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_LIGHTING);
    Jalan();

    if (buildings){

        //Side 1
		Apartment Apartment11(35, 0,210,20,0.1,0.5,0.8,3);
        CollisionRule(Apartment11, &buildings);
        Apartment Apartment12(40, 0,180,30,0.9,0.2,0.9,1);
        CollisionRule(Apartment12, &buildings);
        Apartment Apartment13(45, 0,140,40,0.9,0.2,0.9,2);
        CollisionRule(Apartment13, &buildings);
		Apartment Apartment14(35, 0,-80,20,0.1,0.5,0.8,3);
        CollisionRule(Apartment14, &buildings);
        Apartment Apartment15(40, 0,-40,30,0.9,0.2,0.9,1);
        CollisionRule(Apartment15, &buildings);
        Apartment Apartment16(45, 0,-0,40,0.9,0.2,0.9,2);
        CollisionRule(Apartment16, &buildings);
        Apartment Apartment17(50, 0,-175,50,0.9,0.2,0.9,2);
        CollisionRule(Apartment17, &buildings);
		Sampah Sampah11(30, 0,85,5);
        CollisionRule(Sampah11, &buildings);
		Sampah Sampah12(30, 0,-63,5);
        CollisionRule(Sampah12, &buildings);
        Billboard Billboard11(30, 0,100,2.2);
        CollisionRule(Billboard11, &buildings);
        Billboard Billboard12(30, 0,-212,2.2);
        CollisionRule(Billboard12, &buildings);
		Pohon Pohon11(30, 0,50,1.5);
        CollisionRule(Pohon11, &buildings);
		Pohon Pohon12(30, 0,30,1.5);
        CollisionRule(Pohon12, &buildings);
		Pohon Pohon13(30, 0,70,1.5);
        CollisionRule(Pohon13, &buildings);
        Pohon Pohon15(30, 0,-100,1.5);
        CollisionRule(Pohon15, &buildings);
		Pohon Pohon14(30, 0,-120,1.5);
        CollisionRule(Pohon14, &buildings);
		Pohon Pohon16(30, 0,-140,1.5);
        CollisionRule(Pohon16, &buildings);

        //Sisi 2
		Apartment Apartment21(50, 0,250,50,0.1,0.5,0.8,1);
        CollisionRule(Apartment21, &buildings);
        Apartment Apartment22(100, 0,260,30,0.9,0.2,0.9,2);
        CollisionRule(Apartment22, &buildings);
		Pohon Pohon21(130, 0,270,1.5);
        CollisionRule(Pohon21, &buildings);
		Pohon Pohon22(150, 0,270,1.5);
        CollisionRule(Pohon22, &buildings);
		Sampah Sampah21(160, 0,270,5);
        CollisionRule(Sampah21, &buildings);
		Sampah Sampah23(175, 0,270,5);
        CollisionRule(Sampah23, &buildings);
		Sampah Sampah22(190, 0,270,5);
        CollisionRule(Sampah22, &buildings);
		Pohon Pohon23(200, 0,270,1.5);
        CollisionRule(Pohon23, &buildings);
		Pohon Pohon24(220, 0,270,1.5);
        CollisionRule(Pohon24, &buildings);
		Apartment Apartment23(290, 0,250,50,0.1,0.5,0.8,1);
        CollisionRule(Apartment23, &buildings);
        Apartment Apartment24(245, 0,260,30,0.9,0.2,0.9,2);
        CollisionRule(Apartment24, &buildings);
		
        //Side 3
		Apartment Apartment31(305, 0,210,20,0.1,0.5,0.8,3);
        CollisionRule(Apartment31, &buildings);
        Apartment Apartment32(300, 0,180,30,0.9,0.2,0.9,1);
        CollisionRule(Apartment32, &buildings);
        Apartment Apartment33(295, 0,140,40,0.9,0.2,0.9,2);
        CollisionRule(Apartment33, &buildings);
		Apartment Apartment34(305, 0,-80,20,0.1,0.5,0.8,3);
        CollisionRule(Apartment34, &buildings);
        Apartment Apartment35(300, 0,-40,30,0.9,0.2,0.9,1);
        CollisionRule(Apartment35, &buildings);
        Apartment Apartment36(295, 0,-0,40,0.9,0.2,0.9,2);
        CollisionRule(Apartment36, &buildings);
        Apartment Apartment37(290, 0,-175,50,0.9,0.2,0.9,2);
        CollisionRule(Apartment37, &buildings);
		Sampah Sampah31(315, 0,85,5);
        CollisionRule(Sampah31, &buildings);
		Sampah Sampah32(315, 0,-63,5);
        CollisionRule(Sampah32, &buildings);
        Billboard Billboard31(315, 0,100,2.2);
        CollisionRule(Billboard31, &buildings);
        Billboard Billboard32(315, 0,-212,2.2);
        CollisionRule(Billboard32, &buildings);
		Pohon Pohon31(315, 0,50,1.5);
        CollisionRule(Pohon31, &buildings);
		Pohon Pohon32(315, 0,30,1.5);
        CollisionRule(Pohon32, &buildings);
		Pohon Pohon33(315, 0,70,1.5);
        CollisionRule(Pohon33, &buildings);
        Pohon Pohon35(315, 0,-100,1.5);
        CollisionRule(Pohon35, &buildings);
		Pohon Pohon34(315, 0,-120,1.5);
        CollisionRule(Pohon34, &buildings);
		Pohon Pohon36(315, 0,-140,1.5);
        CollisionRule(Pohon36, &buildings);


        //Sisi 4
		Apartment Apartment41(50, 0,-250,50,0.1,0.5,0.8,1);
        CollisionRule(Apartment41, &buildings);
        Apartment Apartment42(100, 0,-260,30,0.9,0.2,0.9,2);
        CollisionRule(Apartment42, &buildings);
		Pohon Pohon41(130, 0,-270,1.5);
        CollisionRule(Pohon41, &buildings);
		Pohon Pohon42(150, 0,-270,1.5);
        CollisionRule(Pohon42, &buildings);
		Sampah Sampah41(160, 0,-270,5);
        CollisionRule(Sampah41, &buildings);
		Sampah Sampah43(175, 0,-270,5);
        CollisionRule(Sampah43, &buildings);
		Sampah Sampah42(190, 0,-270,5);
        CollisionRule(Sampah42, &buildings);
		Pohon Pohon43(200, 0,-270,1.5);
        CollisionRule(Pohon43, &buildings);
		Pohon Pohon44(220, 0,-270,1.5);
        CollisionRule(Pohon44, &buildings);
		Apartment Apartment43(290, 0,-250,50,0.1,0.5,0.8,1);
        CollisionRule(Apartment43, &buildings);
        Apartment Apartment44(245, 0,-260,30,0.9,0.2,0.9,2);
        CollisionRule(Apartment44, &buildings);
        
    }

    Truk(putaranTruk, tx, ty, tz);
    glutSwapBuffers();
    glFlush();
}

void pressKey(int key, int x, int y) {
    // Fungsi ini akan dijalankan saat tombol keyboard ditekan dan belum dilepas
    if (debugCamera){
        switch (key) {
            case GLUT_KEY_LEFT : deltaAngle = -0.1f;break;
            case GLUT_KEY_RIGHT : deltaAngle = 0.1f;break;
            case GLUT_KEY_UP : deltaMove = 1;break;
            case GLUT_KEY_DOWN : deltaMove = -1;break;
        }
    }else{
        switch (key) {
            case GLUT_KEY_LEFT :
                if (gasDitekan>0){
                    setirDitekan = -1;
                }else if (gasDitekan<0){
                    setirDitekan = 1;
                }else{
                    setirDitekan = -1;
                }
            break;
            case GLUT_KEY_RIGHT :
                if (gasDitekan>0){
                    setirDitekan = 1;
                }else if (gasDitekan<0){
                    setirDitekan = -1;
                }else{
                    setirDitekan = 1;
                }
            break;
            case GLUT_KEY_UP :
                akselerasi = akselerasiDefault;
                gasDitekan = 1;
            break;
            case GLUT_KEY_DOWN :
                akselerasi = -akselerasiDefault;
                gasDitekan = -1;
                if (setirDitekan==1){
                    setirDitekan = -2;
                }else if (setirDitekan==-1){
                    setirDitekan = 2;
                }
            break;
        }
    }
}

void releaseKey(int key, int x, int y) {
    // Fungsi ini akan dijalankan saat tekanan tombol keyboard dilepas
    if (debugCamera){
        switch (key) {
            case GLUT_KEY_LEFT :
                if (deltaAngle < 0.0f)
                deltaAngle = 0.0f;
            break;
            case GLUT_KEY_RIGHT :
                if (deltaAngle > 0.0f)
                deltaAngle = 0.0f;
            break;
            case GLUT_KEY_UP :
                if (deltaMove > 0)
                deltaMove = 0;
            break;
            case GLUT_KEY_DOWN :
                if (deltaMove < 0)
                deltaMove = 0;
            break;
        }
    }else{
        switch (key) {
            case GLUT_KEY_LEFT :
                setirDitekan = 0;
            break;
            case GLUT_KEY_RIGHT :
                setirDitekan = 0;
            break;
            case GLUT_KEY_UP :
                gasDitekan = 0;
            break;
            case GLUT_KEY_DOWN :
                gasDitekan = 0;
            break;
        }
    }
}

// Variable untuk pencahayaan
const GLfloat light_ambient[] = { 0.5f, 0.5f, 0.5f, 0.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 0.0f, 20.0f, 10.0f, 1.0f };
const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

void lighting(){
    // Fungsi mengaktifkan pencahayaan
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}
void init(void){
    glEnable (GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(640,480);
    glutCreateWindow("Demo");
    glutIgnoreKeyRepeat(0);
    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(Reshape);
    lighting();
    init();
    glutMainLoop();
    return(0);
}
