#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>
typedef struct program{
    char name[20];
    int running_time;
    int enter_time;
    int priority;
    int done_time;    //用于时间片轮转
    int copyRunning_time;  //用于时间片轮转
    int start_time;
    program* next;
} Program;
typedef struct programQueue{
    program* firstProg;
    program* LastProg;
    int size;
} programQueue;
void Queueinit(programQueue* queue){
    if(queue==NULL){
        return ;
    }
    queue->size = 0;
    queue->LastProg = (program*)malloc(sizeof(program));
    queue->firstProg = queue->LastProg;
}
void print(program pro[],int num){
    for(int i=0;i<num;i++){
        printf("%d ",pro[i].enter_time);
    }
}
void printQueue(programQueue* queue){
    program* p=queue->firstProg->next;
    while(p!=NULL){
        printf("%s ",p->name);
        p=p->next;
    }
    printf("\n");
}
void EnterQueue(programQueue* queue,program* pro){   //加入进程队列
    queue->LastProg->next = (program*)malloc(sizeof(program));
    queue->LastProg = queue->LastProg->next;
    queue->LastProg->enter_time = pro->enter_time;
    memcpy(queue->LastProg->name,pro->name,sizeof(pro->name));
    queue->LastProg->priority = pro->priority;
    queue->LastProg->running_time = pro->running_time;
    queue->LastProg->copyRunning_time = pro->copyRunning_time;
    queue->LastProg->start_time = pro->start_time;
    queue->size++;
}
program* poll(programQueue* queue){
    program* temp = queue->firstProg->next;
    if(temp == queue->LastProg){
        queue->LastProg=queue->firstProg;
        queue->size--;
        return temp;
    }
    queue->firstProg->next = queue->firstProg->next->next;
    queue->size--;
    return temp;
}
void inputProgram(program pro[],int num){
    for(int i=0;i<num;i++){
        program prog ;
        printf("请输入第%d个进程的名字，到达时间，服务时间，优先级\n",i+1);
        scanf("%s",prog.name);
        scanf("%d",&prog.enter_time) ;
        scanf("%d",&prog.running_time);
        prog.copyRunning_time = prog.running_time;
        scanf("%d",&prog.priority);
        pro[i]=prog;
    }
}
void sortWithEnterTime(program pro[],int num){
    for(int i=1;i<num;i++){
        for(int j= 0;j<num-i;j++){
            if(pro[j].enter_time>pro[j+1].enter_time){
                program temp = pro[j];
                pro[j] = pro[j+1];
                pro[j+1] = temp;
            }
        }
    }
}

void FCFS(program pro[],int num){
    printf("进程 到达时间  服务时间 开始时间 完成时间 周转时间 带权周转时间\n");
    sortWithEnterTime(pro,num);    //按照进入顺序排序
    programQueue* queue = (programQueue*)malloc(sizeof(programQueue));
    Queueinit(queue);
    EnterQueue(queue,&pro[0]);
    int time = pro[0].enter_time;
    int pronum=1;    //记录当前的进程
    float sum_T_time = 0,sum_QT_time = 0 ;
    while(queue->size>0){
        program* curpro = poll(queue);   //从进程队列中取出进程
        if(time<curpro->enter_time)
            time =  curpro->enter_time;
        int done_time = time+curpro->running_time;
        int T_time = done_time - curpro->enter_time;
        sum_T_time += T_time;
        float QT_time = T_time / (curpro->running_time+0.0) ;
        sum_QT_time += QT_time;
        for(int tt = time;tt<=done_time&&pronum<num;tt++){    //模拟进程的执行过程

            if(tt>=pro[pronum].enter_time){
                EnterQueue(queue,&pro[pronum]);
                pronum++;
            }
        }
        printf("%s\t%d\t%d\t%d\t%d\t%d\t%.2f\n",curpro->name,curpro->enter_time,curpro->running_time,time,done_time
                ,T_time,QT_time);
        time +=  curpro->running_time;
        if(queue->size==0&&pronum<num){   //防止出现前一个进程执行完到下一个进程到达之间无进程进入
            EnterQueue(queue,&pro[pronum]);
            pronum++;
        }
    }
    printf("平均周转时间为%.2f\t平均带权周转时间为%.2f\n",sum_T_time/(num+0.0),sum_QT_time/(num+0.0));
}
void sortWithLongth(program pro[],int start,int end){
    int len = end - start;
    if(len == 1) return ;
    for(int i=1;i<len;i++){
        for(int j= start;j<end-i;j++){
            if(pro[j].running_time>pro[j+1].running_time){
                program temp = pro[j];
                pro[j] = pro[j+1];
                pro[j+1] = temp;
            }
        }
    }
}
void SJF(program pro[],int num) {
    printf("进程 到达时间  服务时间 开始时间 完成时间 周转时间 带权周转时间\n");
    sortWithEnterTime(pro,num);
    programQueue* queue = (programQueue*)malloc(sizeof(programQueue));;
    Queueinit(queue);
    EnterQueue(queue,&pro[0]);
    int time = pro[0].enter_time;
    int pronum=1;    //记录当前的进程
    float sum_T_time = 0,sum_QT_time = 0;
    while(queue->size>0){
        program* curpro = poll(queue);   //从进程队列中取出进程
        if(time<curpro->enter_time)
            time =  curpro->enter_time;
        int done_time = time+curpro->running_time;
        int T_time = done_time - curpro->enter_time;
        float QT_time = T_time / (curpro->running_time+0.0) ;
        sum_T_time += T_time;
        sum_QT_time += QT_time;
        int pre = pronum;
        for(int tt = time;tt<=done_time&&pronum<num;tt++){    //模拟进程的执行过程
            if(tt>=pro[pronum].enter_time){ // 统计从此任务开始到结束之间有几个进程到达
                pronum++;
            }
        }
        sortWithLongth(pro,pre,pronum);//将到达的进程按照服务时间排序
        for(int i=pre;i<pronum;i++){    //将进程链入队列
            EnterQueue(queue,&pro[i]);
        }
        pre = pronum;
        printf("%s\t%d\t%d\t%d\t%d\t%d\t%.2f\n",curpro->name,curpro->enter_time,curpro->running_time,time,done_time
                ,T_time,QT_time);
        time +=  curpro->running_time;
        if(queue->size==0&&pronum<num){   //防止出现前一个进程执行完到下一个进程到达之间无进程进入
            EnterQueue(queue,&pro[pronum]);
            pronum++;
        }
    }
    printf("平均周转时间为%.2f\t平均带权周转时间为%.2f\n",sum_T_time/(num+0.0),sum_QT_time/num);
}
void sortWithPriority(program pro[],int start,int end){
    int len = end - start;
    if(len == 1) return ;
    for(int i=1;i<len;i++){
        for(int j= start;j<end-i;j++){
            if(pro[j].priority>pro[j+1].priority){
                program temp = pro[j];
                pro[j] = pro[j+1];
                pro[j+1] = temp;
            }
        }
    }
}

