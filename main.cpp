#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Basic_arg {
 public:
  int intype = 0, outype = 0;
  char *itn, *otn;
  Basic_arg(int intype, int outype, char* itn, char* otn)
      : intype(intype), outype(outype), itn(itn), otn(otn){};
};

class Sample {
 public:
  unsigned ntax, nchar;
  string *taxas, *chars;
  Sample(unsigned ntax, unsigned nchar) : ntax(ntax), nchar(nchar) {
    taxas = new string[ntax];
    chars = new string[ntax];
  };
};

Basic_arg procargs(int nargs, char** arg);
Sample read_input(char* itn, int intype);
void show_help(int help_num);
Sample readFas(char* itn);
Sample readPhy(char* itn);
Sample readTnt(char* itn);
Sample readNex(char* itn);
void write_output(class Sample sam, char* otn, int outype);
void writeFas(class Sample sam, char* otn);
void writePhy(class Sample sam, char* otn);
void writeTnt(class Sample sam, char* otn);
void writeNex(class Sample sam, char* otn);
bool isNum(string strnum);
bool checkalign(class Sample sam);
string to_lower(string stri);
string add_space(char x, string str_old);
string rep_space(string str_old);
string del_space(string str_old);
string checktype(string str);
int countfre(string str, char c);
int checkextension(string str);

Sample readPhy(char* itn) {
  // read file
  ifstream matrixfile;
  matrixfile.open(itn);
  // read number of taxa and charcter
  string sntax, snseq, snall;
  getline(matrixfile, snall);
  istringstream istr(snall);
  istr >> sntax;
  istr >> snseq;
  int ntax, nchar;
  ntax = stoi(sntax);
  nchar = stoi(snseq);  // string to int
  Sample sam(ntax, nchar);
  // read sequence
  unsigned int lennum;
  for (lennum = 0; lennum < sam.ntax; lennum++) {
    getline(matrixfile, snall);
    istringstream istr(snall);
    istr >> sam.taxas[lennum];
    istr >> sam.chars[lennum];
  }
  matrixfile.close();
  // return to class
  return sam;
}

Sample readFas(char* itn) {
  int ntax, nchar, lnum;
  ifstream matrixfile;
  matrixfile.open(itn);
  // use vector to read once use serveral times
  vector<string> file_content;
  // check line number and taxa number
  ntax = 0;
  string temln;
  for (lnum = 0; getline(matrixfile, temln); lnum++) {
    file_content.push_back(temln);
    if (temln[0] == '>') {
      ntax++;
    }
  }
  matrixfile.close();
  // check the nchar
  string str_i;
  int r;
  if (file_content.size() != 0) {
    r = file_content.size() / ntax;
  } else {
    cout << "MiMi:\tInput file contains 0 line" << endl;
    exit(0);
  }
  for (int i = 0; i < r; i++) {
    if (i > 0) {
      file_content[i] = del_space(file_content[i]);
      str_i = str_i + file_content[i];
    }
  }
  nchar = str_i.length();
  // create class
  Sample sam(ntax, nchar);
  // get class
  for (int a = 0, b = 0; a < (int)file_content.size(); a++) {
    if ((a + 1) % r == 1) {
      sam.taxas[b] = file_content[a];
      sam.taxas[b].erase(0, 1);
      sam.taxas[b] = rep_space(sam.taxas[b]);
    }
    if ((a + 1) % r > 1) {
      file_content[a] = del_space(file_content[a]);
      sam.chars[b] = sam.chars[b] + file_content[a];
    }
    if ((a + 1) % r == 0) {
      file_content[a] = del_space(file_content[a]);
      sam.chars[b] = sam.chars[b] + file_content[a];
      b++;
    }
  }
  return sam;
}

Sample readTnt(char* itn) {
  int ntax, nchar;
  ifstream matrixfile;
  matrixfile.open(itn);
  // get nchar and ntax
  string stri, sntax, snchar;
  for (int i = 0; i < 1;) {
    getline(matrixfile, stri);
    istringstream istr(stri);
    istr >> snchar;
    istr >> sntax;
    if (isNum(sntax) && isNum(snchar)) {
      ntax = stoi(sntax);
      nchar = stoi(snchar);
      i++;
    }
  }
  // create class
  Sample sam(ntax, nchar);
  // get class
  unsigned int lennum;
  for (lennum = 0; lennum < sam.ntax; lennum++) {
    getline(matrixfile, stri);
    istringstream istr(stri);
    istr >> sam.taxas[lennum];
    istr >> sam.chars[lennum];
  }
  matrixfile.close();
  return sam;
}

bool isNum(string strnum) {
  char* p;
  strtol(strnum.c_str(), &p, 10);
  return *p == 0;
}

