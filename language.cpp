#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<iomanip>
#include<cstring>
#include<string>
#include<fstream>
#include<numeric>

#define NC '\0'
#define PRIME 107
#define separator '#'
#define newline std::cout << '\n'
#define record Record<V>

void split(std::string& , std::string& , std::vector<std::string>& , char*);
int stringToInt(const std::string&);
int hexToDecimal(const std::string&);
std::set<std::string> pseudoOps{ "START","BYTE","WORD","RESW","RESB","END" };	//set of pseudo ops       


//Opcode class
class OpCode {
private:
    std::string mnemonics;
    std::string machineCode;
    int numberOfOperands;
    int instructionSize;

public:
    OpCode() : mnemonics("undefined"), machineCode("undefined"), numberOfOperands(0), instructionSize(0) {}

    OpCode(const OpCode& opc) : mnemonics(opc.mnemonics), machineCode(opc.machineCode), numberOfOperands(opc.numberOfOperands),
        instructionSize(opc.instructionSize) {}

    OpCode(const std::string& mnics, const std::string& mCode, const int nop, const int size) : mnemonics(mnics), machineCode(mCode),
        numberOfOperands(nop), instructionSize(size) {}

    std::string& getMnemonics() {
        return mnemonics;
    }

    std::string& getMachineCode() {
        return machineCode;
    }

    int getNumberOfOperands() {
        return numberOfOperands;
    }

    int getInstructionSize() {
        return instructionSize;
    }

	//overloads the << operator to write to the console.
    friend std::ostream& operator<<(std::ostream& out, OpCode& opc) {       
        out << opc.mnemonics << ": " << opc.machineCode << " " << opc.numberOfOperands << " " << opc.instructionSize;
        return out;
    }

	//overloads the == operator and compare two mnemonics
    bool operator==(const std::string& key) {       
        return mnemonics == key;
    }

	//overloads the << operator to write to a file
    std::ofstream& operator<<(std::ofstream& outf) {        
        outf << mnemonics << " : " << machineCode << "  " << numberOfOperands << " " << instructionSize;
        return outf;
    }

	//conversion/overloading : converts (tokenizes) the string into OpCode object
    OpCode(const std::string& data) {                   
        std::vector<std::string> vec;
        int i = 0, size = data.size();
        while (i < size) {
            std::string s;
            while (i < size && data[i] != ' ') {
                s.push_back(data[i]);
                ++i;
            }
            
			vec.push_back(s);
            ++i;
        }
                
		mnemonics = vec[0];
		machineCode = vec[1];       
		numberOfOperands = stringToInt(vec[2]);
		instructionSize = stringToInt(vec[3]);
    }
};


class Registers {
private:
    std::string registerName;
    std::string binary;

public:
    Registers() : registerName("undefined"), binary("undefined") {}

    Registers(const Registers& rgc) : registerName(rgc.registerName), binary(rgc.binary) {}

    Registers(const std::string& register_name, const std::string& binary_form) : registerName(register_name), binary(binary_form) {}

    std::string& getRegisterName() {
        return registerName;
    }

    std::string& getBinary() {
        return binary;
    }

	//overloads the << operator to write to the console.
    friend std::ostream& operator<<(std::ostream& outc, Registers& reg) {       
        outc << reg.registerName << ": " << reg.binary;
        return outc;
    }

	//overloads the == operator and compares two registerName(s).
    bool operator==(const std::string& key) {       
        return registerName == key;
    }

	//overloads the << operator to write to a file
    std::ofstream& operator<<(std::ofstream& outf) {        
        outf << registerName << " : " << binary;
        return outf;
    }

	//conversion/overloading : converts (tokenizes) the string into Registers object
    Registers(const std::string& data) {        
        std::vector<std::string> vec;
        int i = 0, size = data.size();
        while (i < size) {
            std::string s;
            while (i< size && data[i] != ' ') {
                s.push_back(data[i]);
                ++i;
            }     
            
			vec.push_back(s);
            ++i;
        }
        
		registerName = vec[0];
		binary = vec[1];		
    }
};


//Symbol class
class Symbol {
private:
    std::string symbolName;
    int address;

public:
    Symbol() : symbolName("undefined"), address(-1) {};
    Symbol(std::string sym, int ads) : symbolName(sym), address(ads) {};

    std::string getSymbolName() {
        return symbolName;
    }

    int getAddress() {
        return address;
    }

	//overloads the << operator to write to the console.
    friend std::ostream& operator<<(std::ostream& outc, Symbol& symbol) {       
        outc << symbol.symbolName << ": " << symbol.address;
        return outc;
    }

