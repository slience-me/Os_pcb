#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip> //格式化输出

#define MAX_DURANCE 1e6
using namespace std;

/**
 * 全局参数
 */
int processCounts;            //进程数
int *comingTimes;            //达到时间
int *serveTimes;             //服务时间
int *priority;               //对应优先级
int *finishedTimes;          //完成时间
int *turnoverTimes;          //周转时间
int *waitingTimes;           //等待时间
float *turnoverTimesWeight; //带权周转时间
int methodChoosen;           //所选调度算法

/**
 * 定义进程控制块的数据结构
 */
struct PCB {
    int number;//进程代号
    int arriveTime;//到达时间
    int serveTime;//服务时间
    int priority;//优先级
    int finishTime = 0;//记录结束运行时刻
};

/**
* 函数声明
*/
void InitProcessCount();    //输入进程个数
void InitComingTime();   //输入进程到达时间
void InitServeTime();    //输入进程服务时间
void InitPriority();    //输入进程优先级
void PrintInformation();     //打印输入信息
void ChooseMethod();       //选择调度算法
void Initialize();          //数组初始化
void SJF();                 //短作业优先
void FCFS();                //先来先服务
void PS();                //抢占式
void NPS();                //非抢占式
void PrintSchedule();   //打印调度信息
void PrintSchedule_pri();   //打印调度信息
void exchange(int x[], int i, int j);

/**
 * 主方法
 * @return
 */
int main() {
    // 1. 输入进程个数
    InitProcessCount();
    // 2. 输入进程到达时间
    InitComingTime();
    // 3. 输入进程服务时间
    InitServeTime();
    // 4. 打印输入信息
    PrintInformation();
    // 5. 数组初始化
    Initialize();
    // 6. 选择调度算法
    ChooseMethod();
    switch (methodChoosen) {
        case 1:
            FCFS();
            PrintSchedule();
            break;
        case 2:
            SJF();
            PrintSchedule();
            break;
        case 3:
            //输入进程优先级
            InitPriority();
            NPS();
            PrintSchedule_pri();
            break;
        default:
            //输入进程优先级
            InitPriority();
            PS();
            PrintSchedule();
            break;
    }
    system("pause");
    return 0;
}

/**
 * 初始化进程数量
 */
void InitProcessCount() {
    cout << "==>请输入进程个数:";
    cin >> processCounts;
}

/**
 * 初始化到来时间
 */
void InitComingTime() {
    comingTimes = new int[processCounts];
    cout << "==>请输入各个进程的到达时间:";
    for (int i = 0; i < processCounts; i++) {
        cin >> comingTimes[i];
    }
}

/**
 * 初始化服务时间
 */
void InitServeTime() {
    serveTimes = new int[processCounts];
    cout << "==>请输入各个进程的要求服务时间:";
    for (int i = 0; i < processCounts; i++) {
        cin >> serveTimes[i];
    }
}

/**
 * 初始化优先级
 */
void InitPriority() {
    priority = new int[processCounts];
    cout << "==>请输入各个进程的优先级:";
    for (int i = 0; i < processCounts; i++) {
        cin >> priority[i];
    }
}

/**
 * 打印信息
 */
void PrintInformation() {
    cout << "==>被进程输入的进程数量: [ " << processCounts << " ] " << endl;
    cout << "==>进程到来的时间展示: ";
    for (int i = 0; i < processCounts; i++) {
        cout << comingTimes[i] << " ";
    }
    cout << endl;
    cout << "==>进程服务的时间展示: ";
    for (int i = 0; i < processCounts; i++) {
        cout << serveTimes[i] << " ";
    }
    cout << endl;
}

/**
 * 选择算法类型
 */
void ChooseMethod() {
    cout << "请选择一种调度方式[ 1-(先来先服务FCFS),  2-(短进程优先SJF),  3-优先级(非抢占),  4-优先级(抢占式)]: ";
    cin >> methodChoosen;
    cout << endl;
}

/**
 * 先来先服务算法
 */
