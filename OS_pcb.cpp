#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>

using namespace std;

/*���̵��ȵ�ģ��ʵ��*/

/**
 * ����ȫ�ֲ���
 */
int processCounts;           /*���������*/
int methodChoosen;          /*��ѡ�����㷨*/

/**
 * ������̿��ƿ�����ݽṹ
 */
typedef struct PCB {
    int num;                        /*���̴���*/
    int comingTimes;                /*����ʱ��*/
    int serveTime;                  /*����ʱ��*/
    int priority;                   /*���ȼ���  ��ֵ�����ȼ���*/
    int startedTimes = 0;           /*��¼��ʼ����ʱ��*/
    int finishedTimes = 0;          /*��¼��������ʱ��*/
    int turnoverTimes = 0;          /*��תʱ��*/
    int waitingTimes = 0;           /*�ȴ�ʱ��*/
    float turnoverTimesWeight = 0;  /*��Ȩ��תʱ��*/
    bool flag = true;               /*���ñ�ʶ*/
    bool startFlag = false;         /*�״ε��� ���ñ�ʶ*/
} PCBblock;

/**
* ��������
* �α���40% �Գ�Ҷ30% ������30% ����ֹ�
*/
void InitProcessCount();                             /*������̸���*/
void InitPCBData(PCBblock pcBblockList[]);           /*��ʼ���ṹ������*/
void PcbCopy(PCBblock pcBblockList1[], PCBblock pcBblockList2[]);  /*����һ��Pcb�ṹ�帱��*/
void PrintInformation(PCBblock pcBblockList[]);     /*��ӡ������Ϣ*/
void ChooseMethod();                                /*ѡ������㷨*/
void SJF(PCBblock pcBblockList[]);                  /*����ҵ����*/
void FCFS(PCBblock pcBblockList[]);                 /*�����ȷ���*/
void PS(PCBblock pcBblockList[]);                   /*��ռʽ*/
void NPS(PCBblock pcBblockList[]);                  /*����ռʽ*/
void PrintSchedule(PCBblock pcBblockList[]);        /*��ӡ������Ϣ*/
void PrintSchedule_Test(PCBblock pcBblockList[]);   /*��ӡ������Ϣ*/
void PrintSchedule_Flag(PCBblock pcBblockList[]);   /*��ӡ������Ϣ*/
void PrintSchedule_1(PCBblock pcBblockList[]);      /*��ӡ������Ϣ*/
void PrintSchedule_Pri(PCBblock pcBblockList[]);    /*��ӡ������Ϣ*/
void begin(int choice, PCBblock pcBblockList[]);    /*��ʼ����*/
bool LessSort_comingTime(PCBblock a, PCBblock b);   /*�����ȷ���������� ���յ���ʱ�� ����������� */
bool LessSort_Num(PCBblock a, PCBblock b);          /*���ս��̱�ʶ�� �����������*/
bool LessSort_StartTime(PCBblock a, PCBblock b);    /*���տ�ʼʱ�� �����������*/

/**
 * ������
 * CodeBy�� �α���
 * @return 0
 */
int main() {
    /*  1. ��ʼ����������      */
    InitProcessCount();
    while (processCounts <= 0) {
        InitProcessCount();
    }
    /*  2. ������̽ṹ������    */
    PCBblock pcBblockList[processCounts];
    /*  3. ��ʼ�����̽ṹ������   */
    InitPCBData(pcBblockList);
    /*  4. ��ӡһ������Ľ��̽ṹ������*/
    PrintInformation(pcBblockList);
    /*  5. ѡ����õ��㷨          */
    ChooseMethod();
    /*  6. ��ʼ����ĳ���㷨         */
    while (methodChoosen != 9999) {
        begin(methodChoosen, pcBblockList);
        ChooseMethod();
    }
    /*  ͣ��   */
    system("pause");
    return 0;
}


