#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct Edge
{
    int destination;
    int weight;
    struct Edge *next;
};

struct Vertex
{
    char city[50];
    struct Edge *edges;
};

struct Graph
{
    int numVertices;
    struct Vertex *vertices;
};

struct Edge *createEdge(int destination, int weight)
{
    struct Edge *newEdge = (struct Edge *)malloc(sizeof(struct Edge));
    newEdge->destination = destination;
    newEdge->weight = weight;
    newEdge->next = NULL;
    return newEdge;
}

void addEdge(struct Graph *graph, int src, int dest, int weight)
{
    struct Edge *newEdge = createEdge(dest, weight);
    newEdge->next = graph->vertices[src].edges;
    graph->vertices[src].edges = newEdge;

    newEdge = createEdge(src, weight);
    newEdge->next = graph->vertices[dest].edges;
    graph->vertices[dest].edges = newEdge;
}

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

int minDistance(int dist[], int visited[], int numVertices)
{
    int min = INT_MAX, min_index;

    for (int v = 0; v < numVertices; v++)
        if (visited[v] == 0 && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

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

    int path[numVertices];
    int pathLength = 0;
    int current = dest;
    while (current != -1)
    {
        path[pathLength++] = current;
        current = parent[current];
    }

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
    int numVertices = 11; 
    struct Graph *graph = createGraph(numVertices);

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

    printf("Choose your source and destination :\n");
    for (int i = 0; i < numVertices; ++i) {
        
        printf("%d.%s ",i+1, graph->vertices[i].city);
        struct Edge* edge = graph->vertices[i].edges;
        while (edge) {
            edge = edge->next;
        }
        printf("\n");
    }

    char sourceCity[50], destCity[50];
    printf("Enter source city: ");
    fgets(sourceCity, sizeof(sourceCity), stdin);
    sourceCity[strcspn(sourceCity, "\n")] = '\0';

    printf("Enter destination city: ");
    fgets(destCity, sizeof(destCity), stdin);
    destCity[strcspn(destCity, "\n")] = '\0';

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
