#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>

using namespace std;

/*进程调度的模拟实现*/

/**
 * 定义全局参数
 */
int processCounts;           /*定义进程数*/
int methodChoosen;          /*所选调度算法*/

/**
 * 定义进程控制块的数据结构
 */
typedef struct PCB {
    int num;                        /*进程代号*/
    int comingTimes;                /*到达时间*/
    int serveTime;                  /*服务时间*/
    int priority;                   /*优先级数  数值大优先级大*/
    int startedTimes = 0;           /*记录开始运行时刻*/
    int finishedTimes = 0;          /*记录结束运行时刻*/
    int turnoverTimes = 0;          /*周转时间*/
    int waitingTimes = 0;           /*等待时间*/
    float turnoverTimesWeight = 0;  /*带权周转时间*/
    bool flag = true;               /*可用标识*/
    bool startFlag = false;         /*首次调用 可用标识*/
} PCBblock;

/**
* 函数声明
* 宋保贤40% 赵成叶30% 王长彬30% 整体分工
*/
void InitProcessCount();                             /*输入进程个数*/
void InitPCBData(PCBblock pcBblockList[]);           /*初始化结构体数组*/
void PcbCopy(PCBblock pcBblockList1[], PCBblock pcBblockList2[]);  /*复制一份Pcb结构体副本*/
void PrintInformation(PCBblock pcBblockList[]);     /*打印输入信息*/
void ChooseMethod();                                /*选择调度算法*/
void SJF(PCBblock pcBblockList[]);                  /*短作业优先*/
void FCFS(PCBblock pcBblockList[]);                 /*先来先服务*/
void PS(PCBblock pcBblockList[]);                   /*抢占式*/
void NPS(PCBblock pcBblockList[]);                  /*非抢占式*/
void PrintSchedule(PCBblock pcBblockList[]);        /*打印调度信息*/
void PrintSchedule_Test(PCBblock pcBblockList[]);   /*打印调度信息*/
void PrintSchedule_Flag(PCBblock pcBblockList[]);   /*打印调度信息*/
void PrintSchedule_1(PCBblock pcBblockList[]);      /*打印调度信息*/
void PrintSchedule_Pri(PCBblock pcBblockList[]);    /*打印调度信息*/
void begin(int choice, PCBblock pcBblockList[]);    /*开始函数*/
bool LessSort_comingTime(PCBblock a, PCBblock b);   /*先来先服务排序规则 按照到来时间 升序排序规则 */
bool LessSort_Num(PCBblock a, PCBblock b);          /*按照进程标识号 升序排序规则*/
bool LessSort_StartTime(PCBblock a, PCBblock b);    /*按照开始时间 升序排序规则*/

/**
 * 主方法
 * CodeBy： 宋保贤
 * @return 0
 */
int main() {
    /*  1. 初始化进程数量      */
    InitProcessCount();
    while (processCounts <= 0) {
        InitProcessCount();
    }
    /*  2. 定义进程结构体数组    */
    PCBblock pcBblockList[processCounts];
    /*  3. 初始化进程结构体数组   */
    InitPCBData(pcBblockList);
    /*  4. 打印一次输入的进程结构体数组*/
    PrintInformation(pcBblockList);
    /*  5. 选择调用的算法          */
    ChooseMethod();
    /*  6. 开始运行某个算法         */
    while (methodChoosen != 9999) {
        begin(methodChoosen, pcBblockList);
        ChooseMethod();
    }
    /*  停顿   */
    system("pause");
    return 0;
}


/**
 * 初始化进程数量
 * CodeBy： 宋保贤
 */
void InitProcessCount() {
    cout << "==>请输入进程个数(进程数量>0):";
    cin >> processCounts;
}

/**
 * 初始化PCB数据
 * CodeBy： 王长彬
 * @param pcBblockList
 */
