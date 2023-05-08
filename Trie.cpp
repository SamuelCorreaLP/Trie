/*
Trabalho 1 - AED2
CEFET-MG

Alunos:  Samuel C. L. Pereira

19/07/21

*/


#include <iostream>
#include <bitset>
#include <stdio.h>
#include <vector>
#include <fstream>

using namespace std;


class Word{

private:
    //Para nó externo
    string bin_word;       //Grava a plavra em binário
    vector<int> recurence; //Grava as posições de recorrencia da palavra
    vector<int> line;      //Localização no eixo horizontal
    vector<int> column;    //Localização vertical
    //Para os nós interno
    int key;               //Buasca a partir do índice 
    int id;                //Grava a posição do nó no vetor
    int left;              //Grava a posição do nó à esquerda
    int right;             //Grava a posição do nó à direita
    int above;             //Grava a posição do nó acima //Usado também para referenciar um nó externo
    int INTERNAL;          //Define se o nó é interno ou externo

public:
   
    void externalNode(string p_bin_word, int p_id, int p_key){
            bin_word = p_bin_word; //String binária
            id = p_id;             //Posição do nó no vetor de nós
            key = p_key;           //Posição no texto
            INTERNAL = 0;
    }

    void interalNode(int p_id,int p_key,int p_above){
        key = p_key;              //Indice de comparação entre as palavras
        INTERNAL = 1;         
        above = p_above;          //Nó acima
        left = -100;    
        right = -100;
        id = p_id;                //Posição do nó no vetor de nós
    }

    void setLeft(int p_left){
        left = p_left;
    }
    void setRight(int p_right){
        right = p_right;
    }
    void setAbove(int p_above){
        above = p_above;
    }

    void setRecurrence(int i_word){
        recurence.push_back(i_word);
    }

    vector<int> getRecurrence(){
        return recurence;
    }
    
    int getAbove(){
        return above;
    }

    int getLeft(){
        return left;
    }
    void setKey(int p_key){
        key = p_key;
    }

    string getBin_word(){
        return bin_word;
    }

    int getKey(){
        return key;
    }

    int getRight(){
        return right;
    }

    int isInternal(){
        return INTERNAL;
    }

    char getChar_word(int c_key){
        return bin_word[c_key];
    }
    int getId(){
        return id;
    }
    void setLocalization(int p_line, int p_column){
        line.push_back(p_line);
        column.push_back(p_column);
    }

    vector<int> getLine(){
        return line;
    }
    vector<int> getColumn(){
        return column;
    }





};


class Trie{//Estrutura base da árvore

    public:
        vector<Word> branch; //Vetor de nós
        int head = 0;        //Nó raiz 
};


void initTree(Trie & trie){//Inicializa a arvore

    Word newWord; 
    newWord.interalNode(0,0,0); //inicializa o nó head como interno
    trie.branch.push_back(newWord); //Insere no vetor de nós

}

int toInt(char character){ //Transforma char para int
    return (int)character;
}

void to16bitWord(string & str){ //Recebe uma string e ajusta seu tamanho para 16 caracteres

    while(str.length()<=16){
        str.push_back('#');
    }
}

string set16bitsring(string str){ //Recebe uma string e a transforma em uma string no padrão 8 bit

    to16bitWord(str);
    string str2;
    for(long unsigned int i = 0; i< str.length(); i++ ){
        str[i] = toupper(str[i]);//Converte todos os caracteres para o minúsculo 
        bitset<8> bin_x(toInt(str[i]));
        str2 += bin_x.to_string();
    }
    return str2;
}

int increaseExternal(Trie & trie, string bin_word, int p_text, int line, int column ){ //Cria um nó externo no vetor de nós

    Word newWord;
    newWord.externalNode(bin_word,trie.branch.size(), p_text);
    newWord.setLocalization(line,column);
    trie.branch.push_back(newWord);
    return trie.branch.size() - 1;

}

int increaseInternal(Trie & trie, int key, int above ){ // Cria um nó no vetor de nós
    Word newWord;
    newWord.interalNode(trie.branch.size(),key,above);
    trie.branch.push_back(newWord);
    return trie.branch.size() - 1;
}

