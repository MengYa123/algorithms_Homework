//
// Created by MengWork on 2018/11/21.
//
#include <iostream>
#include <stack>
#include "function.h"
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include "class_define.h"
#include <string.h>
#include <string>
#include <algorithm>
using namespace std;
char str_in[1024];
int ch_num[1024];
char ch_ch[1024];
int temp_ch_num[1024];
char temp_ch_ch[1024];

/**
 * @description 对字符序列进行排序
 * @param node
 * @param n
 */
void sort(stack<TNode*> &stack_node,int n){
    TNode *temp_node,*node[n];
    for (int k = 0; k < n; ++k) {
        node[k] = stack_node.top();
        stack_node.pop();
    }
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (node[j]->count < node[j+1]->count){
                temp_node = node[j];
                node[j] = node[j+1];
                node[j+1] = temp_node;
            }
        }
    }
    for (int k = 0; k < n; ++k) {
        stack_node.push(node[k]);
    }
}
void display_menu(){
    cout << "***************************" << endl;
    cout << "*    1.创建文件并压缩      *" << endl;
    cout << "*    2.选择文件压缩        *" << endl;
    cout << "*    3.Huffman解压缩       *" << endl;
    cout << "***************************" << endl;
    cout << "*请键入您的选择：";
}
void create_compress(){
    cout << "请输入您所要创建的文件名：";

}
void direct_compress(){
    stack<TNode*> stack_node = stack<TNode*>();
    cout << "请输入您所要压缩的文件名：";
    cin >> str_in;
    char temp_ch;
    int counter = 0;
    ifstream in_file;
    ofstream out_file;
    in_file.open(str_in,ios::in);
    while (in_file >> temp_ch){
        ch_num[int(temp_ch)]++;
        ch_ch[int(temp_ch)]=temp_ch;
    }
    in_file.close();
    for (int i = 0; i < 1024; ++i) {
        if (ch_ch[i] != '\0'){
            temp_ch_ch[counter] = ch_ch[i];
            temp_ch_num[counter] = ch_num[i];
            counter++;
        }
    }
    char ch_charset[counter][1024];
    for (int j = 0; j < counter; ++j) {
        stack_node.push(new TNode(temp_ch_num[j],temp_ch_ch[j]));
    }
    stack<TNode*> stack_node_copy = stack_node;
    TNode *left_node,*right_node;
    for (int k = 0; k < counter - 1; ++k) {
        sort(stack_node,counter - k);
        left_node = stack_node.top();
        stack_node.pop();
        right_node = stack_node.top();
        stack_node.pop();
        TNode* temp_node =new TNode(left_node->count+right_node->count,' ',left_node,right_node);
        left_node->parent = temp_node;
        right_node->parent = temp_node;
        stack_node.push(temp_node);
    }
    TNode *temp;
    int code_counter = 0,code_counter_1 = 0;
    while(!stack_node_copy.empty()){
        code_counter = 0;
        char temp_coding[100] = "";
        char coding[100] = "";
        temp = stack_node_copy.top();
        stack_node_copy.pop();
        temp_ch = temp->data;
        while(temp->parent != nullptr){
            if (temp->parent->lchild == temp) {
                temp_coding[code_counter_1] = '0';
                code_counter_1++;
            }else if(temp->parent->rchild == temp){
                temp_coding[code_counter_1] = '1';
                code_counter_1++;
            }temp = temp->parent;
        }
        for (int j = code_counter_1 - 1; j >= 0 ; --j) {
            coding[code_counter] = temp_coding[j];
            code_counter++;
        }
        for (int i = 0; i < counter; ++i) {
            if (temp_ch_ch[i] == temp_ch)
                strcpy(ch_charset[i],coding);
        }
    }
    code_counter_1 = 0;
    char temp_code[8];
    int temp_eight = 0;
    in_file.open(str_in,ios::in);
    out_file.open("compress.txt",ios::out|ios::binary);

    //将哈编码信息储存在文件中

    while (in_file >> temp_ch){
        for (int i = 0; i < counter; ++i) {
            if (temp_ch_ch[i] == temp_ch)
                for (int j = 0; j < strlen(ch_charset[i]); ++j) {
                    temp_code[code_counter_1] = ch_charset[i][j];
                    code_counter_1++;
                    if (code_counter_1 == 8){
                        for (int k = 0; k < 8; ++k) {
                            if (temp_code[k] == '1'){
                                temp_eight += (int)pow(2,7-k);
                            }
                        }
                        out_file << (char)temp_eight;
                        code_counter_1 = 0;
                    }
                }
        }
    }
    //最后不足八位补齐后输出
    if (code_counter_1 != 0){
        for (int i = code_counter; i < 8; ++i) {
            temp_code[i] = '0';
        }
        for (int k = 0; k < 8; ++k) {
            if (temp_code[k] == '1'){
                temp_eight += (int)pow(2,7-k);
            }
        }
        out_file << (char)temp_eight;
        out_file << '\n';
        out_file << (char)code_counter_1;
    }
    out_file.close();
}

void decompress(){
    cout << "请输入您所要解压缩的文件名：";

}