/**
 * ��ʼ����������
 * CodeBy�� �α���
 */
void InitProcessCount() {
    cout << "==>��������̸���(��������>0):";
    cin >> processCounts;
}

/**
 * ��ʼ��PCB����
 * CodeBy�� ������
 * @param pcBblockList
 */
void InitPCBData(PCBblock pcBblockList[]) {
    cout << "***********************************���ݳ�ʼ����ʼ***********************************" << endl;
    for (int i = 0; i < processCounts; ++i) {
        pcBblockList[i].num = i + 1;
        cout << "==>��������� " << i + 1 << " �ĵ���ʱ�䣺 ";
        cin >> pcBblockList[i].comingTimes;
        cout << "==>��������� " << i + 1 << " �ķ���ʱ�䣺 ";
        cin >> pcBblockList[i].serveTime;
        cout << "==>��������� " << i + 1 << " �����ȼ��� ";
        cin >> pcBblockList[i].priority;
        cout << endl;
    }
    cout << "***********************************���ݳ�ʼ������***********************************" << endl;
}

/**
 * ��ʼ����
 * CodeBy�� �Գ�Ҷ
 * @param choice
 * @param pcBblockList
 */
void begin(int choice, PCBblock pcBblockList[]) {
    switch (choice) {
        case 1:
            FCFS(pcBblockList); /*�����ȷ���*/
            break;
        case 2:
            SJF(pcBblockList);/*����ҵ����*/
            break;
        case 3:
            NPS(pcBblockList);/*���ȼ����ȷ���ռ*/
            break;
        case 4:
            PS(pcBblockList);/*���ȼ�������ռʽ*/
            break;
        default:
            cout << "�����������������룡";
            break;
    }
}


/**
 * ��ӡ��Ϣ
 * CodeBy�� ������
 * @param pcBblockList
 */
void PrintInformation(PCBblock pcBblockList[]) {
    cout << "==>����������Ľ�������:  " << processCounts << "  " << endl;
    cout << "==>���̵�����ʱ��չʾ: ";
    for (int i = 0; i < processCounts; i++) {
        cout << pcBblockList[i].comingTimes << " ";
    }
    cout << endl;
    cout << "==>���̷����ʱ��չʾ: ";
    for (int i = 0; i < processCounts; i++) {
        cout << pcBblockList[i].serveTime << " ";
    }
    cout << endl;
    cout << "==>�������ȼ���չʾ:   ";
    for (int i = 0; i < processCounts; i++) {
        cout << pcBblockList[i].priority << " ";
    }
    cout << endl;
}

/**
 * ѡ���㷨����
 * CodeBy�� �Գ�Ҷ
 */
void ChooseMethod() {
    cout << "��ѡ��һ�ֵ��ȷ�ʽ 1-(�����ȷ���FCFS),  2-(�̽�������SJF),  3-���ȼ�(����ռ),  4-���ȼ�(��ռʽ)  9999-�˳�����: ";
    cin >> methodChoosen;
    cout << endl;
}


/**
 * �����ȷ���������� ���յ���ʱ�� �����������
 * CodeBy�� �Գ�Ҷ
 * @param a
 * @param b
 * @return true/false
 */
bool LessSort_comingTime(PCBblock a, PCBblock b) {
    return a.comingTimes < b.comingTimes;
}

/**
 * ���ս��̱�ʶ�� �����������
 * CodeBy�� ������
 * @param a
 * @param b
 * @return  true/false
 */
bool LessSort_Num(PCBblock a, PCBblock b) {
    return a.num < b.num;
}

/**
 * ���տ�ʼʱ�� �����������
 * CodeBy�� �α���
 * @param a
 * @param b
 * @return true/false
 */
bool LessSort_StartTime(PCBblock a, PCBblock b) {
    return a.startedTimes < b.startedTimes;
}

/**
 * �����ȷ����㷨
 * CodeBy�� �Գ�Ҷ
 * @param pcBblockList
 */
