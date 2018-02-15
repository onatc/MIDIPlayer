#define _USE_MATH_DEFINES

#include <math.h>
#include "instrument.hpp"
#include <iostream>

using namespace std;

DefaultInstrument::DefaultInstrument(const Track & track)
{
	trck = track;
	if (trck.begin() != trck.end())
	{
		it = trck.begin();
	}
	halt = false;
	tempo = 0;
}

bool DefaultInstrument::halted()
{
	if (halt)
	{
		return true;
	}
	return false;
}

double DefaultInstrument::sample(double deltaT, int samprate)
{
	double out = 0;
	bool already_on = false;
	double real_time = 0;


	if (it->isTempoEvent())
	{
		tempo = it->asTempoEvent().getTempo();

	}

	if (it->isNoteEvent())
	{
		if (it->asNoteEvent().getNoteOn())
		{
			for (unsigned int i = 0; i < notes_on.size(); i++)
			{
				if (notes_on[i].note.getNoteNumber() == it->asNoteEvent().getNoteNumber())
				{
					already_on = true;
				}
			}

			if (!already_on)
			{
				Notes x = { it->asNoteEvent(), it->getTickTime(),0, 0 };
				notes_on.push_back(x);
			}
		}

		if (!it->asNoteEvent().getNoteOn())
		{
			for (unsigned int i = 0; i < notes_on.size(); i++)
			{
				if (notes_on[i].note.getNoteNumber() == it->asNoteEvent().getNoteNumber())
				{
					if (notes_on.at(i).on_time >= 0.25)
					{
						notes_on.erase(notes_on.begin() + i);
					}
				}
			}
		}

	}


	for (unsigned int i = 0; i < notes_on.size(); i++)
	{
		double n = notes_on.at(i).x;
		notes_on.at(i).on_time = n / samprate; //moved to top
											  //cout << notes_on.at(i).x << "   " << notes_on.at(i).on_time <<  endl;
		double f = frequency(notes_on.at(i).note);
		double sin_a = sin(2 * M_PI* f * notes_on.at(i).on_time);
		out += 200 * (notes_on.at(i).note.getNoteVelocity()) * envelope(notes_on.at(i).on_time) *sin_a;


		notes_on.at(i).x++;
		//cout << f << " " << sin_a << " " << out << " " << notes_on.at(i).on_time << endl;		
	}

	if (!it->isEndTrackEvent())
	{
		double temp1 = (pow(10, 6) * trck.getTicksPerBeat());
		it++;
		double temp2 = (double)it->getTickTime() * tempo;
		real_time = temp2 / temp1;
		if (deltaT < real_time)
		{
			it--;
		}
	}

	if (it->isEndTrackEvent())
	{
		halt = true;
	}

	if (out > 0)
	{
		out = out + 0.5;
	}
	else if (out < 0)
	{
		out = out - 0.5;
	}
	return out;
	//out += 200 * (it->asNoteEvent().getNoteVelocity()) * envelope(deltaT) * sin(2 * M_PI * frequency(it->asNoteEvent()) * deltaT);
}



double DefaultInstrument::envelope(double x)
{
	double t[5] = {0, 0.0625, 0.11252, 0.20, 0.25 };
	double S = 0.8;
	if (x < 0) { return 0; }
	else if (x <= t[1]) { return (x / t[1]); }
	else if (x <= t[2]) { return (1 - (x - t[1])); }
	else if (x <= t[3]) { return S; }
	else if (x <= t[4]) { return (S - (S / (t[4] - t[3])) * (x - t[3])); }
	else { return 0; }
}