void FCFS() {
    // 1. 设置初始时间0
    int current = 0;
    // 2. 复制一份到达时间的副本copy_comingTimes[]
    int copy_comingTimes[processCounts];
    for (int i = 0; i < processCounts; i++) {
        copy_comingTimes[i] = comingTimes[i];
    }
    // 3. 遍历查找最早到达的进程
    for (int j = 0; j < processCounts; j++) {
        // 先设置默认最早到达的进程earliest是0，默认第1个进程到来时间最早
        int earliestProcess = 0, min = copy_comingTimes[0];
        //先找到当前最先到达的进程，需要使用到达时间的副本来找
        //设min为所有进程中到来最早的时间
        for (int i = 1; i < processCounts; i++) {
            if (copy_comingTimes[i] < min) {
                //遍历找到到来最早的时间与进程
                min = copy_comingTimes[i];
                earliestProcess = i;
            }
        }
        //找到后，设置进程对应的时间禁用
        copy_comingTimes[earliestProcess] = MAX_DURANCE;
        //上一个进程执行完时有可能下个进程还没到达
        if (comingTimes[earliestProcess] > current) {
            current = comingTimes[earliestProcess];
        }
        //更新其完成时间 = 到来时间+服务时间
        finishedTimes[earliestProcess] = current + serveTimes[earliestProcess];
        //等待时间 等待时间 = 当前时间 - 到来时间
        waitingTimes[earliestProcess] = current - comingTimes[earliestProcess];
        //更新当前时间 当前时间 = 当前时间 + 服务时间
        current += serveTimes[earliestProcess];
        //更新周转时间 周转时间 = 服务时间 + 等待时间
        turnoverTimes[earliestProcess] = serveTimes[earliestProcess] + waitingTimes[earliestProcess];
        //更新带权周转时间 带权周转时间 = 周转时间 /服务时间
        turnoverTimesWeight[earliestProcess] =
                (float) turnoverTimes[earliestProcess] / (float) serveTimes[earliestProcess];
    }
}

/**
 * 短进程优先办法
 */
void SJF() {
    // 1. 设置初始时间0
    int current = 0;
    // 2. 复制一份服务时间copy_serveTimes[],到达时间的副本copy_comingTimes[]
    int copy_serveTimes[processCounts], copy_comingTimes[processCounts];
    for (int i = 0; i < processCounts; i++) {
        copy_serveTimes[i] = serveTimes[i];
    }
    for (int i = 0; i < processCounts; i++) {
        copy_comingTimes[i] = comingTimes[i];
    }
    //3. flag 标识当前时间下有无已经到达的进程 1有0无
    int flag = 0;
    for (int i = 0; i < processCounts; i++) {
        // min_p: 当前调度进程的下标
        // min: 当前调度进程的服务时间
        // early:当前调度进程的到达时间
        int min_p = 0, min = copy_serveTimes[0], early = copy_comingTimes[0];
        for (int j = 1; j < processCounts; j++) {
            //进程到来而且该进程服务时间小
            if (copy_comingTimes[j] <= current && copy_serveTimes[j] < min) {
                flag = 1;
                min = copy_serveTimes[j];
                min_p = j;
            }
        }
        //若当前时间无进程到达，则选择即将最早到达的那个进程
        if (flag == 0) {
            for (int m = 1; m < processCounts; m++) {
                if (copy_comingTimes[m] < early) {
                    early = copy_comingTimes[m];
                    min_p = m;
                    current = early;
                }
            }
        }
        // 设置到来时间 服务时间为 禁用
        copy_comingTimes[min_p] = MAX_DURANCE;
        copy_serveTimes[min_p] = MAX_DURANCE;
        //更新其完成时间 = 到来时间+服务时间
        finishedTimes[min_p] = current + serveTimes[min_p];
        //等待时间 等待时间 = 当前时间 - 到来时间
        waitingTimes[min_p] = current - comingTimes[min_p];
        //更新周转时间 周转时间 = 服务时间 + 等待时间
        current = finishedTimes[min_p];
        //更新周转时间 周转时间 = 服务时间 + 等待时间
        turnoverTimes[min_p] = waitingTimes[min_p] + serveTimes[min_p];
        //更新带权周转时间 带权周转时间 = 周转时间 /服务时间
        turnoverTimesWeight[min_p] = (float) turnoverTimes[min_p] / (float) serveTimes[min_p];
    }
}

/**
 * 优先级调度算法(非抢占式)
 */
