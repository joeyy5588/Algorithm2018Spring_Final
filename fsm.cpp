#include "fsm.h"

using namespace std;

void 
FSM::readfsm(fstream& f)
{
    string dummy;
    f >> dummy >>  I >> dummy >> O >> dummy >> P >> dummy >> S >> dummy >> resetstate;
    //cerr << I << " " << O << " " << P << " " << S << " " << resetstate <<endl;
    seqs2state = new vector<tuple<size_t, size_t, string>>[int(pow(2,O))];

    for(size_t i = 0; i < S; i++){
    	vector< vector< pair<string, string> > > v1;
    	for(size_t j = 0; j < S; j++){
    		vector<pair<string, string>> v2;
    		v1.push_back(v2);
    	}
    	state2seqs.push_back(v1);
    }

    for(size_t i = 0; i < P; i++){
        string input, output;
        char s;
        size_t s1, s2;
        f >> input >> s>> s1 >> s >> s2 >> output;
        pair<string, string> temp(input, output);
        state2seqs[s1][s2].push_back(temp);
        int idx = stoi(output, nullptr, 2);
        tuple<size_t, size_t, string> temp1(s1, s2, input);
        seqs2state[idx].push_back(temp1);
        //cerr << input << "/" << s1 << "/" << s2 << "/" << output << endl;
    }
    /*for(size_t i = 0; i < pow(2,O); i++){
		for(vector<tuple<size_t, size_t, string>>::iterator it = seqs2state[i].begin(); it != seqs2state[i].end(); it++){
			cerr << *it << endl;
		}
    }*/
    /*for(size_t i = 0; i < S; i++){
    	for(size_t j = 0; j < S; j++){
    		for(vector<pair<string, string>>::iterator it = state2seqs[i][j].begin(); it != state2seqs[i][j].end(); it++){
				cerr << it->first << " " << it->second << endl;
			}
    	}
		
    }*/
}

void
FSM::readwatermark(fstream& f1, fstream& f2, fstream& f3)
{
	watermark1 = watermark2 = watermark3 = "";
	for(size_t i = 0; i < 32; i++){
        char c1, c2, c3;
        f1 >> c1;
        f2 >> c2;
        f3 >> c3;
        switch (c1)
		{
			case '0': watermark1.append ("0000"); break;
			case '1': watermark1.append ("0001"); break;
			case '2': watermark1.append ("0010"); break;
			case '3': watermark1.append ("0011"); break;
			case '4': watermark1.append ("0100"); break;
			case '5': watermark1.append ("0101"); break;
			case '6': watermark1.append ("0110"); break;
			case '7': watermark1.append ("0111"); break;
			case '8': watermark1.append ("1000"); break;
			case '9': watermark1.append ("1001"); break;
			case 'a': watermark1.append ("1010"); break;
			case 'b': watermark1.append ("1011"); break;
			case 'c': watermark1.append ("1100"); break;
			case 'd': watermark1.append ("1101"); break;
			case 'e': watermark1.append ("1110"); break;
			case 'f': watermark1.append ("1111"); break;
		}
		switch (c2)
		{
			case '0': watermark2.append ("0000"); break;
			case '1': watermark2.append ("0001"); break;
			case '2': watermark2.append ("0010"); break;
			case '3': watermark2.append ("0011"); break;
			case '4': watermark2.append ("0100"); break;
			case '5': watermark2.append ("0101"); break;
			case '6': watermark2.append ("0110"); break;
			case '7': watermark2.append ("0111"); break;
			case '8': watermark2.append ("1000"); break;
			case '9': watermark2.append ("1001"); break;
			case 'a': watermark2.append ("1010"); break;
			case 'b': watermark2.append ("1011"); break;
			case 'c': watermark2.append ("1100"); break;
			case 'd': watermark2.append ("1101"); break;
			case 'e': watermark2.append ("1110"); break;
			case 'f': watermark2.append ("1111"); break;
		}
		switch (c3)
		{
			case '0': watermark3.append ("0000"); break;
			case '1': watermark3.append ("0001"); break;
			case '2': watermark3.append ("0010"); break;
			case '3': watermark3.append ("0011"); break;
			case '4': watermark3.append ("0100"); break;
			case '5': watermark3.append ("0101"); break;
			case '6': watermark3.append ("0110"); break;
			case '7': watermark3.append ("0111"); break;
			case '8': watermark3.append ("1000"); break;
			case '9': watermark3.append ("1001"); break;
			case 'a': watermark3.append ("1010"); break;
			case 'b': watermark3.append ("1011"); break;
			case 'c': watermark3.append ("1100"); break;
			case 'd': watermark3.append ("1101"); break;
			case 'e': watermark3.append ("1110"); break;
			case 'f': watermark3.append ("1111"); break;
		}
    }
    size_t r1 = 128 % (I+O);
    for(size_t i = 0 ; i < r1; i++){
    	watermark1.append("0");
    	watermark2.append("0");
    	watermark3.append("0");
    }
    //cerr << watermark1 << endl << watermark2 << endl << watermark3 << endl;
}