void insertWord(Trie & trie, string bin_word, int id, int id_word, int line, int column){ //Insere um novo nó

    //cout << trie.branch[id].isExternal() << endl;
    if(trie.branch[id].isInternal()){
       
        
        if(bin_word[trie.branch[id].getKey()] == '1'){               //Se a bit da posição de comparação for 1, desvia para a direita
            
            if(trie.branch[id].getRight() == -100){                  //Caso a posição à direita esteja vazia, grava uma palavra na posição

                int pid = increaseExternal(trie,bin_word,id_word,line,column);   //Chama a função para alicar mais um nó
                trie.branch[pid].setAbove(id);                       //Associa o nó recém-criado com o nó acima
                trie.branch[pid].setRecurrence(id_word);             //Grava a posição da palavra no texto no vetor de recorrencia
                trie.branch[id].setRight(pid);                       //Associa o nó acima com o nó recém-criado

            }else{
                insertWord(trie,bin_word,trie.branch[id].getRight(),id_word, line, column);//Caso a posição já esteja ocupada, avança mais profundamente no nó à direita
                
            }
            
        }
        //Realiza o mesmo processo acima, porém à esquerda
       else if(bin_word[trie.branch[id].getKey()] == '0'){//Se a bit da posição de comparação for 1, desvia para a direita

            if(trie.branch[id].getLeft() == -100){                  
                
                int pid = increaseExternal(trie,bin_word, id_word,line,column); 
                trie.branch[pid].setAbove(id);
                trie.branch[pid].setRecurrence(id_word);
                trie.branch[id].setLeft(pid);
                

            }else{
                insertWord(trie,bin_word,trie.branch[id].getLeft(),id_word,line,column);
            }     
            
        }
        
    }else{//Caso não seja um nó interno

        if(bin_word == trie.branch[id].getBin_word()){  //Primeiramente, compara se as palavras são iguais
            trie.branch[id].setRecurrence(id_word);     //Em caso afirmantivo, adiciona-se a posição da palavra no texto no vetor de recorrencia da palavra
            trie.branch[id].setLocalization(line,column);
        }else{
            int key = trie.branch[trie.branch[id].getAbove()].getKey(); //Pega a agualha de comparação do nó acima
            //printf("key = %d\n", key);
            int position;
            int above = trie.branch[id].getAbove();

            while(bin_word[key] == trie.branch[id].getChar_word(key)){//Enquanto os bits forem iguais, novos nós internos são criados
                position = increaseInternal(trie,key,above);
                //cout << "Create new node internal" << endl;
                //printf("P = %d\n", position);
               
                if(bin_word[key] == '0'){//Ajusta a posição de inserção do nó deacordo com o valor do bit comparado
                    trie.branch[above].setLeft(position);
                    trie.branch[position].setAbove(above);
                    
                }else{
                    if(bin_word[key] == '1'){//Ajusta a posição de inserção do nó deacordo com o valor do bit comparado
                        trie.branch[above].setRight(position);
                        trie.branch[position].setAbove(above);
                    }
                }
                above = position;
                key++;
                trie.branch[position].setKey(key);
            }

            if(bin_word[key] == '1'){//Ao final do processo, ele cria o nó para a palavra inserida, e ajusta a posição
                 int pid = increaseExternal(trie,bin_word, id_word,line,column);// dos dois nós no último nó criado pelo laço
                 trie.branch[position].setRight(pid);
                 trie.branch[pid].setAbove(position);
                 trie.branch[pid].setRecurrence(id_word);
                 trie.branch[position].setLeft(id);
                 trie.branch[id].setAbove(position);
            }else{
                 int pid = increaseExternal(trie,bin_word, id_word,line,column);
                 trie.branch[position].setLeft(pid);
                 trie.branch[pid].setAbove(position);
                 trie.branch[pid].setRecurrence(id_word);
                 trie.branch[position].setRight(id);
                 trie.branch[id].setAbove(position);
            }
        }
    }
}



int aux_findWord(Trie & trie, int id, string bin_word){

    if(trie.branch[id].isInternal()){
       
        
        if(bin_word[trie.branch[id].getKey()] == '1'){               //Se a bit da posição de comparação for 1, desvia para a direita
            
            if(trie.branch[id].getRight() == -100){                  //Caso a posição à direita esteja vazia, grava uma palavra na posição
                
                //cout << "Word not found!" << endl;
                return 0;

            }else{
                return aux_findWord(trie,trie.branch[id].getRight(), bin_word);//Caso a posição já esteja ocupada, avança mais profundamente no nó à direita
                
            }
            
        }
        //Realiza o mesmo processo acima, porém à esquerda
       else if(bin_word[trie.branch[id].getKey()] == '0'){//Se a bit da posição de comparação for 1, desvia para a direita

            if(trie.branch[id].getLeft() == -100){  
                
                //cout << "Word not found!" << endl;
                return 0;
                
            }else{
                return aux_findWord(trie,trie.branch[id].getLeft(), bin_word);
            }     
            
        }
        
    }else{//Caso não seja um nó interno
       
        if(bin_word == trie.branch[id].getBin_word()){  //Compara se as palavras são iguais
            //printf("This word exist!\n");     //Em caso afirmantivo, apresenta a palavra e as posiçãoes no texto.
            return id;
        }else{
            //cout << "Word not found!" << endl;
            return 0;
        }
            
    }

    return 0;

}

