#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip> //格式化输出

#define MAX_DURANCE 1e6
using namespace std;

//按服务时间进行冒泡排序
struct FCFS_SJF_r *sortservetime(struct FCFS_SJF_r a[], int n) {
    int i, j;
    struct FCFS_SJF_r t;
    int flag;
    for (i = 1; i < n; i++) {
        flag = 0;
        for (j = 0; j < n - i; j++) {
            if (a[j].arrivetime > a[j + 1].arrivetime)    //将到达时间短的交换到前边
            {
                t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
                flag = 1;//交换
            }
        }
        for (j = 0; j < n - i; j++) {
            if ((a[j].servetime > a[j + 1].servetime) && (a[j].arrivetime >= a[j + 1].arrivetime))//将服务时间短的交换到前边
            {
                t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
                flag = 1;//交换
            }
        }
        if (flag == 0)//如果一趟排序中没发生任何交换，则排序结束
        {
            break;
        }
    }
    return a;    //返回排序后进程数组
}

//按优先级进行冒泡排序
struct FCFS_SJF_r *sortprivity(struct FCFS_SJF_r a[], int n) {
    int i, j;
    struct FCFS_SJF_r t;
    int flag;
    for (i = 1; i < n; i++) {
        flag = 0;
        for (j = 0; j < n - i; j++) {
            if (a[j].arrivetime > a[j + 1].arrivetime)    //将到达时间短的交换到前边
            {
                t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
                flag = 1;//交换
            }
        }
        for (j = 0; j < n - i; j++) {
            if ((a[j].privity > a[j + 1].privity) && (a[j].arrivetime >= a[j + 1].arrivetime))//将服务时间短的交换到前边
            {
                t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
                flag = 1;//交换
            }
        }
        if (flag == 0)//如果一趟排序中没发生任何交换，则排序结束
        {
            break;
        }
    }
    return a;    //返回排序后进程数组
}

//短作业优先算法
void SJF(struct FCFS_SJF_r a[], int n, float &t1, float &t2) {
    int i;
    struct FCFS_SJF_r t;
    a[0].finishtime = a[0].arrivetime + a[0].servetime;    //完成时间=到达时间+服务时间
    a[0].roundtime = a[0].finishtime - a[0].arrivetime;    //周转时间=完成时间-提交时间
    a[0].daiquantime = a[0].roundtime / a[0].servetime;    //带权时间=周转时间/服务时间

    for (i = 1; i < n; i++) {

        for (int c = i; c < n - 1; c++) {
            for (int d = i + 1; d < n; d++)
                if ((a[i - 1].finishtime >= a[c].arrivetime) && (a[i - 1].finishtime >= a[d].arrivetime) &&
                    (a[c].servetime > a[d].servetime)) {
                    t = a[c];
                    a[c] = a[d];
                    a[d] = t;
                }
        }

        if (a[i].arrivetime < a[i - 1].finishtime)    //当前到达时间在上一个作业结束时间之前
        {
            a[i].finishtime = a[i - 1].finishtime + a[i].servetime;    //完成时间=上一个完成时间+服务时间
            a[i].roundtime = a[i].finishtime - a[i].arrivetime;        //周转时间=完成时间-到达时间
            a[i].daiquantime = a[i].roundtime / a[i].servetime;        //带权时间=周转时间/服务时间
        } else    //当前到达时间在上一个作业结束时间之后
        {
            a[i].finishtime = a[i].arrivetime + a[i].servetime;
            a[i].roundtime = a[i].finishtime - a[i].arrivetime;
            a[i].daiquantime = a[i].roundtime / a[i].servetime;
        }

    }
    for (i = 0; i < n; i++) {
        printf("\n---------------------------------------------------\n");
        cout << setw(2) << "进程名：" << a[i].name << " ";
        cout << setw(2) << "到达时间：" << a[i].arrivetime << " ";
        cout << setw(2) << "服务时间：" << a[i].servetime << endl;
        cout << setw(2) << "完成时间：" << a[i].finishtime << endl;
        cout << setw(2) << "周转时间：" << a[i].roundtime << endl;
        cout << setw(2) << "带权周转时间" << a[i].daiquantime << endl;
        t1 += a[i].roundtime;
        t2 += a[i].daiquantime;
    }
}

//优先级算法
void r(struct FCFS_SJF_r a[], int n, float &t1, float &t2) {
    int i;
    struct FCFS_SJF_r t;
    a[0].finishtime = a[0].arrivetime + a[0].servetime;    //完成时间=到达时间+服务时间
    a[0].roundtime = a[0].finishtime - a[0].arrivetime;    //周转时间=完成时间-提交时间
    a[0].daiquantime = a[0].roundtime / a[0].servetime;    //带权时间=周转时间/服务时间
    for (i = 1; i < n; i++) {

        for (int c = i; c < n - 1; c++) {
            for (int d = i + 1; d < n; d++)
                if ((a[i - 1].finishtime >= a[c].arrivetime) && (a[i - 1].finishtime >= a[d].arrivetime) &&
                    (a[c].privity > a[d].privity)) {
                    t = a[c];
                    a[c] = a[d];
                    a[d] = t;
                }
        }

        if (a[i].arrivetime < a[i - 1].finishtime)    //当前到达时间在上一个作业结束时间之前
        {
            a[i].finishtime = a[i - 1].finishtime + a[i].servetime;    //完成时间=上一个完成时间+服务时间
            a[i].roundtime = a[i].finishtime - a[i].arrivetime;        //周转时间=完成时间-到达时间
            a[i].daiquantime = a[i].roundtime / a[i].servetime;        //带权时间=周转时间/服务时间
        } else    //当前到达时间在上一个作业结束时间之后
        {
            a[i].finishtime = a[i].arrivetime + a[i].servetime;
            a[i].roundtime = a[i].finishtime - a[i].arrivetime;
            a[i].daiquantime = a[i].roundtime / a[i].servetime;
        }

    }
    for (i = 0; i < n; i++) {
        printf("\n---------------------------------------------------\n");
        cout << setw(2) << "进程名：" << a[i].name << " ";
        cout << setw(2) << "到达时间：" << a[i].arrivetime << " ";
        cout << setw(2) << "服务时间：" << a[i].servetime << endl;
        cout << setw(2) << "优先级：" << a[i].privity << " ";
        cout << setw(2) << "完成时间：" << a[i].finishtime << endl;
        cout << setw(2) << "周转时间：" << a[i].roundtime << endl;
        cout << setw(2) << "带权周转时间" << a[i].daiquantime << endl;
        t1 += a[i].roundtime;
        t2 += a[i].daiquantime;
    }
}