void InitPCBData(PCBblock pcBblockList[]) {
    cout << "***********************************数据初始化开始***********************************" << endl;
    for (int i = 0; i < processCounts; ++i) {
        pcBblockList[i].num = i + 1;
        cout << "==>请输入进程 " << i + 1 << " 的到达时间： ";
        cin >> pcBblockList[i].comingTimes;
        cout << "==>请输入进程 " << i + 1 << " 的服务时间： ";
        cin >> pcBblockList[i].serveTime;
        cout << "==>请输入进程 " << i + 1 << " 的优先级： ";
        cin >> pcBblockList[i].priority;
        cout << endl;
    }
    cout << "***********************************数据初始化结束***********************************" << endl;
}

/**
 * 开始函数
 * CodeBy： 赵成叶
 * @param choice
 * @param pcBblockList
 */
void begin(int choice, PCBblock pcBblockList[]) {
    switch (choice) {
        case 1:
            FCFS(pcBblockList); /*先来先服务*/
            break;
        case 2:
            SJF(pcBblockList);/*短作业优先*/
            break;
        case 3:
            NPS(pcBblockList);/*优先级调度非抢占*/
            break;
        case 4:
            PS(pcBblockList);/*优先级调度抢占式*/
            break;
        default:
            cout << "输入有误，请重新输入！";
            break;
    }
}


/**
 * 打印信息
 * CodeBy： 王长彬
 * @param pcBblockList
 */
void PrintInformation(PCBblock pcBblockList[]) {
    cout << "==>被进程输入的进程数量:  " << processCounts << "  " << endl;
    cout << "==>进程到来的时间展示: ";
    for (int i = 0; i < processCounts; i++) {
        cout << pcBblockList[i].comingTimes << " ";
    }
    cout << endl;
    cout << "==>进程服务的时间展示: ";
    for (int i = 0; i < processCounts; i++) {
        cout << pcBblockList[i].serveTime << " ";
    }
    cout << endl;
    cout << "==>进程优先级的展示:   ";
    for (int i = 0; i < processCounts; i++) {
        cout << pcBblockList[i].priority << " ";
    }
    cout << endl;
}

/**
 * 选择算法类型
 * CodeBy： 赵成叶
 */
void ChooseMethod() {
    cout << "请选择一种调度方式 1-(先来先服务FCFS),  2-(短进程优先SJF),  3-优先级(非抢占),  4-优先级(抢占式)  9999-退出程序: ";
    cin >> methodChoosen;
    cout << endl;
}


/**
 * 先来先服务排序规则 按照到来时间 升序排序规则
 * CodeBy： 赵成叶
 * @param a
 * @param b
 * @return true/false
 */
bool LessSort_comingTime(PCBblock a, PCBblock b) {
    return a.comingTimes < b.comingTimes;
}

/**
 * 按照进程标识号 升序排序规则
 * CodeBy： 王长彬
 * @param a
 * @param b
 * @return  true/false
 */
bool LessSort_Num(PCBblock a, PCBblock b) {
    return a.num < b.num;
}

/**
 * 按照开始时间 升序排序规则
 * CodeBy： 宋保贤
 * @param a
 * @param b
 * @return true/false
 */
bool LessSort_StartTime(PCBblock a, PCBblock b) {
    return a.startedTimes < b.startedTimes;
}

/**
 * 先来先服务算法
 * CodeBy： 赵成叶
 * @param pcBblockList
 */
