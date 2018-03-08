This is an implementation of a simple search engine for documents
# Compilation and run
A makefile is provided.  
Run the program with ./minisearch -i <docfile> [optional] -k <number>  
where docfile is the file with the documents and k is the number of results you wish to see (default is 10)
# Document file
The document file must be one document (string) per line with an id starting from 0 in front.  
Example:  
0 The quick brown fox leaped over the lazy lazy dog  
1 Quick brown foxes leaped over lazy dogs for fun
#Commands
The programm receives the following commands:
#
/df  
Computes the document frequency for every word in the documents.  
Example:  
/df  
Quick 1  
The 1  
brown 2  
dog 1  
dogs 1  
for 1  
fox 1  
foxes 1  
fun 1  
lazy 2  
leaped 2  
over 2  
quick 1  
the 1
#
/df <word>  
Computes the frequency for specified word.  
Example:  
/df lazy  
lazy 2
#
/tf <document_id> <word>  
Computes the frequency for specified word in specified document.  
Example:  
/tf 0 lazy  
0 lazy 2  
#
/search <q0> [optional] <q1> ... <q9>  
Searches for given queries in documents and print results according to relevance  
Example:  
/search lazy brown  
 1.(1)[-3.29825] Quick brown foxes leaped over lazy dogs for fun              
 2.(0)[-3.74858] The quick brown fox leaped over the lazy lazy dog  
