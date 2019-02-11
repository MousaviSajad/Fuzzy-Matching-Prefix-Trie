#include <vector>
#include <stack>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <iomanip>

#define ALPHA 4
#define MAX_NUMBER_ENTRIES 101

using namespace std;

class PTrieNode
{
public:
    PTrieNode *parent;
    PTrieNode *child[ALPHA];
    vector<int> count;
    bool leaf_check = false;
};

struct stack_branch{
    int mismatch;
    int index;
    vector<char> read_remain;
    PTrieNode *branch_mer;
};

class PTrie
{
public:
    PTrieNode *root;

    PTrie() {
        root = (PTrieNode *) calloc(1, sizeof(PTrieNode));
    }

//This function (int_return) handles the instances when the values of the characters need to be changed to properly index into the vector slots locations (e.g. vector slots [_][_][_][_] -> locations 0,1,2,3 ). It takes in the numerical location in the alphabet of the corresponding letter.
    int int_return(int a){
        //this accounts for if a character is A
        if(a == 0)
            return 0;
        //this accounts for if a character is C
        if(a == 2)
            return 1;
        //this accounts for if a character is G
        if(a == 6)
            return 2;
        //this accounts for if a character is T
        if(a == 19)
            return 3;
        return -1;
    }

//This function (print_return) handles the instances, as in the int_return function, where the indexed locations in the vector/node need to be reassigned letter valuations.
    int print_return(int a){
        //this accounts for if a character is A
        if(a == 0)
            return 0;
        //this accounts for if a character is C
        if(a == 1)
            return 2;
        //this accounts for if a character is G
        if(a == 2)
            return 6;
        //this accounts for if a character is T
        if(a == 3)
            return 19;
        return -1;
    }

//This function (insert) takes in the read/text, assigns it a location, and inserts it into the PT accordingly
    void insert(char text[], int index)
    {
        //Here is where we chose to use the vector template class to store our PT’s data, a vector essentially acts as a node with 4 ‘slots’ for storing the genetic reads.
        vector<char> read(text, text + strlen(text));
        //cout<< read.capacity()<<"\n";

        PTrieNode * temp = root;

        int i = 0;

        while (i < read.size()) {
            //This is where the node creation/read insertion begins by checking if vector locations are occupied
            if (temp->child[int_return(read[i] - 'A')] == NULL)
            {
                //Memory is allocated here, before the ‘parent’ is instantiated
                temp->child[int_return(read[i] - 'A')] = (PTrieNode *) calloc(1, sizeof(PTrieNode));
                //This is where a ‘parent’ node is created after it is determined that there is not a possible child associated with the particular node
                temp->child[int_return(read[i] - 'A')]->parent = temp;
            }
            //The temp/progressing node pointer is reassigned and the process moves on to the next iteration
            temp = temp->child[int_return(read[i] - 'A')];
            ++i;
        }
        //This pushes the occurrence of the recently created node’s location into the count
        temp->count.push_back(index);
    }


