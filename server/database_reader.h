#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <cstdlib>
#include <iostream>
#include <cstdio>

using namespace std;

string get_random_word(){
   ifstream dataBase;
   vector <string> words;
   srand((unsigned )time(NULL));
   string word;
   dataBase.open("words_database.txt"); //open a file to perform read operation using file object
    if (!dataBase.is_open()){
        perror("Blad");
    }
   while (dataBase >> word){
       words.push_back(word);
   }

   int random_index = rand() % words.size();
   return words[random_index];
}