#include <stdio.h>

#define INFINITY 10000	//�ٶ�һ���ϴ����10000���ף�����Ϊ����ͨ����������
#define DONE	1	//��һ�����·���Ѿ�������ȷ������ô����·���ϵĵ��״̬ΪDONE
#define UNDONE	0	//û������ȷ��Ϊ���·�ߣ�������·���ϵĵ��״̬ΪUNDONE

/*����һ��ͼ�Ľṹ��*/
struct GRAPH 
{	
	char vertexsName[10][20];	//����һ���ַ�������,����ͼ�ж�����ʵ���еص����ʵ���ơ���ͼ���������10�����㣨��������ÿ���������19���֡�
	int arcs[10][10];	//����һ���ڽӾ���������������ߵ�Ȩֵ�������Ȩֵ�����������ڵص��ʵ�ʾ��룬����Ϊ��λ��
	int vertexsNum, arcsNum;	//��ǰ�ߺ͵������
};
/*��������*/
void shortestpath(struct GRAPH orientedGraph, int v0, int*DISTANCEptr, int*PATHptr[10]);	//��v0Ϊ��ʼ�㣬��v0��ʣ�µ����̾�������·��
void displaySiteTable(char vertexsName[10][20]);	//��ʾ�������±�Ķ�Ӧ��
int startPoint();	//�������
int endPoint();	//�����յ�
void displayShortestInfo(int distance[10], int path[10][10], char name[10][20],int endPoint);	//������

//���������һ����Ȩ����ͼ���˹�����ʱ��ʵ��
/*
void creatGraph(int vertexsNum, int arcsNum)
{
	scanf("%d%d", &vertexsNum, &arcsNum);	//�����������ͼ�Ķ����������������

	//�ڽӾ����ʼ����ʹ��һ��ʼʮ���ص㲻��ͨ��
	orientedGraph.arcs[10][10] = { 0 };
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			orientedGraph.arcs[i][j] = 10000;	//�ٶ�һ���ϴ����1000���ף�����Ϊ����ͨ����������
		}
	}
	//��ʼ����ϣ���ʱ��ͼΪ��ͼ��

	//¼���ͱ�
	for (int i = 0; i < 10; i++)
	{
		scanf("%s", orientedGraph.vertexsName[i]);
	}	//��ͼ�еĵ㸳����ʵ����
	for (int i = 0; i < orientedGraph.vertexsNum; i++)	//������Щ�㣬����Щ��Ϊ��㣬����¼�������
	{
		printf("%s", orientedGraph.vertexsName[i]);
		for (int j = 0; j < orientedGraph.vertexsNum-1; j++)	//ÿ������ڽ��������Ŀ�ض�С�ڶ�����-1
		{
			int k = 100;
			scanf("%d", &k);	//�����յ��Ӧ�±�
			if (-1 == k) break;	//����-1�˳��������¼�롣
			scanf("%d", &orientedGraph.arcs[i][k]);	//���������ߵ�Ȩֵ
		}

	}
	//¼����� 
}
*/

/*
	�ഺ�㳡	һ��B	ͼ���	����	�ۺ�¥C	����ѧԺ	���ѧԺ	һ��¥	��ѧԺ	������
	0			1		2		3		4		5			6			7		8		9

	0	1	2	3	4	5	6	7	8	9

0	000	354	505	*	*	*	*	*	*	*
1	354	000	296	443	*	*	*	*	*	*
2	505 296	000	335	351	257	297	*	*	*
3	*	*	335	000	224	*	*	*	*	*
4	*	477	351	224	000	*	*	303	*	*
5	*	*	257	*	*	000	*	*	*	*
6	*	*	297	*	*	*	000	401	174	*
7	*	*	*	*	303	*	401	000	*	152
8	*	*	*	*	*	531	174	*	000	*
9	*	*	*	*	*	*	*	152	348	000
*/


