//  main.cpp
//  HW5_p1_noclass
//
//  Created by Adam Belmonte on 11/12/17.
//  Copyright © 2017 Adam Belmonte. All rights reserved.
//
//
//  main.cpp
//  HW5_p1
//
//  Created by Adam Belmonte on 11/8/17.
//  Copyright © 2017 Adam Belmonte. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <string>
#include <fstream>

using namespace std;
#define ALPHA_SIZE 4


struct pt_node{
    struct pt_node *possible_child[ALPHA_SIZE];
    bool is_leaf = false;
    vector<int> occurrences;
};


class pt_trie
{
public:
    pt_node *root = new pt_node();
    void insert(string key, struct pt_node *root);
    bool search(string key, struct pt_node *root);
    void lexicographPrint(pt_node *TRIE, vector<char> printUtilVector);

};


struct pt_node *make_new_node(){
    
    pt_node *node = new pt_node;
    node->is_leaf = false;
    
    for (int i = 0; i < ALPHA_SIZE; i++)
        {
            node->possible_child[i] = NULL;
        }
    return node;
}

//insert function
void pt_trie::insert(string key, struct pt_node *root)
{
    //make a node that will 'crawl' through the trie, starting from the ROOT node
    struct pt_node *pt_crawler = root;
    //scan the trie starting at the top and compare the first letter of the read/key
    for (int i = 0; i < key.length(); i++)
    {
        int index = (int)key[i] - (int)'A';
        cout<<"index is  "<<index<<"\n";
        
        if (!pt_crawler-> possible_child[index]){
            pt_crawler-> possible_child[index] = make_new_node();
            cout<<pt_crawler<<"\n";
        }
        pt_crawler->is_leaf = true;
    }
    cout<<"mer added to prefix trie"<<"\n";
}

//search function; scans the trie for a specific key and returns when key is found or when end of trie is reached.
bool pt_trie::search(string key, struct pt_node *root)
{
    struct pt_node *pt_crawler = root;
    
    for (int i = 0; i < key.length(); i++)
    {
        int index = (int)key[i] - (int)'A';

        if (!pt_crawler-> possible_child[index])
            return false;
        pt_crawler = pt_crawler-> possible_child[index];
    }
    return (pt_crawler != NULL && pt_crawler->is_leaf);
}

//print function from --> http://theoryofprogramming.com/trie-tree-using-cpp-class/

void pt_trie::lexicographPrint(pt_node *TRIE, vector<char> printUtilVector)
{
    bool is_leaf = true;
    //cout<<"i am here"<<"\n";
    
    vector<int>::iterator itr = TRIE->occurrences.begin();
    
    for (int i = 0; i < ALPHA_SIZE; ++i) {
        //cout<<"am i here?"<<"\n";
        if (TRIE->possible_child[i] == NULL) {
            continue;
        } else {
            is_leaf = false;
             printUtilVector.push_back('a' + i);
             // Select a child and explore everything associated with the cild
            lexicographPrint(TRIE->possible_child[i], printUtilVector);
            printUtilVector.pop_back();
        }
    }
    
    if (TRIE->occurrences.size() != 0) {
        // Condition TRIE->occurrences.size() != 0, is a neccessary and sufficient condition to tell if a node is associated with a word or not
        vector<char>::iterator itr = printUtilVector.begin();
        
        while (itr != printUtilVector.end()) {
            printf("%c", *itr);
            ++itr;
        }
        printf(" -> @ index -> ");
        
        vector<int>::iterator counter = TRIE->occurrences.begin();
        
        // This is to print the occurences of the word
        while (counter != TRIE->occurrences.end()) {
            printf("%d, ", *counter);
            ++counter;
        }
        printf("\n");
    }
    printUtilVector.pop_back();
}


double randNum(){
    random_device rd{};
    mt19937 engine{rd()};
    uniform_int_distribution<int> dist{0, 10687};
    int numb = dist(engine);
    return numb;
}

char *makeDengue(){
    string line;
    ifstream file;
    
    //DONT FORGET TO CHANGE THIS FILE PATH
    file.open("/Users/adambelmonte/Desktop/INF503/HW3_p3/sequence.fasta");
    char *dengue = new char[10723];
    while (!file.eof()){
        while(getline(file,line)){
            //cout<< line << "\n";
            if ( line[0] != '>' ){
                strcat(dengue, line.c_str());
            }
        }
    }
    file.close();
    return dengue;
}

int main() {
    
    //make random 36-long mers
    int mer_length = 3;
    int mer_number = 5;
    
    //creates a giant string of the DENV2 genome
    string dengue = makeDengue();
    
    //creates and initializes the array that will contain the random mers from DENV2
    char **random_mers;
    random_mers = new char *[mer_number];
    
    for (int j = 0; j < mer_number; j++){
        random_mers[j] = new char[mer_length];
    }
    
    //populates the random mers array with 36-char long mers randomly selected from DENV2
    for(int i = 0; i < mer_number; i++){
        int numb = randNum();
        string key = dengue.substr(numb, mer_length);
        strcpy(random_mers[i], key.c_str());
        //cout<<random_mers[i]<<"\n";
    }
    
    //////////////////////////////
    ////// CREATE THE TRIE ///////
    //////////////////////////////
    
    pt_trie TRIE;
    
    // Construct trie by looping through the number of nodes to be considered and then call insert function on each read
    
    struct pt_node *root = make_new_node();
    
    for (int i = 0; i < mer_number; i++)
    {
        //TRIE.insert(,root);
        TRIE.insert(random_mers[i], root);
    }

    vector<char> util;
    printf("\n");
    TRIE.lexicographPrint(TRIE.root, util);
    
    //TRIE.search("ATTAGAGCTCTCTCATCAT", root)? cout << "Yes\n" : cout << "No\n";
    //TRIE.search("TGGGAAAA", root)? cout << "Yes\n" : cout << "No\n";

    return 0;
}
