# include<stdio.h>
# include<string.h>
# include<stdlib.h>

#define MAX_TITLE 100
#define MAX_CONTENT 500
#define MAX_TAGS 10
#define MAX_TAG_LEN 50
#define MAX_NOTES 200


typedef struct Note
{
	int id;
	char title[MAX_TITLE];
	char content[MAX_CONTENT];
	char tags[MAX_TAGS][MAX_TAG_LEN];
	int tag_count;
	struct Note* next;
	
}Note;

typedef struct Edge
{
	int to;
	int weight;
	struct Edge* next;
}Edge;

Note* note_head = NULL;
Edge* head[MAX_NOTES];
int note_cnt = 0;
int graph_built = 0;


int split_tags(char* s, char t[][MAX_TAG_LEN]);

void create_note()
{
	Note* new_note = (Note*)malloc(sizeof(Note));
	new_note->id = ++note_cnt;
	getchar();
	printf("请输入笔记标题：");
	fgets(new_note->title,MAX_TITLE,stdin);
	new_note->title[strcspn(new_note->title,"\n")]=0;
	printf("请输入笔记正文：");
	fgets(new_note->content,MAX_CONTENT,stdin);
	new_note->content[strcspn(new_note->content,"\n")] = 0;
	
	char buf[200];
	printf("请输入标签：");
	fgets(buf,200,stdin);
	buf[strcspn(buf,"\n")] = 0;
	new_note->tag_count = split_tags(buf,new_note->tags);
	
	new_note->next = note_head;
	note_head = new_note;
	graph_built = 0;
	printf("笔记创建成功，ID=%d\n",new_note->id );
	
}

void del_note()
{
	int id;
	printf("请输入待删除笔记ID:");
	scanf("%d",&id);
	Note* p = note_head;
	Note* pre = NULL;
	while(p)
	{
		if(p->id == id)
		{
			if(pre==NULL)note_head = p->next;
			else pre->next = p->next;
			free(p);
			graph_built = 0;
			printf("删除成功\n");
			return;
		}
		pre = p;
		p = p->next;
	}
	printf("未找到该ID笔记\n");
	
}

Note* find_note_by_id(int id)
{
	Note* p = note_head;
	while(p)
	{
		if(p->id == id) return p;
		p = p->next;
	}
	return NULL;
}

void modify_note()
{
	int id;
	printf("请输入待修改的笔记ID：");
	scanf("%d",&id);
	Note* n = find_note_by_id(id);
	if(n==NULL)
	{
		printf("该笔记不存在\n");
		return;
	}
	getchar();
	printf("输入新标题：");
	fgets(n->title,MAX_TITLE,stdin);
	n->title[strcspn(n->title,"\n")]=0;
	printf("请输入新笔记正文：");
	fgets(n->content,MAX_CONTENT,stdin);
	n->content[strcspn(n->content,"\n")] = 0;
	
	char buf[200];
	printf("请输入新标签：");
	fgets(buf,200,stdin);
	buf[strcspn(buf,"\n")] = 0;
	n->tag_count = split_tags(buf,n->tags);
	graph_built = 0;
	printf("修改完成\n");
	
	
}

void list_notes()
{
	if(note_head == NULL)
	{
		printf("暂无笔记\n");
		return;
	}
	Note* p = note_head;
	printf("笔记列表：\n");
	while(p)
	{
		printf("ID:%d |标题：%s\n",p->id,p->title);
		printf("标签：");
		for(int i = 0;i<p->tag_count;i++) printf("%s ",p->tags[i]);
		printf("\n");
		p = p->next;
	}
}

void search_by_tag()
{
	char key[MAX_TAG_LEN];
	getchar();
	printf("请输入搜索标签：");
	fgets(key,MAX_TAG_LEN,stdin);
	key[strcspn(key,"\n")]=0;
	if(strlen(key)==0)
	{
		printf("标签不能为空\n");
		return;
	}
	Note* p = note_head;
	int found = 0;
	while(p)
	{
		for(int i=0;i<p->tag_count;i++)
		{
			if(strcmp(p->tags[i],key)==0)
			{
				printf("匹配 ID:%d |标题：%s\n",p->id,p->title);
				found = 1;
				break;
			}
		}
		p = p->next;
	}
	if(!found) printf("无匹配标签笔记\n");
	
}