int main() {
	struct GRAPH orientedGraph = {
		{ "�ഺ�㳡","һ��B","ͼ���","����", "�ۺ�¥C", "����ѧԺ", "���ѧԺ", "һ��¥", "��ѧԺ", "������" },
		{
			{ 0,     354,   505,   10000, 10000, 10000, 10000, 10000, 10000, 10000 },
			{ 354,   0,     296,   443,   10000, 10000, 10000, 10000, 10000, 10000 },
			{ 505,   296,   0,     335,   351,   257,   297,   10000, 10000, 10000 },
			{ 10000, 10000, 335,   0,     224,   10000, 10000, 10000, 10000, 10000 },
			{ 10000, 477,   351,   224,   0,     10000, 10000, 303,   10000, 10000 },
			{ 10000, 10000, 257,   10000, 10000, 0,     10000, 10000, 10000, 10000 },
			{ 10000, 10000, 297,   10000, 10000, 10000, 0,     401,   174,   10000 },
			{ 10000, 10000, 10000, 10000, 303,   10000, 401,   0,     10000, 152 },
			{ 10000, 10000, 10000, 10000, 10000, 531,   174,   10000, 0,     10000 },
			{ 10000, 10000, 10000, 10000, 10000, 10000, 10000, 152,   348,   0 } 
		},
		10,
		28
	};	
	//����һ������ͼ,��ʼ���������Լ���ͼ
	int distance[10] = { 0 };	//���������v0���������̾���
	int* distancePtr = distance;	
	int path[10][10] = { 0 };	//���������v0����������·��
	int* pathPtr[10];
	for (int i = 0; i < 10; i++) { pathPtr[i] = path[i]; }
	//������·�����Ⱥ�·��������������Ӧָ���ʼ��
	
	/*������*/
	puts(" ");
	puts("********************************************************************");
	puts("                     ��ӭʹ�ô����ͼV1.0");
	puts("********************************************************************\n");
	int  mainOption = 4;
	do
	{
		puts(" ");
		puts("��ѡ�������");
		puts(" 1����ѯ��·\t 0���˳�ϵͳ ");
		scanf("%d", &mainOption);	//�����ͷ����0��1��2����ô���벻�����ܣ�scanf�����أ�optionΪ4����
        puts("");
		/*������0��1��2���治��\n������ʾ�������*/
		if (getchar() != '\n')	//�������һ���ַ�
		{
			mainOption = 4;	//������Ч��switch����defaultѡ��
			while (getchar() != '\n');	//��ջ���
		}

		//���˵�
		switch (mainOption)
		{
		case 0:
			break;
		case 1:
			displaySiteTable(orientedGraph.vertexsName);
			shortestpath(orientedGraph, startPoint(), distancePtr, pathPtr);
			displayShortestInfo(distance,path,orientedGraph.vertexsName,endPoint());
			break;
		default:
			puts("������Ч�����������룡");
			break;
		}
	} while (mainOption != 0);
	return 0;
}

