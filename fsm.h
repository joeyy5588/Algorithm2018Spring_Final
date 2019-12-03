#ifndef _FSM_H
#define _FSM_H
#include <queue>
#include <algorithm>
#include <tuple>
#include <utility>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <climits>

using namespace std;

class state
{
public:
	state(int p, int n, vector<int> s): previous(-1) {previous = p; number = n; trace = s;}
	virtual ~state() {}
	int 									previous;
	int 									number;
	vector<int> 							trace;
	vector<tuple<size_t, size_t, string>>* 	addedseq;
private:

};

class FSM
{
public:
	FSM() {}
	virtual ~FSM() {}
	void readfsm(fstream&);
	void readwatermark(fstream&, fstream&, fstream&);
	bool isCFSM();
	bool addwatermark(int);
	bool eqseq(string, string);
	bool extra(int, string, string);
	bool extra2(int, string, string);
	void writewatermark(fstream&);
	bool deadend(state, string, string);

private:
	size_t 													I, O, S, P;
	size_t													r1, r2, r3;
	string 													resetstate;
	string		 											watermark1;
	string 													watermark2;
	string													watermark3;
	vector<vector<vector<pair<string, string>>>>			state2seqs;
	vector<tuple<size_t, size_t, string>>*					seqs2state;
	vector<state>*											path;

};

#endif