Sample readNex(char* itn) {
  int ntax, nchar;
  // open file
  ifstream matrixfile;
  matrixfile.open(itn);
  // some var
  string snall, stri, str_a, str_b;
  bool found_ntax = false, found_nchar = false, found_equal = false;
  char x = '=';
  // get line number and read line to vector
  int lnum;
  unsigned int eulnum;
  vector<string> file_content;
  while (getline(matrixfile, snall)) {
    file_content.push_back(snall);
  }
  matrixfile.close();
  // getline line by line
  for (lnum = 0; lnum < (int)file_content.size(); lnum++) {
    str_a = to_lower(file_content[lnum]);
    str_b = add_space(x, str_a);
    istringstream istr(str_b);
    // convert to words
    while (istr >> stri) {
      if (stri == "ntax") {
        found_ntax = true;
      } else if (stri == "nchar") {
        found_nchar = true;
      } else if (stri == "=") {
        found_equal = true;
      } else if (found_ntax && found_equal) {
        if (stri.back() == ';') {
          stri.pop_back();
        }
        ntax = stoi(stri);
        found_equal = false;
        found_ntax = false;
      } else if (found_nchar && found_equal) {
        if (stri.back() == ';') {
          stri.pop_back();
        }
        nchar = stoi(stri);
        found_equal = false;
        found_nchar = false;
      } else if (stri == "matrix") {
        eulnum = lnum + 1;
      }
    }
  }
  // create class
  Sample sam(ntax, nchar);
  // read line by line, limit line number
  int l = 0;
  for (unsigned int z = eulnum; z < (eulnum + sam.ntax); z++) {
    // convert to word
    istringstream istr(file_content[z]);
    istr >> sam.taxas[l];
    istr >> sam.chars[l];
    l++;
  }
  return sam;
}
string add_space(char x, string str_old) {
  int i;
  string str_new;
  for (i = 0; i < (int)str_old.length(); i++) {
    if (str_old[i] != x) {
      str_new = str_new + str_old[i];
    } else {
      str_new = str_new + " " + str_old[i] + " ";
    }
  }
  return str_new;
}

string del_space(string str_old) {
  int i;
  string str_new;
  char x = ' ';
  for (i = 0; i < (int)str_old.length(); i++) {
    if (str_old[i] != x) {
      str_new = str_new + str_old[i];
    }
  }
  return str_new;
}

string rep_space(string str_old) {
  int i;
  string str_new;
  char x = ' ', c = '_';
  for (i = 0; i < (int)str_old.length(); i++) {
    if (str_old[i] != x) {
      str_new = str_new + str_old[i];
    } else {
      str_new = str_new + c;
    }
  }
  return str_new;
}

string to_lower(string stri) {
  transform(stri.begin(), stri.end(), stri.begin(), ::tolower);
  return stri;
}

void writeFas(class Sample sam, char* otn) {
  ofstream matrixfile(otn);
  for (unsigned int i = 0; i < sam.ntax; i++) {
    matrixfile << ">" << sam.taxas[i] << endl;
    matrixfile << sam.chars[i] << endl;
  }
  matrixfile.close();
}

void writePhy(class Sample sam, char* otn) {
  ofstream matrixfile(otn);
  matrixfile << sam.ntax << " " << sam.nchar << endl;
  for (unsigned int i = 0; i < sam.ntax; i++) {
    matrixfile << sam.taxas[i] << "\t" << sam.chars[i] << endl;
  }
  matrixfile.close();
}

void writeNex(class Sample sam, char* otn) {
  ofstream matrixfile(otn);
  string datatype;
  datatype = checktype(sam.chars[0]);
  matrixfile << "#NEXUS\nBegin data;\n\tDimensions nchar=" << sam.nchar
             << " ntax=" << sam.ntax << ";\n\tFormat datatype=" << datatype
             << " missing=? gap=-;\n\tMatrix" << endl;

  for (unsigned int i2 = 0; i2 < sam.ntax; i2++) {
    matrixfile << "\t\t" << sam.taxas[i2] << "\t" << sam.chars[i2] << endl;
  }
  matrixfile << "\t;\nEnd;" << endl;
  matrixfile.close();
}

string checktype(string str) {
  // some var
  float a, c, t, g, zero, one, two, dna, standard;
  char ca = 'a', cc = 'c', ct = 't', cg = 'g', czero = '0', cone = '1',
       ctwo = '2';
  string datatype;
  // count fre
  a = countfre(str, ca);
  c = countfre(str, cc);
  t = countfre(str, ct);
  g = countfre(str, cg);
  zero = countfre(str, czero);
  one = countfre(str, cone);
  two = countfre(str, ctwo);
  // summary dna or morphology
  dna = a + c + t + g;
  standard = zero + one + two;
  // use percentage to test
  if ((dna / str.length()) > 0.7) {
    datatype = "dna";
  } else if ((standard / str.length()) > 0.7) {
    datatype = "standard";
  } else {
    datatype = "protein";
  }
  return datatype;
}

int countfre(string str, char c) {
  int num;
  str = to_lower(str);
  num = count(str.begin(), str.end(), c);
  return num;
}

void writeTnt(class Sample sam, char* otn) {
  ofstream matrixfile(otn);
  matrixfile << "xread\n\' \'\n" << sam.nchar << " " << sam.ntax << endl;
  for (unsigned int i = 0; i < sam.ntax; i++) {
    matrixfile << sam.taxas[i] << "\t" << sam.chars[i] << endl;
  }
  matrixfile << "proc / ;" << endl;
  matrixfile.close();
}