void FCFS(PCBblock pcBblockList[]) {
    /*  定义一个进程结构体数组作为副本     */
    PCBblock pcBblockList_copy[processCounts];
    /*  调用复制函数  */
    PcbCopy(pcBblockList, pcBblockList_copy);
    /*  设置当前时间  */
    int current = 0;
    /*  调用排序函数  */
    sort(pcBblockList_copy, pcBblockList_copy + processCounts, LessSort_comingTime);
    /* 遍历查找最早到达的进程*/
    for (int i = 0; i < processCounts; i++) {
        /*  如果当前进程到来时间比当前时间大，设置最早来得为当前时间  */
        if (pcBblockList_copy[i].comingTimes > current) {
            current = pcBblockList_copy[i].comingTimes;
        }
        pcBblockList_copy[i].flag = false;
        /*  开始时间 = 当前时间  */
        pcBblockList_copy[i].startedTimes = current;
        /*  完成时间 = 开始时间 + 服务时间   */
        pcBblockList_copy[i].finishedTimes = pcBblockList_copy[i].startedTimes + pcBblockList_copy[i].serveTime;
        /*  等待时间 = 开始时间 - 到来时间     */
        pcBblockList_copy[i].waitingTimes = pcBblockList_copy[i].startedTimes - pcBblockList_copy[i].comingTimes;
        /*  当前时间 = 完成时间     */
        current = pcBblockList_copy[i].finishedTimes;
        /*  周转时间 = 服务时间 + 等待时间      */
        pcBblockList_copy[i].turnoverTimes = pcBblockList_copy[i].serveTime + pcBblockList_copy[i].waitingTimes;
        /*  带权周转时间 = 周转时间 / 服务时间     */
        pcBblockList_copy[i].turnoverTimesWeight = (float) pcBblockList_copy[i].turnoverTimes / (float) pcBblockList_copy[i].serveTime;


        //PrintSchedule_Test(pcBblockList_copy); /*打印调度测试过程*/


    }
    /*  打印进程相关信息    */
    sort(pcBblockList_copy, pcBblockList_copy + processCounts, LessSort_StartTime);
    PrintSchedule(pcBblockList_copy);
    cout<<endl;cout<<endl;
}

/**
 * 短进程优先办法
 * CodeBy： 王长彬
 * @param pcBblockList
 */
void SJF(PCBblock pcBblockList[]) {
    /*  定义一个进程结构体数组作为副本     */
    PCBblock pcBblockList_copy[processCounts];
    /*  调用复制函数  */
    PcbCopy(pcBblockList, pcBblockList_copy);
    /*  调用排序函数  */
    sort(pcBblockList_copy, pcBblockList_copy + processCounts, LessSort_comingTime);
    /*  设置最早来的进程的到来时间为当前时间  */
    int current = pcBblockList_copy[0].comingTimes;
    /* 遍历查找最早到达的进程  */
    for (int j = 0; j < processCounts; j++) {
        /*  定义索引index 记录最短服务时间进程的索引  定义比较进程服务时间的参数 SerNum */
        int index = 0, serNum = 9999;
        /*  找到最短服务时间进程的索引index  */
        for (int i = 1; i < processCounts; ++i) {
            /*  首次到来的进程直接调度     */
            if (j == 0) {
                break;
            }
            /*  如果当前进程没有被调度 且 已经到来 */
            if (pcBblockList_copy[i].flag && pcBblockList_copy[i].comingTimes <= current) {
                if (pcBblockList_copy[i].serveTime < serNum) {
                    serNum = pcBblockList_copy[i].serveTime;
                    index = i;
                }
            }
        }
        /*  设置当前进程已经调度设置flag=flase  */
        pcBblockList_copy[index].flag = false;
        /*  开始时间 = 当前时间  */
        pcBblockList_copy[index].startedTimes = current;
        /*  完成时间 = 开始时间 + 服务时间   */
        pcBblockList_copy[index].finishedTimes = pcBblockList_copy[index].startedTimes + pcBblockList_copy[index].serveTime;
        /*  等待时间 = 开始时间 - 到来时间     */
        pcBblockList_copy[index].waitingTimes = pcBblockList_copy[index].startedTimes - pcBblockList_copy[index].comingTimes;
        /*  当前时间 = 完成时间     */
        current = pcBblockList_copy[index].finishedTimes;
        /*  周转时间 = 服务时间 + 等待时间      */
        pcBblockList_copy[index].turnoverTimes =  pcBblockList_copy[index].serveTime + pcBblockList_copy[index].waitingTimes;
        /*  带权周转时间 = 周转时间 / 服务时间     */
        pcBblockList_copy[index].turnoverTimesWeight =  (float) pcBblockList_copy[index].turnoverTimes / (float) pcBblockList_copy[index].serveTime;


        //PrintSchedule_Test(pcBblockList_copy); /*打印调度测试过程*/


    }
    /*  打印进程相关信息    */
    sort(pcBblockList_copy, pcBblockList_copy + processCounts, LessSort_StartTime);
    PrintSchedule(pcBblockList_copy);
    cout<<endl;cout<<endl;
}

