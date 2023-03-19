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
Sample readTnt();
Sample readNex();
void write_output(class Sample sam);
void writeFas(class Sample sam);
void writePhy(class Sample sam);
void writeTnt(class Sample sam);
void writeNex(class Sample sam);
bool isNum(string strnum);
string to_lower(string stri);
string add_space(char x, string str_old);
string checktype(string str);
int countfre(string str, char c);

Sample readPhy(){
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
  // check line number and taxa number
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
  // check the nchar
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
  // create class
  Sample sam(ntax, nchar);
  // get class
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
  return sam;
}

Sample readTnt(){
  int ntax, nchar;
  ifstream seqfile;
  seqfile.open(fn);
  // get nchar and ntax
  string stri, sntax, snchar;
  for(int i=0;i<1;){
      getline(seqfile,stri);
      istringstream istr(stri); 
      istr >> snchar; istr >> sntax;
      if(isNum(sntax) && isNum(snchar)){
              ntax=stoi(sntax);
	      nchar=stoi(snchar);
	      i++;
      }
  }
  // create class
  Sample sam(ntax,nchar);
  // get class
  int lennum;
  for(lennum=0;lennum<sam.ntax;lennum++){
    getline(seqfile,stri);
    istringstream istr(stri);
    istr >> sam.taxas[lennum]; istr >> sam.chars[lennum];
   cout << "tax" << lennum << " is " << sam.taxas[lennum] << "; seq" << lennum << " is " << sam.chars[lennum] << endl;   
  }
  seqfile.close();
  return sam;
}

bool isNum(string strnum) {
	char* p;
	strtol(strnum.c_str(), &p, 10);
	return *p == 0;
}

Sample readNex(){
  int ntax, nchar;
  // open file 
  ifstream seqfile;
  seqfile.open(fn);
  // some tem 
  string snall, stri, str_a, str_b;
  bool found = false;
  bool found_ntax = false;
  bool found_nchar = false;
  bool found_equal = false;
  char x = '=';
  int lnum, e, eulnum;
  e = 0;
  // getline line by line
  for(lnum=0;getline(seqfile,snall);lnum++){
  str_a = to_lower(snall);
  str_b = add_space(x,str_a);
  istringstream istr(str_b);
        // convert to words
	// e will enter the ntax/nchar function just after statisfy the `=`
	while(istr>> stri){
	if(stri=="dimensions"){
		found = true;
	} 
	
		if(stri=="ntax"){
			found_ntax = true;
		}
		if(stri=="nchar"){
			found_nchar = true;
		}
		if(stri=="="){
			found_equal = true;
		}
		if (found_ntax&&found_equal){
			e++;
		       if(e>1){	
			if(stri.back()==';'){
				stri.pop_back();
				found = false;
			}
			ntax = stoi(stri);
			found_equal = false;
			found_ntax = false;	
			e=0;
		       }
		}
		
		if (found_nchar&&found_equal){
			e++;
			if(e>1){
			if(stri.back()==';'){
				stri.pop_back();
				found = false;
			} 
			nchar = stoi(stri);
			found_equal = false;
			found_nchar = false;
			e=0;
			}
		}
		if(stri=="matrix"){
			// get the position of matrix
			eulnum = lnum+1;
		}
    }
  }
  // go back
  seqfile.clear();
  seqfile.seekg(0);
  // create class
  Sample sam(ntax,nchar);
  // some temp, z is line number, l is the string arrary number
  int z=0; int l=0;
  // read line by line
  while(getline(seqfile,snall)){
  // convert to word
  istringstream istr(snall);
  // limit the read line number
  if(z>(eulnum-1)&&z<(eulnum+sam.ntax)){
    istr >> sam.taxas[l];
    istr >> sam.chars[l];
    cout << "tax" << l << " is " << sam.taxas[l] << "; seq" << l << " is " << sam.chars[l] << "; l=" << l << endl; 
    l++;
  }
  z++;
  }
  return sam;
}
string add_space (char x, string str_old) {
	int i;
	string str_new;
	for (i=0; i<str_old.length(); i++) {
		if (str_old[i] != x) {
			str_new=str_new+str_old[i];
		} else {
			str_new=str_new+" "+str_old[i]+" ";
		}
	}
	return str_new;
}

string to_lower(string stri){
  transform(stri.begin(),stri.end(),stri.begin(),::tolower);
  return stri;
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

void writeNex(class Sample sam){
  ofstream outputFile(otn);
  string datatype;
  datatype = checktype(sam.chars[0]);
  if (outputFile.is_open()) {
      outputFile << "#NEXUS" << endl; 
//      outputFile << "Begin TAXA;" << endl << "\tDimensions ntax=" << sam.ntax << ";" << endl << "\tTaxLabels";
//      for(int i1=0;i1<sam.ntax;i1++){
//        outputFile << " " << sam.taxas[i1];
//      }
//      outputFile << ";" << endl << "End;" << endl << endl;
      outputFile << "Begin data;" << endl << "\tDimensions nchar=" << sam.nchar << " ntax=" << sam.ntax << ";" << endl << "\tFormat datatype=" << datatype << " missing=? gap=-;" << endl << "\tMatrix" << endl;
      for(int i2=0;i2<sam.ntax;i2++){
        outputFile << "\t\t" << sam.taxas[i2] << "\t" << sam.chars[i2] << endl;
      }
      outputFile << "\t;" << endl << "End;" << endl;
    } else {
      cout << "File can't be written" << endl;
    }
  outputFile.close();
}

string checktype(string str){
  float a, c, t, g, zero, one, two, dna, standard;
  char ca='a', cc='c', ct='t', cg='g', czero='0', cone='1', ctwo='2';
  string datatype;
  a = countfre(str, ca);
  c = countfre(str, cc);
  t = countfre(str, ct);
  g = countfre(str, cg);
  zero = countfre(str, czero);
  one = countfre(str, cone);
  two = countfre(str, ctwo);

  dna = a+c+t+g;
  standard = zero+one+two;

  if((dna/str.length())>0.7){
      datatype = "dna"; 
  } else if ((standard/str.length())>0.7){
      datatype = "standard";
  } else {
      datatype = "protein";
  }
  return datatype;
}

int countfre(string str, char c){
  int num;
  str = to_lower(str);
  num = count(str.begin(),str.end(),c); 
  return num; 
}

void writeTnt(class Sample sam){
  ofstream outputFile(otn);
  if (outputFile.is_open()) { 
      outputFile << "xread" << endl << "\' \'" << endl;
      outputFile << sam.nchar << " " << sam.ntax << endl;
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
	if (*cp=='t') {intype=4;}
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
  if (intype==2) sam = readNex();
  if (intype==3) sam = readPhy();
  if (intype==4) sam = readTnt();
  return sam;
}

void write_output (class Sample sam){
  if (outype==1) writeFas(sam);
  if (outype==2) writeNex(sam);
  if (outype==3) writePhy(sam);
  if (outype==4) writeTnt(sam);
}

bool checkalign(class Sample sam){
  int a=0, b=0;
  a = sam.chars[0].length();
  bool aligned = true;
  for(int i=1;i<(sam.ntax-1)&&aligned;i++){
    b = sam.chars[i].length();
    aligned = (a==b);
  }
  return aligned;
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
