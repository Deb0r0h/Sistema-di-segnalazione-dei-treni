//@author:Riccardo Patrizio


#include "Controller.h"
#include "Controller3.cpp"
#include "Controller2.cpp"

using namespace std;
Controller::Controller()
{
	readfile("line_description.txt","pollo.txt");
	current_time = 0;
}

void Controller::sistema_tt()
{
	cout << "stampo nome" << endl;
	cout << stazioni_sx[1]->get_Nome() << endl;
    int i = 0;
    int j = 4;
    int time = 0;
    int tipo = 0;
    int from = 0;
    int supposed_size;//grandezza che dovrebbe avere
    int orarioMin;

    while(i<timetable.size())
	{
		j = 4;
        if(timetable[i][3]>=24*60)//elimina treni che partono il giorno dopo
		{
            timetable.erase(timetable.begin()+i);
            if(i>=timetable.size())
			{
                break;
            }
        }
        from = timetable[i][1]; //
        tipo = timetable[i][2];
        if(tipo==1){//grandezza che dovrebbe avere riga
            supposed_size = stazioni_sx.size()+3;
        }else{
            supposed_size = index_position_sx.size()+3;
        }

        while(j<timetable[i].size() && j < supposed_size)//finche' non finisco la riga o la supp 
		{	
			if (from == 0)
			{
				if (tipo == 1)//se regionale
				{
					orarioMin = calcMin(stazioni_sx[j - 3]->get_Distance() - stazioni_sx[j - 4]->get_Distance(), timetable[i][j - 1], tipo);
				}
				else	//se veloce

					orarioMin = calcMin(stazioni_sx[index_position_sx[j - 3]]->get_Distance() - stazioni_sx[index_position_sx[j - 4]]->get_Distance(), timetable[i][j - 1], tipo);
			
			}
			else
			{
				if(tipo == 1)//se regionale
				{
					orarioMin = calcMin(stazioni_dx[j-3]->get_Distance()-stazioni_dx[j-4]->get_Distance(), timetable[i][j-1], tipo);
				}
				else
				{
					orarioMin = calcMin(stazioni_dx[stazioni_dx.size()-1-index_position_sx[index_position_sx.size()-(j-3)-1]] -> get_Distance() - 
					stazioni_dx[stazioni_dx.size()-1-index_position_sx[index_position_sx.size()-(j-4)-1]] -> get_Distance(),timetable[i][j-1], tipo);
				}
			}
            if(timetable[i][j] < orarioMin)//se orario impossibile
			{ 
                    timetable[i][j] = orarioMin;
            }
			//cout << "j->" << j << endl;
            j++;
        }
		if (timetable[i].size() > supposed_size) 
		{//se troppi dati
			timetable[i].erase(timetable[i].begin() + j, timetable[i].end());
		}
        if(timetable[i].size() < supposed_size)
		{//se mancano dati
            if(from == 0)//se da sx
			{
                while(j < supposed_size )
				{
						if(tipo==1)
						{
							timetable[i].push_back(calc10min(stazioni_sx[j-3]->get_Distance()-stazioni_sx[j-4]->get_Distance(), timetable[i][j-1], tipo));
						}else
						{
							
							timetable[i].push_back(calc10min(stazioni_sx[index_position_sx[j-3]]->get_Distance()-stazioni_sx[index_position_sx[j-4]]->get_Distance(), timetable[i][j-1], tipo));
						}
					
					j++;
                }
            }else//se da dx
			{
                while(j < supposed_size )
				{
                    if(tipo==1)
					{
                        timetable[i].push_back(calc10min(stazioni_dx[j-3]->get_Distance()-stazioni_dx[j-4]->get_Distance(), timetable[i][j-1], tipo));
                    }
					else
					{	//	
                        timetable[i].push_back(calc10min(stazioni_dx[stazioni_dx.size()-1-index_position_sx[index_position_sx.size()-(j-3)-1]]->get_Distance()
						-stazioni_dx[stazioni_dx.size()-1-index_position_sx[index_position_sx.size()-(j-4)-1]]->get_Distance(), timetable[i][j-1], tipo));//
                    }
					j++;
                }
            }
        }
        i++;
    }

}