/**
 * PCB复制一份结构体数组副本
 * CodeBy： 宋保贤
 * @param pcBblockList1
 * @param pcBblockList2
 */
void PcbCopy(PCBblock pcBblockList1[], PCBblock pcBblockList2[]) {
    for (int i = 0; i < processCounts; ++i) {
        pcBblockList2[i].num = pcBblockList1[i].num;
        pcBblockList2[i].comingTimes = pcBblockList1[i].comingTimes;
        pcBblockList2[i].serveTime = pcBblockList1[i].serveTime;
        pcBblockList2[i].priority = pcBblockList1[i].priority;
    }
}

/**
 * 优先级调度算法(非抢占式)  优先级数大 的 优先级高
 * CodeBy： 宋保贤
 * @param pcBblockList
 */
void NPS(PCBblock pcBblockList[]) {
    /*  1. 定义一个进程结构体数组作为副本     */
    PCBblock pcBblockList_copy[processCounts];
    /*  2. 调用复制函数  */
    PcbCopy(pcBblockList, pcBblockList_copy);
    /*  3. 调用排序函数  */
    sort(pcBblockList_copy, pcBblockList_copy + processCounts, LessSort_comingTime);
    /*  4. 设置最早来的进程的到来时间为当前时间  */
    int current = pcBblockList_copy[0].comingTimes;
    /*  5. 遍历查找最早到达的进程  */
    for (int j = 0; j < processCounts; j++) {
        /*  定义索引index 记录最高优先级进程的索引  定义比较进程优先级的参数 priNum */
        int index = 0, priNum = 0;
        /*  找到最高优先级进程的索引index  */
        for (int i = 0; i < processCounts; ++i) {
            /*  如果当前进程没有被调度 且 已经到来 */
            if (pcBblockList_copy[i].flag && pcBblockList_copy[i].comingTimes <= current) {
                if (pcBblockList_copy[i].priority > priNum) {
                    priNum = pcBblockList_copy[i].priority;
                    index = i;
                }
            }
        }
        /*  设置当前进程已经调度设置flag=flase  */
        pcBblockList_copy[index].flag = false;
        /*  开始时间 = 当前时间  */
        pcBblockList_copy[index].startedTimes = current;
        /*  完成时间 = 开始时间 + 服务时间   */
        pcBblockList_copy[index].finishedTimes = pcBblockList_copy[index].startedTimes + pcBblockList_copy[index].serveTime;
        /*  等待时间 = 开始时间 - 到来时间     */
        pcBblockList_copy[index].waitingTimes = pcBblockList_copy[index].startedTimes - pcBblockList_copy[index].comingTimes;
        /*  当前时间 = 完成时间     */
        current = pcBblockList_copy[index].finishedTimes;
        /*  周转时间 = 服务时间 + 等待时间      */
        pcBblockList_copy[index].turnoverTimes = pcBblockList_copy[index].serveTime + pcBblockList_copy[index].waitingTimes;
        /*  带权周转时间 = 周转时间 / 服务时间     */
        pcBblockList_copy[index].turnoverTimesWeight = (float) pcBblockList_copy[index].turnoverTimes / (float) pcBblockList_copy[index].serveTime;


        //PrintSchedule_Test(pcBblockList_copy); /*打印调度测试过程*/


    }
    sort(pcBblockList_copy, pcBblockList_copy + processCounts, LessSort_StartTime);
    PrintSchedule_Pri(pcBblockList_copy);
    cout<<endl;cout<<endl;
}


 /**
  * 优先级调度算法(抢占式)
  * CodeBy： 宋保贤
  * @param pcBblockList
  */
