# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct NoteNode
{
	int id;
	char title[64];
	char content[256];
	struct NoteNode* next;

}NoteNode;

NoteNode* note_head = NULL;
NoteNode* note_tail = NULL;

typedef enum
{
	STATE_NONE,
	STATE_NOTES,
	STATE_EDGES,
	STATE_CONFIG
	
}ParseState;

void note_append(NoteNode* new_node)
{
	if(note_head == NULL)
	{
		note_head = new_node;
		note_tail = new_node;
	}
	else
	{
		note_tail->next = new_node;
		note_tail = new_node;
		
	}
	new_node->next = NULL;
}

void print_all_notes()
{
	NoteNode* p = note_head;
	while(p != NULL)
	{
		printf("id:%d 标题:%s 内容:%s",p->id,p->title,p->content);
		p = p->next;
	}
	
}

void free_note_list()
{
	NoteNode* p = note_head;
	while(p != NULL)
	{
		NoteNode* temp = p;
		p = p->next;
		free(p);
	}
	note_head = NULL;
	note_tail = NULL;
	
}

void load_notes(const char* filepath)
{
	FILE* fp = fopen(filepath,"r");
	if(fp == NULL)
	{
		printf("警告：文件%s不存在，无笔记加载\n",filepath);
		return;
	}
	char buffer[512];
	ParseState cur_state = STATE_NONE;
	while(fgets(buffer,sizeof(buffer),fp)!=NULL)
	{
		size_t len = strlen(buffer);
		if(len>0 && buffer[len-1]=='\n')
		{
			buffer[len-1]='\0';
			len --;
		}
		
		if(len>0 && buffer[len-1]=='\r')
		{
			buffer[len-1]='\0';
		}
		
		if (strlen(buffer)==0) continue;
		if (buffer[0]=='#') continue;
		
		if(strcmp(buffer,"[NOTES]")==0)
		{
			cur_state = STATE_NOTES;
			continue;
		}
		if(strcmp(buffer,"[EDGES]")==0)
		{
			cur_state = STATE_EDGES;
			continue;
		}
		if(strcmp(buffer,"[CONFIG]")==0)
		{
			cur_state = STATE_CONFIG;
			continue;
		}
		
		if(cur_state == STATE_NOTES)
		{
			char line[512];
			strcpy(line,buffer);
			char* token = strtok(line,",");
			if(token==NULL)continue;
			NoteNode* new_note = (NoteNode*)malloc(sizeof(NoteNode));
			new_note->id = atoi(token);
			
			token = strtok(NULL,",");
			if(token) strcpy(new_note->title,token);
			
			token = strtok(NULL,",");
			if(token) strcpy(new_note->content,token);
			note_append(new_note);
			
		}
		
	}
	fclose(fp);
	printf("加载笔记完成\n");
	
}

int save_notes(const char* filepath)
{
	const char* tmp_path = "data.tmp";
	FILE* fp = fopen(tmp_path,"w");
	if(fp==NULL)
	{
		printf("写入失败：无法创建临时文件\n");
		return 0;
	}
	
	fprintf(fp,"[NOTES]\n");
	NoteNode* p = note_head;
	while(p!=NULL)
	{
		fprintf(fp,"%d,%s,%s",p->id,p->title,p->content);
		p = p->next;
	}
	fprintf(fp,"[EDGES]\n");
	//边链表
	fprintf(fp,"[CONFIG]\nversion=1.0\n");
	fclose(fp);
	remove(filepath);
	rename(tmp_path,filepath);
	printf("保存成功\n");
	return 1;
}

int main(void){
    //1.加载文件
    load_notes("data.txt");
    //2.打印校验链表
    print_all_notes();
    //3.保存
    save_notes("data.txt");
    //清空链表
    free_note_list();
    //4.二次加载，验证数据还在
    load_notes("data.txt");
    print_all_notes();

    free_note_list();
    return 0;
}




















