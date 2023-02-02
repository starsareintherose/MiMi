#include <iostream>
#include <fstream>
#include<sstream> 
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int datatype=0, ntax, nseq;
char * fn;

void show_help(int help_num);
void read_input(void);

int readPhy(){
  int num=0;
  cout << "readphy is working, filename is " << fn << endl;
  //read file
  ifstream seqfile;
  seqfile.open(fn);
  //read number of taxa and charcter
  string sntax, snseq, snall;
  cout << "Reading from the file" << endl; 
  getline(seqfile,snall);
  istringstream istr(snall); 
  istr >> sntax; istr >> snseq;
  ntax = stoi (sntax); nseq = stoi (snseq); // string to int
  cout << "ntax=" << ntax << " nseq=" << nseq << endl;    
  //read sequence

  seqfile.close();
  return num;
}

void procargs (int nargs, char ** arg){ //*arg 视为整体，是字符串指针
  int i;
  char * cp;//cp 字符指针 *p 第一个char

  //no arg, show help
  if (nargs==1){
    show_help(0);
    exit(0);
  }
  //recognize arg
  for (i=1;i<nargs;i++){
    cp = *(arg+i);
    //error arg
    if (*cp != '-'){
      cout << "Unknown arguments, please use -h to check" << endl;
      exit(0);
    }
    //check arg
    switch (*++cp){//先++
      case 'f':
        cp++;
	if (*cp=='f') datatype=1; 
	if (*cp=='n') datatype=2;
	if (*cp=='p') {datatype=3; cout << "datatype is phylip, " << datatype << endl;}
	break;
      case 'h': show_help(1); break;
      case 'i': cp++; fn = cp; break;
      default: cout << "Unrecognized agruments " << *(arg+i) << endl; exit(0);
    }
  }
}


void show_help (int help_num){
  if (help_num == 0){
    cout  << "SeqConvert GPL, please use -h to see more help" << endl;
  } else {
    cout << "-f from: -ff fasta; -fn nexus; -fp phylip;\n-h help;\n-i input: -i${filename};" << endl;
  }
}

void read_input (void){
  if (datatype==3) readPhy();
}

int main(int argc, char **argv){
  procargs (argc, argv);
  read_input();
  int  i;
  for (i = 0; i < argc; i++) cout << "argument "<< i << " is " << *(argv+i) << endl;
  return 1;
}