	//overloads the == operator and compares two registerName(s).
    bool operator==(const std::string& key) {       
        return symbolName == key;
    }

	//overloads the << operator to write to a file
    std::ofstream& operator<<(std::ofstream& outf) {        
        outf << symbolName << " : " << address;
        return outf;
    }

	//conversion/overloading : converts (tokenizes) the string into symbol object
    Symbol(const std::string& data) {        
        std::vector<std::string> vec;
        int i = 0, size = data.size();
        while (i < size) {
            std::string s;
            while (i < size && data[i] != ' ') {
                s.push_back(data[i]);
                ++i;
            }
            vec.push_back(s);
            ++i;
        }

        symbolName = vec[0];        
        address = stringToInt(vec[1]);
    }
};

//class of type for the node of chaining in hashtable
template<typename V>
class Record {
public:
    /*const*/ V v;
    Record* next;

    Record(V& iv) : v(iv), next(nullptr) {}

	//conversion/overloading : converts a string into Record pointer
    Record(const std::string& data) : next(nullptr) {       
        v = data;       
    }
    
    ~Record(){
		delete next;
	}
};


//Hashtable class
template <typename K, typename V>
class HashTable {
private:
    int size = 10;	// default size of the hash table

    std::vector<Record<V>*> htable;	 // vector (hash table of type Record<V> pointer

public:
    HashTable() {
        htable.assign(size, nullptr);
    }

	//constructor : intializes the hash table with null pointer at every index.
    HashTable(int sizeOfTable) : size(sizeOfTable) {                      
        htable.assign(size, nullptr);
        
    }

	//conversion/overloading : convert a vector of strings into hashtable
    HashTable(const std::vector<std::string>&);                         
    
    //prints the hash table
    void printTable();   
    
    //computes the index for the key
    int computeHash(const std::string&);                                    
    
    // returns true if the key exist in the hash table
    bool hasKey(const std::string&);                           
    
    //adds a new record into the hash table                 
    void addRecord(const std::string&,  V&);                   
    
    // handles collision using chaining technique
    void handleCollision(const int, const std::string&,  V&);          
    
    //returns the record associated with the key, if exist
    V getRecord(const std::string&);                  
    
    //replaces the values of the key, if already exist, else simply add the key:values to the table                          
    void replaceRecord(const std::string&, V&);       
    
	//returns size of the hashtable
    int getSize() {        
        return size;
    }

	//returns the hash table
    std::vector<Record<V>*> getTable() const {      
        return htable;
    }
    
    //~HashTable(){
		//for (int i = 0;i < size;i++) {
        //if (htable[i] != nullptr) {
            //Record<V>* curr = htable[i];
            //while (curr != nullptr) {
				//Record<V>* node = curr->next;
				//delete curr;
				//curr = node;
            //}
        //}
        //else
            //delete htable[i];
		//}
    //}
			
};

template<typename K, typename V>
HashTable<K, V>::HashTable(const std::vector<std::string>& strings) {
    int numberOfStrings = strings.size();
    size = numberOfStrings;
    std::vector<Record<V>*> table;
    table.assign(size,nullptr);
    
 
    for (int k = 0;k < numberOfStrings;k++) {
        std::string str = strings[k];
        int i = 0, s = str.size();
        while (i < s && str[i] != '\n') {
            std::string value;
            while (i < s && str[i] != separator) {
                value.push_back(str[i]);
                ++i;
            }

            if (value.size() > 1) {		        
                if (table[k] == nullptr)
                    table[k] = new Record<V>(value);
                else {                    
                    auto curr = table[k];
                    while (curr->next != nullptr)
                        curr = curr->next;
                    curr->next = new Record<V>(value);
                }
                
            }
            ++i;
        }
    }
        
    
    htable = table;
}

template<typename K, typename V>
void HashTable<K, V>::printTable() {    
    for (int i = 0;i < size;i++) {
        if (htable[i] != nullptr) {
            Record<V>* curr = htable[i];
            while (curr != nullptr) {
                std::cout << "{" << curr->v << "}    ";
                curr = curr->next;
            }
            newline;
        }
        else
            std::cout << "null pointer\n";
    }
}

template<typename K, typename V>
int HashTable<K, V>::computeHash(const std::string& key) {
    int sum = 0;
    for (const char& c : key) {
        sum += c * PRIME;
    }
    return sum % size;
}
 
template<typename K, typename V>
bool HashTable<K, V>::hasKey(const std::string& key) {
    int index = computeHash(key);    
    if (htable[index] != nullptr)
        return true;
    else
        return false;
}

