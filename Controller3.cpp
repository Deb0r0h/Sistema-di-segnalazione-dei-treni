#include<iostream>
#include "Controller.h"

using namespace std;
void Controller::stampa_tutto()
{
	for (int i = 0; i < stazioni_sx.size(); i++)
	{
		cout << stazioni_sx[i]->get_Nome() << " "<<stazioni_sx[i]->get_Distance() << endl;
	}
	cout << endl << endl;
	for (int i = 0; i < stazioni_dx.size(); i++)
	{
		cout << stazioni_dx[i]->get_Nome() << " " << stazioni_dx[i]->get_Distance() << endl;
	}
}

void Controller::simulation()
{
	while(treni.size()!=0)
	{	
		controllo_situazione();
		update();
	}
	cout << "FINE" << endl;
}
 void Controller::readfile(string line_d, string time_t)
 {
	 ifstream f1(line_d);
	 ifstream f2(time_t);
	 string line = "";
	 int i = 0;
	 string nome = "", temp = "";
	 int tipo_stazione = -1;
	 int distance = -1;
	 if (f1.is_open())
	 {
		 
		 getline(f1, line);
		 stazioni_sx.push_back(new Stazione_principale (line, 0));
		 index_position_sx.push_back(0);

		 //gestire i treni verso destra fare push_back in stazioni_dx
		 while (getline(f1, line))
		 {
			 for (i = 0; i < line.length(); i++)
			 {
				 if (!isdigit(line[i]))
				 {
					 nome += line[i];
				 }
				 else
					 break;
			 }
			 tipo_stazione = line[i] - '0';
			 for (int j = i + 1; j < line.length(); j++)
			 {
				 temp += line[j];
			 }
			 distance = stoi(temp);


			 if (tipo_stazione == 0)
			 {
				 stazioni_sx.push_back(new Stazione_principale (nome, distance));
				 index_position_sx.push_back(stazioni_sx.size() - 1);
			 }
			 else if (tipo_stazione == 1)
			 {				
				 stazioni_sx.push_back(new Stazione_locale (nome, distance));
			 }
			 nome = "";
			 temp = "";
			 distance = 0;
			 i = 0;
		 }
		 f1.close();
	 }

	 line = "";
	 int numero_treno = -1, tipo_treno = -1, partenza = -1;
	 //partenza:0 origine 1 capolinea

	 string s2 = "";
	 temp = "";
	 int riga_m = 0;
	 if (f2.is_open()) 
	 {
		 while (getline(f2, s2))
		 {
			 timetable.push_back(vector<int>());//inizializzo la riga della matrice
			 i = 0;
			 temp = "";
			 while (i < s2.length())
			 {
				 temp = "";
				 while (i < s2.length() && s2[i] != ' ')
				 {
					 temp = temp + s2[i];
					 i++;
				 }
				 timetable[riga_m].push_back(stoi(temp));

				 i++;
			 }
			 riga_m++;
		 }
		 f2.close();
		 cout << "dopo lettura file\n";
		 //set other vec qui
		 set_other_vector();
		 //cout << "prima di sistema tt";
		 sistema_tt();//controlla tt e la sistema
		 cout << "prima di check stazioni";
		 check_stazioni();//elimina stazioni troppo vicine e aggiorna tt
	 }
	 for (int j = 0; j < timetable.size(); j++) 
	 {
		int origin = timetable[j][1];
		if (timetable[j][2] == 1) 
		{
			treni.push_back(new Regionale(timetable[j][0], 0, origin));
		}
		else if (timetable[j][2] == 2) 
		{
			treni.push_back(new AltaVelocita (timetable[j][0], 0, origin));
		}
		else 
		{
			treni.push_back(new AltaVelocitaS (timetable[j][0], 0, origin));
		}
		
	 }
	 cout << "size di treni->" + treni.size();
	 
 }