int findWord(Trie & trie, int id, string word){//Procura uma palavra na árvore
   string bin_word = set16bitsring(word);      //Trasforma a palavra recebida no padrão de 128 bits
   int pid = aux_findWord(trie,id,bin_word);   //Função de busca
   return pid;                                 //Retorna a posição do nó
}

int main(int argc, char* argv[]){

    Trie trie;
    Word newWord;
    initTree(trie);

    if(argc !=3){
        printf("Use: ./a.out input.txt find.txt\n");
        exit(0);
    }

    fstream input;
    input.open(argv[1], std::ios::in); //abre arquivo para inserir na arvore

    if (!input) {
        cerr << "("<< argv[1] << ")  - File not found!" << endl;
        exit(1);
    }


    char ch;
    string str;
    string bin_word;
    int wordCounter = 0;
    int enable = 0;
    int line = 0, column = 0;
    //LÊ O ARQUIVO COM O TEXTO DE ENTRADA
    while (input.get(ch)) {

        if(ch == '\n' ){
            line++; column = 0;
        }

        while(!isspace(ch) && (ch != '(') && (ch != ')') && (ch != ',') && //vai ficar nesse while ate terminar de ler a palavra
              (ch != '.') && (ch != '?') && (ch != '*') && (ch != '\n') && //essa logica tambem garante q n vai ser contado nenhum desses caracteres no final 
              (ch != EOF) && (ch != ':') && (str.length() <= 16)) {  //tambem garante q str vai ter no maximo tamanho 16
            //std::cout << ch << std::endl;
            str.push_back(ch);
            input.get(ch);
            
            enable = 1;
        } //quando sair do while mais interno str vai ter gravado uma palavra

        if(enable){
            //cout << str << endl;
            bin_word = set16bitsring(str); //insere a palavra na arvore
            insertWord(trie,bin_word,trie.head,wordCounter,line,column);
            wordCounter++;  column ++;
        }
        enable = 0;
        //cout <<  bin_word << endl;
        str.clear();
    }
    input.close();

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    input.open(argv[2], std::ios::in); //abre arquivo para inserir na arvore

    if (!input) {
        cerr << "("<< argv[2] << ")  - File not found!" << endl;
        exit(1);
    }

    enable = 0;
    while (input.get(ch)) {

        while(!isspace(ch) && (ch != '(') && (ch != ')') && (ch != ',') && //vai ficar nesse while ate terminar de ler a palavra
              (ch != '.') && (ch != '?') && (ch != '*') && (ch != '\n') && //essa logica tambem garante q n vai ser contado nenhum desses caracteres no final 
              (ch != EOF) && (ch != ':') && (str.length() <= 16)) {  //tambem garante q str vai ter no maximo tamanho 16
            //std::cout << ch << std::endl;
            str.push_back(ch);
            input.get(ch);
            enable = 1;
        } //quando sair do while mais interno str vai ter gravado uma palavra

        if(enable){
        
            int pid = findWord(trie,trie.head,str);
            if(pid !=0){
                printf("\n------------------------------------------------------\n");
                cout << "Word: "<< str << endl;
                vector<int> t =  trie.branch[pid].getRecurrence();
                vector<int> l =  trie.branch[pid].getLine();
                vector<int> c =  trie.branch[pid].getColumn();
                printf("Recurrence %ld time(s)\nPositions = ", t.size());
                for(int j = 0; j < t.size(); j++){
                    printf("%d(%d,%d) ",t[j],l[j],c[j]);
                }
                printf("\n");
            }
            else{
                printf("\n------------------------------------------------------\n");
                cout << "Word: "<< str << "\nWord not found" << endl;

            }
        }
        enable = 0;
        //cout <<  bin_word << endl;
        str.clear();
    }
    input.close();
}
