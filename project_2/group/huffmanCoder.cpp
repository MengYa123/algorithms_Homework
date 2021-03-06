// project2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "stdlib.h"
#include <iostream>
#include <bitset>
#include <string>
#include <map>
#include <fstream>
#include <queue>
#include<regex>
#include<windows.h>
using namespace std;
struct Tree {      
	int times;         
	char value;    
	Tree *leftNode ;
	Tree *rightNode ;
	int ready;
};
void saveTable(map<char, string> Table) {
	ofstream ofile;     //说明输入文件流对象ifile
	map<char, string>::iterator iter;
	ofile.open("example.txt");
	iter = Table.begin();
	while (iter != Table.end()) {
		if (iter->first == '\n') {
			ofile << "\\n" << iter->second << endl;
		}
		else if (iter->first == '\r')
		{
			ofile << "\\r" << iter->second << endl;
		}
		else {
			ofile << iter->first << iter->second << endl;
		}
		iter++;
	}
	ofile.close();
}
string readFile(string fileName) {
	string oneLine;
	string result = "";
	fstream fs;
	fs.open(fileName, ios::in | ios::_Nocreate);
	if (fs) {	
		while (!fs.eof()) {
			getline(fs, oneLine);
			if (oneLine.empty()) {
				result += "\r\n";
			}
			else {
				result += (oneLine + "\n");
			}
		}
	}
	else {
		cout << "File name false" << endl;
		exit(0);
	}
	fs.close();
	return result;
}  //读文件
void writeFile(string fileName, string content) {
	ofstream ofs;
	ofs.open(fileName);
	if (ofs) {
		ofs << content;
	}
	else {
		cout << "File name false" << endl;
		exit(0);
	}
	ofs.close();
}     //写文件
map<char, int> encodTable(string content) {
	map<char, int> result;
	for (unsigned int i = 0; i < content.size(); i++)
	{
		if (!result[content[i]]) {
			result[content[i]] = 1;
		}
		else {
			result[content[i]] += 1;
		}
	}
	return result;
}//字符出现次数
map<char,string> encodeing(string i,Tree t) {
	map<char, string> result;
	map<char, string> left;
	map<char, string> right;
	map<char, string>::iterator iter;
	if (t.leftNode!=0) {	
		left = encodeing(i + "0",*t.leftNode);		
		iter = left.begin();
		while (iter != left.end()) {
			result[iter->first] = iter->second;
			iter++;
		}
	}
	if (t.rightNode != 0)
	{
		right = encodeing(i+"1", *t.rightNode);
		iter = right.begin();
		while (iter != right.end()) {
			result[iter->first] = iter->second;
			iter++;
		}
	}
	if (t.leftNode == 0 && t.rightNode == 0)
	{
		result[t.value] = i;
		/*if (t.value == '\n') {
			std::cout << "\\n" << " :: " << i << endl;
		}
		else if (t.value == '\r')
		{
			std::cout << "\\r" << " :: " << i << endl;
		}
		else{
			std::cout << t.value << " :: " << i << endl;
		}*/
	}
	saveTable(result);
	return result;
}//字符与二进制编码
queue<Tree*> setQueue(map<char,int> source) {
	queue<Tree*> s;
	Tree d = {};
	int k = 0;
	Tree* group = new Tree[source.size()];
	map<char, int>::iterator iter;
	iter = source.begin();
	while (iter != source.end()) {
		group[k].times = iter->second;
		group[k].value = iter->first;
		group[k].leftNode = 0;
		group[k].rightNode = 0;
		k++;
		iter++;
	}
	int a = source.size();
	for (int i = 0; i < a; i++)
	{
		for (int j = i+1; j < a; j++)
		{
			if (group[i].times > group[j].times) {
				d = group[i];
				group[i] = group[j];
				group[j] = d;
			}
		}
	}

	for (int i = 0; i < a; i++)
	{
		s.push(&group[i]);
	}
	while (s.size()>1) {
		int i = 0;
		Tree* c =  new Tree();	
		c->leftNode = s.front();
		s.pop();
		c->rightNode = s.front();
		s.pop();		
		c->times = c->leftNode->times + c->rightNode->times;
		c->value = '\0';
		int g = s.size();
		if (s.size()!=0)
		{
			for (int i = 0; i < g + 1; i++)
			{
				if ((s.front()->times >= c->times && c->ready != 1) || (i == g && c->ready != 1)) {
					s.push(c);
					c->ready = 1;
				}
				else {
					s.push(s.front());
					s.pop();
				}
			}
		}
		else
		{
			s.push(c);
		}
	}
	
	return s;
}//建树
string enc(map<char, string> encodingTable,string content) {
	string result = "";
	string part;
	string aotherResult = "";
	int h = 0;
	int add = 1;
	char cha;
	for (unsigned int i = 0; i < content.size(); i++)
	{
		result += encodingTable[content[i]];
	}
	//cout << "result(old) :\n" << result.size() << endl;
	int longs = result.size();
	//cout << "long" << longs << endl;
	ofstream ofs;
	ofs.open("longth.txt");
	if (ofs) {
		ofs << longs;
	}
	else {
		cout << "File name false" << endl;
		exit(0);
	}
	ofs.close();
	while ((int)result.length() % 8 != 0) {
		result += "0";
	}
	//cout << "result :\n" << result.size() << endl;
	for (unsigned int i = 0; i + 8 < result.size();  i += 8) {
		h = 0;
		part = result.substr(i, 8);		
		for (int j = 0; j < 8; j++)
		{
			add = 1;
			if(part[7-j] == '1'){
				for (int k= 0; k < j; k++)
				{
					add = add * 2;
				}
				h += add;
			}
		}
		cha = h;
		//cout <<  endl;
		//cout << "part :" << part << endl;
		//cout << "h :" << h << endl;	
		//cout << "cha :" << cha << endl;
		aotherResult += cha;	
	}
	//std::cout << aotherResult << endl;
	return aotherResult;
}
//编码

