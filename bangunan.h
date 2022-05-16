#ifndef BANGUNAN_H
#define BANGUNAN_H

class Objek
{
private:
    float bx;
    float by;
    float bz;
    float bsize;
    int bhitval;

public:
    //setter
    void setX(float x);
    void setY(float y);
    void setZ(float z);
    void setSize(float s);
    void setHitValue(int v);

    //getter
    float getX();
    float getY();
    float getZ();
    float getSize();
    int getHitValue();
};

class Wall: public Objek
{
public:
    Wall(float x, float y, float z, float s, float cr, float cg, float cb); 
};

class Apartment: public Objek
{
public:
    Apartment(float x, float y, float z, float s, float cr, float cg, float cb, int stackbangunan);
};

class Pohon: public Objek
{
public:
	Pohon(float x, float y, float z, float s);
};

class Billboard: public Objek
{
public:
	Billboard(float x, float y, float z, float s);
};

#endif // BANGUNAN_H
