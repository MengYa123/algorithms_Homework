#include <iostream>
#include <stack>
#include <stdlib.h>
#include <string.h>
using namespace std;
/**
 * @description 此处为该程序的全局变量定义部分
 * @global_variable operation代表一个保存着操作符优先级的二维数组
 * @global_variable temp是一个临时保存读取到的运算符的变量
 * @global_variable ch代表在运算函数中使用的运算符临时变量
 * @global_variable total保存表达式最后的结果
 * @global_variable temp_total是一个保存每一步结果的临时变量
 * @global_variable leftNum代表运算过程中的左操作数
 * @global_variable rightNum代表运算过程中的右操作数
 * @global_variable str保存着每次读取到的表达式字符串
 * @global_variable error表示表达式的运算过程中是否有错（true为无错误，可以输出结果；false为出现错误，不输出结果）
 */
const char operation[][2] = {{'*','/'},{'+','-'}};
char temp,ch;
double total = 0,leftNum,rightNum;
char str[1024];
bool error,divided_zero,oper_err;
int counter,sign,sign_judge;//sign_judge非法字符错误
/**
 * @param ch 表示读取到的非数字字符
 * @return 返回该字符所在的优先级，用于后续的入栈判断
 */
int getPrior(char ch){
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            if (ch == operation[i][j])
                return i;
    return 10;//输出一个很大的优先级
}
/**
 * @param counter 表示对输入的表达式读取的当前字符的位置
 * @param str 读取到的表达式字符串
 * @param num 操作数栈
 * @param sign 用于保存该读取数字的正负号，1代表负号，0代表正号
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
 * @param oper 运算符栈
 * @param num 操作数栈
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
 * @return 0,表示退出该主函数，结束程序的执行
 * @variable num和oper分别为操作数栈和操作符栈
 */
int main() {
    stack<double> num;
    stack<char> oper;
    cout << "请输入表达式:";
    while (cin >> str && strcmp(str,"stop") != 0){
        error = true;
        divided_zero = false;
        oper_err= false;
        sign_judge = 1;
        for (int i = 0; i < strlen(str); ++i)
            if (!(str[i] >= '0' && str[i] <= '9') && str[i] != '.' && str[i] != '+' && str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '#' && str[i] != '(' && str[i] != ')'){
                cout << "发生错误->出现非法字符" << endl;
                error = false;
                sign_judge = 0;
                break;
            }
        if (!sign_judge)
            continue;
        cout << "逆波兰表示法：";
        oper = stack<char>();
        num = stack<double>();
        counter = 0,sign = 0;//sign表示正负号
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
                            cout << endl << "发生错误->右括号未匹配!";
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
                cout  << endl <<"发生错误->左括号未匹配!" << endl;
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
            cout << "表达式有误->操作符个数不正确" << endl;
        else if (divided_zero)
            cout << "表达式有误->除数为零" << endl;
        else if (oper_err)
            cout << "表达式有误->操作符位置错误" << endl;
        else
            if (error)
                cout << num.top() << endl;
            else
                cout << "表达式有误->操作数个数不正确" << endl;
        cout << "请输入表达式:";
    }
    return 0;
}