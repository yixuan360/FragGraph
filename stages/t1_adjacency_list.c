# include<stdio.h>
# include<stdlib.h>
# include<string.h>

# define MAX_NOTES 100

typedef struct Edge
{
	int dest;
	int weight;
	struct Edge* next;
}Edge;

typedef struct Graph
{
	Edge* head[MAX_NOTES];
	int node_count;
	int edge_count;
}Graph;


Graph* create_graph()
{
	Graph* g = (Graph*)malloc(sizeof(Graph));
	g->node_count = 0;
	g->edge_count = 0;
	memset(g->head,0,sizeof(g->head));
	return g;
}

int has_edge(Graph* g,int i,int j)
{
	Edge* p = g->head[i];
	while(p != NULL)
	{
		if(p->dest == j) return 1;
		p = p->next;
		
	}
	return 0;
}

void add_single_edge(Graph* g,int from,int to,int w)
{
	if(from<0||from>=MAX_NOTES||to<0||to>=MAX_NOTES)
	{
		printf("结点编号越界\n");
		return ;
	}
	if(has_edge(g,from,to))
	{
		Edge* p = g->head[from];
		while (p)
		{
			if(p->dest==to)
			{
				p->weight += w;
				break;
			}
			p = p->next;
		}
		return;
	}
	Edge* new_e = (Edge*)malloc(sizeof(Edge));
	new_e->dest = to;
	new_e->weight = w;
	new_e->next = g->head[from];
	g->head[from] = new_e;
	g->edge_count ++;
	
}

void add_undirected_edge(Graph* g,int i,int j,int w)
{
	add_single_edge(g,i,j,w);
	add_single_edge(g,j,i,w);
}

void print_neighbors(Graph* g,int node_id)
{
	printf("笔记%d的邻居列表:",node_id);
	Edge* p = g->head[node_id];
	while(p!=NULL)
	{
		printf("[id:%d,权重:%d] ",p->dest,p->weight );
		p = p->next;
	}
	printf("\n");
}

void free_graph(Graph* g)
{
	for(int i=0;i<MAX_NOTES;i++)
	{
		Edge* p = g->head[i];
		while(p!=NULL)
		{
			Edge* temp =p;
			p = p->next;
			free(temp);
		}
	}
	free(g);
}

int main(void)
{
    Graph* g = create_graph();
    g->node_count = 4; 
    add_undirected_edge(g,0,1,2);
    add_undirected_edge(g,0,3,1);
    add_undirected_edge(g,1,2,1);
    
    print_neighbors(g,0);
    print_neighbors(g,1);
    print_neighbors(g,2);
    print_neighbors(g,3);
    free_graph(g);
    return 0;
}





























