#include <iostream>
#include "function.h"
#include "class_define.h"
using namespace std;
int main() {
    int choice;
    while(true){
        display_menu();
        cin >> choice;
        switch (choice){
            case 1:
                create_compress();
                break;
            case 2:
                direct_compress();
                break;
            case 3:
                decompress();
                break;
            default:
                cout << "*你的选择有误！请重新输入" << endl;
                continue;
        }
        break;
    }
    return 0;
}