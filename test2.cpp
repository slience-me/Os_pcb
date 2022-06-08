#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip> //��ʽ�����

#define MAX_DURANCE 1e6
using namespace std;

/**
 * ȫ�ֲ���
 */
int processCounts;            //������
int *comingTimes;            //�ﵽʱ��
int *serveTimes;             //����ʱ��
int *priority;               //��Ӧ���ȼ�
int *finishedTimes;          //���ʱ��
int *turnoverTimes;          //��תʱ��
int *waitingTimes;           //�ȴ�ʱ��
float *turnoverTimesWeight; //��Ȩ��תʱ��
int methodChoosen;           //��ѡ�����㷨

/**
 * ������̿��ƿ�����ݽṹ
 */
struct PCB {
    int number;//���̴���
    int arriveTime;//����ʱ��
    int serveTime;//����ʱ��
    int priority;//���ȼ�
    int finishTime = 0;//��¼��������ʱ��
};

/**
* ��������
*/
void InitProcessCount();    //������̸���
void InitComingTime();   //������̵���ʱ��
void InitServeTime();    //������̷���ʱ��
void InitPriority();    //����������ȼ�
void PrintInformation();     //��ӡ������Ϣ
void ChooseMethod();       //ѡ������㷨
void Initialize();          //�����ʼ��
void SJF();                 //����ҵ����
void FCFS();                //�����ȷ���
void PS();                //��ռʽ
void NPS();                //����ռʽ
void PrintSchedule();   //��ӡ������Ϣ
void PrintSchedule_pri();   //��ӡ������Ϣ
void exchange(int x[], int i, int j);

/**
 * ������
 * @return
 */
int main() {
    // 1. ������̸���
    InitProcessCount();
    // 2. ������̵���ʱ��
    InitComingTime();
    // 3. ������̷���ʱ��
    InitServeTime();
    // 4. ��ӡ������Ϣ
    PrintInformation();
    // 5. �����ʼ��
    Initialize();
    // 6. ѡ������㷨
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
            //����������ȼ�
            InitPriority();
            NPS();
            PrintSchedule_pri();
            break;
        default:
            //����������ȼ�
            InitPriority();
            PS();
            PrintSchedule();
            break;
    }
    system("pause");
    return 0;
}

/**
 * ��ʼ����������
 */
void InitProcessCount() {
    cout << "==>��������̸���:";
    cin >> processCounts;
}

/**
 * ��ʼ������ʱ��
 */
void InitComingTime() {
    comingTimes = new int[processCounts];
    cout << "==>������������̵ĵ���ʱ��:";
    for (int i = 0; i < processCounts; i++) {
        cin >> comingTimes[i];
    }
}

/**
 * ��ʼ������ʱ��
 */
void InitServeTime() {
    serveTimes = new int[processCounts];
    cout << "==>������������̵�Ҫ�����ʱ��:";
    for (int i = 0; i < processCounts; i++) {
        cin >> serveTimes[i];
    }
}

/**
 * ��ʼ�����ȼ�
 */
void InitPriority() {
    priority = new int[processCounts];
    cout << "==>������������̵����ȼ�:";
    for (int i = 0; i < processCounts; i++) {
        cin >> priority[i];
    }
}

/**
 * ��ӡ��Ϣ
 */
void PrintInformation() {
    cout << "==>����������Ľ�������: [ " << processCounts << " ] " << endl;
    cout << "==>���̵�����ʱ��չʾ: ";
    for (int i = 0; i < processCounts; i++) {
        cout << comingTimes[i] << " ";
    }
    cout << endl;
    cout << "==>���̷����ʱ��չʾ: ";
    for (int i = 0; i < processCounts; i++) {
        cout << serveTimes[i] << " ";
    }
    cout << endl;
}

/**
 * ѡ���㷨����
 */
void ChooseMethod() {
    cout << "��ѡ��һ�ֵ��ȷ�ʽ[ 1-(�����ȷ���FCFS),  2-(�̽�������SJF),  3-���ȼ�(����ռ),  4-���ȼ�(��ռʽ)]: ";
    cin >> methodChoosen;
    cout << endl;
}

