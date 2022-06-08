#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#define TIME 2 //定义一个时间片
typedef struct pcb
{
    char name[10];
    int status;//运行态(1)、就绪态(0)
    int priority;
    int times;
}pcb;
typedef pcb ElemType;
typedef struct node
{
    ElemType data;//data表示pcb结构体
    struct node* next;
}PCB;
//定义队列
typedef struct linkquene
{
    PCB *front;
    PCB *rear;
}LinkQuene;
//初始化pcb队列
void initPcbQuene(LinkQuene *pcbquene){
    pcbquene->front = pcbquene->rear = (PCB*)malloc(sizeof(PCB));
    if(!pcbquene->front){
        printf("申请失败");
    }
    pcbquene->front->next = NULL;
}

//录入进程信息
void InputProInfo(int n,LinkQuene *pcbquene){//要录入的进程数
    PCB *pt = NULL;
    int i=0;
    for(i =0;i<n;i++){
        pt = (PCB*)malloc(sizeof(PCB));
        printf("请输入%d号进程的相关信息\n",i+1);
        printf("名称:");
        scanf("%s",pt->data.name);
        pt->data.status = 0;//初始化都为就绪态
        printf("优先数:");
        scanf("%d",&pt->data.priority);
        printf("运行时间:");
        scanf("%d",&pt->data.times);
        //进队列操作
        pt->next = NULL;
        pcbquene->rear->next = pt;
        pcbquene->rear = pt;
    }
}
//根据优先级排序
void sortProcess(LinkQuene *pcbquene){
    PCB *head = pcbquene->front;
    PCB *temp=NULL;
    PCB *p = NULL;
    ElemType t;//中间变量
    for(temp = head->next;temp->next != NULL;temp =temp->next){
        for(p = head->next;p->next != NULL;p=p->next){
            if(p->data.priority < p->next->data.priority){
                t= p->data;
                p->data = p->next->data;
                p->next->data = t;
            }
        }
    }
}

//打印进程相关信息
void printProcess(LinkQuene *pcbquene){
    PCB *pt = pcbquene->front;
//	if(pcbquene->front->next == pcbquene->rear){
//        pcbquene->front == pcbquene->rear;
//	}
    if(pcbquene->front == pcbquene->rear){
        printf("就绪队列为空");
    }else{
        printf("-------------------\n");
        printf("就绪队列\n");
        printf("名称  运行状态  优先级  剩余运行时间\n");
        while(pt->next != NULL){
            if(pt->next->data.status == 1){//正在运行的进程
                printf("%3s   运行态 %7d %7d\n",pt->next->data.name,pt->next->data.priority,pt->next->data.times);
            }else if(pt->next->data.status == 0){//就绪队列中等待着的进程
                printf("%3s   就绪态 %7d %7d\n",pt->next->data.name,pt->next->data.priority,pt->next->data.times);
            }
            pt = pt->next;
        }
        printf("-------------------\n");
    }
}

//遍历队列
int Traversal(LinkQuene *pcbquene){
    int i=0;
    PCB *pt = pcbquene->front;
    while(pt->next!=NULL){
//        printf("%s",pt->next->data.name);
        i++;
        pt = pt->next;
    }
    return i;
}

//运行函数(修改优先级，将队首元素插入至队尾)
void run(LinkQuene *pcbquene){
    PCB *pt = pcbquene->front->next;
    //队首进程出队列
    ElemType tmp;
    if(pcbquene->front == pcbquene->rear){
        return;
    }
    tmp = pt->data;
    pcbquene->front->next = pt->next;
    //修改状态为运行态
    tmp.status = 1;
    tmp.priority-=2;
    if(tmp.times-2 <= 0){
        printf("进程名称:%s  状态:运行态  优先级%d  剩余运行时间%d\n",tmp.name,tmp.priority,0);
    }else{
        printf("进程名称:%s  状态:运行态  优先级%d  剩余运行时间%d\n",tmp.name,tmp.priority,tmp.times-2);
    }
    if(tmp.times <= 2){//如果当前进程的运行时间小于一个时间片
        printf("%s运行结束!\n",tmp.name);
        if(Traversal(pcbquene) != 0){
            printProcess(pcbquene);
        }
    }else{//大于时间片
        tmp.times-=TIME;
        tmp.status = 0;
        printProcess(pcbquene);
        //出去的进程入队列至队尾
        PCB* curpt = (PCB*)malloc(sizeof(PCB));
        if(curpt){
            curpt->data = tmp;
            curpt->next=NULL;
            pcbquene->rear->next = curpt;
            pcbquene->rear = curpt;
        }else{
            exit(0);
        }
        sortProcess(pcbquene);
    }
}

int main()
{
    LinkQuene pcbquene;
    initPcbQuene(&pcbquene);
    int n;
    printf("输入进程数:");
    scanf("%d",&n);
    InputProInfo(n,&pcbquene);
    sortProcess(&pcbquene);
//    Traversal(&pcbquene);
    printProcess(&pcbquene);
    while(pcbquene.rear != pcbquene.front){
        run(&pcbquene);
    }
    return 0;
}