    void print(PTrieNode *trie, vector<char> vector_to_print)
    {
        int i;
        bool has_no_child = true;

        vector<int>::iterator itr = trie->count.begin();

        for (i = 0; i < ALPHA; ++i) {
            if (trie->child[i] == NULL) {
                continue;
            } else {
                has_no_child = false;
                vector_to_print.push_back('A' + print_return(i));
                //recursive call to print function, allowing for a full run through related nodes
                print(trie->child[i], vector_to_print);
                //This removes the letter as it is passed over and processed
                vector_to_print.pop_back();

            }
        }
        //Tests whether a child has any further associations, i.e. if its size is >0
        if (trie->count.size() != 0)
        {
            vector<char>::iterator itr = vector_to_print.begin();

            while (itr != vector_to_print.end())
            {
                printf("%c", *itr);
                ++itr;
            }
            printf(" -> node is located at the index number -> ");

            vector<int>::iterator ctr = trie->count.begin();

            while (ctr != trie->count.end())
            {
                printf("%d, ", *ctr);
                ++ctr;
            }

            printf("\n");
        }
        //Again, this deals with eliminating the recently processed/printed letter
        vector_to_print.pop_back();
    }

//This is the essence of our Fuzzy Search alignment mechanism. It takes in what becomes a root node, pointer to the read, and a number corresponding to the maximum allowable mismatches
    vector<vector<char>> fuzzy_search(PTrieNode *trie, char *text, int mismatch)
    {
        //Create a character vector to hold the read, based on its length
        vector<char> read(text, text + strlen(text));
        //Create the temp (‘crawler’) node pointer
        PTrieNode *temp = trie;
        //Instantiates the stack vector structure that is used to house the temporary information and location associated with a mismatch induced branch exploration. More specifically, this stack object contains the ‘crawler’ node pointer, an index variable, a mismatch counter, and the remaining characters in the sequence being compared for alignment.
        stack<stack_branch> stack1;
        stack_branch start;
        start.branch_mer = temp;
        start.index = 0;
        start.mismatch = mismatch;
        start.read_remain = read;
        //Here is where all the above described information is pushed on to the stack.
        stack1.push(start);
        //Here is where a new vector of character vectors is instantiated, this vector will account for and eventually display the resulting alignment. It is a nested set of vectors due to the nature of having to investigate and record each sub-structure.
        vector<char> vector_to_print;
        vector<vector<char>> search_results;
        PTrieNode *parent;
        stack<vector<char>> stack_print;
        //This begins the process of scanning through the initial ‘temporary’ stack until all options are extinguished.
        while (stack1.size() != 0)
        {
            stack_branch sb = stack1.top();
            stack1.pop();
            int index = sb.index;
            int m = sb.mismatch;
            vector<char> read = sb.read_remain;
            temp = sb.branch_mer;
            if(parent == temp)
                vector_to_print.pop_back();

            int i = index;

            for (i; i < ALPHA; ++i)
            {
                if (temp->child[i] == NULL)
                {
                    continue;
                } else {
                    if (i == int_return(read[0] - 'A'))
                    {
                        if (i < ALPHA)
                        {
                            stack_branch sb_temp;
                            sb_temp.branch_mer = temp;
                            sb_temp.index = i + 1;
                            sb_temp.mismatch = m;
                            sb_temp.read_remain = read;
                            stack1.push(sb_temp);
                        }
                        vector_to_print.push_back('A' + print_return(i));
                        temp = temp->child[i];
                        read.erase(read.begin());
                        stack_branch sb_temp;
                        sb_temp.branch_mer = temp;
                        sb_temp.index = 0;
                        sb_temp.mismatch = m;
                        sb_temp.read_remain = read;
                        stack1.push(sb_temp);

                    } else {
                        m--;
                        if (m < 0 )
                        {
                            parent  = temp->parent;
                            continue;
                        } else {
                            vector_to_print.push_back('_');
                            if (i < ALPHA)
                            {
                                stack_branch sb_temp;
                                sb_temp.branch_mer = temp;
                                sb_temp.index = i + 1;
                                sb_temp.mismatch = m+1;
                                sb_temp.read_remain = read;
                                stack1.push(sb_temp);
                            }

                            temp = temp->child[i];
                            read.erase(read.begin());
                            stack_branch sb_temp;
                            sb_temp.branch_mer = temp;
                            sb_temp.index = 0;
                            sb_temp.mismatch = m;
                            sb_temp.read_remain = read;
                            stack1.push(sb_temp);
                        }
                    }
                }
                break;
            }

            parent = temp->parent;

            if (temp->count.size() != 0 && !temp->leaf_check)
            {
                temp->leaf_check = true;
                vector<char>::iterator itr = vector_to_print.begin();
                // uncomment to print out
                while (itr != vector_to_print.end())
                {
                 //   printf("%c", *itr);
                    ++itr;
                }
               // printf(" -> node is located at the index number -> ");

                vector<int>::iterator counter = temp->count.begin();

                while (counter != temp->count.end())
                {
                  //  printf("%d, ", *counter);
                    ++counter;
                }
              //  printf("\n");
                search_results.push_back(vector_to_print);
            }
        }
        return  search_results;
    }


//This is the search function, which is takes in the current PT and a character pointer and is able to search the PT for the text provided by the character pointer.
    PTrieNode * PT_search(PTrieNode *trie, char *text)
    {
        vector<char> read(text, text + strlen(text));
        PTrieNode * temp = trie;

        while (read.size() != 0)
        {
            if (temp->child[int_return(read[0] - 'A')] != NULL)
            {
                temp = temp->child[int_return(read[0] - 'A')];
                read.erase(read.begin());
            } else {
                break;
            }
        }

        if (read.size() == 0 && temp->count.size() != 0)
        {
            return temp;
        } else {
            return NULL;
        }
    }

//DO WE NEED TO STILL INCLUDE THIS FUNCTION, I DONT THINK WE USE IT ANYWHERE...???
    void removeread(PTrieNode * trie, char * read)
    {
        PTrieNode * temp = PT_search(trie, read);

        if (temp == NULL) {
            return;
        }

        temp->count.pop_back();    // Deleting the occurence

        bool has_no_child = true;

        int childCount = 0;
        int i;

        for (i = 0; i < ALPHA; ++i)
        {
            if (temp->child[i] != NULL)
            {
                has_no_child = false;
                ++childCount;
            }
        }

        if (!has_no_child)
        {
            return;
        }

        PTrieNode * traverse;     // variable to assist in traversal

        while (temp->count.size() == 0 && temp->parent != NULL && childCount < 2) {
            // temp->count.size() -> tells if the node is associated with another
            // read
            //
            // temp->parent != NULL -> is the base case sort-of condition, we simply ran
            // out of nodes to be deleted, as we reached the root
            //
            // childCount -> does the same thing as explained in the beginning, to every
            // node we reach

            traverse = temp->parent;

            for (i = 0; i < ALPHA; ++i) {
                if (temp == traverse->child[i]) {
                    traverse->child[i] = NULL;
                    break;
                }
            }

            free(temp);
            temp = traverse;

            for (i = 0; i < ALPHA; ++i) {
                if (temp->child[i] != NULL) {
                    ++childCount;
                }
            }
        }
    }
};

