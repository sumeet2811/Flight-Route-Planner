#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Structure to represent an edge in the graph
struct Edge
{
    int destination;
    int weight;
    struct Edge *next;
};

// Structure to represent a vertex in the graph
struct Vertex
{
    char city[50];
    struct Edge *edges;
};

// Structure to represent the graph
struct Graph
{
    int numVertices;
    struct Vertex *vertices;
};

// Function to create a new edge
struct Edge *createEdge(int destination, int weight)
{
    struct Edge *newEdge = (struct Edge *)malloc(sizeof(struct Edge));
    newEdge->destination = destination;
    newEdge->weight = weight;
    newEdge->next = NULL;
    return newEdge;
}

// Function to add an edge to the graph
void addEdge(struct Graph *graph, int src, int dest, int weight)
{
    // Adding edge from src to dest
    struct Edge *newEdge = createEdge(dest, weight);
    newEdge->next = graph->vertices[src].edges;
    graph->vertices[src].edges = newEdge;

    // For an undirected graph, add the reverse edge as well
    newEdge = createEdge(src, weight);
    newEdge->next = graph->vertices[dest].edges;
    graph->vertices[dest].edges = newEdge;
}

// Function to initialize the graph
struct Graph *createGraph(int numVertices)
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->numVertices = numVertices;
    graph->vertices = (struct Vertex *)malloc(numVertices * sizeof(struct Vertex));

    for (int i = 0; i < numVertices; ++i)
    {
        graph->vertices[i].edges = NULL;
        strcpy(graph->vertices[i].city, "");
    }

    return graph;
}

// Function to find the minimum distance vertex in the set of vertices not yet included in the shortest path tree
int minDistance(int dist[], int visited[], int numVertices)
{
    int min = INT_MAX, min_index;

    for (int v = 0; v < numVertices; v++)
        if (visited[v] == 0 && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

// Function to print the shortest path from source to target using Dijkstra's algorithm
// Function to print the shortest path from source to target using Dijkstra's algorithm
void shortestPath(struct Graph *graph, int src, int dest)
{
    int numVertices = graph->numVertices;
    int dist[numVertices];
    int visited[numVertices];
    int parent[numVertices];

    for (int i = 0; i < numVertices; i++)
    {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }

    dist[src] = 0;
    parent[src] = -1;

    for (int count = 0; count < numVertices - 1; count++)
    {
        int u = minDistance(dist, visited, numVertices);
        visited[u] = 1;

        struct Edge *edge = graph->vertices[u].edges;
        while (edge != NULL)
        {
            int v = edge->destination;
            if (!visited[v] && dist[u] != INT_MAX && dist[u] + edge->weight < dist[v])
            {
                dist[v] = dist[u] + edge->weight;
                parent[v] = u;
            }
            edge = edge->next;
        }
    }

    printf("\nShortest Path from %s to %s:\n", graph->vertices[src].city, graph->vertices[dest].city);
    if (dist[dest] == INT_MAX)
    {
        printf("Path does not exist\n");
        return;
    }

    // Collect the path in an array
    int path[numVertices];
    int pathLength = 0;
    int current = dest;
    while (current != -1)
    {
        path[pathLength++] = current;
        current = parent[current];
    }

    // Print the path in reverse order
    for (int i = pathLength - 1; i >= 0; i--)
    {
        printf("%s", graph->vertices[path[i]].city);
        if (i > 0)
        {
            printf(" -> ");
        }
    }
    printf("\nShortest Distance: %d\n", dist[dest]);
}

int main()
{
    int numVertices = 11; // Number of cities
    struct Graph *graph = createGraph(numVertices);

    // Adding edges with weights

    addEdge(graph, 0, 9, 20);
    addEdge(graph, 0, 1, 20);
    addEdge(graph, 1, 4, 20);
    addEdge(graph, 1, 2, 20);
    addEdge(graph, 2, 3, 20);
    addEdge(graph, 3, 4, 20);
    addEdge(graph, 4, 6, 20);
    addEdge(graph, 4, 9, 20);
    addEdge(graph, 5, 7, 20);
    addEdge(graph, 5, 8, 20);
    addEdge(graph, 6, 5, 20);
    addEdge(graph, 6, 7, 20);
    addEdge(graph, 7, 8, 20);
    addEdge(graph, 7, 9, 20);
    addEdge(graph, 8, 10, 20);
    addEdge(graph, 10, 9, 20);

    // Setting city names
    strcpy(graph->vertices[0].city, "Panaji");
    strcpy(graph->vertices[1].city, "Bengaluru");
    strcpy(graph->vertices[2].city, "Thiruvananthapuram");
    strcpy(graph->vertices[3].city, "Chennai");
    strcpy(graph->vertices[4].city, "hyderabad");
    strcpy(graph->vertices[5].city, "Bhopal");
    strcpy(graph->vertices[6].city, "Kolkata");
    strcpy(graph->vertices[7].city, "Delhi");
    strcpy(graph->vertices[8].city, "Jaipur");
    strcpy(graph->vertices[9].city, "Mumbai");
    strcpy(graph->vertices[10].city, "Ahmedabad");

    //Example: Printing the graph
    printf("Choose your source and destination :\n");
    for (int i = 0; i < numVertices; ++i) {
        
        printf("%d.%s ",i+1, graph->vertices[i].city);
        struct Edge* edge = graph->vertices[i].edges;
        while (edge) {
            edge = edge->next;
        }
        printf("\n");
    }

    // Getting source and destination cities from user input
    char sourceCity[50], destCity[50];
    printf("Enter source city: ");
    fgets(sourceCity, sizeof(sourceCity), stdin);
    sourceCity[strcspn(sourceCity, "\n")] = '\0';

    printf("Enter destination city: ");
    fgets(destCity, sizeof(destCity), stdin);
    destCity[strcspn(destCity, "\n")] = '\0';

    // Finding indices of source and destination cities
    int sourceIndex = -1, destIndex = -1;
    for (int i = 0; i < numVertices; ++i)
    {
        if (strcmp(graph->vertices[i].city, sourceCity) == 0)
        {
            sourceIndex = i;
        }
        if (strcmp(graph->vertices[i].city, destCity) == 0)
        {
            destIndex = i;
        }
    }

    if (sourceIndex != -1 && destIndex != -1)
    {
        shortestPath(graph, sourceIndex, destIndex);
    }
    else
    {
        printf("City not found in the graph.\n");
    }

    // Free the memory allocated for the graph
    for (int i = 0; i < numVertices; ++i)
    {
        struct Edge *edge = graph->vertices[i].edges;
        while (edge)
        {
            struct Edge *temp = edge;
            edge = edge->next;
            free(temp);
        }
    }
    free(graph->vertices);
    free(graph);

    return 0;
}