bool
FSM::isCFSM()
{
	for(size_t i = 0; i < S; i++){
		size_t sum = 0;
		for(size_t j = 0; j < S; j++){
			sum += state2seqs[i][j].size();
		}
		if(sum != pow(2,I)){
			return false;
		}
	}
	cerr << "It is a complete FSM" << endl;
	return true;
}

bool
FSM::eqseq(string seq, string transition)
{
	for(size_t i = 0; i < transition.length(); i++){
		if(transition[i] == '-'){
			continue;
		}else if(seq[i] != transition[i]){
			return false;
		}
	}
	return true;
}

bool
FSM::extra(int state, string input, string output)
{
	for(size_t i = 0; i < S; i++){
		for(vector<pair<string, string>>::iterator it = state2seqs[state][i].begin(); it != state2seqs[state][i].end(); it++){
			if(eqseq(input, it->first)){
				if(it->second != output){
					return false;
				}
			}
		}
    }
    return true;
}

bool
FSM::extra2(int state, string input, string output)
{
	int sum = 0;
	for(size_t i = 0; i < S; i++){
		for(vector<pair<string, string>>::iterator it = state2seqs[state][i].begin(); it != state2seqs[state][i].end(); it++){
			sum++;
		}
    }
    if (sum == int(pow(2,I))) return false;
    return true;
}

