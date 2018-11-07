#include <iostream>
#include <stack>
#include <stdlib.h>
#include <string.h>
using namespace std;

const char operation[][2] = {{'*','/'},{'+','-'}};
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
int main() {
    double total = 0,temp_total = 0;
    char str[1024];
    stack<double> num = stack<double>();
    stack<char> oper = stack<char>();
    scanf("%s",str);
    int counter = 0,sign = 0;//sign标志位，标志出后一个数字的正负号
    char temp,ch;
    double left,right;

    while (counter <= strlen(str) - 1){
        getNum(counter,str,num,sign);
        temp = str[counter];
        if (counter <= strlen(str) - 1 && temp != '\0'){
            if (temp == '#'){
                sign = 1;
            } else{
                if (temp == ')'){
                    while (oper.top() != '('){
                        ch = oper.top();
                        oper.pop();
                        right = num.top();
                        num.pop();
                        left = num.top();
                        num.pop();
                        switch (ch){
                            case '+':
                                temp_total = left + right;
                                break;
                            case '-':
                                temp_total = left - right;
                                break;
                            case '*':
                                temp_total = left * right;
                                break;
                            case '/':
                                temp_total = left / right;
                                break;
                            default:
                                break;
                        }
                        total = temp_total;
                        num.push(temp_total);
                    }
                    oper.pop();
                } else{
                    if (oper.empty() || getPrior(oper.top()) > getPrior(temp) || temp == '('){
                        oper.push(temp);
                    } else{
                        while (!oper.empty() && getPrior(oper.top()) <= getPrior(temp)){
                            ch = oper.top();
                            oper.pop();
                            right = num.top();
                            num.pop();
                            left = num.top();
                            num.pop();
                            switch (ch){
                                case '+':
                                    temp_total = left + right;
                                    break;
                                case '-':
                                    temp_total = left - right;
                                    break;
                                case '*':
                                    temp_total = left * right;
                                    break;
                                case '/':
                                    temp_total = left / right;
                                    break;
                                default:
                                    break;
                            }
                            total = temp_total;
                            num.push(temp_total);
                        }
                        oper.push(temp);
                    }
                }
            }
            counter ++;
        }
    }
    while(!oper.empty()){
        ch = oper.top();
        oper.pop();
        right = num.top();
        num.pop();
        left = num.top();
        num.pop();
        switch (ch){
            case '+':
                temp_total = left + right;
                break;
            case '-':
                temp_total = left - right;
                break;
            case '*':
                temp_total = left * right;
                break;
            case '/':
                temp_total = left / right;
                break;
            default:
                break;
        }
        total = temp_total;
        num.push(temp_total);
    }
    cout << total;
    return 0;
}