/**
 * �����ȷ����㷨
 */
void FCFS() {
    // 1. ���ó�ʼʱ��0
    int current = 0;
    // 2. ����һ�ݵ���ʱ��ĸ���copy_comingTimes[]
    int copy_comingTimes[processCounts];
    for (int i = 0; i < processCounts; i++) {
        copy_comingTimes[i] = comingTimes[i];
    }
    // 3. �����������絽��Ľ���
    for (int j = 0; j < processCounts; j++) {
        // ������Ĭ�����絽��Ľ���earliest��0��Ĭ�ϵ�1�����̵���ʱ������
        int earliestProcess = 0, min = copy_comingTimes[0];
        //���ҵ���ǰ���ȵ���Ľ��̣���Ҫʹ�õ���ʱ��ĸ�������
        //��minΪ���н����е��������ʱ��
        for (int i = 1; i < processCounts; i++) {
            if (copy_comingTimes[i] < min) {
                //�����ҵ����������ʱ�������
                min = copy_comingTimes[i];
                earliestProcess = i;
            }
        }
        //�ҵ������ý��̶�Ӧ��ʱ�����
        copy_comingTimes[earliestProcess] = MAX_DURANCE;
        //��һ������ִ����ʱ�п����¸����̻�û����
        if (comingTimes[earliestProcess] > current) {
            current = comingTimes[earliestProcess];
        }
        //���������ʱ�� = ����ʱ��+����ʱ��
        finishedTimes[earliestProcess] = current + serveTimes[earliestProcess];
        //�ȴ�ʱ�� �ȴ�ʱ�� = ��ǰʱ�� - ����ʱ��
        waitingTimes[earliestProcess] = current - comingTimes[earliestProcess];
        //���µ�ǰʱ�� ��ǰʱ�� = ��ǰʱ�� + ����ʱ��
        current += serveTimes[earliestProcess];
        //������תʱ�� ��תʱ�� = ����ʱ�� + �ȴ�ʱ��
        turnoverTimes[earliestProcess] = serveTimes[earliestProcess] + waitingTimes[earliestProcess];
        //���´�Ȩ��תʱ�� ��Ȩ��תʱ�� = ��תʱ�� /����ʱ��
        turnoverTimesWeight[earliestProcess] =
                (float) turnoverTimes[earliestProcess] / (float) serveTimes[earliestProcess];
    }
}

/**
 * �̽������Ȱ취
 */
void SJF() {
    // 1. ���ó�ʼʱ��0
    int current = 0;
    // 2. ����һ�ݷ���ʱ��copy_serveTimes[],����ʱ��ĸ���copy_comingTimes[]
    int copy_serveTimes[processCounts], copy_comingTimes[processCounts];
    for (int i = 0; i < processCounts; i++) {
        copy_serveTimes[i] = serveTimes[i];
    }
    for (int i = 0; i < processCounts; i++) {
        copy_comingTimes[i] = comingTimes[i];
    }
    //3. flag ��ʶ��ǰʱ���������Ѿ�����Ľ��� 1��0��
    int flag = 0;
    for (int i = 0; i < processCounts; i++) {
        // min_p: ��ǰ���Ƚ��̵��±�
        // min: ��ǰ���Ƚ��̵ķ���ʱ��
        // early:��ǰ���Ƚ��̵ĵ���ʱ��
        int min_p = 0, min = copy_serveTimes[0], early = copy_comingTimes[0];
        for (int j = 1; j < processCounts; j++) {
            //���̵������Ҹý��̷���ʱ��С
            if (copy_comingTimes[j] <= current && copy_serveTimes[j] < min) {
                flag = 1;
                min = copy_serveTimes[j];
                min_p = j;
            }
        }
        //����ǰʱ���޽��̵����ѡ�񼴽����絽����Ǹ�����
        if (flag == 0) {
            for (int m = 1; m < processCounts; m++) {
                if (copy_comingTimes[m] < early) {
                    early = copy_comingTimes[m];
                    min_p = m;
                    current = early;
                }
            }
        }
        // ���õ���ʱ�� ����ʱ��Ϊ ����
        copy_comingTimes[min_p] = MAX_DURANCE;
        copy_serveTimes[min_p] = MAX_DURANCE;
        //���������ʱ�� = ����ʱ��+����ʱ��
        finishedTimes[min_p] = current + serveTimes[min_p];
        //�ȴ�ʱ�� �ȴ�ʱ�� = ��ǰʱ�� - ����ʱ��
        waitingTimes[min_p] = current - comingTimes[min_p];
        //������תʱ�� ��תʱ�� = ����ʱ�� + �ȴ�ʱ��
        current = finishedTimes[min_p];
        //������תʱ�� ��תʱ�� = ����ʱ�� + �ȴ�ʱ��
        turnoverTimes[min_p] = waitingTimes[min_p] + serveTimes[min_p];
        //���´�Ȩ��תʱ�� ��Ȩ��תʱ�� = ��תʱ�� /����ʱ��
        turnoverTimesWeight[min_p] = (float) turnoverTimes[min_p] / (float) serveTimes[min_p];
    }
}

