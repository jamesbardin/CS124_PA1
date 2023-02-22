using namespace std;
#include <unordered_set>

// Swap two elements - Utility function  
void swap(int* a, int* b) 
{ 
    int t = *a; 
    *a = *b; 
    *b = t; 
} 
   
// partition the array using last element as pivot
int partition (int arr[], int low, int high) 
{ 
    int pivot = arr[high];    // pivot 
    int i = (low - 1);   
   
    for (int j = low; j <= high- 1; j++) 
    { 
        //if current element is smaller than pivot, increment the low element
        //swap elements at i and j
        if (arr[j] <= pivot) 
        { 
            i++;    // increment index of smaller element 
            swap(&arr[i], &arr[j]); 
        } 
    } 
    swap(&arr[i + 1], &arr[high]); 
    return (i + 1); 
} 
   
//quicksort algorithm
void quickSort(int arr[], int low, int high) 
{ 
    if (low < high) 
    { 
        //partition the array 
        int pivot = partition(arr, low, high); 
   
        //sort the sub arrays independently 
        quickSort(arr, low, pivot - 1); 
        quickSort(arr, pivot + 1, high); 
    } 
} 

// Should have one argument (x)
int MAKESET(int parent[], int rank[], int x)
{
    std::unordered_set <int> vSet

    parent[x] = x;
    rank[x] = 0;
}

std::unordered_set <int> MAKESET(int x)
{
    std::unordered_set <int> vSET;
    vSET.append(x);
}

// Should have one argument (x)
void FIND(int parent[], int x)
{
    if(parent[x] != x){
        parent[x] = FIND(parent[x]);
        return parent[x];
    }
}

// Should have two arguments (x, y)
void LINK(x, y)
{
    if (rank[x] > rank[y]){
        parent[y] = x;
    }
    if (rank[x] == rank[y]){
        rank[y]++;
    }
    parent[x] = y;
    return (y);
}

int UNION(x, y)
{
    LINK(FIND(x), FIND(y));
}

void KRUSKAL(vector<vector<double> > G)
{
    std::unordered_set <int> vSet;

    EDGE = quickSort(EDGE, EDGE[0], EDGE[-1]);
    for (int i = 0; i < sizeof(VERTEX); i++)
    {
        MAKESET(VERTEX[i]);
    }
    for (int i = 0; i < sizeof(EDGE); i++)
    {
        if (FIND(EDGE[i][0]) != FIND(EDGE[i][1]))
        {
            vSet = vSet.insert(EDGE[i]);
            UNION(EDGE[i][0], EDGE[i][1]);
        }
    }
    return vSet;
}

