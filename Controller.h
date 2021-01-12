//#ifndef Controller_h
//#define Controller_h

#ifndef Controller_h
#define Controller_h

#include "Stazione.h"
#include <string>
#include "Treno.h"
#include "Binario.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

class Controller 
{
private:
    const int delta_t = 12; //12 secondi tempo di avs a max speed di fare un km facciamo un controllo ogni 12 secondi       
    bool fine = false;
    void readfile(std::string line_d, std::string time_t);
    void set_other_vector();
    int current_time; 
    int limit = 7200;    
    const int min_distance = 20;
    std::vector<Stazione*> stazioni_sx, stazioni_dx;
    std::vector<std::vector<int>> timetable; //matrice timetable
    std::vector<Treno*> treni;
    std::vector<int> index_position_sx;
    
public:
    Controller(); //in input i 2 file, inizializza vector/array di stazioni e treni
    void simulation();
    void sistema_tt();//ricc tt
    int calcMin(int dist, int partenza, int tipo);//ricc tt
    int calc10min(int dist, int partenza, int tipo);//ricc tt
    void check_stazioni();//elimina stazioni troppo vicine
    void stampa_tutto();
    void controllo_situazione();
    void update();
    void selectionSort(std::vector<Treno*> &treni);
    ~Controller();
};

 int getPos(std::vector<int>v, int x);

#endif