map<char, string> readTable() {
	map<char, string> result;
	string aLine;
	ifstream ifile;
	ifile.open("example.txt");
	while (getline(ifile,aLine)) {
		if (aLine.substr(0,2)=="\\n")
		{
			result['\n'] = aLine.substr(2);
		}
		else if (aLine.substr(0,2)=="\\r")
		{
			result['\r'] = aLine.substr(2);
		}
		else {
			result[aLine[0]] = aLine.substr(1);

		}
	}
	ifile.close();
	return result;
}
string bitString(string content) {
	string result;
	int acii;
	for (unsigned int i = 0; i < content.size(); i++)
	{
		acii = content[i];
		bitset<8> bar(acii);
		result += bar.to_string();
	}
	return result;
}
string decodeTable(string bitContent , map<char, string> table) {
	string res;
	regex pattern;
	string stringResult = "";
	smatch result;
	map<char, string>::iterator iter;
	int l;
	//cout << bitContent.size() << endl;
	fstream fs;
	fs.open("longth.txt", ios::in | ios::_Nocreate);
	if (fs) {
		fs >> l;
	}
	else {
		cout << "File name false" << endl;
		exit(0);
	}
	fs.close();
	//cout << l << endl;
	bitContent = bitContent.substr(0, l);
	while (bitContent.size()!=0) {
		iter = table.begin();
		//cout << bitContent.size() << endl;
		while (iter != table.end()) {
			res = "^" + iter->second;
			pattern = regex(res);
			if (regex_search(bitContent, pattern)) {
				bitContent = bitContent.substr(iter->second.size());
				stringResult += iter->first;
				break;
			}if (++iter == table.end()) {
				bitContent = "";
				break;
			}
			
		}
	}
	return stringResult;
}



int main()
{
	int choose = 0;
	string fileName = "";
	string content;
	map<char, int> encodeTable;
	map<char, string> encode;
	string newFileName;
	while (true)
	{
		
		std::cout << "1、Huffman compress\n";
		std::cout << "2、Huffman decompress\n";
		std::cout << "3、Exit\n";
		std::cout << "Please select:";
		cin >> choose;
		cin.clear();
		cin.ignore(1024,'\n');
		switch ((int)choose)
		{
				
		case 1:
			cout << "Please input source file name(size less than 4GB):";
			cin >> fileName;
			content = readFile(fileName);
			encodeTable = encodTable(content);
			encode = encodeing("", *setQueue(encodeTable).back());
			content = enc(encode, content);
			cout << "Please input code file name:";
			cin >> newFileName;
			std::cout << "Pocessing..." << endl;
			Sleep(1000);
			std::cout << "Process end.\n\n\n" << endl;
			writeFile(newFileName, content);
			break;
		case 2:
			cout << "Please input code file name:";
			cin >> fileName;
			content = readFile(fileName);
			content = decodeTable(bitString(content), readTable());
			cout << "Please input target file name:";
			cin >> newFileName;
			std::cout << "Pocessing..." << endl;
			Sleep(1000);
			std::cout << "Process end.\n\n\n" << endl;
			writeFile(newFileName, content.substr(0,content.size()-1));
			break;
		case 3:
			exit(0);
		default:
			cout << "Error instruction " << choose << endl;
			std::cout << "\n\n\n" << endl;
			Sleep(1000);
			break;
		}
		
	}
	
}





// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

