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

// Should have one argument (x); need to change for data structure maybe you can do this
int MAKESET(int x)
{
    // this should maybe be Graph.x.parent or x.parent; not sure
    Graph.Vertex.parent = x;
    Graph.Vertex.rank = 0;

}


// Should have one argument (x); needs to be altered according to data structure
void FIND(int x)
{
    // same applied to this
    if(Graph.Vertex[x].parent != x)
    {
        Graph.Vertex[x].parent = FIND(Graph.Vertex[x].parent);
        return Graph.Vertex[x].parent;
    }
}

// Should have two arguments (x, y); needs to be altered according to data stucture
void LINK(x, y)
{
    if (Graph.Vertex[x].rank > Graph.Vertex[y].rank){
        Graph.Vertex[y].parent = x;
    }
    if (Graph.Vertex[x].rank == Graph.Vertex[y].rank){
        Graph.Vertex[y].rank++;
    }
    Graph.Vertex[x].parent = y;
    return (y);
}

int UNION(x, y)
{
    LINK(FIND(x), FIND(y));
}

void KRUSKAL(vector<vector<double> > G)
{
    std::unordered_set <int> vSet; // set X; could be another data type since we are only returning a sum

    Graph.Edge.weight = sort(Graph.Edge.weight);
    // not sure how to set up for loop in this case; maybe you can help
    for (int i = 0; i < sizeof(Graph.Vertex); i++)
    {
        MAKESET(Graph.Vertex[i].id);
    }
    // same here, not sure how to set up for loop
    for (int i = 0; i < sizeof(Graph.Edge); i++)
    {
        if (FIND(Graph.Edges[i].u) != FIND(Graph.Edges[i].v))
        {
            // add weight of corresponding edge to our running sum; this can be altered once we choose data structure to store
            vSet = vSet.insert(EDGE[i]);
            UNION(Graph.Edges[i].u, Graph.Edges[i].v);
        }
    }
    return vSet;
}

