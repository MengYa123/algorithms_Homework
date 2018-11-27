//
// Created by MengWork on 2018/11/22.
//

#ifndef CLASS_2_CLASS_DEFINE_H
#define CLASS_2_CLASS_DEFINE_H

#endif //CLASS_2_CLASS_DEFINE_H

#include <iostream>
class TNode{
public:
    char data;
    TNode *lchild,*rchild,*parent = nullptr;
    int count;
    TNode(int count = 0,char data = ' ',TNode *lchild = nullptr,TNode *rchild = nullptr){
        this->count = count;
        this->data = data;
        this->rchild = rchild;
        this->lchild = lchild;
    }

};