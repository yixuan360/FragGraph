# include<stdio.h>
# include<stdlib.h>

#define QUEUE_MAX 100
int queue[QUEUE_MAX];
int front=0;
int rear=0;

void enqueue(int node_id)
{
	queue[rear++]=node_id;
}

int dequeue()
{
	return queue[front++];
}

int is_empty()
{
	return front == rear;
}

void reset_queue()
{
	front = 0;
	rear = 0;
}

typedef struct Edge
{
	int to;
	struct Edge* next;
}Edge;

#define N 4
Edge* head[N];
int visited[N];

void add_edge(int u,int v)
{
	Edge* e1 = (Edge*)malloc(sizeof(Edge));
	e1->to = v;
	e1->next = head[u];
	head[u]= e1;
	
	Edge* e2 = (Edge*)malloc(sizeof(Edge));
	e2->to=u;
	e2->next = head[v];
	head[v] = e2;
}

void bfs(int start)
{
	reset_queue();
	for(int i=0;i<N;i++) visited[i]=0;
	enqueue(start);
	visited[start]=1;
	printf("BFS访问顺序：");
	while(!is_empty())
	{
		int u = dequeue();
		printf("%d ",u);
		Edge* p = head[u];
		while(p!=NULL)
		{
			int v = p->to;
			if(visited[v]==0)
			{
				visited[v]=1;
				enqueue(v);
			}
			p = p->next;
			
		}
		
	}
	printf("\n");
	
}

void detect_isolated()
{
	printf("孤立节点: ");
	int has_isolated = 0;
	for(int i=0;i<N;i++)
	{
		if(head[i]==NULL)
		{
			printf("%d ",i);
			has_isolated = 1;
		}
	}
	if(!has_isolated) printf("无");
	printf("\n");
}

int count_component()
{
	for(int i=0;i<N;i++) visited[i]=0;
	int cnt = 0;
	for (int i=0;i<N;i++)
	{
		if(visited[i]=0)
		{
			reset_queue();
			enqueue(i);
			visited[i]=1;
			while(!is_empty())
			{
				int u = dequeue();
				Edge* p =head[u];
				while(p!=NULL)
				{
					int v = p->to;
					if(visited[v]==0)
					{
						visited[v]=1;
						enqueue(v);
					}
					p = p->next;
					
				}				
			}
			cnt++;
		}
	}
	return cnt++;
}

//测试队列（任务1单独测试）
void test_queue()
{
    reset_queue();
    printf("=====队列测试=====\n");
    enqueue(10);
    enqueue(20);
    enqueue(30);
    enqueue(40);
    enqueue(50);
    printf("出队顺序：");
    while(!is_empty())
    {
        int x = dequeue();
        printf("%d ",x);
    }
    printf("\n\n");
}


int main()
{
    //任务1 测试队列
    test_queue();

    //初始化邻接表头全部为空
    for(int i=0;i<N;i++)
        head[i] = NULL;

    //构造测试图：0--1--2，3孤立
    add_edge(0,1);
    add_edge(1,2);

    //任务2 BFS测试
    bfs(0);
    bfs(3);

    //任务3 孤立节点检测
    detect_isolated();

    //任务4 连通分量
    int comp = count_component();
    printf("连通分量总数：%d\n",comp);
    if(comp>1)
        printf("图谱不连通，存在多组无关知识群组\n");

    return 0;
}





