void FCFS(PCBblock pcBblockList[]) {
    /*  ����һ�����̽ṹ��������Ϊ����     */
    PCBblock pcBblockList_copy[processCounts];
    /*  ���ø��ƺ���  */
    PcbCopy(pcBblockList, pcBblockList_copy);
    /*  ���õ�ǰʱ��  */
    int current = 0;
    /*  ����������  */
    sort(pcBblockList_copy, pcBblockList_copy + processCounts, LessSort_comingTime);
    /* �����������絽��Ľ���*/
    for (int i = 0; i < processCounts; i++) {
        /*  �����ǰ���̵���ʱ��ȵ�ǰʱ���������������Ϊ��ǰʱ��  */
        if (pcBblockList_copy[i].comingTimes > current) {
            current = pcBblockList_copy[i].comingTimes;
        }
        pcBblockList_copy[i].flag = false;
        /*  ��ʼʱ�� = ��ǰʱ��  */
        pcBblockList_copy[i].startedTimes = current;
        /*  ���ʱ�� = ��ʼʱ�� + ����ʱ��   */
        pcBblockList_copy[i].finishedTimes = pcBblockList_copy[i].startedTimes + pcBblockList_copy[i].serveTime;
        /*  �ȴ�ʱ�� = ��ʼʱ�� - ����ʱ��     */
        pcBblockList_copy[i].waitingTimes = pcBblockList_copy[i].startedTimes - pcBblockList_copy[i].comingTimes;
        /*  ��ǰʱ�� = ���ʱ��     */
        current = pcBblockList_copy[i].finishedTimes;
        /*  ��תʱ�� = ����ʱ�� + �ȴ�ʱ��      */
        pcBblockList_copy[i].turnoverTimes = pcBblockList_copy[i].serveTime + pcBblockList_copy[i].waitingTimes;
        /*  ��Ȩ��תʱ�� = ��תʱ�� / ����ʱ��     */
        pcBblockList_copy[i].turnoverTimesWeight = (float) pcBblockList_copy[i].turnoverTimes / (float) pcBblockList_copy[i].serveTime;


        //PrintSchedule_Test(pcBblockList_copy); /*��ӡ���Ȳ��Թ���*/


    }
    /*  ��ӡ���������Ϣ    */
    sort(pcBblockList_copy, pcBblockList_copy + processCounts, LessSort_StartTime);
    PrintSchedule(pcBblockList_copy);
    cout<<endl;cout<<endl;
}

/**
 * �̽������Ȱ취
 * CodeBy�� ������
 * @param pcBblockList
 */