void PS(PCBblock pcBblockList[]) {
     /*  定义一个进程结构体数组作为副本     */
     PCBblock pcBblockList_copy[processCounts], pcBblockList1[processCounts];
     /*  调用复制函数  复制两份 因为其中的一份服务时间会减小到0 对数据进行了修改 */
     PcbCopy(pcBblockList, pcBblockList_copy);
     PcbCopy(pcBblockList, pcBblockList1);
     /*  调用排序函数  */
     sort(pcBblockList_copy, pcBblockList_copy + processCounts, LessSort_comingTime);
     sort(pcBblockList1, pcBblockList1 + processCounts, LessSort_comingTime);
     /*  定义总时间 得出循环次数 */
     int sumTime = 0;
     int serveTime=0;
     /* 将所有的进程的服务时间求和 */
     for (int i = 0; i < processCounts; ++i) {
         serveTime+=pcBblockList_copy[i].serveTime;
     }
     /* 再加上最后一个进程到来时间 得到最大循环次数 */
     sumTime = serveTime+pcBblockList_copy[processCounts-1].comingTimes;
     /*  设置最早来的进程的到来时间为当前时间  */
     int current = pcBblockList1[0].comingTimes;
     /*  index代表当前调度进程的索引  local代表上一个调度进程的索引*/
     int index = 0, local = 0;
     /* 遍历时间循环  */
     cout << "***********************************进程调度信息打印显示***********************************" << endl;
     for (; current <=sumTime; ++current) {
         int priNum = 0;
         for (int i = 0; i < processCounts; ++i) {
             /*  如果当前进程没有被调度 且 已经到来 */
             if (pcBblockList1[i].flag && pcBblockList1[i].comingTimes <= current) {
                 /*  判断当前到来进程的优先级是否比当前运行的进程的优先级大 */
                 if (pcBblockList1[i].priority > priNum) {
                     priNum = pcBblockList1[i].priority;
                     /* 发生进程调度 index记录当前要调度的进程 */
                     index = i;
                     /*  判断当前要调度进程是否是第一次调度  */
                     if (!pcBblockList_copy[index].startFlag) {
                         pcBblockList_copy[index].startedTimes = current;
                         pcBblockList_copy[index].finishedTimes = current;
                         pcBblockList1[index].startedTimes = current;
                         pcBblockList1[index].finishedTimes = current;
                         pcBblockList_copy[index].startFlag = true;
                     }
                 }
             }
         }
         /*  判断当前时间运行进程是否等于之前运行的进程   */
         if (local != index) {
             if (local != processCounts - 1) {
                 cout << "进程" << pcBblockList_copy[local].num << "->";
             }
             /*  把当前的进程的索引给local  local记录上一个调度的进程索引  */
             local = index;//代表打印过
         }
         /*  设置结束时间  进程未被完全服务完毕  */
         if(pcBblockList1[index].flag){
             /*不断修改它的完成时间，服务时间*/
             pcBblockList_copy[index].finishedTimes = current + 1;
             pcBblockList1[index].finishedTimes = current + 1;
             pcBblockList1[index].serveTime -= 1;
             /*  判断当前进程是否被服务完成 */
             if (pcBblockList1[index].serveTime == 0) {
                 pcBblockList1[index].flag = false;
             }


             //PrintSchedule_Test(pcBblockList1);/*测试打印*/


         }

     }
     cout << "进程" << pcBblockList_copy[index].num << "" << endl;
     for (int index = 0; index < processCounts; ++index) {
         /*  等待时间 = 结束时间 - 到来时间 - 服务时间   */
         pcBblockList_copy[index].waitingTimes = pcBblockList_copy[index].finishedTimes - pcBblockList_copy[index].comingTimes - pcBblockList_copy[index].serveTime;
         /*  周转时间 = 服务时间 + 等待时间 */
         pcBblockList_copy[index].turnoverTimes = pcBblockList_copy[index].serveTime + pcBblockList_copy[index].waitingTimes;
         /*  带权周转时间 = 周转时间 / 服务时间 */
         pcBblockList_copy[index].turnoverTimesWeight = (float) pcBblockList_copy[index].turnoverTimes / (float) pcBblockList_copy[index].serveTime;
     }
     sort(pcBblockList_copy, pcBblockList_copy + processCounts, LessSort_Num);
     PrintSchedule_1(pcBblockList_copy);
     cout<<endl;cout<<endl;
}

/**
 * 打印调度信息(含有可用标识)
 * CodeBy： 赵成叶
 * @param pcBblockList
 */
