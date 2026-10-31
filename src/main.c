#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 100
#define MAX_BODY  500
#define MAX_TAG   50
#define MAX_VERTEX 200

//==================== 模块一：笔记链表结构 ====================
typedef struct Note {
    int id;
    char title[MAX_TITLE];
    char body[MAX_BODY];
    char tag[MAX_TAG];
    struct Note *next;
} Note;

Note* note_head = NULL;
int next_id = 1;          // ID自增，删除不复用

//==================== 模块二：知识图谱‑邻接表 ====================
typedef struct AdjNode {
    int vid;
    struct AdjNode* next;
} AdjNode;

AdjNode* adj[MAX_VERTEX];
int vertex_cnt = 0;

//==================== 链表笔记函数声明 ====================
void add_note();
void del_note();
void modify_note();
void list_notes();
void search_by_tag();
void free_all_notes();

//==================== 图谱函数声明 ====================
void rebuild_graph();
void bfs_traverse();
void find_isolate();
void clear_graph();
int get_note_by_id(int id, Note** out);

//==================== 文件读写 ====================
void save_to_file();
void load_from_file();

//==================== 菜单主循环 ====================
void show_menu()
{
    printf("\n====== 知识图谱管理系统 ======\n");
    printf("1. 添加笔记\n");
    printf("2. 删除笔记\n");
    printf("3. 修改笔记\n");
    printf("4. 查看笔记列表\n");
    printf("5. 按标签搜索\n");
    printf("6. 重建知识图谱\n");
    printf("7. BFS遍历（从指定笔记出发）\n");
    printf("8. 查看孤立笔记\n");
    printf("9. 保存到文件\n");
    printf("10. 从文件加载\n");
    printf("0. 退出\n");
    printf("请输入选项: ");
}

//==================== 笔记链表实现 ====================
void add_note()
{
    Note* newp = (Note*)malloc(sizeof(Note));
    if(!newp){
        printf("内存分配失败!\n");
        return;
    }
    newp->id = next_id++;
    getchar();
    printf("输入标题:");
    fgets(newp->title,MAX_TITLE,stdin);
    newp->title[strcspn(newp->title,"\n")]=0;

    printf("输入正文:");
    fgets(newp->body,MAX_BODY,stdin);
    newp->body[strcspn(newp->body,"\n")]=0;

    printf("输入标签:");
    fgets(newp->tag,MAX_TAG,stdin);
    newp->tag[strcspn(newp->tag,"\n")]=0;

    newp->next = NULL;
    //尾插
    if(note_head == NULL){
        note_head = newp;
    }else{
        Note* p=note_head;
        while(p->next) p=p->next;
        p->next = newp;
    }
    printf("添加成功\n");
}

int get_note_by_id(int id, Note** out)
{
    Note* p=note_head;
    while(p){
        if(p->id == id){
            *out = p;
            return 1;
        }
        p=p->next;
    }
    return 0;
}

void del_note()
{
    int id;
    printf("请输入要删除笔记ID:");
    scanf("%d",&id);

    Note *pre=NULL,*cur=note_head;
    while(cur){
        if(cur->id == id){
            if(pre == NULL){
                note_head = cur->next;
            }else{
                pre->next = cur->next;
            }
            free(cur);
            printf("删除成功\n");
            rebuild_graph();  // 删除后重建图谱
            return;
        }
        pre=cur;
        cur=cur->next;
    }
    printf("未找到该ID笔记\n");
}

void modify_note()
{
    int id;
    printf("输入要修改笔记ID:");
    scanf("%d",&id);
    Note* np;
    if(!get_note_by_id(id,&np)){
        printf("笔记不存在\n");
        return;
    }
    printf("=== 当前内容 ===\n");
    printf("标题:%s\n正文:%s\n标签:%s\n",np->title,np->body,np->tag);
    getchar();
    printf("输入新标题:");
    fgets(np->title,MAX_TITLE,stdin);
    np->title[strcspn(np->title,"\n")]=0;
    printf("输入新正文:");
    fgets(np->body,MAX_BODY,stdin);
    np->body[strcspn(np->body,"\n")]=0;
    printf("输入新标签:");
    fgets(np->tag,MAX_TAG,stdin);
    np->tag[strcspn(np->tag,"\n")]=0;
    printf("修改完成\n");
    rebuild_graph(); // 修改重建图谱
}

void list_notes()
{
    if(note_head == NULL){
        printf("暂无笔记\n");
        return;
    }
    Note* p=note_head;
    while(p){
        printf("\nID:%d\n标题:%s\n标签:%s\n正文:%s\n-----\n",
               p->id,p->title,p->tag,p->body);
        p=p->next;
    }
}

void search_by_tag()
{
    char key[MAX_TAG];
    getchar();
    printf("输入搜索标签:");
    fgets(key,MAX_TAG,stdin);
    key[strcspn(key,"\n")]=0;
    int found=0;
    Note* p=note_head;
    while(p){
        if(strstr(p->tag,key)){
            printf("\nID:%d 标题:%s 标签:%s\n",p->id,p->title,p->tag);
            found=1;
        }
        p=p->next;
    }
    if(!found) printf("没有匹配标签的笔记\n");
}

void free_all_notes()
{
    Note* p=note_head;
    while(p){
        Note* t=p;
        p=p->next;
        free(t);
    }
    note_head=NULL;
}

