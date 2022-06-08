#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#define TIME 2 //����һ��ʱ��Ƭ
typedef struct pcb
{
    char name[10];
    int status;//����̬(1)������̬(0)
    int priority;
    int times;
}pcb;
typedef pcb ElemType;
typedef struct node
{
    ElemType data;//data��ʾpcb�ṹ��
    struct node* next;
}PCB;
//�������
typedef struct linkquene
{
    PCB *front;
    PCB *rear;
}LinkQuene;
//��ʼ��pcb����
void initPcbQuene(LinkQuene *pcbquene){
    pcbquene->front = pcbquene->rear = (PCB*)malloc(sizeof(PCB));
    if(!pcbquene->front){
        printf("����ʧ��");
    }
    pcbquene->front->next = NULL;
}

//¼�������Ϣ
void InputProInfo(int n,LinkQuene *pcbquene){//Ҫ¼��Ľ�����
    PCB *pt = NULL;
    int i=0;
    for(i =0;i<n;i++){
        pt = (PCB*)malloc(sizeof(PCB));
        printf("������%d�Ž��̵������Ϣ\n",i+1);
        printf("����:");
        scanf("%s",pt->data.name);
        pt->data.status = 0;//��ʼ����Ϊ����̬
        printf("������:");
        scanf("%d",&pt->data.priority);
        printf("����ʱ��:");
        scanf("%d",&pt->data.times);
        //�����в���
        pt->next = NULL;
        pcbquene->rear->next = pt;
        pcbquene->rear = pt;
    }
}
//�������ȼ�����
void sortProcess(LinkQuene *pcbquene){
    PCB *head = pcbquene->front;
    PCB *temp=NULL;
    PCB *p = NULL;
    ElemType t;//�м����
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

//��ӡ���������Ϣ
void printProcess(LinkQuene *pcbquene){
    PCB *pt = pcbquene->front;
//	if(pcbquene->front->next == pcbquene->rear){
//        pcbquene->front == pcbquene->rear;
//	}
    if(pcbquene->front == pcbquene->rear){
        printf("��������Ϊ��");
    }else{
        printf("-------------------\n");
        printf("��������\n");
        printf("����  ����״̬  ���ȼ�  ʣ������ʱ��\n");
        while(pt->next != NULL){
            if(pt->next->data.status == 1){//�������еĽ���
                printf("%3s   ����̬ %7d %7d\n",pt->next->data.name,pt->next->data.priority,pt->next->data.times);
            }else if(pt->next->data.status == 0){//���������еȴ��ŵĽ���
                printf("%3s   ����̬ %7d %7d\n",pt->next->data.name,pt->next->data.priority,pt->next->data.times);
            }
            pt = pt->next;
        }
        printf("-------------------\n");
    }
}

//��������
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

//���к���(�޸����ȼ���������Ԫ�ز�������β)
void run(LinkQuene *pcbquene){
    PCB *pt = pcbquene->front->next;
    //���׽��̳�����
    ElemType tmp;
    if(pcbquene->front == pcbquene->rear){
        return;
    }
    tmp = pt->data;
    pcbquene->front->next = pt->next;
    //�޸�״̬Ϊ����̬
    tmp.status = 1;
    tmp.priority-=2;
    if(tmp.times-2 <= 0){
        printf("��������:%s  ״̬:����̬  ���ȼ�%d  ʣ������ʱ��%d\n",tmp.name,tmp.priority,0);
    }else{
        printf("��������:%s  ״̬:����̬  ���ȼ�%d  ʣ������ʱ��%d\n",tmp.name,tmp.priority,tmp.times-2);
    }
    if(tmp.times <= 2){//�����ǰ���̵�����ʱ��С��һ��ʱ��Ƭ
        printf("%s���н���!\n",tmp.name);
        if(Traversal(pcbquene) != 0){
            printProcess(pcbquene);
        }
    }else{//����ʱ��Ƭ
        tmp.times-=TIME;
        tmp.status = 0;
        printProcess(pcbquene);
        //��ȥ�Ľ������������β
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
    printf("���������:");
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