void PrintSchedule_Flag(PCBblock pcBblockList[]) {
    cout << "***********************************进程调度信息打印显示***********************************" << endl;
    cout << "进程(ID)    到达时间    服务时间        优先级    等待时间    开始时间    完成时间    周转时间    带权周转时间    flag" << endl;
    for (int i = 0; i < processCounts; i++) {
        printf("%5d    %6d    %7d    %7d    %7d    %7d    %7d    %7d    %.2f      %d\n", pcBblockList[i].num,
               pcBblockList[i].comingTimes, pcBblockList[i].serveTime, pcBblockList[i].priority,
               pcBblockList[i].waitingTimes, pcBblockList[i].startedTimes, pcBblockList[i].finishedTimes,
               pcBblockList[i].turnoverTimes, pcBblockList[i].turnoverTimesWeight, pcBblockList[i].flag);
    }
    /*平均周转时间和总周转时间*/
    float average_turnover_time, sum_turnover_time = 0.0;
    /*平均带权周转时间和总带权周转时间*/
    float average_turnover_time_weight, sum_turnover_time_weight = 0;
    for (int i = 0; i < processCounts; i++) {
        sum_turnover_time += pcBblockList[i].turnoverTimes;
        sum_turnover_time_weight += pcBblockList[i].turnoverTimesWeight;
    }
    average_turnover_time = sum_turnover_time / processCounts;
    average_turnover_time_weight = sum_turnover_time_weight / processCounts;
    cout << "==>平均周转时间：" << average_turnover_time << " " << endl;
    cout << "==>平均带权周转时间：" << average_turnover_time_weight << " " << endl;
    cout << "***********************************进程调度信息打印结束***********************************" << endl;
}

/**
 * 打印调度信息
 * @param pcBblockList
 */
void PrintSchedule_1(PCBblock pcBblockList[]) {
    cout << "进程(ID)    到达时间    服务时间        优先级    等待时间    开始时间    完成时间    周转时间    带权周转时间" << endl;
    for (int i = 0; i < processCounts; i++) {
        printf("%5d    %6d    %7d    %7d    %7d    %7d    %7d    %7d         %.2f\n", pcBblockList[i].num,
               pcBblockList[i].comingTimes, pcBblockList[i].serveTime, pcBblockList[i].priority,
               pcBblockList[i].waitingTimes, pcBblockList[i].startedTimes, pcBblockList[i].finishedTimes,
               pcBblockList[i].turnoverTimes, pcBblockList[i].turnoverTimesWeight);
    }
    /*平均周转时间和总周转时间*/
    float average_turnover_time, sum_turnover_time = 0.0;
    /*平均带权周转时间和总带权周转时间*/
    float average_turnover_time_weight, sum_turnover_time_weight = 0;
    for (int i = 0; i < processCounts; i++) {
        sum_turnover_time += pcBblockList[i].turnoverTimes;
        sum_turnover_time_weight += pcBblockList[i].turnoverTimesWeight;
    }
    average_turnover_time = sum_turnover_time / processCounts;
    average_turnover_time_weight = sum_turnover_time_weight / processCounts;
    cout << "==>平均周转时间：" << average_turnover_time << " " << endl;
    cout << "==>平均带权周转时间：" << average_turnover_time_weight << " " << endl;
    cout << "***********************************进程调度信息打印结束***********************************" << endl;
}

/**
 * 打印调度信息(test用)  0调度了，1未调度
 * CodeBy： 宋保贤
 * @param pcBblockList
 */
void PrintSchedule_Test(PCBblock pcBblockList[]) {
    cout << "---------------------------------进程调度过程测试打印显示---------------------------------" << endl;
    cout << "进程(ID)    到达时间    服务时间        优先级    等待时间    开始时间    完成时间    flag(0: 完成 , 1: 未完成)" << endl;
    for (int i = 0; i < processCounts; i++) {
        printf("%5d    %6d    %7d    %7d    %7d    %7d    %7d          %d\n", pcBblockList[i].num,
               pcBblockList[i].comingTimes, pcBblockList[i].serveTime, pcBblockList[i].priority,
               pcBblockList[i].waitingTimes, pcBblockList[i].startedTimes, pcBblockList[i].finishedTimes,
               pcBblockList[i].flag);
    }
    cout << "---------------------------------进程调度过程测试打印结束---------------------------------" << endl;
}