void SJF(PCBblock pcBblockList[]) {
    /*  ����һ�����̽ṹ��������Ϊ����     */
    PCBblock pcBblockList_copy[processCounts];
    /*  ���ø��ƺ���  */
    PcbCopy(pcBblockList, pcBblockList_copy);
    /*  ����������  */
    sort(pcBblockList_copy, pcBblockList_copy + processCounts, LessSort_comingTime);
    /*  �����������Ľ��̵ĵ���ʱ��Ϊ��ǰʱ��  */
    int current = pcBblockList_copy[0].comingTimes;
    /* �����������絽��Ľ���  */
    for (int j = 0; j < processCounts; j++) {
        /*  ��������index ��¼��̷���ʱ����̵�����  ����ȽϽ��̷���ʱ��Ĳ��� SerNum */
        int index = 0, serNum = 9999;
        /*  �ҵ���̷���ʱ����̵�����index  */
        for (int i = 1; i < processCounts; ++i) {
            /*  �״ε����Ľ���ֱ�ӵ���     */
            if (j == 0) {
                break;
            }
            /*  �����ǰ����û�б����� �� �Ѿ����� */
            if (pcBblockList_copy[i].flag && pcBblockList_copy[i].comingTimes <= current) {
                if (pcBblockList_copy[i].serveTime < serNum) {
                    serNum = pcBblockList_copy[i].serveTime;
                    index = i;
                }
            }
        }
        /*  ���õ�ǰ�����Ѿ���������flag=flase  */
        pcBblockList_copy[index].flag = false;
        /*  ��ʼʱ�� = ��ǰʱ��  */
        pcBblockList_copy[index].startedTimes = current;
        /*  ���ʱ�� = ��ʼʱ�� + ����ʱ��   */
        pcBblockList_copy[index].finishedTimes = pcBblockList_copy[index].startedTimes + pcBblockList_copy[index].serveTime;
        /*  �ȴ�ʱ�� = ��ʼʱ�� - ����ʱ��     */
        pcBblockList_copy[index].waitingTimes = pcBblockList_copy[index].startedTimes - pcBblockList_copy[index].comingTimes;
        /*  ��ǰʱ�� = ���ʱ��     */
        current = pcBblockList_copy[index].finishedTimes;
        /*  ��תʱ�� = ����ʱ�� + �ȴ�ʱ��      */
        pcBblockList_copy[index].turnoverTimes =  pcBblockList_copy[index].serveTime + pcBblockList_copy[index].waitingTimes;
        /*  ��Ȩ��תʱ�� = ��תʱ�� / ����ʱ��     */
        pcBblockList_copy[index].turnoverTimesWeight =  (float) pcBblockList_copy[index].turnoverTimes / (float) pcBblockList_copy[index].serveTime;


        //PrintSchedule_Test(pcBblockList_copy); /*��ӡ���Ȳ��Թ���*/


    }
    /*  ��ӡ���������Ϣ    */
    sort(pcBblockList_copy, pcBblockList_copy + processCounts, LessSort_StartTime);
    PrintSchedule(pcBblockList_copy);
    cout<<endl;cout<<endl;
}

/**
 * PCB����һ�ݽṹ�����鸱��
 * CodeBy�� �α���
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
 * ���ȼ������㷨(����ռʽ)  ���ȼ����� �� ���ȼ���
 * CodeBy�� �α���
 * @param pcBblockList
 */
void NPS(PCBblock pcBblockList[]) {
    /*  1. ����һ�����̽ṹ��������Ϊ����     */
    PCBblock pcBblockList_copy[processCounts];
    /*  2. ���ø��ƺ���  */
    PcbCopy(pcBblockList, pcBblockList_copy);
    /*  3. ����������  */
    sort(pcBblockList_copy, pcBblockList_copy + processCounts, LessSort_comingTime);
    /*  4. �����������Ľ��̵ĵ���ʱ��Ϊ��ǰʱ��  */
    int current = pcBblockList_copy[0].comingTimes;
    /*  5. �����������絽��Ľ���  */
    for (int j = 0; j < processCounts; j++) {
        /*  ��������index ��¼������ȼ����̵�����  ����ȽϽ������ȼ��Ĳ��� priNum */
        int index = 0, priNum = 0;
        /*  �ҵ�������ȼ����̵�����index  */
        for (int i = 0; i < processCounts; ++i) {
            /*  �����ǰ����û�б����� �� �Ѿ����� */
            if (pcBblockList_copy[i].flag && pcBblockList_copy[i].comingTimes <= current) {
                if (pcBblockList_copy[i].priority > priNum) {
                    priNum = pcBblockList_copy[i].priority;
                    index = i;
                }
            }
        }
        /*  ���õ�ǰ�����Ѿ���������flag=flase  */
        pcBblockList_copy[index].flag = false;
        /*  ��ʼʱ�� = ��ǰʱ��  */
        pcBblockList_copy[index].startedTimes = current;
        /*  ���ʱ�� = ��ʼʱ�� + ����ʱ��   */
        pcBblockList_copy[index].finishedTimes = pcBblockList_copy[index].startedTimes + pcBblockList_copy[index].serveTime;
        /*  �ȴ�ʱ�� = ��ʼʱ�� - ����ʱ��     */
        pcBblockList_copy[index].waitingTimes = pcBblockList_copy[index].startedTimes - pcBblockList_copy[index].comingTimes;
        /*  ��ǰʱ�� = ���ʱ��     */
        current = pcBblockList_copy[index].finishedTimes;
        /*  ��תʱ�� = ����ʱ�� + �ȴ�ʱ��      */
        pcBblockList_copy[index].turnoverTimes = pcBblockList_copy[index].serveTime + pcBblockList_copy[index].waitingTimes;
        /*  ��Ȩ��תʱ�� = ��תʱ�� / ����ʱ��     */
        pcBblockList_copy[index].turnoverTimesWeight = (float) pcBblockList_copy[index].turnoverTimes / (float) pcBblockList_copy[index].serveTime;


        //PrintSchedule_Test(pcBblockList_copy); /*��ӡ���Ȳ��Թ���*/


    }
    sort(pcBblockList_copy, pcBblockList_copy + processCounts, LessSort_StartTime);
    PrintSchedule_Pri(pcBblockList_copy);
    cout<<endl;cout<<endl;
}


 /**
  * ���ȼ������㷨(��ռʽ)
  * CodeBy�� �α���
  * @param pcBblockList
  */
