Code Filter - using LCP | Suffix Arrays

Compiling :
    g++ codefilter.cpp -o codefilter

Running :
    ./codefilter < input_file.txt
    ./codefilter

Input Format :
    1 - No of programs to compare.
    2 - Length of each program seperated by space.
    3 - Programs, each seperated by return statement.
    4 - Matching length chosen.

Output :
    1 - Lines carrying the no of elements with certain beginning pattern followed by their left 
        and right indices in the LCP array.
    2 - Max value of all such groups.

For more information on input format view codefilter.txt