void shortestpath(struct GRAPH orientedGraph, int v0, int*DISTANCEptr, int*PATHptr[10])	//��v0Ϊ��ʼ�㣬��v0��ʣ�µ����̾�������·��
{
	int distance[10] = { 0 };	//������� ��̬�� ��� ��ǰ״̬�� ��㵽��������������·�߳���
								/*������� ��̬�� ��� ��ǰ״̬�� ��㵽��������������·�ߵ�,��ʼ��Ϊ-1*/
	int path[10][10];
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			path[i][j] = -1;
		}
	}
	int final[10] = { UNDONE };	//������飬��������Ƿ��Ѿ����v0������������·��

	/*��ʼ��distance��path��
	�������v0��v0�����·�߳��Ⱥ͵㣬
	�൱�ڸ�����һ�����·��*/
	for (int i = 0; i < 10; i++)
	{
		distance[i] = orientedGraph.arcs[v0][i];	//��distance��ʼ����һ��ʼ�ٶ����v0����������vi�����·��Ϊ��v0��vi��
		path[i][0] = v0;
		path[i][1] = i;		//��path��ʼ��
	}
	distance[v0] = 0;
	final[v0] = DONE;	//v0��v0�ľ���Ϊ0����v0��v0����·���Ѿ�ȷ��
	/*��ʼ����ɣ���ʱ��״̬Ϊ��
	1.�Ѿ�����˵�һ�����·�ߵĸ���
	2.��û�п�ʼ�ҶԵ�һ������̵�·��distanceMin
	3.����û���ҵ���һ���е���̵�·��distanceMin����û�ж�·�߽��и��£���û�н���ڶ��ˡ�
	4.�����v0��v0�����·��·�ߣ������·�߱����ΪDONE״̬
	5.������������ѭ��*/

	/*��ѭ����ʼ��˼·Ϊ��
	�Ȼ��ڵ�n-1��ѭ����·�ߵĸ��£���õ�n��ѭ���е�distanceMin����ɵ�n���㷨��
	���ڵ�n��ѭ���н��е�n+1���㷨��·�ߵĸ��£�Ϊ��n+1��ѭ������distanceMin������ɵ�n+1���㷨���̵�*/
	for (int i = 0; i < orientedGraph.vertexsNum - 1; i++)	//ֻ��Ҫ���� �ܶ�����-1 ��
	{
		int distanceMin = INFINITY;	//ÿ��ѭ����Ҫˢ��һ�£���ȷ��ֻ�ڵ�ΪUNDONE��·��������Сֵ
		int vertexMin = 10;	//��ʾ��ǰ������̵�·�����յ㣬��ʼ��Ϊ10������ԡ�

		for (int v = 0; v < 10; v++)	//v for vertex���ҳ���δ������·������̵�����·�ߣ�����䳤�Ⱥ��յ�
		{
			if (final[v] == UNDONE && distance[v] < distanceMin)
			{
				vertexMin = v;
				distanceMin = distance[v];
			}

		}
		final[vertexMin] = DONE;	//����ȷ������һ�����·��

		for (int v = 0; v < 10; v++)	//�������·�߳��Ⱥ�·��
		{
			if ((final[v] == UNDONE) && (distanceMin + orientedGraph.arcs[vertexMin][v] < distance[v]))
			{
				distance[v] = distanceMin + orientedGraph.arcs[vertexMin][v];

				int j = 0;
				while (path[vertexMin][j] != -1)
				{
					path[v][j] = path[vertexMin][j];
					j++;
				}
				path[v][j] = v;
			}
		}
	}
	/*��ѭ������*/

	/*��������*/
	for (int i = 0; i < 10; i++)
	{
		*(DISTANCEptr + i) = distance[i];
		for (int k = 0; k < 10; k++)
		{
			*(PATHptr[i] + k) = (path[i][k]);
		}
	}
}
void displaySiteTable(char vertexsName[10][20])	//��ʾ�������±�Ķ�Ӧ��
{
	puts("�������ʾ������Ӧ������Ӧ�ı��!");
	puts("������Ӧ�ı�����£�");
	puts("");
	for (int i = 0; i < 10; i++)
	{
		printf("%d��%-15s", i, vertexsName[i]);	//�����
		if ((i + 1) % 2 == 0)	puts("");	//2������
	}
	puts("");
	

}
int startPoint()	//�������
{
	int start = -1;
	int check = -1;

	do {
		printf("%s","����������������������������Ӧ�����֣� ");
		check = scanf("%d", &start);	//�������
		if(start<0||start>9){check=-1;}
		if (getchar() != '\n')	//����������ַ�
		{ 
			check = -1;	//��ͨ��ѭ�������ܳɹ�����
			while (getchar() != '\n');	//��ջ���
		}
		if (check != 1) { puts("������Ч�����������룡"); }
	} while (check != 1);
	return start;
}
int endPoint()	//�����յ�
{
	int end = -1;
	int check = -1;

	do {
		printf("%s","�����㵽���������������Ӧ�����֣�");
		check = scanf("%d", &end);	//�����յ�
		if(end<0||end>9){check=-1;}
		if (getchar() != '\n')	//����������ַ�
		{
			check = -1;	//��ͨ��ѭ�������ܳɹ�����
			while (getchar() != '\n');	//��ջ���
		}
		if (check != 1) { puts("������Ч�����������룡"); }
	} while (check != 1);
	return end;
}
void displayShortestInfo(int distance[10], int path[10][10],char name[10][20],int endPoint)
{
     puts("");
	printf("%s\t", "���·��Ϊ��");
	int i = 0;
	do {
        if(i!=0){printf("%s"," --> ");}
		printf("%s", name[path[endPoint][i]]);
		
		i++;
	} while (path[endPoint][i] != -1);
	puts("");
	printf("%s\t%d","����Ϊ��",distance[endPoint]);
	puts("");
	
}