int Controller::calcMin(int dist, int partenza, int tipo)
{
	double tempo;
    int speed = 0;
    if(tipo == 0)
	{
        speed = 160;
    }else if(tipo == 1)
	{
        speed = 240;
    }else
	{
        speed = 300;
    }
    tempo = (double)partenza + (10*60/80) + ((dist-10)*60/speed);
    return std::ceil(tempo);
}

int Controller::calc10min(int dist, int partenza, int tipo)
{
    return (calcMin(dist, partenza, tipo))+10;
}
void Controller::check_stazioni()//rimuove staz e orari da tt
{
	int indice = 0;

	for (int i = 1; i < stazioni_sx.size(); i++)
	{
		
		if ((stazioni_sx[i]->get_Distance() - stazioni_sx[i - 1]->get_Distance()) < min_distance)
		{
			stazioni_sx.erase(stazioni_sx.begin() + i);
			stazioni_dx.erase(stazioni_dx.end() - i - 1); 
			indice = getPos(index_position_sx, i);
			if(indice != -1)
			{//se principale
				for(int j = 0; j < timetable.size(); j++)
				{ // per ogni treno della time table
					
						if(timetable[j][1]==0)
						{//se da sx
							if(timetable[j][2]==1)
							{//se regionale
								timetable[j].erase(timetable[j].begin()+i+3);//2
							}
							else
							{//se veloce
								timetable[j].erase(timetable[j].begin()+indice+3);
							}
						}
						else
						{//se da dx
							if(timetable[j][2]==1)
							{//se regionale
								timetable[j].erase(timetable[j].end()- i - 1);
							}else
							{//se veloce
								timetable[j].erase(timetable[j].end()-indice - 1);//--
							}
						}
				}
				//elimina elemento in index_position_sx e scala ind array
				for(int k = indice+1; k < index_position_sx.size(); k++)
				{
				 	index_position_sx[k]--;
				}
				index_position_sx.erase(index_position_sx.begin()+indice);
				
			}
			else//se locale
			{
				for(int j = 0; j < timetable.size(); j++)
				{
					if(timetable[j][2]==1)
					{//se regionale
						if(timetable[j][1]==0)//se da sx
						{
							timetable[j].erase(timetable[j].begin()+i+3);
						}else
						{
							timetable[j].erase(timetable[j].end()-i-1);
						}

					}
				}
				//scala indici array
				int z = 0;
				while(index_position_sx[z] < i)
				{
					z++;
				}
				for (; z < index_position_sx.size(); z++) 
				{
					index_position_sx[z]--;
				}
			}
		}					 
	}
	
}
void Controller::controllo_situazione()
{
	vector<Treno*>relative_distance_sx, relative_distance_dx;//di treni |vector<int>relative_distance_sx, relative_distance_dx;
	vector<int>indici_treni_sx, indici_treni_dx;
	int ind = 0;
	
	for (int i = 0; i < treni.size(); i++) //prendi indice per stazione
	{
		if (treni[i]->get_fw() == 0) 
		{
			if (timetable[i][2] == 1) //se regionale
			{
				ind = treni[i]->get_next();		//indice in stazione
			}
			else
			{				   //se veloce
				ind = index_position_sx[treni[i]->get_next()];//{0,1,4,7}sx - {0,3,6,7}dx
			}
		}
		else
		{
			if (timetable[i][2] == 1) //se regionale
			{
				ind = treni[i]->get_next();			//indice in stazione
			}
			else
			{				   //se veloce
				ind =stazioni_sx.size()-1-index_position_sx[index_position_sx.size()-1-treni[i]->get_next()];//stazioni.size()-1-index_p_sx[index.size()-1-treni[i]->get_next()]
			}
		}
		if (treni[i]->getSpeed() != 0) //se in movimento
		{
			if (treni[i]->get_fw() == 0) //sx
			{
				//se non ho già fatto la richiesta e sono circa a 20 km
				if (treni[i]->get_richiesta()==false && stazioni_sx[ind]->get_Distance()*1000-treni[i]->getDistance()<= 20600 && stazioni_sx[ind]->get_Distance() *1000-treni[i]->getDistance()>=19000)
				{
					cout << "Minuto:" + current_time << " Treno n." << treni[i]->getId() << "chiede alla stazione " << stazioni_sx[ind]->get_Nome() << " se ha un binario std libero" << endl;
					bool* binari = stazioni_sx[ind]->get_status_Bin_standard();
					treni[i]->set_richiesta(true);
					if(binari[0])
					{
						cout << "Stazione " + stazioni_sx[ind]->get_Nome() << " assegna il binario 1" << endl;
						stazioni_sx[ind]->set_status_Bin_standard(0, false);
						treni[i]->set_reserved(true);
						treni[i]->set_num_bin(0);
					}
					else if(binari[1])
					{
						cout << "Stazione " + stazioni_sx[ind]->get_Nome() << " assegna il binario 2" << endl;
						stazioni_dx[ind]->set_status_Bin_standard(1, false);
						treni[i]->set_reserved(true);
						treni[i]->set_num_bin(1);
					}
					else
					{
						cout << "Stazione " + stazioni_sx[ind]->get_Nome() << " non puo' gestire la richiesta, binari occupati, utilizzare parcheggio" << endl;

					}
				}
			}
			else //dx
			{
				//se non ho già fatto la richiesta e sono circa a 20 km
				if (!treni[i]->get_richiesta() &&  stazioni_dx[ind]->get_Distance()*1000 - treni[i]->getDistance()<=20600 && stazioni_dx[ind]->get_Distance()*1000- treni[i]->getDistance() >=19000)
				{
					cout << "Minuto:" + current_time << " Treno n." << treni[i]->getId() << "chiede alla stazione " << stazioni_dx[ind]->get_Nome() << " se ha un binario std libero" << endl;


					bool* binari = stazioni_dx[ind]->get_status_Bin_standard();
					treni[i]->set_richiesta(true);
					if (binari[0])
					{
						cout << "Stazione " + stazioni_dx[ind]->get_Nome() << " assegna il binario 3" << endl;
						stazioni_dx[ind]->set_status_Bin_standard(0, false);
						treni[i]->set_reserved(true);
						treni[i]->set_num_bin(0);
					}
					else if (binari[1])
					{
						cout << "Stazione " + stazioni_dx[ind]->get_Nome() << " assegna il binario 4" << endl;
						stazioni_dx[ind]->set_status_Bin_standard(1, false);
						treni[i]->set_reserved(true);
						treni[i]->set_num_bin(1);
					}
					else
					{
						cout << "Stazione " + stazioni_dx[ind]->get_Nome() << " non puo' gestire la richiesta, binari occupati, utilizzare parcheggio" << endl;

					}
				}//
			}
		}

//speed > 80
		if (treni[i]->getSpeed() > 80) //solo se v>80
		{
			//divisione parcheggio/binari
			if (treni[i]->get_fw() == 0) 
			{
				bool* binari = stazioni_sx[ind]->get_status_Bin_standard();
				int n_b;
				if (binari[0])n_b = 0;
				else if (binari[1])n_b = 1;
				else n_b = -1;
				//se v>80 e distanza 5k da staz
				if (stazioni_sx[ind]->get_Distance()*1000 -treni[i]->getDistance() <=5000 && stazioni_sx[ind]->get_Distance()*1000-treni[i]->getDistance()>=4000) {
					if (treni[i]->get_reserved() || (n_b != -1 && stazioni_sx[ind]->get_park_size()==0)) 
					{
						treni[i]->setSpeed(80);
						if (!treni[i]->get_reserved()) {
							treni[i]->set_num_bin(n_b);
							stazioni_sx[ind]->set_status_Bin_standard(n_b, false);
							treni[i]->set_reserved(true);
						}
					}
					else
					{
						treni[i]->setSpeed(0);
						stazioni_sx[ind]->goPark(*treni[i]);
					}
					treni[i]->set_richiesta(false);
				}
			}
			else{
				bool* binari = stazioni_sx[ind]->get_status_Bin_standard();
				int n_b;
				if (binari[0])n_b = 0;
				else if (binari[1])n_b = 1;
				else n_b = -1;
				//se v>80 e distanza 5k da staz
				if (stazioni_dx[ind]->get_Distance()*1000-treni[i]->getDistance() <=5000 && stazioni_dx[ind]->get_Distance()*1000-treni[i]->getDistance()>=4000) {
					if (treni[i]->get_reserved() || (n_b != -1 && stazioni_dx[ind]->get_park_size()==0)) {
						treni[i]->setSpeed(80);
						if (!treni[i]->get_reserved()) {
							treni[i]->set_num_bin(n_b);
							stazioni_dx[ind]->set_status_Bin_standard(n_b, false);
							treni[i]->set_reserved(true);
						}
					}
					else{
						treni[i]->setSpeed(0);
						stazioni_dx[ind]->goPark(*treni[i]);
					}
					treni[i]->set_richiesta(false);
				}
			}//
			if (treni[i]->getSpeed() > 80) {
				if (treni[i]->get_fw() == 0) {//se da sx
					relative_distance_sx.push_back(treni[i]);//togliere distance |relative_distance_sx.push_back(treni[i]->getDistance());
					indici_treni_sx.push_back(i);
				}
				else {					  //se da dx
					relative_distance_dx.push_back(treni[i]);//togliere distance |relative_distance_dx.push_back(treni[i]->getDistance());
					indici_treni_dx.push_back(i);
				}
			}
		}
//speed == 80
		else if (treni[i]->getSpeed() == 80) //se in zona 5 km
		{
			if (treni[i]->get_fw() == 0) //se da sx
			{		//se arrivate
				if ((stazioni_sx[ind]->get_Distance()*1000 - treni[i]->getDistance()) >=-140 &&(stazioni_sx[ind]->get_Distance()*1000 - treni[i]->getDistance())<=140 ) {//se arrivato al binario
					treni[i]->setSpeed(0);
					cout <<"Minuto "<<(int)(current_time/60)<< ": Treno n." << treni[i]->getId() << " arrivato alla stazione " << stazioni_sx[treni[i]->get_next()]->get_Nome() << endl;
					int diff = (timetable[i][ind+3]) * 60 - current_time;
					if (diff > 0)
					{
						treni[i]->setTimer(diff + 300);
					}
					else {
						treni[i]->setTimer(300); //se è in orario o in ritardo lo faccio aspettare 5 min
						if(diff < 0){
							cout << ", con un ritardo di "<<(int)(-diff/60)<<" minuto/i";
						}
						//dico se sono arrivato in ritardo confrontando ora con valore in tt
					}
				}
				else if ((treni[i]->getDistance() - stazioni_sx[ind - 1]->get_Distance()*1000) > 4800 && (treni[i]->getDistance() - stazioni_sx[ind - 1]->get_Distance()*1000)<5200)
				{
					treni[i]->setSpeed(300);
				}
			}
			else
			{//se da dx
				if ((stazioni_dx[ind]->get_Distance()*1000 - treni[i]->getDistance()) >=-140 &&(stazioni_dx[ind]->get_Distance()*1000 - treni[i]->getDistance())<=140 ) 
				{
					treni[i]->setSpeed(0);
					
					cout << "treno n." << treni[i]->getId() << " arrivato alla stazione " << stazioni_dx[treni[i]->get_next()]->get_Nome() << endl;
					//inizio timer...
					int diff = (timetable[i][ind]) * 60 - current_time;
					if (diff > 0)
						treni[i]->setTimer(diff + 300);
					else{
						treni[i]->setTimer(300); //se è in orario o in ritardo lo faccio aspettare 5 min
						if(diff < 0){
							cout << ", con un ritardo di "<<(int)(-diff/60)<<" minuto/i";
						}
						//dico se sono arrivato in ritardo confrontando ora con valore in tt
					}
				}
				else if ((treni[i]->getDistance() - stazioni_dx[ind -1]->get_Distance()*1000) < 5200 && treni[i]->getDistance() - stazioni_dx[ind -1]->get_Distance()*1000 > 4800)
				{
					treni[i]->setSpeed(300);//
				}
			}
		}
//speed == 0
		else 
		{//velocita zero
				
			//se fermo e deve partire dall' origine
			if ((current_time) == timetable[i][3] * 60)
			{
				string s = "";
				if (treni[i]->get_fw() == 0)
				{
					s = stazioni_sx[0]->get_Nome();
				}
				else
				{
					s = stazioni_dx[0]->get_Nome();
				}
				treni[i]->incr_next();
				cout<<"Minuto "<<(int)(current_time/60) << ": Treno n." << treni[i]->getId() << " partito dalla Stazione " << s << endl;
				treni[i]->setSpeed(80);//parte
			}
			else
			{//se fermo in una stazione e timer==0
				if (treni[i]->get_next() != 0 && treni[i]->getSpeed() == 0) {//se fermo in una staz non prima
					if (treni[i]->get_fw() == 0)
					{//sx
						if ( stazioni_sx[ind]->get_Distance()*1000 - treni[i]->getDistance()>= -140 &&stazioni_sx[ind]->get_Distance()*1000 -treni[i]->getDistance() <= 140 && treni[i]->getTimer() == 0)//se deve partire
						{
							treni[i]->setSpeed(80);
							treni[i]->set_reserved(false);
							
							int num_b = treni[i]->get_num_bin();//deve essere settato prima

							cout << "Minuto " << (int)(current_time / 60) << ": Treno n." + treni[i]->getId() << " partito dalla Stazione " + stazioni_sx[ind]->get_Nome() << endl;
							treni[i]->set_num_bin(-1);
							stazioni_sx[ind]->set_status_Bin_standard(num_b, true);
							stazioni_sx[ind]->espelli(num_b);
							treni[i]->incr_next();
							if (stazioni_sx[stazioni_sx.size() - 1]->get_Distance()*1000- treni[i]->getDistance() >=-140 &&stazioni_sx[stazioni_sx.size() - 1]->get_Distance()*1000- treni[i]->getDistance()<=140) {//se ultima
								delete treni[i];
								treni.erase(treni.begin() + i);
								timetable.erase(timetable.begin() + i);
							}
						}
					}
					else//dx
					{
						if ( stazioni_dx[ind]->get_Distance()*1000 -treni[i]->getDistance() >=-140 && stazioni_dx[ind]->get_Distance()*1000-treni[i]->getDistance() <=140  && treni[i]->getTimer() == 0)//se deve partire
						{
							treni[i]->setSpeed(80);
							treni[i]->set_reserved(false);

							int num_b = treni[i]->get_num_bin();
							cout << "Minuto " << (int)(current_time / 60) << ": Treno n." + treni[i]->getId() << " partito dalla Stazione " + stazioni_dx[ind]->get_Nome() << endl;
							treni[i]->set_num_bin(-1);
							stazioni_dx[ind]->set_status_Bin_standard(num_b, true);
							stazioni_dx[ind]->espelli(num_b);
							treni[i]->incr_next();
							//se ultima stazione
							if (stazioni_dx[stazioni_dx.size() - 1]->get_Distance()*1000- treni[i]->getDistance() >=-140 &&stazioni_dx[stazioni_dx.size() - 1]->get_Distance()*1000- treni[i]->getDistance()<=140) {
								treni.erase(treni.begin() + i);
								timetable.erase(timetable.begin() + i);
							}
						}
					}
				}
					
			}
		}
	}
	selectionSort(relative_distance_sx);
	selectionSort(relative_distance_dx);

	int j = 1;
	//riaggiustamento velocita' se treno ha pista libera
	if(relative_distance_sx.size()!= 0)
	{
		relative_distance_sx[0]->setSpeed(relative_distance_sx[0]->getSpeedMax());
	}
	while(j<relative_distance_sx.size())
	{
		while(j<relative_distance_sx.size()&& (relative_distance_sx[j-1]->getDistance()-relative_distance_sx[j]->getDistance()<=10))
		{
			relative_distance_sx[j]->setSpeed(relative_distance_sx[j - 1]->getSpeed());
			j++;
		}
		if(j<relative_distance_sx.size())
		{
			relative_distance_sx[j]->setSpeed(relative_distance_sx[j]->getSpeedMax());
			j++;
		}
	}

	j = 1;
	//riaggiustamento velocita' se treno ha pista libera
	if(relative_distance_dx.size()!= 0){
		relative_distance_dx[0]->setSpeed(relative_distance_dx[0]->getSpeedMax());
	}
	while(j<relative_distance_dx.size()){
		while(j<relative_distance_dx.size()&& (relative_distance_dx[j-1]->getDistance()-relative_distance_dx[j]->getDistance()<=10000))
		{
			relative_distance_dx[j]->setSpeed(relative_distance_dx[j - 1]->getSpeed());
			j++;
		}
		if(j<relative_distance_dx.size()){
			relative_distance_dx[j]->setSpeed(relative_distance_dx[j]->getSpeedMax());
			j++;
		}
	}
}