template<typename K, typename V>
void HashTable<K, V>::addRecord(const std::string& key, V& v) {
    int index = computeHash(key);

    if (htable[index] == nullptr) {
        htable[index] = new Record<V>(v);
        return;
    }

    handleCollision(index, key, v);
}

template<typename K, typename V>
void HashTable<K, V>::handleCollision(const int index, const std::string& key, V& v) {
    Record<V>* curr = htable[index];

    while (curr->next != nullptr) {
        if (curr->v == key) {
            std::cout << "Duplicate key entries not allowed.\n";
            return;
        }
        curr = curr->next;
    }

    if (curr->v == key) {
        std::cout << "Duplicate key entries not allowed.\n";
        return;
    }

    curr->next = new Record<V>(v);
}

template<typename K, typename V>
V HashTable<K, V>::getRecord(const std::string& key) {
    int index = computeHash(key);
    Record<V>* node = htable[index];

    while (node != nullptr) {
        if (node->v == key) {
            return node->v;
            break;
        }
        node = node->next;
    }

    V v;
    return v;
}

template<typename K, typename V>
void HashTable<K, V>::replaceRecord(const std::string& key,  V& v) {
    int index = computeHash(key);
    Record<V>* curr = htable[index];

    if (curr == nullptr) {
        std::cout << "Record with the corresponding key doesn't exist.\n";
        return;
    }

    Record<V>* prev = nullptr;
    while (curr != nullptr) {
        if (curr->v == key) {
            if (prev != nullptr){
                prev->next = new Record<V>(v);
                prev->next->next = curr->next;
			}
            else{
                htable[index] = new Record<V>(v);
                htable[index]->next = curr->next;
			}
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    
    

    std::cout << "Record with the corresponding key doesn't exist.\n";
}


// Serialization
template<typename hashTable>
class FTWrapper {
public:
	//it will serialize the data of the hash table passed as an arguement.
    void serialize(hashTable&, std::ofstream&);     
    
    //it will store the serialized data into a file for later use.
    void saveToFile(hashTable&, const std::string&);        
    
    /*it will deserialize/regenerate the hash table from the serialized data stored in the file passed as an arguement.*/
    void loadFromFile(hashTable&, const std::string&);      
                                                                                            
};

template<typename hashTable>
void FTWrapper<hashTable>::serialize(hashTable& hTable, std::ofstream& fout) {
    auto table = hTable.getTable();
    int size = table.size();

    for (int i = 0;i < size;i++) {
        auto curr = table[i];
        if (curr != nullptr) {
            while (curr != nullptr) {
                fout << curr->v << separator;
                curr = curr->next;
            }
            fout << '\n';
        }
        else
            fout << '\n';
    }
}

template<typename hashTable>
void FTWrapper<hashTable>::saveToFile(hashTable& hTable, const std::string& file) {
    std::ofstream fout;
    fout.open(file);
    serialize(hTable, fout);
    fout.close();
}

template<typename hashTable>
void FTWrapper<hashTable>::loadFromFile(hashTable& _hTable, const std::string& file) {
    std::ifstream fin;
    fin.open(file);   
	
    char ch = fin.get();
    std::vector<std::string> strings;
    while (!fin.eof()) {
        std::string str;        
        while (!fin.eof() && ch != '\n') {
            str.push_back(ch);
            ch = fin.get();
        }

        strings.push_back(str);
        ch = fin.get();
    }
    
    fin.close();        
    _hTable = strings;     
}




// assembler class
class Assembler {
    int codelines = 0, errorFlag = 0;
    char* sourceFile, *destinationFile;
    
    HashTable<std::string, Registers> registerTable;    
    HashTable<std::string, OpCode> opCodeTable;         
    HashTable<std::string, Symbol> symbolTable;    
    std::vector<std::pair<std::string, int>> SYMTAB;

public:
    Assembler(char* sfile, char* dfile) : sourceFile(sfile), destinationFile(dfile) {
        FTWrapper< HashTable<std::string, Registers>> regrapper;
        regrapper.loadFromFile(registerTable, "registers.txt");

        FTWrapper< HashTable<std::string, OpCode>> opcrapper;
        opcrapper.loadFromFile(opCodeTable, "opcodes.txt");
    }

	//it will return the array of character strings that each line of the file passed as an arguement.
    char** LoadFile();   
    
    //it will store the array of character strings in a new file passed as an arguement;
    int SaveFile(char**);    
    
    int validateCode(char**);
    void validateOperand(const std::string&);
    void firstpass(char** );
    void checkAndStoreSymbol(const int, const int, std::string&);
    void printSymbolTable(){
    	for(auto p: SYMTAB){
    		std::cout<<p.first<<" "<<p.second<<'\n';
    	}
    }
    
};

char** Assembler::LoadFile() {
    std::ifstream fin;
    fin.open(sourceFile);
    
    if (!fin) {
        std::cout<<"Error : Couldn't open the file.\n";
        return nullptr;
    }

    int lines = 0, mx = 0;    
    while (!fin.eof()) {
        std::string line;
        getline(fin, line);        
        mx = std::max(mx, (int)line.length());
        lines++;
    }           
    fin.seekg(0, std::ios::beg);

    if (lines == 0) {
        return nullptr;
    }

    char** sourceCodeStrings;    
    sourceCodeStrings = (char**) malloc(lines * sizeof(char*));
           
    int i = 0;
    char ch = fin.get();
    while (!fin.eof() && i<lines) {
        char* str;
        str = (char*) malloc((mx+2) * sizeof(char));
        int j = 0;
        while (!fin.eof() && ch != '\n' && j<mx) {
            str[j++] = ch;                                 
            ch = fin.get();
        }
        str[j] = '\0';
        sourceCodeStrings[i++] = str;
        ch = fin.get();                
    }
    

    codelines = lines;
    return sourceCodeStrings;
}

int Assembler::SaveFile(char** sourceCodeStrings) {
    if (sourceCodeStrings == nullptr)
        return 0;

    std::ofstream fout;
    fout.open(destinationFile);   

    if(!fout)
        return 0;
    
    for (int i = 0;i < codelines;i++) {
        int j = 0;
        while (sourceCodeStrings[i][j] != '\0') {
            fout << sourceCodeStrings[i][j++];
        }
        fout << '\n';
    }
    fout.close();   

    /*std::ifstream fin;
    fin.open(file); 
    char ch = fin.get();
    while (!fin.eof()) {
        std::cout << ch;
        ch = fin.get();
    }*/
        
    return 1;
}

void Assembler::checkAndStoreSymbol(const int locCtr, const int line, std::string& symbolName) {    
    if (!symbolTable.hasKey(symbolName)) {
        Symbol S(symbolName, locCtr);
        symbolTable.addRecord(symbolName, S);
        SYMTAB.push_back({ symbolName,locCtr });
    }
    else {
        Symbol s = symbolTable.getRecord(symbolName);
        if (s.getAddress() == -1) {
            Symbol s1(symbolName, locCtr);
            symbolTable.addRecord(symbolName, s1);
            SYMTAB.push_back({ symbolName,locCtr });
        }
        else {
            std::cout << "Line " << line+1 << ": Duplicate symbol declaration.\n";
        }
    }
}


void Assembler::validateOperand(const std::string& operand) {
    if (registerTable.hasKey(operand) || operand[0] == '#')
        return;          

    if (!symbolTable.hasKey(operand)) {
        Symbol s(operand, -1);
        symbolTable.addRecord(operand, s);
        SYMTAB.push_back({ operand,-1 });      
    }
        
}


int Assembler::validateCode(char** sourceCode) {    
    for (int line = 0;line < codelines;line++) {
        std::string err = "Line ";
        err.push_back(line + 1 + '0');       
        err.append(": ");

        std::string label, opcode;
        std::vector<std::string> operands;        
        split(label, opcode, operands, sourceCode[line]);        

        if (pseudoOps.find(opcode) != pseudoOps.end()) {
            continue;
        }                

		// if the instruction is label, add it to the symbol table.            
        if (!label.empty()) {                              
            if (symbolTable.hasKey(label)) {
                std::string error(err);
                error.append("Duplicate label declaration.");
                std::cout << error << '\n';
                errorFlag = 1;
            }
              else {
                  Symbol s(label, -1);
                  symbolTable.addRecord(label, s);
              }
                
            continue;
        }

		// checks if the opcode is known/unknown.
        if (!opCodeTable.hasKey(opcode)) {              
            std::string error(err);
            error.append("Unknown operation ");
            error.append(opcode + ".");
            std::cout << error << '\n';
            errorFlag = 1;
            continue;
        }

		// validates all the operands
        if (opCodeTable.hasKey(opcode)) {                              
            for (const auto& operand : operands) {
                if ((operand == "L1" && !symbolTable.hasKey(operand)) || (operand == "L2" && !symbolTable.hasKey(operand))) {                    
                    std::string error(err);
                    error.append(operand);
                    error.append(" was never declared in the program.");
                    std::cout << error << '\n';
                    errorFlag = 1;
                }               
            }
        }
    }    

    if (!errorFlag)
        return 1;

    std::cout << "ASSEMBLER: Found some errors in your program. Cannot proceed!\n";
    return 0;
}


void Assembler::firstpass(char** sourcefile) {        
    int locCtr = 0, line = 0, WORDSIZE = 3;

    while (sourcefile[line][0] == ';')
        ++line;
    
    std::string label, opcode;
    std::vector<std::string> operands;       
    split(label, opcode, operands, sourcefile[line++]);
        
    if (opcode == "START") {        
        locCtr += hexToDecimal(operands[0]);
        if (!label.empty()) {
            Symbol s(label, locCtr);
            symbolTable.addRecord(label, s);
            SYMTAB.push_back({ label, locCtr });
        }
    }
    
    //std::cout << "locCtr = " << locCtr << '\n';
    while (1) {
        if (sourcefile[line][0] == ';') {
            line++;
            continue;
        }

        //std::cout << "locCtr = " << locCtr << '\n';
        label = opcode = "";
        operands.clear();
        split(label, opcode, operands, sourcefile[line]);                

        if (opcode == "END")
            break;

        if (!label.empty()) {                        
            if (symbolTable.hasKey(label)) {
                ++line;
                std::cout << "Line " << line << ": Duplicate label declaration\n";                
                continue;
            }
            Symbol s(label, locCtr);
            symbolTable.addRecord(label, s);
            SYMTAB.push_back({ label,locCtr });            
        }

        
        if (opCodeTable.hasKey(opcode)) {            
            OpCode opc = opCodeTable.getRecord(opcode);
            locCtr += opc.getInstructionSize();
            for (const auto& operand : operands) {
                validateOperand(operand);
            }
        }
        else {
        	int operand1 = stringToInt(operands[0]);
        	if(opcode == "WORD")
        		locCtr += WORDSIZE;
        	else if(opcode == "RESW")
        		locCtr += WORDSIZE * operand1;
            else if (opcode == "RESB"){
                locCtr += operand1;                                
            }
            else if (opcode == "BYTE"){
            	locCtr += 1;   
            }
            else {
                ++line;
                std::cout << "Line " << line << ": Opcode not found, " << opcode << '\n';                
                continue;
            }
            checkAndStoreSymbol(locCtr, line, operands[0]);
        }

        ++line;
    }

    int errors = 0;
    for (const auto& p : SYMTAB) {
        std::cout << p.first<<" ";
        if (p.second == -1) {
            std::cout << "was never defined in the program.\n";
            errors++;
            continue;
        }

        std::cout << p.second << '\n';
    }

    if(errors > 0)
        std::cout << "ASSEMBLER: Found some errors in your program. Cannot proceed!\n";
    
}




int main(int argc, char* argv[]) {
    char* source_file = argv[1];   
    char destination_file[] = "code.asm";./
    //std::cout<<source_file<<" "<<destination_file<<'\n';

    Assembler assm(source_file,destination_file);    
    char** arrOfStrings = assm.LoadFile();      
    
    assm.SaveFile(arrOfStrings);
//    assm.validateCode(arrOfStrings);
    assm.firstpass(arrOfStrings);
    assm.printSymbolTable();

	
    return 0;
}


void split(std::string& label, std::string& opcode, std::vector<std::string>& operands, char* codeline) {
    int j = 0;
    std::string word;
    char ch = codeline[j++];
    while (ch != NC) {
        if (ch == ' ') {
            opcode = word;
            break;
        }
        else if (ch == ':') {
            label = word;
            ch = codeline[j++];
            word.clear();
        }

        if ((ch >= 65 && ch <= 90) || (ch >= 48 && ch <= 57))
            word.push_back(ch);
        ch = codeline[j++];
    }

    if (!word.empty())
        opcode = word;

    word.clear();
    while (ch != NC) {
        if ((ch >= 65 && ch <= 90) || (ch >= 48 && ch <= 57) || ch == '#') {
            word.push_back(ch);
        }
        else {
            if (!word.empty())
                operands.push_back(word);
            word.clear();
        }

        ch = codeline[j++];
    }

    if (!word.empty())
        operands.push_back(word);
}

int stringToInt(const std::string& strnum) {
    int num = 0, m = 10;
    for (const char& c : strnum) {
        num = num * m + (c - '0');
    }
    return num;
}


int hexToDecimal(const std::string& operand) {
    int num = stringToInt(operand);

    if (operand[1] == 'x') {
        int hexnum = 0, mul = 1;
        while (num) {
            hexnum = hexnum + (num % 10) * 1LL * mul;
            mul = mul * 1LL * 16;
            num /= 10;
        }
        num = hexnum;
    }

    return num;
}