void NPS() {
    //找到进程执行的序列 work_list
    int copy_comingTimes[processCounts];
    for (int i = 0; i < processCounts; i++) {
        copy_comingTimes[i] = comingTimes[i];
    }
    int work_list[processCounts];
    for (int i = 0; i < processCounts; i++) {//元素代表执行的进程在p_list中的下标，下标代表执行顺序
        work_list[i] = i;
    }
    //先按到达时间排序
    int temp = 0;
    for (int i = 0; i < processCounts - 1; i++) {
        for (int j = 0; j < processCounts - i - 1; j++) {
            if (copy_comingTimes[j] > copy_comingTimes[j + 1]) {
                temp = copy_comingTimes[j];
                copy_comingTimes[j] = copy_comingTimes[j + 1];
                copy_comingTimes[j + 1] = temp;
                temp = work_list[j];
                work_list[j] = work_list[j + 1];
                work_list[j + 1] = temp;
            }
        }
    }
    for (int i = 0; i < processCounts - 1; i++) {//再按优先级排序
        for (int j = 0; j < processCounts; j++) {
            if (i == j) {
                break;
            }
            if (copy_comingTimes[i] == copy_comingTimes[j] && priority[j] < priority[j + 1]) {
                temp = copy_comingTimes[j];
                copy_comingTimes[j] = copy_comingTimes[j + 1];
                copy_comingTimes[j + 1] = temp;
                temp = work_list[j];
                work_list[j] = work_list[j + 1];
                work_list[j + 1] = temp;
            }
        }
    }
    int current = comingTimes[work_list[0]];
    // 3. 遍历查找最早到达的进程
    for (int j = 0; j < processCounts; j++) {
        // 先设置默认最早到达的进程earliest是0，默认第1个进程到来时间最早 priorCur当前进程优先级
        //更新其完成时间 = 到来时间+服务时间
        finishedTimes[work_list[j]] = current + serveTimes[work_list[j]];
        //等待时间 等待时间 = 当前时间 - 到来时间
        waitingTimes[work_list[j]] = current - comingTimes[work_list[j]];
        //更新当前时间 当前时间 = 当前时间 + 服务时间
        current += serveTimes[work_list[j]];
        //更新周转时间 周转时间 = 服务时间 + 等待时间
        turnoverTimes[work_list[j]] = serveTimes[work_list[j]] + waitingTimes[work_list[j]];
        //更新带权周转时间 带权周转时间 = 周转时间 /服务时间
        turnoverTimesWeight[work_list[j]] =
                (float) turnoverTimes[work_list[j]] / (float) serveTimes[work_list[j]];
    }
}

/**
 * 交换函数
 */
void exchange(int x[], int i, int j) {
    int temp = x[i];
    x[i] = x[j];
    x[j] = temp;
}

/**
 * 优先级调度算法(抢占式)
 */
void PS() {
//    int sum = 0;
//    int copy_serveTimes[processCounts], copy_comingTimes[processCounts];
//    for (int i = 0; i < processCounts; i++) {
//        copy_serveTimes[i] = serveTimes[i];
//    }
//    for (int i = 0; i < processCounts; i++) {
//        copy_comingTimes[i] = comingTimes[i];
//    }
//    int work_list[processCounts];
//    for (int i = 0; i < processCounts; i++) {//元素代表执行的进程在p_list中的下标，下标代表执行顺序
//        work_list[i] = i;
//    }
//    //先按到达时间排序
//    int temp = 0;
//    for (int i = 0; i < processCounts - 1; i++) {
//        for (int j = 0; j < processCounts - i - 1; j++) {
//            if (copy_comingTimes[j] > copy_comingTimes[j + 1]) {
//                temp = copy_comingTimes[j];
//                copy_comingTimes[j] = copy_comingTimes[j + 1];
//                copy_comingTimes[j + 1] = temp;
//                temp = work_list[j];
//                work_list[j] = work_list[j + 1];
//                work_list[j + 1] = temp;
//            }
//        }
//    }
//    for (int i = 0; i < processCounts; ++i) {
//        sum += serveTimes[i];
//    }
//    int current_pro=0;
//    for (int current = copy_comingTimes[work_list[current_pro]]; current < sum; ++current) {
//        cout<<"current="<<current<<endl;
//        if (current <= copy_comingTimes[work_list[current_pro + 1]]) {
//            copy_serveTimes[work_list[current_pro]] -= 1;
//            if (copy_serveTimes[work_list[current_pro + 1]]<=0){
//                copy_comingTimes[work_list[current_pro + 1]] = -1;
//            }
//        } else {
//            if (priority[work_list[current_pro]] >= priority[work_list[current_pro + 1]]) {
//                copy_serveTimes[work_list[current_pro + 1]] -= 1;
//                if (copy_serveTimes[work_list[current_pro + 1]]<=0){
//                    copy_comingTimes[work_list[current_pro + 1]] = -1;
//                }
//            } else {
//                current_pro=current_pro + 1;
//                current_pro+=1;
//            }
//        }
//        if (current_pro==processCounts-1){
//            current_pro=0;
//        }
//    }
//    cout
//            << "P_name(ID)      arrived_time      served_time      priority"
//            << endl;
//    for (int i = 0; i < processCounts; i++) {
//        printf("%10d    %12d    %11d    %11d    %14d    %13d    %20f\n", i, copy_comingTimes[i], copy_serveTimes[i], priority[i]);
//    }
//    int current = comingTimes[work_list[0]];
//    // 3. 遍历查找最早到达的进程
//    for (int j = 0; j < processCounts; j++) {
//        // 先设置默认最早到达的进程earliest是0，默认第1个进程到来时间最早 priorCur当前进程优先级
//        //更新其完成时间 = 到来时间+服务时间
//        finishedTimes[work_list[j]] = current + serveTimes[work_list[j]];
//        //等待时间 等待时间 = 当前时间 - 到来时间
//        waitingTimes[work_list[j]] = current - comingTimes[work_list[j]];
//        //更新当前时间 当前时间 = 当前时间 + 服务时间
//        current += serveTimes[work_list[j]];
//        //更新周转时间 周转时间 = 服务时间 + 等待时间
//        turnoverTimes[work_list[j]] = serveTimes[work_list[j]] + waitingTimes[work_list[j]];
//        //更新带权周转时间 带权周转时间 = 周转时间 /服务时间
//        turnoverTimesWeight[work_list[j]] =
//                (float) turnoverTimes[work_list[j]] / (float) serveTimes[work_list[j]];
//    }
}

