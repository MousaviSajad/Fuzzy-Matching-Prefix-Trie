Fuzzy-Matching Prefix Trie

Project Goal: Implement an exhaustive fuzzy alignment of genetic sequence data using a prefix trie (PT).

Being given a reference genome (roughly 20 million base-pairs long) as well as multiple sets of genetic sequence 'reads' (ranging from 1 to 100 million 50 character long fragments) our task is to attempt to align each read with the reference genome and record the metrics associated with any successful alignment. As indicated by the project title and goal, the alignment of character/string sequences is performed in a 'fuzzy' manner, meaning that there is a predetermined amount of individual character-to-character mismatches allowed for each individual alignment. For example, an attempt to align the sequences [A C T G C T A] and [A C C T C T C] would be successful if a mismatch threshold of 3 was used, however a 2 character mismatch threshold would not yield a successful alignment.

This project presents unique considerations regarding dataset size, processing speed, and desired output. What follows is a general workflow for structuring implementation as well as a list of required inputs, functions, and eventual outputs.

Construct a PT from a set of reads --> scan genome and subset (via hashtable or else on the fly?) a 50 character sequence --> use search function to compare sequence to read PT --> when mismatch occurs create a stack and push a pointer to the node that triggered the mismatch on the stack (include location in PT as well as # of mismatches until that point) --> proceed with search algorithm until mismatch threshold is reached or else a full alignment is found --> if full alignment is found record location in the reference genome and the total nuber of mismatches --> if alignment is not successful then refer to the stack and pop off the top node and proceed down next branch.

INPUTS:
-high throughput sequencing read sets, each read being a 50 character-long string of letters A,C,T, or G.
-a reference genome consisting of 20 million base-pairs of letters A, C, T, or G.
-a mismatch tolerance/threshold

FUNCTIONS:
-insert function; used to insert each 50 character-long read into the PT
-search function; used to scan through PT and located matching characters between the read and sequence from the reference genome 

OUTPUTS:
-linked list of locations in reference genome where successful alignments occurred as well as the number of mismatches associated with each individual alignment