int split_tags(char* s,char t[][MAX_TAG_LEN])
{
	int tag_cnt = 0;
	char buf[500];
	strncpy(buf,s,sizeof(buf)-1);
	buf[sizeof(buf)-1] = '\0';
	char* token = strtok(buf,",，");
	while(token != NULL&&tag_cnt<MAX_TAGS)
	{
		while(*token ==' '||*token == '\t') token++;
		int len = strlen(token);
		while(len>0&&(token[len-1]==' '||token[len-1]=='\t'))
		{
			token[--len]='\0';
		}
		if(len>0)
		{
			strcpy(t[tag_cnt],token);
			tag_cnt++;
		}
		
		token = strtok(NULL,",，");
	}
	return tag_cnt;
}

int find_shared_tags(Note* n1,Note* n2)
{
	int tag_cnt = 0;
	for(int i=0;i<n1->tag_count;i++)
	{
		for(int j=0;j<n2->tag_count;j++)
		{
			if(strcmp(n1->tags[i],n2->tags[j])==0) tag_cnt++;
		}
	}
	return tag_cnt;
}


void add_edge(int from,int to,int w)
{
	Edge* e = (Edge*)malloc(sizeof(Edge));
	e->to = to;
	e->weight = w;
	e->next = head[from];
	head[from] = e;
}

void build_graph()
{
	for(int i=0;i<MAX_NOTES;i++)
	{
		Edge* p = head[i];
		while(p)
		{
			Edge* tmp = p;
			p=p->next;
			free(tmp);
		}
		head[i]=NULL;
	}
	Note* i = note_head;
	while(i)
	{
		Note* j = note_head;
		while(j)
		{
			if(i->id<j->id )
			{
				int share = find_shared_tags(i,j);
				if(share>0)
				{
					add_edge(i->id,j->id,share);
					add_edge(j->id,i->id,share);
				}
			}
			j = j->next;
		}
		i = i->next;
	}
	graph_built = 1;
	printf("知识图谱构建完成\n");
}

void print_graph()
{
	if(!graph_built)
	{
		printf("请先构建知识图谱\n");
		return;
	}
	printf("知识图谱：\n");
	for(int id=1;id<=note_cnt;id++)
	{
		if(find_note_by_id(id)==NULL) continue;
		printf("笔记%d : ",id);
		Edge* e = head[id];
		if(e==NULL)
		{
			printf("无关联\n");
			continue;
		}
		while(e)
		{
			printf("[->%d,权重：%d] ",e->to,e->weight);
			e = e->next;
		}
		printf("\n");
	}
	

}

void bfs(int start_id)
{
	if(!graph_built)
	{
		printf("请先构建知识图谱\n");
		return;
	}
	
	if(find_note_by_id(start_id)==NULL)
	{
		printf("起始笔记ID不存在\n");
		return;
	}
	
	if(start_id>=MAX_NOTES)
	{
		printf("ID超出最大限制\n");
		return;
	}
	int queue[MAX_NOTES],visited[MAX_NOTES]={0};
	int front=0,rear = 0;
	queue[rear++] = start_id;
	visited[start_id]=1;
	printf("从笔记%d出发的关联笔记链",start_id);
	while(front<rear)
	{
		int i = queue[front++];
		printf("%d -> ",i);
		Edge* e = head[i];
		while(e)
		{
			if(!visited[e->to])
			{
				visited[e->to]=1;
				queue[rear++] = e->to;
			}
			e=e->next;
		}
		if(front<rear) printf("->");
	}
	printf("END\n");
}