Basic_arg procargs(int nargs, char** arg) {
  int i, sta = 0, intype = 0, outype = 0;
  string para, inputfile, outputfile;
  char *itn, *otn;
  // no arg, show help
  if (nargs == 1) {
    show_help(0);
    exit(0);
    sta = 2;
  }
  // recognize arg
  for (i = 1; i < nargs; i++) {
    // to string
    string para(arg[i]);
    if ((para == "-h") || (para == "--help")) {
      show_help(1);
      sta = 2;
    } else if ((para == "-i") || (para == "--input")) {
      if ((i + 1) < nargs) {
        i++;
        itn = arg[i];
        string inputfile(arg[i]);
        intype = checkextension(inputfile);
        sta++;
      } else {
        cout << "MiMi:\tOInput file name must be defined" << endl;
      }
    } else if (((para == "-o") || (para == "--output"))) {
      if ((i + 1) < nargs) {
        i++;
        otn = arg[i];
        string outputfile(arg[i]);
        outype = checkextension(outputfile);
        sta++;
      } else {
        cout << "MiMi:\tOutput file name must be defined" << endl;
      }
    } else {
      cout << "MiMi:\tUnknown arguments, please use -h to check" << endl;
      exit(0);
    }
  }
  if (sta != 2) {
    cout << "MiMi:\tInput and Output can't be empty" << endl;
    exit(0);
  }
  Basic_arg arguvar(intype, outype, itn, otn);
  return arguvar;
}

int checkextension(string str) {
  int loc, type;
  string extension;
  loc = str.rfind('.');
  if (loc) {
    extension = str.substr(loc + 1);
  } else {
    cout << "MiMi:\tPlease sepecifc the extension name" << endl;
    exit(0);
  }
  extension = to_lower(extension);
  if ((extension == "fas") || (extension == "fasta")) {
    type = 1;
  } else if ((extension == "nex") || (extension == "nexus")) {
    type = 2;
  } else if ((extension == "phy") || (extension == "phylip")) {
    type = 3;
  } else if ((extension == "tnt") || (extension == "ss")) {
    type = 4;
  } else {
    cout << "MiMi:\tUnknown format" << endl;
    exit(0);
  }
  return type;
}

void show_help(int help_num) {
  if (help_num == 0) {
    cout << "MiMi, GPL, Guoyi Zhang, 2023.\nPlease use -h to see more help"
         << endl;
  } else {
    cout << "\n  ／l、    \t.  . .  .\n（ﾟ､ ｡ ７    \t|\\/|*|\\/|*\n  l  ~ヽ   "
            " \t|  |||  ||\n  じしf_,)ノ\t|  |||  ||\n\nMorphology into "
            "Molecules into\nGPL;\tGuoyi "
            "Zhang;\t2023\n\nArguments:\n-h\t--help;\n-i\t--input\t\t${"
            "filename};\n-o\t--output\t${filename};\n\nAccepted "
            "formats:\nfas\tfasta;\nnex\tnexus;\nphy\tphylip;\ntnt\tss;"
         << endl;
  }
}

Sample read_input(char* itn, int intype) {
  int ntax = 0, nchar = 0;
  Sample sam(ntax, nchar);
  ifstream matrixfile;
  matrixfile.open(itn);
  if (matrixfile.is_open()) {
    if (intype == 1) sam = readFas(itn);
    if (intype == 2) sam = readNex(itn);
    if (intype == 3) sam = readPhy(itn);
    if (intype == 4) sam = readTnt(itn);
  } else {
    cout << "MiMi:\tInput file can't be open" << endl;
    exit(0);
  }
  return sam;
}

void write_output(class Sample sam, char* otn, int outype) {
  ofstream matrixfile(otn);
  if (matrixfile.is_open()) {
    if (outype == 1) writeFas(sam, otn);
    if (outype == 2) writeNex(sam, otn);
    if (outype == 3) writePhy(sam, otn);
    if (outype == 4) writeTnt(sam, otn);
  } else {
    cout << "MiMi:\tOutput file can't be open" << endl;
    exit(0);
  }
}

bool checkalign(class Sample sam) {
  int a = 0, b = 0;
  unsigned int x = 0;
  a = sam.nchar;
  bool aligned = true;
  for (unsigned int i = 0; i < sam.ntax; i++) {
    b = sam.chars[i].length();
    if (a == b) {
      x++;
    }
  }
  if (x != sam.ntax) {
    aligned = false;
  }
  return aligned;
}

int main(int argc, char** argv) {
  Basic_arg arguvar = procargs(argc, argv);
  if (arguvar.intype != 0 && arguvar.outype != 0) {
    Sample sam = read_input(arguvar.itn, arguvar.intype);
    cout << "MiMi:\tInput\tfinished" << endl;
    if (!checkalign(sam)) {
      cout << "MiMi:\tInput file should be aligned" << endl;
      exit(0);
    }
    write_output(sam, arguvar.otn, arguvar.outype);
    cout << "MiMi:\tOutput\tfinished" << endl;
  }
  return 0;
}