void PS(PCBblock pcBblockList[]) {
     /*  ����һ�����̽ṹ��������Ϊ����     */
     PCBblock pcBblockList_copy[processCounts], pcBblockList1[processCounts];
     /*  ���ø��ƺ���  �������� ��Ϊ���е�һ�ݷ���ʱ����С��0 �����ݽ������޸� */
     PcbCopy(pcBblockList, pcBblockList_copy);
     PcbCopy(pcBblockList, pcBblockList1);
     /*  ����������  */
     sort(pcBblockList_copy, pcBblockList_copy + processCounts, LessSort_comingTime);
     sort(pcBblockList1, pcBblockList1 + processCounts, LessSort_comingTime);
     /*  ������ʱ�� �ó�ѭ������ */
     int sumTime = 0;
     int serveTime=0;
     /* �����еĽ��̵ķ���ʱ����� */
     for (int i = 0; i < processCounts; ++i) {
         serveTime+=pcBblockList_copy[i].serveTime;
     }
     /* �ټ������һ�����̵���ʱ�� �õ����ѭ������ */
     sumTime = serveTime+pcBblockList_copy[processCounts-1].comingTimes;
     /*  �����������Ľ��̵ĵ���ʱ��Ϊ��ǰʱ��  */
     int current = pcBblockList1[0].comingTimes;
     /*  index����ǰ���Ƚ��̵�����  local������һ�����Ƚ��̵�����*/
     int index = 0, local = 0;
     /* ����ʱ��ѭ��  */
     cout << "***********************************���̵�����Ϣ��ӡ��ʾ***********************************" << endl;
     for (; current <=sumTime; ++current) {
         int priNum = 0;
         for (int i = 0; i < processCounts; ++i) {
             /*  �����ǰ����û�б����� �� �Ѿ����� */
             if (pcBblockList1[i].flag && pcBblockList1[i].comingTimes <= current) {
                 /*  �жϵ�ǰ�������̵����ȼ��Ƿ�ȵ�ǰ���еĽ��̵����ȼ��� */
                 if (pcBblockList1[i].priority > priNum) {
                     priNum = pcBblockList1[i].priority;
                     /* �������̵��� index��¼��ǰҪ���ȵĽ��� */
                     index = i;
                     /*  �жϵ�ǰҪ���Ƚ����Ƿ��ǵ�һ�ε���  */
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
         /*  �жϵ�ǰʱ�����н����Ƿ����֮ǰ���еĽ���   */
         if (local != index) {
             if (local != processCounts - 1) {
                 cout << "����" << pcBblockList_copy[local].num << "->";
             }
             /*  �ѵ�ǰ�Ľ��̵�������local  local��¼��һ�����ȵĽ�������  */
             local = index;//�����ӡ��
         }
         /*  ���ý���ʱ��  ����δ����ȫ�������  */
         if(pcBblockList1[index].flag){
             /*�����޸��������ʱ�䣬����ʱ��*/
             pcBblockList_copy[index].finishedTimes = current + 1;
             pcBblockList1[index].finishedTimes = current + 1;
             pcBblockList1[index].serveTime -= 1;
             /*  �жϵ�ǰ�����Ƿ񱻷������ */
             if (pcBblockList1[index].serveTime == 0) {
                 pcBblockList1[index].flag = false;
             }


             //PrintSchedule_Test(pcBblockList1);/*���Դ�ӡ*/


         }

     }
     cout << "����" << pcBblockList_copy[index].num << "" << endl;
     for (int index = 0; index < processCounts; ++index) {
         /*  �ȴ�ʱ�� = ����ʱ�� - ����ʱ�� - ����ʱ��   */
         pcBblockList_copy[index].waitingTimes = pcBblockList_copy[index].finishedTimes - pcBblockList_copy[index].comingTimes - pcBblockList_copy[index].serveTime;
         /*  ��תʱ�� = ����ʱ�� + �ȴ�ʱ�� */
         pcBblockList_copy[index].turnoverTimes = pcBblockList_copy[index].serveTime + pcBblockList_copy[index].waitingTimes;
         /*  ��Ȩ��תʱ�� = ��תʱ�� / ����ʱ�� */
         pcBblockList_copy[index].turnoverTimesWeight = (float) pcBblockList_copy[index].turnoverTimes / (float) pcBblockList_copy[index].serveTime;
     }
     sort(pcBblockList_copy, pcBblockList_copy + processCounts, LessSort_Num);
     PrintSchedule_1(pcBblockList_copy);
     cout<<endl;cout<<endl;
}

/**
 * ��ӡ������Ϣ(���п��ñ�ʶ)
 * CodeBy�� �Գ�Ҷ
 * @param pcBblockList
 */
void PrintSchedule_Flag(PCBblock pcBblockList[]) {
    cout << "***********************************���̵�����Ϣ��ӡ��ʾ***********************************" << endl;
    cout << "����(ID)    ����ʱ��    ����ʱ��        ���ȼ�    �ȴ�ʱ��    ��ʼʱ��    ���ʱ��    ��תʱ��    ��Ȩ��תʱ��    flag" << endl;
    for (int i = 0; i < processCounts; i++) {
        printf("%5d    %6d    %7d    %7d    %7d    %7d    %7d    %7d    %.2f      %d\n", pcBblockList[i].num,
               pcBblockList[i].comingTimes, pcBblockList[i].serveTime, pcBblockList[i].priority,
               pcBblockList[i].waitingTimes, pcBblockList[i].startedTimes, pcBblockList[i].finishedTimes,
               pcBblockList[i].turnoverTimes, pcBblockList[i].turnoverTimesWeight, pcBblockList[i].flag);
    }
    /*ƽ����תʱ�������תʱ��*/
    float average_turnover_time, sum_turnover_time = 0.0;
    /*ƽ����Ȩ��תʱ����ܴ�Ȩ��תʱ��*/
    float average_turnover_time_weight, sum_turnover_time_weight = 0;
    for (int i = 0; i < processCounts; i++) {
        sum_turnover_time += pcBblockList[i].turnoverTimes;
        sum_turnover_time_weight += pcBblockList[i].turnoverTimesWeight;
    }
    average_turnover_time = sum_turnover_time / processCounts;
    average_turnover_time_weight = sum_turnover_time_weight / processCounts;
    cout << "==>ƽ����תʱ�䣺" << average_turnover_time << " " << endl;
    cout << "==>ƽ����Ȩ��תʱ�䣺" << average_turnover_time_weight << " " << endl;
    cout << "***********************************���̵�����Ϣ��ӡ����***********************************" << endl;
}

/**
 * ��ӡ������Ϣ
 * @param pcBblockList
 */
void PrintSchedule_1(PCBblock pcBblockList[]) {
    cout << "����(ID)    ����ʱ��    ����ʱ��        ���ȼ�    �ȴ�ʱ��    ��ʼʱ��    ���ʱ��    ��תʱ��    ��Ȩ��תʱ��" << endl;
    for (int i = 0; i < processCounts; i++) {
        printf("%5d    %6d    %7d    %7d    %7d    %7d    %7d    %7d         %.2f\n", pcBblockList[i].num,
               pcBblockList[i].comingTimes, pcBblockList[i].serveTime, pcBblockList[i].priority,
               pcBblockList[i].waitingTimes, pcBblockList[i].startedTimes, pcBblockList[i].finishedTimes,
               pcBblockList[i].turnoverTimes, pcBblockList[i].turnoverTimesWeight);
    }
    /*ƽ����תʱ�������תʱ��*/
    float average_turnover_time, sum_turnover_time = 0.0;
    /*ƽ����Ȩ��תʱ����ܴ�Ȩ��תʱ��*/
    float average_turnover_time_weight, sum_turnover_time_weight = 0;
    for (int i = 0; i < processCounts; i++) {
        sum_turnover_time += pcBblockList[i].turnoverTimes;
        sum_turnover_time_weight += pcBblockList[i].turnoverTimesWeight;
    }
    average_turnover_time = sum_turnover_time / processCounts;
    average_turnover_time_weight = sum_turnover_time_weight / processCounts;
    cout << "==>ƽ����תʱ�䣺" << average_turnover_time << " " << endl;
    cout << "==>ƽ����Ȩ��תʱ�䣺" << average_turnover_time_weight << " " << endl;
    cout << "***********************************���̵�����Ϣ��ӡ����***********************************" << endl;
}

/**
 * ��ӡ������Ϣ(test��)  0�����ˣ�1δ����
 * CodeBy�� �α���
 * @param pcBblockList
 */
void PrintSchedule_Test(PCBblock pcBblockList[]) {
    cout << "---------------------------------���̵��ȹ��̲��Դ�ӡ��ʾ---------------------------------" << endl;
    cout << "����(ID)    ����ʱ��    ����ʱ��        ���ȼ�    �ȴ�ʱ��    ��ʼʱ��    ���ʱ��    flag(0: ��� , 1: δ���)" << endl;
    for (int i = 0; i < processCounts; i++) {
        printf("%5d    %6d    %7d    %7d    %7d    %7d    %7d          %d\n", pcBblockList[i].num,
               pcBblockList[i].comingTimes, pcBblockList[i].serveTime, pcBblockList[i].priority,
               pcBblockList[i].waitingTimes, pcBblockList[i].startedTimes, pcBblockList[i].finishedTimes,
               pcBblockList[i].flag);
    }
    cout << "---------------------------------���̵��ȹ��̲��Դ�ӡ����---------------------------------" << endl;
}



/**
 * ��ӡ������Ϣ(�������ȼ�)
 * CodeBy�� ������
 * @param pcBblockList
 */
void PrintSchedule_Pri(PCBblock pcBblockList[]) {
    cout << "***********************************���̵�����Ϣ��ӡ��ʾ***********************************" << endl;
    cout << "==>���ȹ���: ";
    for (int i = 0; i < processCounts; ++i) {
        if (i != processCounts - 1) {
            cout << "����" << pcBblockList[i].num << "->";
        } else {
            cout << "����" << pcBblockList[i].num << "" << endl;
        }
    }
    cout << "����(ID)    ����ʱ��    ����ʱ��     ���ȼ�     �ȴ�ʱ��     ��ʼʱ��    ���ʱ��    ��תʱ��    ��Ȩ��תʱ��" << endl;
    for (int i = 0; i < processCounts; i++) {
        printf("%5d    %6d    %7d    %7d    %7d    %7d    %7d    %7d         %.2f\n", pcBblockList[i].num,
               pcBblockList[i].comingTimes, pcBblockList[i].serveTime, pcBblockList[i].priority,
               pcBblockList[i].waitingTimes, pcBblockList[i].startedTimes, pcBblockList[i].finishedTimes,
               pcBblockList[i].turnoverTimes, pcBblockList[i].turnoverTimesWeight);
    }
    /*ƽ����תʱ�������תʱ��*/
    float average_turnover_time, sum_turnover_time = 0.0;
    /*ƽ����Ȩ��תʱ����ܴ�Ȩ��תʱ��*/
    float average_turnover_time_weight, sum_turnover_time_weight = 0;
    for (int i = 0; i < processCounts; i++) {
        sum_turnover_time += pcBblockList[i].turnoverTimes;
        sum_turnover_time_weight += pcBblockList[i].turnoverTimesWeight;
    }
    average_turnover_time = sum_turnover_time / processCounts;
    average_turnover_time_weight = sum_turnover_time_weight / processCounts;
    cout << "==>ƽ����תʱ�䣺" << average_turnover_time << " " << endl;
    cout << "==>ƽ����Ȩ��תʱ�䣺" << average_turnover_time_weight << " " << endl;
    cout << "***********************************���̵�����Ϣ��ӡ����***********************************" << endl;
}

/**
 * ��ӡ������Ϣ(������ӡ)
 * CodeBy�� �α���
 * @param pcBblockList
 */
void PrintSchedule(PCBblock pcBblockList[]) {
    cout << "***********************************���̵�����Ϣ��ӡ��ʾ***********************************" << endl;
    cout << "==>���ȹ���: ";
    for (int i = 0; i < processCounts; ++i) {
        if (i != processCounts - 1) {
            cout << "����" << pcBblockList[i].num << "->";
        } else {
            cout << "����" << pcBblockList[i].num << "" << endl;
        }
    }
    cout << "����(ID)    ����ʱ��    ����ʱ��    �ȴ�ʱ��    ��ʼʱ��    ���ʱ��    ��תʱ��    ��Ȩ��תʱ��" << endl;
    for (int i = 0; i < processCounts; i++) {
        printf("%5d    %6d    %7d    %7d    %7d    %7d    %7d      %.2f\n", pcBblockList[i].num,
               pcBblockList[i].comingTimes, pcBblockList[i].serveTime, pcBblockList[i].waitingTimes,
               pcBblockList[i].startedTimes, pcBblockList[i].finishedTimes, pcBblockList[i].turnoverTimes,
               pcBblockList[i].turnoverTimesWeight);
    }
    /*ƽ����תʱ�������תʱ��*/
    float average_turnover_time, sum_turnover_time = 0.0;
    /*ƽ����Ȩ��תʱ����ܴ�Ȩ��תʱ��*/
    float average_turnover_time_weight, sum_turnover_time_weight = 0;
    for (int i = 0; i < processCounts; i++) {
        sum_turnover_time += pcBblockList[i].turnoverTimes;
        sum_turnover_time_weight += pcBblockList[i].turnoverTimesWeight;
    }
    average_turnover_time = sum_turnover_time / processCounts;
    average_turnover_time_weight = sum_turnover_time_weight / processCounts;
    cout << "==>ƽ����תʱ�䣺" << average_turnover_time << " " << endl;
    cout << "==>ƽ����Ȩ��תʱ�䣺" << average_turnover_time_weight << " " << endl;
    cout << "***********************************���̵�����Ϣ��ӡ����***********************************" << endl;
}

