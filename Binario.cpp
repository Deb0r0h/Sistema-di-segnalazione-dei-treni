#include "Binario.h"

Binario::Binario(bool s): status{s} {}

Binario_standard::Binario_standard(bool s):Binario(s){}

Binario_transito::Binario_transito(bool s):Binario(s){}

unsigned short Binario_standard::limit_speed()
{
    return 80;
}

unsigned short Binario_transito::limit_speed()
{
    return 400;
}
