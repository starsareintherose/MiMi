#include <iostream>
#include <fstream>
#include <sstream> 
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int intype=0, outype=0;
char *fn;
char *otn;

class Sample {
public:
	unsigned ntax, nchar;
	string *taxas, *chars;
	Sample(unsigned ntax, unsigned nchar):
       		ntax(ntax), nchar(nchar) {
			taxas = new string[ntax];
			chars = new string[ntax];
		};
//	~Sample(){
//		delete[] taxas;
//		delete[] chars;	
//	}
};

void show_help(int help_num);
Sample read_input();
Sample readFas();
Sample readPhy();
void write_output(class Sample sam);
void writeFas(class Sample sam);
void writePhy(class Sample sam);
void writeTnt(class Sample sam);
void writeNexus(class Sample sam);

Sample readPhy(){
//  unsigned ntax, nchar;
//  string *taxas, *chars;
//  Sample sam(ntax, nchar); 
  int num=0;
  //read file
  ifstream seqfile;
  seqfile.open(fn);
  //read number of taxa and charcter
  string sntax, snseq, snall;
  cout << "Reading from the file" << endl; 
  getline(seqfile,snall);
  istringstream istr(snall); 
  istr >> sntax; istr >> snseq;
  int ntax, nchar;
  ntax = stoi (sntax); nchar = stoi (snseq); // string to int
  cout << "ntax=" << ntax << " nseq=" << nchar << endl;
  Sample sam(ntax, nchar);   
  //read sequence
  int lennum;
  for(lennum=0;lennum<sam.ntax;lennum++){
    getline(seqfile,snall);
    istringstream istr(snall);
    istr >> sam.taxas[lennum]; istr >> sam.chars[lennum];
   cout << "tax" << lennum << " is " << sam.taxas[lennum] << "; seq" << lennum << " is " << sam.chars[lennum] << endl;   
  }
  seqfile.close();
  // return to class
  return sam;
}

Sample readFas(){
  int ntax, nchar, lnum;
  ifstream seqfile;
  seqfile.open(fn);
  ntax = 0;
  string temln;
  for(lnum=0;getline(seqfile,temln);lnum++){
    if(temln[0]=='>'){
      ntax++;
    }
  }
  cout << "lnum= " << lnum << ", ntax= " << ntax << endl;
  seqfile.clear();
  seqfile.seekg(0);

  string *str_a = new string;
  string *str_b = new string;
  int r = lnum/ntax;
  for (int i=0; i<r;i++){
    getline(seqfile, *str_a);
    if(i>0){
       *str_b = *str_b + *str_a; 
    } 
  }
  nchar = (*str_b).length();
  delete str_a;
  str_a = nullptr;
  delete str_b;
  str_b = nullptr;

  cout << "ntax= " << ntax << ", nchar= " << nchar << endl; 
  seqfile.clear();
  seqfile.seekg(0);

  Sample sam(ntax, nchar);
  string *str_c = new string;
  for (int a=1, b=0;a<=lnum;a++){
    if(a%r==1){
      getline(seqfile,sam.taxas[b]);
      sam.taxas[b].erase(0,1);  
    }
    if(a%r>1){
      getline(seqfile,*str_c);
      sam.chars[b] = sam.chars[b] + *str_c;
    }
    if(a%r==0){
      getline(seqfile,*str_c);
      sam.chars[b] = sam.chars[b] + *str_c;
      b++;
    }   
  }
  delete str_c;
  str_c = nullptr; 
  seqfile.close();
  int lennum;
  for(lennum=0;lennum<sam.ntax;lennum++){
   cout << "tax" << lennum << " is " << sam.taxas[lennum] << "; seq" << lennum << " is " << sam.chars[lennum] << endl;
  }
  return sam;
}

Sample readTnt(){
  int ntax, nchar;
  Sample sam(ntax,nchar);
  return sam;
}

void writeFas(class Sample sam){
  ofstream outputFile(otn);
  if (outputFile.is_open()) {
      for(int i=0;i<sam.ntax;i++){
        outputFile << ">" << sam.taxas[i] << endl;
        outputFile << sam.chars[i] << endl;
      }
    } else {
      cout << "File can't be written" << endl;
    }
  outputFile.close();
}

void writePhy(class Sample sam){
  ofstream outputFile(otn);
  if (outputFile.is_open()) {
      outputFile << sam.ntax << " " << sam.nchar << endl;
      for(int i=0;i<sam.ntax;i++){
        outputFile << sam.taxas[i] << "\t" << sam.chars[i]  << endl;
      }
    } else {
      cout << "File can't be written" << endl;
    }
  outputFile.close();
}

void writeNexus(class Sample sam){
  ofstream outputFile(otn);
  if (outputFile.is_open()) {
      outputFile << "#NEXUS" << endl << "Begin TAXA;" << endl << "\tDimensions ntax=" << sam.ntax << ";" << endl << "\tTaxLabels";
      for(int i1=0;i1<sam.ntax;i1++){
        outputFile << " " << sam.taxas[i1] << endl;
      }
      outputFile << ";" << endl << "End;" << endl << endl << "Begin data;" << endl << "\tDimensions nchar=" << sam.nchar << ";" << endl << "\tFormat datatype=dna missing=? gap=-;" << endl << "\tMatrix" << endl;
      for(int i2=0;i2<sam.ntax;i2++){
        outputFile << "\t\t" << sam.taxas[i2] << "\t" << sam.chars[i2] << endl;
      }
      outputFile << "\t;" << endl << "End;" << endl;
    } else {
      cout << "File can't be written" << endl;
    }
  outputFile.close();
}

void writeTnt(class Sample sam){
  ofstream outputFile(otn);
  if (outputFile.is_open()) {
      outputFile << sam.nchar << " " << sam.ntax << endl;
      outputFile << "\' \'" << endl;
      for(int i=0;i<sam.ntax;i++){
        outputFile << sam.taxas[i] << "\t" << sam.chars[i]  << endl;
      }
      outputFile << "proc / ;" << endl;
    } else {
      cout << "File can't be written" << endl;
    }
  outputFile.close();
}


void procargs (int nargs, char ** arg){ //*arg 视为整体，是字符串指针
  int i;
  char * cp;//cp 字符指针 *p 第一个char
  //string ext1, ext2;
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
	if (*cp=='f') {intype=1;  cout << "intype is fasta, " << intype << endl;}
	if (*cp=='n') intype=2;
	if (*cp=='p') {intype=3; cout << "intype is phylip, " << intype << endl;}
	break;
      case 'h': show_help(1); break;
      case 'i': cp++; fn = cp; break;
      case 'o': cp++; otn = cp; break;
      case 't':
	cp++;
	if (*cp=='f') outype=1; 
	if (*cp=='n') outype=2;
	if (*cp=='p') {outype=3; cout << "outype is phylip, " << intype << endl;}
	if (*cp=='t') outype=4;
	break;
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

Sample read_input (void){
  int ntax, nchar;
  Sample sam(ntax,nchar);
  if (intype==1) sam = readFas();
  if (intype==3) sam = readPhy();
  return sam;
}

void write_output (class Sample sam){
  if (outype==1) writeFas(sam);
  if (outype==2) writeNexus(sam);
  if (outype==3) writePhy(sam);
  if (outype==4) writeTnt(sam);
}

int main(int argc, char **argv){
  procargs (argc, argv);
  if(intype!=0&&outype!=0){
  Sample sam = read_input();
  write_output(sam);
  }
  int  i;
  for (i = 0; i < argc; i++) cout << "argument "<< i << " is " << *(argv+i) << endl;
  return 1;
}