double DefaultInstrument::frequency(NoteEvent x)
{
	double C = 261.63;
	double C_ = 277.18;
	double D = 293.66;
	double D_ = 311.13;
	double E = 329.63;
	double F = 349.63;
	double F_ = 369.994;
	double G = 392.0;
	double G_ = 415.305;
	double A = 440.0;
	double A_ = 466.164;
	double B = 493.88;

	if (x.getNoteNumber() % 12 == 0)
	{
		if (x.getNoteNumber() == 0) { return C / pow(2, 5); }
		else if(x.getNoteNumber() == 12) { return C / pow(2, 4); }
		else if (x.getNoteNumber() == 24) { return C / pow(2, 3); }
		else if (x.getNoteNumber() == 36) { return C / pow(2, 2); }
		else if (x.getNoteNumber() == 48) { return C / pow(2, 1); }
		else if (x.getNoteNumber() == 60) { return C; }
		else if (x.getNoteNumber() == 72) { return C * pow(2, 1); }
		else if (x.getNoteNumber() == 84) { return C * pow(2, 2); }
		else if (x.getNoteNumber() == 96) { return C * pow(2, 3); }
		else if (x.getNoteNumber() == 108) { return C * pow(2, 4); }
		else if (x.getNoteNumber() == 120) { return C * pow(2, 5); }

	}
	else if (x.getNoteNumber() % 12 == 1)
	{
		if (x.getNoteNumber() == 1) { return C_ / pow(2, 5); }
		else if (x.getNoteNumber() == 13) { return C_ / pow(2, 4); }
		else if (x.getNoteNumber() == 25) { return C_ / pow(2, 3); }
		else if (x.getNoteNumber() == 37) { return C_ / pow(2, 2); }
		else if (x.getNoteNumber() == 49) { return C_ / pow(2, 1); }
		else if (x.getNoteNumber() == 61) { return C_; }
		else if (x.getNoteNumber() == 73) { return C_ * pow(2, 1); }
		else if (x.getNoteNumber() == 85) { return C_ * pow(2, 2); }
		else if (x.getNoteNumber() == 97) { return C_ * pow(2, 3); }
		else if (x.getNoteNumber() == 109) { return C_ * pow(2, 4); }
		else if (x.getNoteNumber() == 121) { return C_ * pow(2, 5); }
	}
	else if (x.getNoteNumber() % 12 == 2)
	{
		if (x.getNoteNumber() == 2) { return D / pow(2, 5); }
		else if (x.getNoteNumber() == 14) { return D / pow(2, 4); }
		else if (x.getNoteNumber() == 26) { return D / pow(2, 3); }
		else if (x.getNoteNumber() == 38) { return D / pow(2, 2); }
		else if (x.getNoteNumber() == 50) { return D / pow(2, 1); }
		else if (x.getNoteNumber() == 62) { return D; }
		else if (x.getNoteNumber() == 74) { return D * pow(2, 1); }
		else if (x.getNoteNumber() == 86) { return D * pow(2, 2); }
		else if (x.getNoteNumber() == 98) { return D * pow(2, 3); }
		else if (x.getNoteNumber() == 110) { return D * pow(2, 4); }
		else if (x.getNoteNumber() == 122) { return D * pow(2, 5); }
	}
	else if (x.getNoteNumber() % 12 == 3)
	{
		if (x.getNoteNumber() == 3) { return D_ / pow(2, 5); }
		else if (x.getNoteNumber() == 15) { return D_ / pow(2, 4); }
		else if (x.getNoteNumber() == 27) { return D_ / pow(2, 3); }
		else if (x.getNoteNumber() == 39) { return D_ / pow(2, 2); }
		else if (x.getNoteNumber() == 51) { return D_ / pow(2, 1); }
		else if (x.getNoteNumber() == 63) { return D_; }
		else if (x.getNoteNumber() == 75) { return D_ * pow(2, 1); }
		else if (x.getNoteNumber() == 87) { return D_ * pow(2, 2); }
		else if (x.getNoteNumber() == 99) { return D_ * pow(2, 3); }
		else if (x.getNoteNumber() == 111) { return D_ * pow(2, 4); }
		else if (x.getNoteNumber() == 123) { return D_ * pow(2, 5); }
	}
	else if (x.getNoteNumber() % 12 == 4)
	{
		if (x.getNoteNumber() == 4) { return E / pow(2, 5); }
		else if (x.getNoteNumber() == 16) { return E / pow(2, 4); }
		else if (x.getNoteNumber() == 28) { return E / pow(2, 3); }
		else if (x.getNoteNumber() == 40) { return E / pow(2, 2); }
		else if (x.getNoteNumber() == 52) { return E / pow(2, 1); }
		else if (x.getNoteNumber() == 64) { return E; }
		else if (x.getNoteNumber() == 76) { return E * pow(2, 1); }
		else if (x.getNoteNumber() == 88) { return E * pow(2, 2); }
		else if (x.getNoteNumber() == 100) { return E * pow(2, 3); }
		else if (x.getNoteNumber() == 112) { return E * pow(2, 4); }
		else if (x.getNoteNumber() == 124) { return E * pow(2, 5); }
	}
	else if (x.getNoteNumber() % 12 == 5)
	{
		if (x.getNoteNumber() == 5) { return F / pow(2, 5); }
		else if (x.getNoteNumber() == 17) { return F / pow(2, 4); }
		else if (x.getNoteNumber() == 29) { return F / pow(2, 3); }
		else if (x.getNoteNumber() == 41) { return F / pow(2, 2); }
		else if (x.getNoteNumber() == 53) { return F / pow(2, 1); }
		else if (x.getNoteNumber() == 65) { return F; }
		else if (x.getNoteNumber() == 77) { return F * pow(2, 1); }
		else if (x.getNoteNumber() == 89) { return F * pow(2, 2); }
		else if (x.getNoteNumber() == 101) { return F * pow(2, 3); }
		else if (x.getNoteNumber() == 113) { return F * pow(2, 4); }
		else if (x.getNoteNumber() == 125) { return F * pow(2, 5); }
	}
	else if (x.getNoteNumber() % 12 == 6)
	{
		if (x.getNoteNumber() == 6) { return F_ / pow(2, 5); }
		else if (x.getNoteNumber() == 18) { return F_ / pow(2, 4); }
		else if (x.getNoteNumber() == 30) { return F_ / pow(2, 3); }
		else if (x.getNoteNumber() == 42) { return F_ / pow(2, 2); }
		else if (x.getNoteNumber() == 54) { return F_ / pow(2, 1); }
		else if (x.getNoteNumber() == 66) { return F_; }
		else if (x.getNoteNumber() == 78) { return F_ * pow(2, 1); }
		else if (x.getNoteNumber() == 90) { return F_ * pow(2, 2); }
		else if (x.getNoteNumber() == 102) { return F_ * pow(2, 3); }
		else if (x.getNoteNumber() == 114) { return F_ * pow(2, 4); }
		else if (x.getNoteNumber() == 126) { return F_ * pow(2, 5); }
	}
	else if (x.getNoteNumber() % 12 == 7)
	{
		if (x.getNoteNumber() == 7) { return G / pow(2, 5); }
		else if (x.getNoteNumber() == 19) { return G / pow(2, 4); }
		else if (x.getNoteNumber() == 31) { return G / pow(2, 3); }
		else if (x.getNoteNumber() == 43) { return G / pow(2, 2); }
		else if (x.getNoteNumber() == 55) { return G / pow(2, 1); }
		else if (x.getNoteNumber() == 67) { return G; }
		else if (x.getNoteNumber() == 79) { return G * pow(2, 1); }
		else if (x.getNoteNumber() == 91) { return G * pow(2, 2); }
		else if (x.getNoteNumber() == 103) { return G * pow(2, 3); }
		else if (x.getNoteNumber() == 115) { return G * pow(2, 4); }
		else if (x.getNoteNumber() == 127) { return G * pow(2, 5); }
	}
	else if (x.getNoteNumber() % 12 == 8)
	{
		if (x.getNoteNumber() == 8) { return G_ / pow(2, 5); }
		else if (x.getNoteNumber() == 20) { return G_ / pow(2, 4); }
		else if (x.getNoteNumber() == 32) { return G_ / pow(2, 3); }
		else if (x.getNoteNumber() == 44) { return G_ / pow(2, 2); }
		else if (x.getNoteNumber() == 56) { return G_ / pow(2, 1); }
		else if (x.getNoteNumber() == 68) { return G_; }
		else if (x.getNoteNumber() == 80) { return G_ * pow(2, 1); }
		else if (x.getNoteNumber() == 92) { return G_ * pow(2, 2); }
		else if (x.getNoteNumber() == 104) { return G_ * pow(2, 3); }
		else if (x.getNoteNumber() == 116) { return G_ * pow(2, 4); }

	}
	else if (x.getNoteNumber() % 12 == 9)
	{
		if (x.getNoteNumber() == 9) { return A / pow(2, 5); }
		else if (x.getNoteNumber() == 21) { return A / pow(2, 4); }
		else if (x.getNoteNumber() == 33) { return A / pow(2, 3); }
		else if (x.getNoteNumber() == 45) { return A / pow(2, 2); }
		else if (x.getNoteNumber() == 57) { return A / pow(2, 1); }
		else if (x.getNoteNumber() == 69) { return A; }
		else if (x.getNoteNumber() == 81) { return A * pow(2, 1); }
		else if (x.getNoteNumber() == 93) { return A * pow(2, 2); }
		else if (x.getNoteNumber() == 105) { return A * pow(2, 3); }
		else if (x.getNoteNumber() == 117) { return A * pow(2, 4); }
	}
	else if (x.getNoteNumber() % 12 == 10)
	{
		if (x.getNoteNumber() == 10) { return A_ / pow(2, 5); }
		else if (x.getNoteNumber() == 22) { return A_ / pow(2, 4); }
		else if (x.getNoteNumber() == 34) { return A_ / pow(2, 3); }
		else if (x.getNoteNumber() == 46) { return A_ / pow(2, 2); }
		else if (x.getNoteNumber() == 58) { return A_ / pow(2, 1); }
		else if (x.getNoteNumber() == 70) { return A_; }
		else if (x.getNoteNumber() == 82) { return A_ * pow(2, 1); }
		else if (x.getNoteNumber() == 94) { return A_ * pow(2, 2); }
		else if (x.getNoteNumber() == 106) { return A_ * pow(2, 3); }
		else if (x.getNoteNumber() == 118) { return A_ * pow(2, 4); }
	}
	else if (x.getNoteNumber() % 12 == 11)
	{
		if (x.getNoteNumber() == 11) { return B / pow(2, 5); }
		else if (x.getNoteNumber() == 23) { return B / pow(2, 4); }
		else if (x.getNoteNumber() == 35) { return B / pow(2, 3); }
		else if (x.getNoteNumber() == 47) { return B / pow(2, 2); }
		else if (x.getNoteNumber() == 59) { return B / pow(2, 1); }
		else if (x.getNoteNumber() == 71) { return B; }
		else if (x.getNoteNumber() == 83) { return B * pow(2, 1); }
		else if (x.getNoteNumber() == 95) { return B * pow(2, 2); }
		else if (x.getNoteNumber() == 107) { return B * pow(2, 3); }
		else if (x.getNoteNumber() == 119) { return B * pow(2, 4); }
	}

	return 0;
}

