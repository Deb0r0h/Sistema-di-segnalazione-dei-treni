#include<iostream>
#include "Controller.h"

using namespace std;

void Controller::selectionSort(vector<Treno*>& treni)
{
	for (int i = 1; i < treni.size(); i++)
	{
		for (int j = i - 1; j >= 0; j--)
			if (treni[j]->getDistance() < treni[j + 1]->getDistance())
			{
				swap(treni[j], treni[j + 1]);
			}
			else
				break;
	}
}

int getPos(vector<int> v, int x)
{
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i] == x)
			return i;
	}
	return -1;
}

Controller::~Controller()
{
	for (int i = 0; i < stazioni_sx.size(); i++)
	{
		delete stazioni_sx[i];
		delete stazioni_dx[i];
	}
}


void Controller::set_other_vector()
{
	vector<Stazione*> temp;
	vector<int>t;
	string nome = "";
	for (int i = 0; i < stazioni_sx.size(); i++)
	{
		if (getPos(index_position_sx, i) != -1)

			temp.push_back(new Stazione_principale(stazioni_sx[i]->get_Nome(), stazioni_sx[i]->get_Distance()));

		else
			temp.push_back(new Stazione_locale(stazioni_sx[i]->get_Nome(), stazioni_sx[i]->get_Distance()));
	}
	reverse(temp.begin(), temp.end());
	for (int i = 0; i < temp.size(); i++)
		stazioni_dx.push_back(temp[i]);

	int n = 0;
	for (int i = stazioni_sx.size() - 1; i > 0; i--)
	{
		int n = abs(stazioni_sx[i]->get_Distance() - stazioni_sx[i - 1]->get_Distance());
		t.push_back(n);
	}
	t.insert(t.begin(), 0);
	int sum = 0;
	for (int i = 0; i < t.size(); i++)
	{
		if (i == 0)
			t[i] = 0;
		else
		{
			t[i] = t[i] + sum;
			sum = t[i];
		}
	}
	for (int i = 0; i < stazioni_dx.size(); i++)
		stazioni_dx[i]->set_Distance(t[i]);
}