char *makeGenome(char *genome, string filename) {
    string line;
    ifstream file;
    file.open(filename);
    if (!file) {
        cerr << "Unable to open file " << filename  << endl;
        exit(1);
    }
    while (!file.eof()){
        while (getline(file, line)){
            if (line[0] != '>') {
                strcat(genome, line.c_str());
            }
        }
    }
    file.close();
    return genome;
}
int length_file(string filename){
   ifstream file;
    file.open (filename.c_str());
    if (!file) {
        cerr << "Unable to open file " << filename << endl;
        exit(1);
    }
    file.seekg (0, ios::end);
    int  length = file.tellg();
    file.close();
    return  length;
}
 int numOfReads(string filereads){

string line;
ifstream readFile;
readFile.open(filereads);
     if (!readFile) {
         cerr << "Unable to open file " << filereads;
         exit(1);
     }
int num_reads = 0;
int counter = 0;

while (!readFile.eof())
{
while (getline(readFile, line))
{
if (line[counter] == '>')
{
num_reads++;
}
}
counter++;
}
     return num_reads;
}
char** create_RefGenFrags(string genome, int num_frags, int mers){
    char **seq = new char*[num_frags];
    int len = genome.length();
    for(int i = 0; i < num_frags; i++) {
        seq[i] = new char[mers];
        string str = genome.substr(i, mers);
        strcpy(seq[i], str.c_str());
    }
    return seq;
}

