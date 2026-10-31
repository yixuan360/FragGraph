# include<stdio.h>
# include<stdlib.h>
# include<string.h>

typedef struct EdgeNode
{
	int to;
	int weight;
	struct EdgeNode* next;
}EdgeNode;

# define MAX_NOTE 100
EdgeNode* graph[MAX_NOTE];

typedef struct Note
{
	char tags[20][20];
	int tag_cnt;
}Note;

int count_shared_tag(Note* a,Note* b)
{
	int cnt=0;
	for(int i=0;i<a->tag_cnt;i++)
	{
		for(int j=0;j<b->tag_cnt;j++)
		{
			if(strcmp(a->tags[i],b->tags[j])==0) cnt++;
		}
	}
	return cnt;
}

void add_edge(EdgeNode* graph[],int src,int dest,int weight)
{
	EdgeNode* new_edge = (EdgeNode*)malloc(sizeof(EdgeNode));
	new_edge->to = dest;
	new_edge->weight = weight;
	new_edge->next = graph[src];
	graph[src] = new_edge;
	
}

void print_graph(EdgeNode* graph[],int n)
{
	for(int i=0;i<n;i++)
	{
		printf("节点%d->",i);
		EdgeNode* p = graph[i];
		if(p==NULL)
		{
			printf("空\n");
			continue;
		}
		while(p!=NULL)
		{
			printf("[到%d,权%d]",p->to,p->weight);
			p=p->next;
		}
		printf("\n");
	}
}

void free_graph(EdgeNode* graph[],int n)
{
	for(int i=0;i<n;i++)
	{
		EdgeNode* p = graph[i];
		while(p!=NULL)
		{
			EdgeNode* temp = p;
			p = p->next;
			free(temp);
		}
		graph[i]=NULL;
	}
}

void build_graph(Note notes[],int n,EdgeNode* graph[],int threshold)
{
	for(int i=0;i<n;i++) graph[i]=NULL;
	for(int i=0;i<n;i++)
	{
		for(int j=i+1;j<n;j++)
		{
			int w = count_shared_tag(&notes[i],&notes[j]);
			if(w>=threshold)
			{
				add_edge(graph,i,j,w);
				add_edge(graph,j,i,w);
			}
		}
	}
	
	
}

int main(void)
{
    Note notes[3];
    //笔记0
    notes[0].tag_cnt = 2;
    strcpy(notes[0].tags[0], "指针");
    strcpy(notes[0].tags[1], "内存");
    //笔记1
    notes[1].tag_cnt = 2;
    strcpy(notes[1].tags[0], "链表");
    strcpy(notes[1].tags[1], "指针");
    //笔记2
    notes[2].tag_cnt = 2;
    strcpy(notes[2].tags[0], "图");
    strcpy(notes[2].tags[1], "BFS");

    //阈值=1
    build_graph(notes,3,graph,1);
    print_graph(graph,3);

    free_graph(graph,3);
    return 0;
}

























