#include <iostream>
#include <stack>
#include <stdlib.h>
#include <string.h>
using namespace std;

const char operation[][2] = {{'*','/'},{'+','-'}};
char temp,ch;
double total = 0,temp_total = 0,leftNum,rightNum;

int getPrior(char ch){
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (ch == operation[i][j]){
                return i;
            }
        }
    }
    return 10;//仅仅是为了消除一个警告，让某个符号能够放到括号上边
}
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
        } else{
            temp = strtod(str_num, nullptr);
        }
        num.push(temp);
    } else{
        return ;
    }
}
void compute(stack<char> &oper,stack<double> &num){
    ch = oper.top();
    oper.pop();
    rightNum = num.top();
    num.pop();
    leftNum = num.top();
    num.pop();
    switch (ch){
        case '+':
            temp_total = leftNum + rightNum;
            break;
        case '-':
            temp_total = leftNum - rightNum;
            break;
        case '*':
            temp_total = leftNum * rightNum;
            break;
        case '/':
            temp_total = leftNum / rightNum;
            break;
        default:
            break;
    }
    total = temp_total;
    num.push(temp_total);
    cout << leftNum << " " << rightNum << " " << ch << " ";
}
int main() {
    char str[1024];
    bool error = true;
    stack<double> num;
    stack<char> oper;
    int counter,sign,sign_judge;//sign_judge表示检测非法字符
    
    while (scanf("%s",str) && strcmp(str,"stop") != 0){
        sign_judge = 1;
        for (int i = 0; i < strlen(str); ++i) {
            if (!(str[i] >= '0' && str[i] <= '9') && str[i] != '.' && str[i] != '+' && str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '#' && str[i] != '(' && str[i] != ')'){
                cout << "表达式错误->出现非法字符！" << endl;
                sign_judge = 0;
                break;
            }
        }
        if (!sign_judge)
            continue;
        cout << "逆波兰表达式：";
        oper = stack<char>();
        num = stack<double>();
        counter = 0,sign = 0;//sign标志位，标志出后一个数字的正负号

        while (counter <= strlen(str) - 1){
            getNum(counter,str,num,sign);
            temp = str[counter];
            if (counter <= strlen(str) - 1 && temp != '\0'){
                if (temp == '#'){
                    sign = 1;
                } else{
                    if (temp == ')'){
                        while (oper.top() != '('){
                            compute(oper,num);
                        }
                        oper.pop();
                    } else{
                        if (oper.empty() || getPrior(oper.top()) > getPrior(temp) || temp == '('){
                            oper.push(temp);
                        } else{
                            while (!oper.empty() && getPrior(oper.top()) <= getPrior(temp)){
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
                cout << "表达式错误->括号未闭合!" << endl;
                error = false;
                break;
            }
            compute(oper,num);
        }
        cout << endl;
        if (error){
            cout << total << endl;
        }
    }
    return 0;
}