//==================== 图谱邻接表操作 ====================
void clear_graph()
{
    for(int i=0;i<MAX_VERTEX;i++){
        AdjNode* p=adj[i];
        while(p){
            AdjNode* t=p;
            p=p->next;
            free(t);
        }
        adj[i]=NULL;
    }
    vertex_cnt=0;
}

// 根据标签相同自动建边（模块二建边逻辑示例）
void rebuild_graph()
{
    clear_graph();
    Note* p1 = note_head;
    int edge_num=0;
    while(p1){
        Note* p2 = note_head;
        while(p2){
            if(p1->id != p2->id && strcmp(p1->tag,p2->tag)==0){
                // 增加一条边 p1->p2
                AdjNode* newn = (AdjNode*)malloc(sizeof(AdjNode));
                newn->vid = p2->id;
                newn->next = adj[p1->id];
                adj[p1->id]=newn;
                edge_num++;
            }
            p2=p2->next;
        }
        p1=p1->next;
    }
    vertex_cnt = next_id-1;
    printf("重建图谱完成，总边数:%d\n",edge_num);
}

//==================== BFS遍历 ====================
void bfs_traverse()
{
    int start_id;
    printf("输入起始笔记ID:");
    scanf("%d",&start_id);
    Note* np;
    if(!get_note_by_id(start_id,&np)){
        printf("起始ID不存在\n");
        return;
    }
    int queue[MAX_VERTEX];
    int visited[MAX_VERTEX]={0};
    int front=0,rear=0;
    queue[rear++]=start_id;
    visited[start_id]=1;
    printf("BFS遍历序列(ID+标题):\n");
    while(front < rear){
        int cur = queue[front++];
        Note* n;
        get_note_by_id(cur,&n);
        printf("[%d] %s\n",cur,n->title);
        AdjNode* p=adj[cur];
        while(p){
            int nid = p->vid;
            if(!visited[nid]){
                visited[nid]=1;
                queue[rear++]=nid;
            }
            p=p->next;
        }
    }
}

void find_isolate()
{
    printf("孤立笔记列表(无相连节点):\n");
    Note* np=note_head;
    int has=0;
    while(np){
        if(adj[np->id]==NULL){
            printf("ID:%d,标题:%s\n",np->id,np->title);
            has=1;
        }
        np=np->next;
    }
    if(!has) printf("无孤立笔记\n");
}

//==================== 文件保存与加载 ====================
void save_to_file()
{
    FILE* fp=fopen("kb_data.txt","w");
    if(!fp){
        printf("打开文件失败\n");
        return;
    }
    Note* p=note_head;
    while(p){
        fprintf(fp,"NOTE|%d|%s|%s|%s\n",p->id,p->title,p->body,p->tag);
        p=p->next;
    }
    fprintf(fp,"[EDGES]\n");
    for(int i=1;i<next_id;i++){
        AdjNode* ap=adj[i];
        while(ap){
            fprintf(fp,"%d->%d\n",i,ap->vid);
            ap=ap->next;
        }
    }
    fclose(fp);
    printf("保存成功\n");
}

void load_from_file()
{
    free_all_notes();
    clear_graph();
    FILE* fp=fopen("kb_data.txt","r");
    if(!fp){
        printf("文件不存在\n");
        return;
    }
    char buf[1024];
    int load_edge=0;
    while(fgets(buf,1024,fp)){
        buf[strcspn(buf,"\n")]=0;
        if(strcmp(buf,"[EDGES]")==0){
            load_edge=1;
            continue;
        }
        if(!load_edge && strstr(buf,"NOTE|")){
            int id;
            char title[MAX_TITLE],body[MAX_BODY],tag[MAX_TAG];
            sscanf(buf,"NOTE|%d|%[^|]|%[^|]|%s",&id,title,body,tag);
            Note* newp=(Note*)malloc(sizeof(Note));
            newp->id=id;
            strcpy(newp->title,title);
            strcpy(newp->body,body);
            strcpy(newp->tag,tag);
            newp->next=NULL;
            //尾插
            if(note_head==NULL) note_head=newp;
            else{
                Note* pp=note_head;
                while(pp->next) pp=pp->next;
                pp->next=newp;
            }
            if(id >= next_id) next_id=id+1;
        }
    }
    fclose(fp);
    rebuild_graph();
    printf("加载完成\n");
}

//==================== 主函数‑菜单循环 while(1)+switch ====================
int main(void)
{
    int sel;
    while(1)
    {
        show_menu();
        if(scanf("%d",&sel)!=1){
            //输入字母等非法输入处理，清空缓冲区防死循环
            while(getchar()!='\n');
            printf("输入非法，请输入数字!\n");
            continue;
        }
        switch(sel)
        {
            case 1: add_note(); break;
            case 2: del_note(); break;
            case 3: modify_note(); break;
            case 4: list_notes(); break;
            case 5: search_by_tag(); break;
            case 6: rebuild_graph(); break;
            case 7: bfs_traverse(); break;
            case 8: find_isolate(); break;
            case 9: save_to_file(); break;
            case 10: load_from_file(); break;
            case 0:
                free_all_notes();
                clear_graph();
                printf("程序退出\n");
                return 0;
            default:
                printf("选项超出范围，请重新选择!\n");
        }
    }
    return 0;
}
