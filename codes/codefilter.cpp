// https://www.geeksforgeeks.org/suffix-array-set-2-a-nlognlogn-algorithm/

#include <iostream>
#include <math.h>
#include <string.h>
#include <vector>

using namespace std;

const int MAXN = 10000;

int SA[MAXN][3] = {0};
int SA2[MAXN][3] = {0};
int Pi[MAXN] = {0};
int PLCP[MAXN] = {0};
int LCP[MAXN] = {0};

int cut[MAXN] = {0};

int hash_owner[MAXN];
int group[MAXN];
int group_members[MAXN];

int M,L;
int N[MAXN];
char input[MAXN];

// merge sort
void merge(int arr[][3], int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 


    int L[n1][3], R[n2][3]; 

    for (i = 0; i < n1; i++) {
        L[i][0] = arr[l + i][0]; 
        L[i][1] = arr[l + i][1];
        L[i][2] = arr[l + i][2];
    }

    for (j = 0; j < n2; j++) {
        R[j][0] = arr[m + 1+ j][0]; 
        R[j][1] = arr[m + 1+ j][1]; 
        R[j][2] = arr[m + 1+ j][2];
    }

    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 

    while (i < n1 && j < n2) 
    {   // ascending order
        if (L[i][0] < R[j][0]) {   
            arr[k][0] = L[i][0]; 
            arr[k][1] = L[i][1];
            arr[k][2] = L[i][2];
            i++;
            k++; 
        } 
        else if (L[i][0] == R[j][0] && L[i][1] < R[j][1]) {
            arr[k][0] = L[i][0]; 
            arr[k][1] = L[i][1];
            arr[k][2] = L[i][2];
            i++;
            k++;
        }
        else {   
            arr[k][0] = R[j][0]; 
            arr[k][1] = R[j][1]; 
            arr[k][2] = R[j][2]; 
            j++; 
            k++; 
        }
    } 

    while (i < n1) 
    { 
        arr[k][0] = L[i][0]; 
        arr[k][1] = L[i][1];
        arr[k][2] = L[i][2];  
        i++; 
        k++; 
    } 

    while (j < n2) 
    {   
        arr[k][0] = R[j][0]; 
        arr[k][1] = R[j][1];
        arr[k][2] = R[j][2];  
        j++; 
        k++; 
    } 
} 

void mergeSort(int arr[][3], int l, int r) 
{ 
    if (l < r) 
    { 
        int m = l+(r-l)/2; 

        mergeSort(arr, l, m); 
        mergeSort(arr, m+1, r);   
        merge(arr, l, m, r); 
    } 
} 
// end of merge sort

// suffix array
void suffixarray(char arr[],int size) {
    int k = 1;
    

    for(int i = 0; i < size; i++) {
        SA[i][0] = arr[i] - 'A';
        SA[i][2] = i;
    }
    mergeSort(SA, 0, size-1);


    int hash_for_sorter[MAXN][2];
    int rank = 1;
    int mark[2] = {-1};

    for(int j = 1 ; k < size ; j++) {
        rank = 1;
        mark[0] = SA[0][0]; // mark 
        mark[1] = SA[0][1]; // mark
        hash_for_sorter[SA[0][2]][0] = SA[0][2]; // location
        hash_for_sorter[SA[0][2]][1] = rank;

        for(int i = 1 ; i < size; i++) {
            if(mark[0] < SA[i][0]) {
                rank++;
                hash_for_sorter[SA[i][2]][0] = SA[i][2];
                hash_for_sorter[SA[i][2]][1] = rank;
                mark[0] = SA[i][0];
                mark[1] = SA[i][1];
            }

            else {
                if(mark[1] < SA[i][1]) {
                    rank++;
                    hash_for_sorter[SA[i][2]][0] = SA[i][2];
                    hash_for_sorter[SA[i][2]][1] = rank;
                    mark[0] = SA[i][0];
                    mark[1] = SA[i][1];
                }

                else {
                    hash_for_sorter[SA[i][2]][0] = SA[i][2];
                    hash_for_sorter[SA[i][2]][1] = rank;
                }
            }
        }

        for(int i = 0 ; i < size; i++) {
            SA2[i][0] = hash_for_sorter[i][1];
            if(i + k < size) {
                SA2[i][1] = hash_for_sorter[i+k][1];
            }
            else {
                SA2[i][1] = 0;
            }
            SA2[i][2] = hash_for_sorter[i][0]; 
        }

        mergeSort(SA2,0,size-1);

        for(int i = 0; i < size; i++) {
            SA[i][0] = SA2[i][0];
            SA[i][1] = SA2[i][1];
            SA[i][2] = SA2[i][2];   
        }

        k = pow(2,j);
    }  
} // O(nlognlogn)
// end of suffix array