void find_isolate()
{
	if(!graph_built)
	{
		printf("请先构建知识图谱\n");
		return;
	}
	Note* p = note_head;
	printf("孤立笔记：\n");
	int has_isolate = 0;
	while(p)
	{
		if(head[p->id]==NULL)
		{
			printf("ID:%d 标题：%s是孤立的知识碎片\n",p->id,p->title);
			has_isolate = 1;
	    }
	    p = p->next;
    }
    if(!has_isolate) printf("没有孤立笔记碎片\n");  
}


void save_to_file()
{
	FILE* fp = fopen("knowledge.txt","w");
	if(!fp)
	{
		printf("打开失败\n");
		return;
	}
	Note* p = note_head;
	while(p)
	{
		fprintf(fp,"[NOTE]|%d|%s|%s|",p->id,p->title,p->content );
		for(int i =0;i<p->tag_count;i++)
		{
			fprintf(fp,"%s",p->tags[i]);
			if(i != p->tag_count-1) fprintf(fp,",");
			
		}
		fprintf(fp,"\n");
		p = p->next;
		
		
	}
	fclose(fp);
	printf("笔记成功保存到knowledge.txt\n");
}

void load_from_file()
{
	Note* p = note_head;
	while(p)
	{
		Note* tmp = p;
		p = p->next;
		free(tmp);
	}
	note_head = NULL;
	note_cnt=0;
	graph_built = 0;
	FILE* fp = fopen("knowledge.txt","r");
	if(!fp)
	{
		printf("读取失败");
		return;
	}
	char line[1024];
	while(fgets(line,sizeof(line),fp))
	{
		line[strcspn(line,"\n")]=0;
		if(strlen(line)==0) continue;
		Note* new_note = (Note*)malloc(sizeof(Note));
		memset(new_note,0,sizeof(Note));
		char tagbuf[500]={0};
		sscanf(line,"[NOTE]|%d|%[^|]|%[^|]|%[^\n]",&new_note->id,new_note->title,new_note->content,tagbuf);
		new_note->tag_count = split_tags(tagbuf,new_note->tags);
		note_cnt = new_note->id > note_cnt?new_note->id : note_cnt;
		new_note->next = note_head;
		note_head = new_note;
    }
    fclose(fp);
    printf("笔记从knowledge.txt加载成功\n");
}


void free_all()
{
	for(int i = 0;i<MAX_NOTES;i++)
	{
		Edge* e = head[i];
		while(e)
		{
			Edge* t = e;
			e = e->next;
			free(t);
		}
	}
	Note* p = note_head;
	while(p)
	{
		Note* t = p;
		p = p->next;
		free(t);
	}
	note_head = NULL;
	note_cnt = 0;
}


void menu()
{
	int op;
	while(1)
	{
		printf("\n========个人知识碎片整理&关联图谱生成器========\n");
        printf("1. 添加笔记\n");
        printf("2. 删除笔记\n");
        printf("3. 修改笔记\n");
        printf("4. 查看笔记列表\n");
        printf("5. 按标签搜索笔记\n");
        printf("6. 重建知识图谱\n");
        printf("7. 图谱遍历(BFS,输出关联笔记链)\n");
        printf("8. 查看孤立知识碎片笔记\n");
        printf("9. 保存笔记到文件\n");
        printf("10.从文件加载笔记\n");
        printf("0.退出程序\n");
        printf("请输入功能选项:");
        scanf("%d",&op);	
        switch(op)
        {
        	case 1:create_note(); break;
        	case 2: del_note(); break;
            case 3: modify_note(); break;
            case 4: list_notes(); break;
            case 5: search_by_tag(); break;
            case 6: build_graph();print_graph();break;
            case 7: 
            		{
            			int id;
            			printf("请输入起始笔记ID:");
            			scanf("%d",&id);
            			bfs(id);
            			break;
					}
			case 8:find_isolate(); break;
			case 9:save_to_file(); break;
			case 10: load_from_file(); break;
			case 0: free_all(); printf("程序退出\n");return;
			default: printf("无效选项\n");
            	
		}
	}
}


int main()
{
	menu();
	return 0;
}




