/**
 * ���ȼ������㷨(����ռʽ)
 */
void NPS() {
    //�ҵ�����ִ�е����� work_list
    int copy_comingTimes[processCounts];
    for (int i = 0; i < processCounts; i++) {
        copy_comingTimes[i] = comingTimes[i];
    }
    int work_list[processCounts];
    for (int i = 0; i < processCounts; i++) {//Ԫ�ش���ִ�еĽ�����p_list�е��±꣬�±����ִ��˳��
        work_list[i] = i;
    }
    //�Ȱ�����ʱ������
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
    for (int i = 0; i < processCounts - 1; i++) {//�ٰ����ȼ�����
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
    // 3. �����������絽��Ľ���
    for (int j = 0; j < processCounts; j++) {
        // ������Ĭ�����絽��Ľ���earliest��0��Ĭ�ϵ�1�����̵���ʱ������ priorCur��ǰ�������ȼ�
        //���������ʱ�� = ����ʱ��+����ʱ��
        finishedTimes[work_list[j]] = current + serveTimes[work_list[j]];
        //�ȴ�ʱ�� �ȴ�ʱ�� = ��ǰʱ�� - ����ʱ��
        waitingTimes[work_list[j]] = current - comingTimes[work_list[j]];
        //���µ�ǰʱ�� ��ǰʱ�� = ��ǰʱ�� + ����ʱ��
        current += serveTimes[work_list[j]];
        //������תʱ�� ��תʱ�� = ����ʱ�� + �ȴ�ʱ��
        turnoverTimes[work_list[j]] = serveTimes[work_list[j]] + waitingTimes[work_list[j]];
        //���´�Ȩ��תʱ�� ��Ȩ��תʱ�� = ��תʱ�� /����ʱ��
        turnoverTimesWeight[work_list[j]] =
                (float) turnoverTimes[work_list[j]] / (float) serveTimes[work_list[j]];
    }
}

/**
 * ��������
 */
void exchange(int x[], int i, int j) {
    int temp = x[i];
    x[i] = x[j];
    x[j] = temp;
}

/**
 * ���ȼ������㷨(��ռʽ)
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
//    for (int i = 0; i < processCounts; i++) {//Ԫ�ش���ִ�еĽ�����p_list�е��±꣬�±����ִ��˳��
//        work_list[i] = i;
//    }
//    //�Ȱ�����ʱ������
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
//    // 3. �����������絽��Ľ���
//    for (int j = 0; j < processCounts; j++) {
//        // ������Ĭ�����絽��Ľ���earliest��0��Ĭ�ϵ�1�����̵���ʱ������ priorCur��ǰ�������ȼ�
//        //���������ʱ�� = ����ʱ��+����ʱ��
//        finishedTimes[work_list[j]] = current + serveTimes[work_list[j]];
//        //�ȴ�ʱ�� �ȴ�ʱ�� = ��ǰʱ�� - ����ʱ��
//        waitingTimes[work_list[j]] = current - comingTimes[work_list[j]];
//        //���µ�ǰʱ�� ��ǰʱ�� = ��ǰʱ�� + ����ʱ��
//        current += serveTimes[work_list[j]];
//        //������תʱ�� ��תʱ�� = ����ʱ�� + �ȴ�ʱ��
//        turnoverTimes[work_list[j]] = serveTimes[work_list[j]] + waitingTimes[work_list[j]];
//        //���´�Ȩ��תʱ�� ��Ȩ��תʱ�� = ��תʱ�� /����ʱ��
//        turnoverTimesWeight[work_list[j]] =
//                (float) turnoverTimes[work_list[j]] / (float) serveTimes[work_list[j]];
//    }
}

/**
 * ��ӡ������Ϣ
 */
void PrintSchedule() {
    cout << "==>���̵�����Ϣ��ӡ:" << endl;
    cout << "P_name(ID)    arrived_time    served_time    finished_time    turnover_time    turnover_time_weight"
         << endl;
    for (int i = 0; i < processCounts; i++) {
        printf("%10d    %12d    %11d    %14d    %13d    %20f\n", i, comingTimes[i], serveTimes[i], finishedTimes[i],
               turnoverTimes[i], turnoverTimesWeight[i]);
    }
    //ƽ����תʱ�������תʱ��
    float average_turnover_time, sum_turnover_time = 0;
    //ƽ����Ȩ��תʱ����ܴ�Ȩ��תʱ��
    float average_turnover_time_weight, sum_turnover_time_weight = 0;
    for (int i = 0; i < processCounts; i++) {
        sum_turnover_time += turnoverTimes[i];
        sum_turnover_time_weight += turnoverTimesWeight[i];
    }
    average_turnover_time = sum_turnover_time / processCounts;
    average_turnover_time_weight = sum_turnover_time_weight / processCounts;
    cout << "==>ƽ����תʱ��Ϊ��" << average_turnover_time << endl;
    cout << "==>��Ȩƽ����תʱ��Ϊ��" << average_turnover_time_weight << endl;
}

/**
 * ��ӡ������Ϣ
 */
void PrintSchedule_pri() {
    cout << "==>���̵�����Ϣ��ӡ:" << endl;
    cout
            << "P_name(ID)      arrived_time      served_time      priority      finished_time      turnover_time      turnover_time_weight"
            << endl;
    for (int i = 0; i < processCounts; i++) {
        printf("%10d    %12d    %11d    %11d    %14d    %13d    %20f\n", i, comingTimes[i], serveTimes[i], priority[i],
               finishedTimes[i],
               turnoverTimes[i], turnoverTimesWeight[i]);
    }
    //ƽ����תʱ�������תʱ��
    float average_turnover_time, sum_turnover_time = 0;
    //ƽ����Ȩ��תʱ����ܴ�Ȩ��תʱ��
    float average_turnover_time_weight, sum_turnover_time_weight = 0;
    for (int i = 0; i < processCounts; i++) {
        sum_turnover_time += turnoverTimes[i];
        sum_turnover_time_weight += turnoverTimesWeight[i];
    }
    average_turnover_time = sum_turnover_time / processCounts;
    average_turnover_time_weight = sum_turnover_time_weight / processCounts;
    cout << "==>ƽ����תʱ��Ϊ��" << average_turnover_time << endl;
    cout << "==>��Ȩƽ����תʱ��Ϊ��" << average_turnover_time_weight << endl;
}

/**
 * ��ʼ������
 */
void Initialize() {
    // 1. ���ʱ��
    finishedTimes = new int[processCounts];
    // 2. ��תʱ��
    turnoverTimes = new int[processCounts];
    // 3. �ȴ�ʱ��
    waitingTimes = new int[processCounts];
    // 4. ��Ȩ��תʱ��
    turnoverTimesWeight = new float[processCounts];
}