void Controller::update()
{
	current_time+=delta_t;
	for(int i = 0; i < treni.size(); i++){//update posizione dei treni
		treni[i]->setDistance(treni[i]->getDistance()+ (int)(treni[i]->getSpeed()*delta_t/3.6));//dist_att+(v*tempo)
		cout <<"posizione del treno "<<treni[i]->getId() <<" "<< treni[i]->getDistance()<< " con vel="<<treni[i]->getSpeed()<<" \n";
		int ind = 0;
		if (treni[i]->get_fw() == 0) 
		{
			if (timetable[i][2] == 1) //se regionale
			{
				ind = treni[i]->get_next();		//indice in stazione
			}
			else
			{				   //se veloce
				ind = index_position_sx[treni[i]->get_next()];//{0,1,4,7}sx - {0,3,6,7}dx
			}
		}
		else
		{
			if (timetable[i][2] == 1) //se regionale
			{
				ind = treni[i]->get_next();			//indice in stazione
			}
			else
			{				   //se veloce
				ind =stazioni_sx.size()-1-index_position_sx[index_position_sx.size()-1-treni[i]->get_next()];//stazioni.size()-1-index_p_sx[index.size()-1-treni[i]->get_next()]
			}
		}
		if (treni[i]->get_fw() == 0)
		{
			if ((stazioni_sx[ind]->get_Distance()*1000 - treni[i]->getDistance()) >=-140 &&(stazioni_sx[ind]->get_Distance()*1000 - treni[i]->getDistance()) <=140)
				treni[i]->decrTimer(delta_t);
		}
		else
		{
			if ((stazioni_dx[ind]->get_Distance()*1000 - treni[i]->getDistance()) >=-140 &&(stazioni_dx[ind]->get_Distance()*1000 - treni[i]->getDistance()) <=140)
				treni[i]->decrTimer(delta_t);
		}		
	}
}