void HPF(program pro[],int num){
    printf("进程 到达时间  服务时间 开始时间 完成时间 周转时间 带权周转时间\n");
    sortWithEnterTime(pro,num);
    programQueue* queue = (programQueue*)malloc(sizeof(programQueue));;
    Queueinit(queue);
    EnterQueue(queue,&pro[0]);
    int time = pro[0].enter_time;
    int pronum=1;    //记录当前的进程
    float sum_T_time = 0,sum_QT_time = 0;
    while(queue->size>0){
        program* curpro = poll(queue);   //从进程队列中取出进程
        if(time<curpro->enter_time)
            time =  curpro->enter_time;
        int done_time = time+curpro->running_time;
        int T_time = done_time - curpro->enter_time;
        float QT_time = T_time / (curpro->running_time+0.0) ;
        sum_T_time += T_time;
        sum_QT_time += QT_time;
        int pre = pronum;
        for(int tt = time;tt<=done_time&&pronum<num;tt++){    //模拟进程的执行过程
            if(tt>=pro[pronum].enter_time){ // 统计从此任务开始到结束之间有几个进程到达
                pronum++;
            }
        }
        sortWithPriority(pro,pre,pronum);//将到达的进程按照服务时间排序
        for(int i=pre;i<pronum;i++){    //将进程链入队列
            EnterQueue(queue,&pro[i]);
        }
        pre = pronum;
        printf("%s\t%d\t%d\t%d\t%d\t%d\t%.2f\n",curpro->name,curpro->enter_time,curpro->running_time,time,done_time
                ,T_time,QT_time);
        time +=  curpro->running_time;
        if(queue->size==0&&pronum<num){   //防止出现前一个进程执行完到下一个进程到达之间无进程进入
            EnterQueue(queue,&pro[pronum]);
            pronum++;
        }
    }
    printf("平均周转时间为%.2f\t平均带权周转时间为%.2f\n",sum_T_time/(num+0.0),sum_QT_time/(num+0.0));
}

void choiceMenu(){
    printf("请选择进程调度算法：\n\n");
    printf("1.先来先服务算法\n2.短进程优先算法\n3.高优先级优先\n\n");
}
void menu(){
    int proNum;
    printf("请输入进程的个数：");
    scanf("%d",&proNum);
    program pro[proNum];
    inputProgram(pro,proNum);
    choiceMenu();
    int choice;
    while(1){
        scanf("%d",&choice);
        switch(choice){
            case 1:system("cls");FCFS(pro,proNum);choiceMenu();break;
            case 2:system("cls");SJF(pro,proNum);choiceMenu();break;
            case 3:system("cls");HPF(pro,proNum);choiceMenu();break;
            case 4:return;
        }
    }

}
int main(){
    menu();
    return 0;
}