void destroy(PTrieNode *trie)
{
    if (trie != NULL)	{
        destroy(trie->child[0]);
        trie->child[0] = NULL;
        destroy(trie->child[1]);
        trie->child[0] = NULL;
        destroy(trie->child[2]);
        trie->child[0] = NULL;
        destroy(trie->child[3]);
        trie->child[0] = NULL;
        delete trie;
    }
}
char retrunALPHA(int index){
    switch(index) {
        case 0 :
            return 'A';
        case 1 :
            return 'C';
        case 2 :
            return 'G';
        case 3 :
            return 'T';
        default:
            return 'N';
    }
}
int main()
{
    //Create the PT here; first by instantiating the PT object and then by populating with reads via the insert function.
    //////HANDLE GENOME DATA////////
    ////////////////////////////////
    //Scan through file and get the genome_length of the genome
  // string genomeAddress = "C:\\hpc files\\Projects\\INF503FinalProject\\ref_genome.txt";
  string genomeAddress = "//common//contrib//classroom//inf503//project_2//test_genome.fasta";
   string filereads = "//common//contrib//classroom//inf503//project_2//test_reads.fasta";
  //  string filereads = "C:\\hpc files\\Projects\\INF503FinalProject\\reads100.txt";

    int genome_length = length_file(genomeAddress);
    cout << "genome_length: " << genome_length <<"\n";



    //create a large string from the reference genome file
    char genome_array[genome_length];
    makeGenome(genome_array,genomeAddress);
    string genome(genome_array, genome_length);
//    genome = genome.substr(1,genome.length());
//    cout << genome.length() << endl;
    //create individual arrays of sequence fragments 50 mers long
    int fragSize = 100;
    int num_frags = genome_length - fragSize +1;
    char **refGenFrags = create_RefGenFrags(genome, num_frags,fragSize);

    ////////////////////////////////////////////////////////////////////
    //////DERIVE THE READS --> read from file then insert into PT///////
    ////////////////////////////////////////////////////////////////////

    string line;
//    int num_reads = numOfReads(filereads);
//    cout<<"num of reads: " <<num_reads<<"\n";

    ifstream readFile2(filereads);
    if (!readFile2) {
        cerr << "Unable to open file " << filereads;
        exit(1);
    }
    int index = 0;
    int num_reads[] = {100, 200, 500, 1000, 2000, 5000, 10000, 15000, 50000, 100000, 150000,200000, 500000,1000000,1500000,2000000};

    char read[MAX_NUMBER_ENTRIES];

    for (int ij = 0; ij < 16;ij++)
    {
    PTrie trie;
    clock_t begin_time = clock();
    if (!readFile2.eof()){
        while (getline(readFile2, line) and index < num_reads[ij]){
            if (line[0] != '>'){
                strcpy(read, line.c_str());
                trie.insert(read, index + 1);
                index++;
            }
        }
    }
       cout << "\nElapsed time to build the trie with " << num_reads[ij] << " reads is " << float(clock() - begin_time) / CLOCKS_PER_SEC << "s";
       // destroy the trie
       //cout << "destroy the trie";
       destroy(trie.root);
       // go to the beginning of the file
        readFile2.clear();
        readFile2.seekg (0, ios::beg);
        index = 0;
    }
    readFile2.close();

    printf("\n");

    //incorporate refGenFrags here somehow...
    //char text[101] = "AGGGTTCAGGAAAGAGTTTGGAAGGATGCTGAACATCTTGAATAGGAGACAGGGTTCAGGAAAGAGTTTGGAAGGATGCTGAACATCTTGAATAGGAGAC";
    ///FIX TO WORK ON ENTIRE GENOME OF FRAGMENTS


    //cout << "Looking for \"" << text << "\"" <<endl;

    // build tire from trie
    PTrie trie;
    clock_t begin_time = clock();
    for (int i = 0; i < num_frags; i++){
        trie.insert(refGenFrags[i], i + 1);
    }

    cout << "\nElapsed time to build the trie with " << num_frags << " reads from the reference genome is " << float(clock() - begin_time) / CLOCKS_PER_SEC << "s" << endl;


    // do some changes to the reference genome and do fuzzy search
    int count = 0;
    int mismatch = 1;

    for (int i = 0; i < num_frags; i++) {
        int m = mismatch;
        while (m > 0) {
            int randNum = rand() % fragSize;
            int alpha_index = rand() % ALPHA;
            refGenFrags[i][randNum] = retrunALPHA(alpha_index);
            m--;
        }
        vector<vector<char>> search_results = trie.fuzzy_search(trie.root, refGenFrags[i], mismatch);
        if (search_results.size()!=0)
           count++;
    }

    cout << "Sensitivity is: " << (count/double(num_frags))* 100 <<"%";

//    PTrieNode *result = trie.PT_search(trie.root, text);


    return 0;
}

//
// Created by sm3276 on 12/6/2017.
//