/**
 * 打印调度信息(含有优先级)
 * CodeBy： 王长彬
 * @param pcBblockList
 */
void PrintSchedule_Pri(PCBblock pcBblockList[]) {
    cout << "***********************************进程调度信息打印显示***********************************" << endl;
    cout << "==>调度过程: ";
    for (int i = 0; i < processCounts; ++i) {
        if (i != processCounts - 1) {
            cout << "进程" << pcBblockList[i].num << "->";
        } else {
            cout << "进程" << pcBblockList[i].num << "" << endl;
        }
    }
    cout << "进程(ID)    到达时间    服务时间     优先级     等待时间     开始时间    完成时间    周转时间    带权周转时间" << endl;
    for (int i = 0; i < processCounts; i++) {
        printf("%5d    %6d    %7d    %7d    %7d    %7d    %7d    %7d         %.2f\n", pcBblockList[i].num,
               pcBblockList[i].comingTimes, pcBblockList[i].serveTime, pcBblockList[i].priority,
               pcBblockList[i].waitingTimes, pcBblockList[i].startedTimes, pcBblockList[i].finishedTimes,
               pcBblockList[i].turnoverTimes, pcBblockList[i].turnoverTimesWeight);
    }
    /*平均周转时间和总周转时间*/
    float average_turnover_time, sum_turnover_time = 0.0;
    /*平均带权周转时间和总带权周转时间*/
    float average_turnover_time_weight, sum_turnover_time_weight = 0;
    for (int i = 0; i < processCounts; i++) {
        sum_turnover_time += pcBblockList[i].turnoverTimes;
        sum_turnover_time_weight += pcBblockList[i].turnoverTimesWeight;
    }
    average_turnover_time = sum_turnover_time / processCounts;
    average_turnover_time_weight = sum_turnover_time_weight / processCounts;
    cout << "==>平均周转时间：" << average_turnover_time << " " << endl;
    cout << "==>平均带权周转时间：" << average_turnover_time_weight << " " << endl;
    cout << "***********************************进程调度信息打印结束***********************************" << endl;
}

/**
 * 打印调度信息(基本打印)
 * CodeBy： 宋保贤
 * @param pcBblockList
 */
void PrintSchedule(PCBblock pcBblockList[]) {
    cout << "***********************************进程调度信息打印显示***********************************" << endl;
    cout << "==>调度过程: ";
    for (int i = 0; i < processCounts; ++i) {
        if (i != processCounts - 1) {
            cout << "进程" << pcBblockList[i].num << "->";
        } else {
            cout << "进程" << pcBblockList[i].num << "" << endl;
        }
    }
    cout << "进程(ID)    到达时间    服务时间    等待时间    开始时间    完成时间    周转时间    带权周转时间" << endl;
    for (int i = 0; i < processCounts; i++) {
        printf("%5d    %6d    %7d    %7d    %7d    %7d    %7d      %.2f\n", pcBblockList[i].num,
               pcBblockList[i].comingTimes, pcBblockList[i].serveTime, pcBblockList[i].waitingTimes,
               pcBblockList[i].startedTimes, pcBblockList[i].finishedTimes, pcBblockList[i].turnoverTimes,
               pcBblockList[i].turnoverTimesWeight);
    }
    /*平均周转时间和总周转时间*/
    float average_turnover_time, sum_turnover_time = 0.0;
    /*平均带权周转时间和总带权周转时间*/
    float average_turnover_time_weight, sum_turnover_time_weight = 0;
    for (int i = 0; i < processCounts; i++) {
        sum_turnover_time += pcBblockList[i].turnoverTimes;
        sum_turnover_time_weight += pcBblockList[i].turnoverTimesWeight;
    }
    average_turnover_time = sum_turnover_time / processCounts;
    average_turnover_time_weight = sum_turnover_time_weight / processCounts;
    cout << "==>平均周转时间：" << average_turnover_time << " " << endl;
    cout << "==>平均带权周转时间：" << average_turnover_time_weight << " " << endl;
    cout << "***********************************进程调度信息打印结束***********************************" << endl;
}

