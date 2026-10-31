# include<stdio.h>
# include<string.h>
# include<stdlib.h>
# include<ctype.h>

# define MAX_TAGS 10
# define TAG_LEN 32
# define NOTE_TITLE_LEN 64

typedef struct Note
{
	char title[NOTE_TITLE_LEN];
	char tags[MAX_TAGS][TAG_LEN];
	int tagCount;
	struct Note* next;
}Note;

void trim(char* s)
{
	int i,j;
	int len = strlen(s);
	for(i=len-1;i>=0&&isspace((unsigned char)s[i]);i--) s[i]='\0';
	for(i=0;isspace((unsigned char)s[i]);i++)
	if(i>0)
	{
		for(j=0;s[i];j++,i++) s[j]=s[i];
		s[j]='\0';
	}
}

int split_tags(const char* inputStr,char outTags[][TAG_LEN])
{
	for(int i=0;i<MAX_TAGS;i++) outTags[i][0]='\0';
	if(inputStr == NULL||strlen(inputStr)==0) return 0;
	
	char buf[256];
	strcpy(buf,inputStr);
	char* token = strtok(buf,",");
	int count = 0;
	while(token!=NULL&&count<MAX_TAGS)
	{
		trim(token);
		if(strlen(token)>0)
		{
			strncpy(outTags[count],token,TAG_LEN-1);
			outTags[count][TAG_LEN-1]='\0';
			count++;
		}
		token = strtok(NULL,",");
		
	}
	return count;
	
	
}

int find_shared_tags(char tagsA[][TAG_LEN],int countA,
					char tagsB[][TAG_LEN],int countB,
					char outShared[][TAG_LEN]
)
{
	for(int i=0;i<MAX_TAGS;i++) outShared[i][0]='\0';
	int sharedCnt = 0;
	for(int i=0;i<countA;i++)
	{
		for(int j=0;j<countB;j++)
		{
			if(strcmp(tagsA[i],tagsB[j])==0)
			{
				strncpy(outShared[sharedCnt],tagsA[i],TAG_LEN-1);
				outShared[sharedCnt][TAG_LEN-1]='\0';
				sharedCnt++;
				break;
			}
		}
	}
	return sharedCnt;
	
	
}

void search_note_by_tag(Note* head,const char* keyword)
{
	Note* p = head;
	int hit = 0;
	while(p!=NULL)
	{
		for(int i=0;i<p->tagCount;i++)
		{
			if(strcmp(p->tags[i],keyword)==0)
			{
				printf("命中笔记：%s\n",p->title);
				hit = 1;
				break;
			}
		}
		p = p->next;
	}
	if(hit==0) printf("未找到\n");
	
}

Note* create_note(const char* title,const char* tagStr)
{
	Note* n = (Note*)malloc(sizeof(Note));
	strncpy(n->title,title,NOTE_TITLE_LEN-1);
	n->title[NOTE_TITLE_LEN-1] = '\0';
	n->tagCount = split_tags(tagStr,n->tags);
	n->next = NULL;
	return n;
}

int main(void)
{
    // ========= 测试用例：3篇笔记数据 =========
    Note* n1 = create_note("笔记1(指针基础)", "指针,内存");
    Note* n2 = create_note("笔记2(链表入门)", "链表,指针");
    Note* n3 = create_note("笔记3(图算法)", "图,树,算法");
    // 构建链表
    n1->next = n2;
    n2->next = n3;
    Note* head = n1;

    // 测试1：标签拆分
    char t1[MAX_TAGS][TAG_LEN];
    int c1 = split_tags("指针,内存", t1);
    printf("【拆分测试】\"指针,内存\" 标签数=%d\n", c1);
    for(int i = 0; i < c1; i++)
    {
        printf("tag[%d]=%s\n", i, t1[i]);
    }

    // 测试2：共享标签匹配 笔记1 vs 笔记2
    char shared[MAX_TAGS][TAG_LEN];
    int sharedCnt = find_shared_tags(n1->tags, n1->tagCount,
                                     n2->tags, n2->tagCount,
                                     shared);
    printf("\n【共享标签测试】共享数量=%d\n", sharedCnt);
    for(int i = 0; i < sharedCnt; i++)
    {
        printf("共享标签:%s\n", shared[i]);
    }

    // 测试3：标签搜索
    printf("\n【搜索标签：指针】\n");
    search_note_by_tag(head, "指针");

    printf("\n【搜索标签：图】\n");
    search_note_by_tag(head, "图");

    printf("\n【搜索标签：不存在的标签】\n");
    search_note_by_tag(head, "二叉树");

    // =====边界测试=====
    char emptyTags[MAX_TAGS][TAG_LEN];
    int emptyCnt = split_tags("", emptyTags);
    printf("\n【空字符串拆分测试】标签数=%d\n", emptyCnt);

    char spaceTest[MAX_TAGS][TAG_LEN];
    int spCnt = split_tags("指针,, 内存 ", spaceTest);
    printf("【连续逗号+尾部空格】标签数=%d\n", spCnt);
    for(int i = 0; i < spCnt; i++)
    {
        printf("tag[%d] = [%s]\n",i,spaceTest[i]);
    }

    // 释放链表内存
    Note* p = head;
    while(p != NULL)
    {
        Note* temp = p;
        p = p->next;
        free(temp);
    }
    return 0;
}



























