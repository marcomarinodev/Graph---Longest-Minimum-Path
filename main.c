#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int n_adj;
    int* adj;
} vertex;

// Ho bisogno di usare la Queue per usare la BFS
typedef struct {
    int* values; // Valori attuali della coda
    int capacity; // Capacità della coda
    int back_index; // Inserisco da qui
    int front_index; // Prendo da qui
} queue;

// Inizializzo la coda
void queue_init(queue* q, int n) {
    q -> capacity = n; // La capacità è tanto quanto il numero dei vertici nel grafo
    q -> values = malloc(n * sizeof(int)); // Alloco la memoria per i valori interi
    q -> back_index = 0; 
    q -> front_index = 0;
}

void queue_pushBack(queue* q, int value) {
    q -> values[q -> back_index] = value; // Inserisco in back_index

    q -> back_index++; // Mando avanti il back_index

    // Se è arrivato oltre la capacità massima -> ritorna a 0
    if (q -> back_index > q -> capacity) {
        q -> back_index = 0;
    }

}

int queue_popFront(queue* q) {
    // Ottengo da front_index
    int value_taken = q -> values[q -> front_index];

    // Mando avanti il front_index
    q -> front_index++;

    // Se è arrivato oltre la capacità massima -> ritorna a 0
    if (q -> front_index > q -> capacity) {
        q -> front_index = 0;
    }

    // Ritorna il valor ottenuto
    return value_taken;
}

int is_queue_empty(queue* q) {
    if (q -> back_index == q -> front_index) {
        return 1; // empty
    }

    return 0; // not empty
}

vertex* read_graph(int n) {
    vertex* E = malloc(n * sizeof(vertex));
    int current_n_adj;
    int current_vertex;
    
    for (int i = 0; i < n; i++) {
        scanf("%d", &current_n_adj);
        E[i].n_adj = current_n_adj;
        E[i].adj = malloc(current_n_adj * sizeof(int));

        for (int j = 0; j < current_n_adj; j++) {
            scanf("%d", &current_vertex);
            E[i].adj[j] = current_vertex;
        }
    }

    return E;
}

// Breadth First Search (Single)
int single_breadth_visit(vertex* E, int n, int from) {
    int* colors = malloc(n * sizeof(int));
    int* distances = malloc(n * sizeof(int));
    queue q; // Coda
    int max_distance = 0;
    int dest;

    // Inizializzo i colori
    for (int i = 0; i < n; i++) colors[i] = 0;
    colors[from] = 1;

    // Inizalizzo le distanze
    for (int i = 0; i < n; i++) distances[i] = 0;

    // Inizializzo la coda
    queue_init(&q, n);
    queue_pushBack(&q, from); // Inserisco il nodo principale dellp spanning tree

    while (!is_queue_empty(&q)) {
        // Estraggo il l'elemento dalla coda
        int src = queue_popFront(&q);

        // Controllo gli elementi di adiacenza (Visita in ampiezza)
        for (int i = 0; i < E[src].n_adj; i++) {
            dest = E[src].adj[i]; // Ottengo il vertice di adiacenza

            // Se il colore è bianco (vertice non visitato)
            if (colors[dest] == 0) {
                colors[dest] = 1; // Lo visito marcandolo di nero
                // Aggiorno incrementando la distanza dal vertice principale
                distances[dest] = distances[src] + 1; 
                // Metto in coda un altro elemento da visitare in ampiezza
                queue_pushBack(&q, dest); 
            }
        }

    }

    // Ottengo la più lunga distanza tra il vertice from con un altro vertice connesso
    // Quello sarà il più lungo cammino minimo  di quel determinato spanning tree
    for (int i = 0; i < n; i++) {
        // Se trovo un vertice non colorato, allora il grafo non è connesso
        if (colors[i] == 0) {
            return -1;
        }

        // Conservo il più lungo cammino
        if (distances[i] > max_distance) {
            max_distance = distances[i];
        }
    }

    return max_distance;
}

int longest_min_path(vertex* E, int n) {
    int longest_min_path = 0; // Attuale cammino minimo più lungo (CML)
    int possible_longest_min_path = 0; // Possibile cammino minimo più lungo (PCML)

    for (int i = 0; i < n; i++) {  
        // Ottengo PCML
        possible_longest_min_path = single_breadth_visit(E, n, i);

        // Se ho riscontrato nella BFS che il grafo non è connesso, ritorno -1
        if (possible_longest_min_path == -1) {
            return -1;  
        }

        // Se PCML > CML -> CML = PCML
        if (possible_longest_min_path > longest_min_path) {
            longest_min_path = possible_longest_min_path;
        }
    }

    return longest_min_path;
}

int main() {
    int n;
    vertex* graph = NULL;

    scanf("%d", &n);

    graph = read_graph(n);
    
    printf("%d\n", longest_min_path(graph, n));

    return 0;
}
