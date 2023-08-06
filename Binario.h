//#pragma once
#ifndef Binario_h
#define Binario_h

class Binario 
{   

protected:
    Binario(bool s);
    virtual unsigned short limit_speed()=0;
    bool status;
};

class Binario_standard : public Binario 
{
public:
    Binario_standard(bool s);
    unsigned short limit_speed()override;
};
class Binario_transito : public Binario 
{
public:
    Binario_transito(bool s);
    unsigned short limit_speed()override;
};

#endif
