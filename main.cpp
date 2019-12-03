#include "fsm.h"
#include "lib/tm_usage.h"

using namespace std;

void help_message() {
    cout << "usage: time watermark -i ifsm.kiss -o ofsm.kiss -m md5_1.dat -m md5_2.dat -m md5_3.dat" << endl;
}

int main(int argc, char* argv[])
{
    if(argc != 11) {
    	cerr << argc;
       help_message();
       return 0;
    }
    CommonNs::TmUsage tmusg;
    CommonNs::TmStat stat;
    FSM fsm;
    //////////// read the input file /////////////
    fstream fin(argv[2]);
    fstream fout;
    fout.open(argv[4],ios::out);
    fstream f1(argv[6]);
    fstream f2(argv[8]);
    fstream f3(argv[10]);
    fsm.readfsm(fin);
    fsm.readwatermark(f1,f2,f3);

    tmusg.periodStart();
    if(fsm.isCFSM()){cout << "It is a complete FSM" << endl; return 0;} //Output message is included in isCFM()
    fsm.addwatermark(1);
    fsm.writewatermark(fout);
    fsm.addwatermark(2);
    fsm.addwatermark(3);

    tmusg.getPeriodUsage(stat);
    cout <<"# run time = " << (stat.uTime + stat.sTime) / 1000000.0 << "sec" << endl;
    cout <<"# memory =" << stat.vmPeak / 1000.0 << "MB" << endl;
    
    fin.close();
    f1.close();
    f2.close();
    f3.close();
    fout.close();
}