// binary search for finding code owner
int binsearch(int e, int l, int r) 
{ 
    if (r >= l) { 
        int mid = l + (r - l) / 2; 
        if (cut[mid] == e) 
            return mid; 
  
        if (cut[mid] > e) 
            return binsearch(e, l, mid - 1); 

        return binsearch(e, mid + 1, r); 
    } 
    return r; 
}
// end of binary search


// https://link.springer.com/chapter/10.1007/978-3-642-02441-2_17
// comuting LCP with PLCP
void compute_PLCP(char Arr[], int size) {
    Pi[SA[0][2]] = -1;

    for( int i = 0; i < size; i++) {
        Pi[SA[i][2]] = SA[i-1][2];
    } 

    int l = 0;

    for(int i = 0; i < size; i++) {
        if(Pi[i] == -1) {
            PLCP[i] = 0; continue;
        }
        
        
        while(i + l <size && Pi[i] + l < size && Arr[i+l] == Arr[Pi[i] + l]) {
            l++;
        }

        int loc = binsearch(i,0,M-1)+1;
        int max_size = cut[loc] - i;

        PLCP[i] = (l > max_size)? max_size:l;
        
        if(l > 0) {
            l = l-1;
        }

    }
    for(int i = 0; i < size; i++) {
        LCP[i] = PLCP[SA[i][2]];
    }

} // O(nlogn)
// end of LCP



// check ownership of a suffixes
// form groups that have at least L characters same
void check_ownership(int size, int M, int L) {
    vector <vector <int> > queue_stack;
    vector <vector <int> > results;
    vector <int> dummy(3);

    for(int i = 0; i < M; i++) {
        group[i] = -1;
        group_members[i] = 0;
    }
    
   
   for(int i = 0; i < size; i++) {
       hash_owner[i] = binsearch(SA[i][2],0,M-1)+1;
   } // O(nlogn)

   cout << "hash owner computed"  << endl;

    int lptr = 0;
    int rptr = 1;
    int count = 0;
    
    int group_max = 0;

    while(rptr <= size) {
    
        if(LCP[rptr] < L) {
            cout << "Count " << count+1 << " in lptr " << lptr-1 << " and rptr "<< rptr-1 << endl;
            dummy[0] = lptr-1; dummy[1] = rptr-1; dummy[2] = count + 1;
            if(count+1 > group_max) {
                group_max = count+1;
            }

            lptr = rptr+1;
            rptr = lptr;
            count = 0;

            for(int i = 0; i < M; i++) {
                group[i] = -1;
                group_members[i] = 0;
            }
            queue_stack.clear();
        }

        else if(LCP[rptr] >= L) { 
            if(group[hash_owner[rptr]] == -1) { 
                vector <int> temp_vector;
                temp_vector.push_back(LCP[rptr]);
                queue_stack.push_back(temp_vector);
                
                group[hash_owner[rptr]] = count; 
                group_members[hash_owner[rptr]] = 1; 
                count++;
                rptr++;
            }
            
 
            else {
                int last_element; 
                
                while( sizeof(queue_stack[group[hash_owner[rptr]]]) > 0) {
                    group_members[hash_owner[rptr]]++;
                    last_element = queue_stack[group[hash_owner[rptr]]].back();
                    queue_stack[group[hash_owner[rptr]]].pop_back();
                    if(last_element <= LCP[rptr]) {
                        queue_stack[group[hash_owner[rptr]]].push_back(last_element);
                        break;
                    }
                }     
                queue_stack[group[hash_owner[rptr]]].push_back(LCP[rptr]);
                rptr++;
            }

        }
    } 
    cout << group_max << endl;   
}
// end of check ownership


int main() {
    cin >> M;

    for(int i = 0; i < M; i++) {
        cin >> N[i];
    }

    int k = 0;
    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N[i]; j++) {
            scanf("%c",&input[k]);
            k++;
        }
        cut[i] = k;       
    }

    suffixarray(input,k);
    cout << "Suffix Array Success" << endl;
    compute_PLCP(input,k);
    cout << "PLCP Success" << endl;
    
    cin >> L;
    if(L < 30) {
        cout << "Argument value too low" << endl;
    } 
    check_ownership(k,M,L);
    cout << "Ownership Success" << endl;

}