bool
FSM::deadend(state node, string input, string output)
{
	for(size_t i = 0; i < S; i++){
		for(size_t j = 0; j < state2seqs[node.number][i].size(); j++){
			if(eqseq(input, state2seqs[node.number][i][j].first)){
				if(output != state2seqs[node.number][i][j].second){
					return true;
				}
			}
		}
	}
	for(size_t a = 0; a < int(pow(2,O)); a++){
		for(size_t na = 0; na < node.addedseq[a].size(); na++){
			if(node.addedseq[a].size()!=0){
				if(get<0>(node.addedseq[a][na]) == node.number){
					if(eqseq(input, get<2>(node.addedseq[a][na]))){
						if(a != stoi(output, nullptr, 2)){
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool
FSM::addwatermark(int w)
{
	int times = (watermark1.length()/(I+O));
	string* in1 = new string[times];
	string* out1 = new string[times];
	string tempwatermark;
	switch(w)
	{
		case 1: tempwatermark = watermark1; break;
		case 2: tempwatermark = watermark2; break;
		case 3: tempwatermark = watermark3; break;
	}
	for(size_t count = 0 ; count < times; count++){
		string current_input = tempwatermark.substr(0, I);
		string current_output = tempwatermark.substr(I, O);
		tempwatermark = tempwatermark.substr(I+O);
		in1[count] = current_input;
		out1[count] = current_output;
	}
	vector<int> cost;
	vector<queue<state>> sol;
	int mincost = INT_MAX;
	bool finish = false;
	for(size_t i = 0; i < S; i++){
		int addtransition = 0;
		int current_state = i;
		int current_seq = 0;
		vector<int> initrace;
		state initial_state(-1, i, initrace);
		initial_state.addedseq = new vector< tuple<size_t, size_t, string> >[int(pow(2,O))];
		queue<state> possible_sol;
		possible_sol.push(initial_state);
		int fail = 0;
		while(current_seq < times){
			//cerr << i << endl;
			//cerr << "c" << current_seq << endl;
			string current_input = in1[current_seq];
			string current_output = out1[current_seq];
			//cerr << current_input << " " << current_output << endl;
			queue<state> possible_sol2 = possible_sol;
			int intouput = stoi(current_output, nullptr, 2);
			size_t numofsols = possible_sol.size();
			bool move2next = false;
			bool addstate = true;
			for(size_t k = 0; k < numofsols; k++){
				state current_node = possible_sol.front();
				current_state = current_node.number;
				possible_sol.pop();
				if(deadend(current_node, current_input, current_output))continue;
				for(size_t a = 0; a < int(pow(2,O)); a++){
					for(size_t na = 0; na < current_node.addedseq[a].size(); na++){
						if(current_node.addedseq[a].size()!=0){
							cerr <<"seq " << get<0>(current_node.addedseq[a][na]) << get<1>(current_node.addedseq[a][na]) << get<2>(current_node.addedseq[a][na]) << " " << a;
						}
						
					}
				}
				for(size_t a = 0 ; a < current_node.trace.size(); a++){
					cerr << " ntrace " << current_node.trace[a] ; 
				}
				cerr << endl;
				if(current_node.addedseq[intouput].size() != 0){
					//cerr << "vect " << current_state << " " << get<2>(current_node.addedseq[intouput][0]) << endl;
					for(size_t na = 0; na < current_node.addedseq[intouput].size(); na++){
						if((get<0>(current_node.addedseq[intouput][na]) == current_state)){
							if(eqseq(current_input,get<2>(current_node.addedseq[intouput][na]) )){
								if(!move2next){
									queue<state> emp;
									swap(possible_sol2,emp);
									fail = 0;
								}
								size_t next = get<1>(current_node.addedseq[intouput][na]);
								//cerr <<"n" << current_node.number << next << get<2>(current_node.addedseq[intouput][na])<< endl;
								state node(current_state, (int)next, current_node.trace);
								node.trace.push_back(current_state);
								node.addedseq = new vector<tuple<size_t, size_t, string> >[int(pow(2,O))];
								for(size_t a = 0; a < int(pow(2,O)); a++){
									*(node.addedseq+a) = *(current_node.addedseq+a);
								}
								possible_sol2.push(node);
								move2next = true;
								addstate = false;
								break;
							}							
						}
					}
				}
				if(!seqs2state[intouput].empty()){
					for(vector< tuple<size_t, size_t, string> >::iterator it = seqs2state[intouput].begin(); it != seqs2state[intouput].end(); it++){
						if((get<0>(*it) == current_state)){
							if(eqseq(current_input,get<2>(*it) )){
								if(!move2next){
									queue<state> emp;
									swap(possible_sol2,emp);
									fail = 0;
								}
								size_t next = get<1>(*it);
								state node(current_state, (int)next, current_node.trace);
								node.trace.push_back(current_state);
								node.addedseq = new vector<tuple<size_t, size_t, string> >[int(pow(2,O))];
								for(size_t a = 0; a < int(pow(2,O)); a++){
									*(node.addedseq+a) = *(current_node.addedseq+a);
								}
								possible_sol2.push(node);
								move2next = true;
								addstate = false;
								break;
							}
						}
					}
				}
				if(!move2next){
					if(extra(current_state, current_input, current_output)){
						for(size_t u = 0; u < S; u++){
							if(extra2((int)u, current_input, current_output)){
								//cerr << "extra " << current_state << u << " " << current_input << " " << current_output << " " << intouput << endl;
								state node(current_state, (int)u, current_node.trace);
								node.addedseq = new vector<tuple<size_t, size_t, string> >[int(pow(2,O))];
								for(size_t a = 0; a < int(pow(2,O)); a++){
									*(node.addedseq+a) = *(current_node.addedseq+a);
								}
								tuple<size_t, size_t, string> tempseq(current_state, u, current_input);
								node.addedseq[intouput].push_back(tempseq);
								//cerr << "addedseq" << node.addedseq[intouput].size() << endl;
								node.trace.push_back(current_state);
								possible_sol.push(node);
								addstate = false;
								/*for(size_t a = 0; a < int(pow(2,O)); a++){
									for(size_t na = 0; na < current_node.addedseq[a].size(); na++){
										if(current_node.addedseq[a].size()!=0){
											cerr <<"c_seq " << get<0>(current_node.addedseq[a][na]) << get<1>(current_node.addedseq[a][na]) << get<2>(current_node.addedseq[a][na]) << " " << a;
										}
										
									}
								}
								cerr << endl;*/
							}
						}
						
					}
				}
			}
			/*if(addstate && deadend(current_node, current_input, current_output)){
				S++;
				vector< vector< pair<string, string> > > v1;
				for(size_t st1 = 0; st1 < S; st1++){
					vector<pair<string, string>> v2;
					if(st1 != S-1)state2seqs[st1].push_back(v2);
					v1.push_back(v2);
				}
				state2seqs.push_back(v1);
				current_seq--;
				possible_sol = possible_sol2;
			}*/
			if(possible_sol.empty()&&current_seq != times){
				cost.push_back(INT_MAX);
				queue<state> emptyq;
				sol.push_back(emptyq);
				break;
			}
			if(move2next){addtransition--; move2next = false; possible_sol = possible_sol2;}
			else{fail++;}
			if(fail > 3){
				fail = 0;
				queue<state> emp;
				swap(possible_sol,emp);
				possible_sol.push(possible_sol2.front());
			}
			addtransition++;
			current_seq++;
			if(addtransition > mincost){
				cost.push_back(INT_MAX);
				queue<state> emptyq;
				sol.push_back(emptyq);
				break;
			}
			if(current_seq == times)finish = true;

		}
		if(addtransition < mincost && finish){
			mincost = addtransition;
			cost.push_back(addtransition);
			sol.push_back(possible_sol);
			for(size_t h = 0; h < possible_sol.front().trace.size(); h++){
				cerr << possible_sol.front().trace[h] << " ";
			}
			cerr << endl;
			for(size_t a = 0; a < int(pow(2,O)); a++){
				for(size_t na = 0; na < possible_sol.front().addedseq[a].size(); na++){
					if(possible_sol.front().addedseq[a].size()!=0){
						cerr <<"seq " << get<0>(possible_sol.front().addedseq[a][na]) << get<1>(possible_sol.front().addedseq[a][na]) << get<2>(possible_sol.front().addedseq[a][na]) << " " << a;
					}
					
				}
			}
			cerr << endl;
		}
		if(i == S-1 && !finish){
			S++;
			vector< vector< pair<string, string> > > v1;
			for(size_t st1 = 0; st1 < S; st1++){
				vector<pair<string, string>> v2;
				if(st1 != S-1)state2seqs[st1].push_back(v2);
				v1.push_back(v2);
			}
			state2seqs.push_back(v1);
		}
	}
	size_t minindex = 0;
	for(size_t i = 0; i < S; i++){
		if(cost[i] == mincost){
			minindex = i;
			break;
		}
	}
	switch(w)
	{
		case 1: r1 = minindex; break;
		case 2: r2 = minindex; break;
		case 3: r3 = minindex; break;
	}
	for(size_t i = 0; i < pow(2,O); i++){
		for(size_t j = 0; j < sol[minindex].front().addedseq[i].size(); j++){
			seqs2state[i].push_back(sol[minindex].front().addedseq[i][j]);
			size_t s1 = get<0>(sol[minindex].front().addedseq[i][j]);
			size_t s2 = get<1>(sol[minindex].front().addedseq[i][j]);
			string appinput = get<2>(sol[minindex].front().addedseq[i][j]);
			string appoutput = "";
			for(size_t k = 0; k < O; k++){
				appoutput += '0';
			}
			int c = i;
			for(size_t k = 0; k < O; k++){
				if(c%2){
					appoutput[O-1-k] = '1';
				}
				c/=2;
			}
			pair<string, string> tempp(appinput, appoutput);
        	state2seqs[s1][s2].push_back(tempp);
		}
	}
	return true;
}

void
FSM::writewatermark(fstream& f)
{
	f << ".i " << I << '\n' << ".o " << O << '\n' << ".s " << S << '\n' << ".r s" << r1 << '\n';
	for(size_t i = 0; i < S; i++){
    	for(size_t j = 0; j < S; j++){
    		for(vector<pair<string, string>>::iterator it = state2seqs[i][j].begin(); it != state2seqs[i][j].end(); it++){
				f << it->first << " s" << i << " s" << j << " " << it->second << '\n';
			}
    	}
		
    }
    f << ".e";
}