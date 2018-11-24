#include <iostream>
#include <stack>
#include <stdlib.h>
#include <string.h>
using namespace std;
/**
 * @description �˴�Ϊ�ó����ȫ�ֱ������岿��
 * @global_variable operation����һ�������Ų��������ȼ��Ķ�ά����
 * @global_variable temp��һ����ʱ�����ȡ����������ı���
 * @global_variable ch���������㺯����ʹ�õ��������ʱ����
 * @global_variable total������ʽ���Ľ��
 * @global_variable temp_total��һ������ÿһ���������ʱ����
 * @global_variable leftNum������������е��������
 * @global_variable rightNum������������е��Ҳ�����
 * @global_variable str������ÿ�ζ�ȡ���ı��ʽ�ַ���
 * @global_variable error��ʾ���ʽ������������Ƿ��д�trueΪ�޴��󣬿�����������falseΪ���ִ��󣬲���������
 */
const char operation[][2] = {{'*','/'},{'+','-'}};
char temp,ch;
double total = 0,leftNum,rightNum;
char str[1024];
bool error,divided_zero,oper_err;
int counter,sign,sign_judge;//sign_judge�Ƿ��ַ�����
/**
 * @param ch ��ʾ��ȡ���ķ������ַ�
 * @return ���ظ��ַ����ڵ����ȼ������ں�������ջ�ж�
 */
int getPrior(char ch){
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            if (ch == operation[i][j])
                return i;
    return 10;//���һ���ܴ�����ȼ�
}
/**
 * @param counter ��ʾ������ı��ʽ��ȡ�ĵ�ǰ�ַ���λ��
 * @param str ��ȡ���ı��ʽ�ַ���
 * @param num ������ջ
 * @param sign ���ڱ���ö�ȡ���ֵ������ţ�1�����ţ�0��������
 */
void getNum(int &counter,const char* str,stack<double> &num,int &sign){
    char ch;
    double temp = 0;
    int counter_num = 0;
    char str_num[50];
    while (((ch = str[counter]) >= '0' && (ch = str[counter]) <= '9') || ch == '.' ){
        str_num[counter_num] = ch;
        counter_num ++;
        counter ++;
    }
    if (counter_num != 0){
        if (sign == 1){
            temp = strtod(str_num, nullptr) * (-1);
            sign = 0;
        } else
            temp = strtod(str_num, nullptr);
        num.push(temp);
        cout << temp << " ";
    } else
        if (sign == 1){
            error = false;
            return;
        } else
            return ;
}
/**
 * @param oper �����ջ
 * @param num ������ջ
 */
void compute(stack<char> &oper,stack<double> &num){
    if (!num.empty()){
        rightNum = num.top();
        num.pop();
    } else{
        oper.pop();
        error = false;
        return;
    }
    ch = oper.top();
    oper.pop();
    if (!num.empty()){
        leftNum = num.top();
        num.pop();
    } else{
        error = false;
        return;
    }
    switch (ch){
        case '+':
            total = leftNum + rightNum;
            break;
        case '-':
            total = leftNum - rightNum;
            break;
        case '*':
            total = leftNum * rightNum;
            break;
        case '/':
            if (rightNum == 0){
                divided_zero = true;
                return;
            }
            total = leftNum / rightNum;
            break;
        default:
            break;
    }
    num.push(total);
}
/**
 * @return 0,��ʾ�˳��������������������ִ��
 * @variable num��oper�ֱ�Ϊ������ջ�Ͳ�����ջ
 */
int main() {
    stack<double> num;
    stack<char> oper;
    cout << "��������ʽ:";
    while (cin >> str && strcmp(str,"stop") != 0){
        error = true;
        divided_zero = false;
        oper_err= false;
        sign_judge = 1;
        for (int i = 0; i < strlen(str); ++i)
            if (!(str[i] >= '0' && str[i] <= '9') && str[i] != '.' && str[i] != '+' && str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '#' && str[i] != '(' && str[i] != ')'){
                cout << "��������->���ַǷ��ַ�" << endl;
                error = false;
                sign_judge = 0;
                break;
            }
        if (!sign_judge)
            continue;
        cout << "�沨����ʾ����";
        oper = stack<char>();
        num = stack<double>();
        counter = 0,sign = 0;//sign��ʾ������
        while (counter <= strlen(str) - 1){
            getNum(counter,str,num,sign);
            temp = str[counter];
            if (counter <= strlen(str) - 1 && temp != '\0'){
                if (temp == '#'){
                    sign = 1;
                } else{
                    if (temp == ')'){
                        while (!oper.empty() && oper.top() != '('){
                            cout << oper.top() << " ";
                            compute(oper,num);
                        }
                        if (!oper.empty() && oper.top() == '('){
                            oper.pop();
                        } else{
                            cout << endl << "��������->������δƥ��!";
                            error = false;
                            break;
                        }

                    } else{
                        if (oper.empty() || getPrior(oper.top()) > getPrior(temp) || temp == '('){
                            oper.push(temp);
                        } else{
                            while (!oper.empty() && getPrior(oper.top()) <= getPrior(temp)){
                                cout << oper.top() << " ";
                                compute(oper,num);
                            }
                            oper.push(temp);
                        }
                    }
                }
                counter ++;
            }
        }
        while(!oper.empty()){
            if(oper.top() == '('){
                cout  << endl <<"��������->������δƥ��!" << endl;
                error = false;
                break;
            }
            if (!error){
                break;
            } else{
                cout << oper.top() << " ";
                compute(oper,num);
            }
        }
        cout << endl;
        if (!num.empty() && num.size() != 1)
            cout << "���ʽ����->��������������ȷ" << endl;
        else if (divided_zero)
            cout << "���ʽ����->����Ϊ��" << endl;
        else if (oper_err)
            cout << "���ʽ����->������λ�ô���" << endl;
        else
            if (error)
                cout << num.top() << endl;
            else
                cout << "���ʽ����->��������������ȷ" << endl;
        cout << "��������ʽ:";
    }
    return 0;
}