/**
 * 打印调度信息
 */
void PrintSchedule() {
    cout << "==>进程调度信息打印:" << endl;
    cout << "P_name(ID)    arrived_time    served_time    finished_time    turnover_time    turnover_time_weight"
         << endl;
    for (int i = 0; i < processCounts; i++) {
        printf("%10d    %12d    %11d    %14d    %13d    %20f\n", i, comingTimes[i], serveTimes[i], finishedTimes[i],
               turnoverTimes[i], turnoverTimesWeight[i]);
    }
    //平均周转时间和总周转时间
    float average_turnover_time, sum_turnover_time = 0;
    //平均带权周转时间和总带权周转时间
    float average_turnover_time_weight, sum_turnover_time_weight = 0;
    for (int i = 0; i < processCounts; i++) {
        sum_turnover_time += turnoverTimes[i];
        sum_turnover_time_weight += turnoverTimesWeight[i];
    }
    average_turnover_time = sum_turnover_time / processCounts;
    average_turnover_time_weight = sum_turnover_time_weight / processCounts;
    cout << "==>平均周转时间为：" << average_turnover_time << endl;
    cout << "==>带权平均周转时间为：" << average_turnover_time_weight << endl;
}

/**
 * 打印调度信息
 */
void PrintSchedule_pri() {
    cout << "==>进程调度信息打印:" << endl;
    cout
            << "P_name(ID)      arrived_time      served_time      priority      finished_time      turnover_time      turnover_time_weight"
            << endl;
    for (int i = 0; i < processCounts; i++) {
        printf("%10d    %12d    %11d    %11d    %14d    %13d    %20f\n", i, comingTimes[i], serveTimes[i], priority[i],
               finishedTimes[i],
               turnoverTimes[i], turnoverTimesWeight[i]);
    }
    //平均周转时间和总周转时间
    float average_turnover_time, sum_turnover_time = 0;
    //平均带权周转时间和总带权周转时间
    float average_turnover_time_weight, sum_turnover_time_weight = 0;
    for (int i = 0; i < processCounts; i++) {
        sum_turnover_time += turnoverTimes[i];
        sum_turnover_time_weight += turnoverTimesWeight[i];
    }
    average_turnover_time = sum_turnover_time / processCounts;
    average_turnover_time_weight = sum_turnover_time_weight / processCounts;
    cout << "==>平均周转时间为：" << average_turnover_time << endl;
    cout << "==>带权平均周转时间为：" << average_turnover_time_weight << endl;
}

/**
 * 初始化数组
 */
void Initialize() {
    // 1. 完成时间
    finishedTimes = new int[processCounts];
    // 2. 周转时间
    turnoverTimes = new int[processCounts];
    // 3. 等待时间
    waitingTimes = new int[processCounts];
    // 4. 带权周转时间
    turnoverTimesWeight = new float[processCounts];
}

