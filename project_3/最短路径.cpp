#include <stdio.h>

#define INFINITY 10000	//假定一个较大的数10000（米），作为不连通的无穷大距离
#define DONE	1	//当一个最短路线已经被彻底确定后，那么这条路线上的点的状态为DONE
#define UNDONE	0	//没有最终确定为最短路线，这样的路线上的点的状态为UNDONE

/*构造一个图的结构体*/
struct GRAPH 
{	
	char vertexsName[10][20];	//构建一个字符串数组,保存图中顶点在实际中地点的真实名称。该图最多能容纳10个顶点（地名），每个地名最多19个字。
	int arcs[10][10];	//构建一个邻接矩阵，用来储存有向边的权值。这里的权值代表两个相邻地点的实际距离，以米为单位。
	int vertexsNum, arcsNum;	//当前边和点的数量
};
/*函数声明*/
void shortestpath(struct GRAPH orientedGraph, int v0, int*DISTANCEptr, int*PATHptr[10]);	//设v0为初始点，求v0到剩下点的最短距离和最短路径
void displaySiteTable(char vertexsName[10][20]);	//显示地名和下标的对应表
int startPoint();	//输入起点
int endPoint();	//输入终点
void displayShortestInfo(int distance[10], int path[10][10], char name[10][20],int endPoint);	//输出结果

//向程序输入一个带权有向图，此功能暂时不实现
/*
void creatGraph(int vertexsNum, int arcsNum)
{
	scanf("%d%d", &vertexsNum, &arcsNum);	//输入这个有向图的顶点数和有向边数。

	//邻接矩阵初始化，使得一开始十个地点不连通。
	orientedGraph.arcs[10][10] = { 0 };
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			orientedGraph.arcs[i][j] = 10000;	//假定一个较大的数1000（米），作为不连通的无穷大距离
		}
	}
	//初始化完毕，此时此图为零图。

	//录入点和边
	for (int i = 0; i < 10; i++)
	{
		scanf("%s", orientedGraph.vertexsName[i]);
	}	//把图中的点赋予真实地名
	for (int i = 0; i < orientedGraph.vertexsNum; i++)	//遍历这些点，以这些点为起点，依次录入有向边
	{
		printf("%s", orientedGraph.vertexsName[i]);
		for (int j = 0; j < orientedGraph.vertexsNum-1; j++)	//每个点的邻接有向边数目必定小于顶点数-1
		{
			int k = 100;
			scanf("%d", &k);	//输入终点对应下标
			if (-1 == k) break;	//输入-1退出这个起点的录入。
			scanf("%d", &orientedGraph.arcs[i][k]);	//输入该有向边的权值
		}

	}
	//录入完成 
}
*/

/*
	青春广场	一教B	图书馆	南门	综合楼C	建环学院	软件学院	一基楼	法学院	体育馆
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
		{ "青春广场","一教B","图书馆","南门", "综合楼C", "建环学院", "软件学院", "一基楼", "法学院", "体育馆" },
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
	//建立一个有向图,初始化放入我自己的图
	int distance[10] = { 0 };	//最终求出的v0到各点的最短距离
	int* distancePtr = distance;	
	int path[10][10] = { 0 };	//最终求出的v0到各点的最短路径
	int* pathPtr[10];
	for (int i = 0; i < 10; i++) { pathPtr[i] = path[i]; }
	//存放最短路径长度和路径于主函数，对应指针初始化
	
	/*主界面*/
	puts(" ");
	puts("********************************************************************");
	puts("                     欢迎使用川大地图V1.0");
	puts("********************************************************************\n");
	int  mainOption = 4;
	do
	{
		puts(" ");
		puts("请选择操作：");
		puts(" 1：查询线路\t 0：退出系统 ");
		scanf("%d", &mainOption);	//如果开头不是0，1，2，那么输入不被接受，scanf不返回，option为4不变
        puts("");
		/*防错，若0，1，2后面不是\n，就提示输入错误*/
		if (getchar() != '\n')	//如果输入一串字符
		{
			mainOption = 4;	//输入无效，switch中是default选项
			while (getchar() != '\n');	//清空缓存
		}

		//主菜单
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
			puts("输入无效，请重新输入！");
			break;
		}
	} while (mainOption != 0);
	return 0;
}

void shortestpath(struct GRAPH orientedGraph, int v0, int*DISTANCEptr, int*PATHptr[10])	//设v0为初始点，求v0到剩下点的最短距离和最短路径
{
	int distance[10] = { 0 };	//这个数组 动态地 存放 当前状态下 起点到所有其他点的最短路线长度
								/*这个数组 动态地 存放 当前状态下 起点到所有其他点的最短路线点,初始化为-1*/
	int path[10][10];
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			path[i][j] = -1;
		}
	}
	int final[10] = { UNDONE };	//这个数组，代表的是是否已经求出v0到其他点的最短路线

	/*初始化distance和path，
	并且求出v0到v0的最短路线长度和点，
	相当于更新了一次最短路线*/
	for (int i = 0; i < 10; i++)
	{
		distance[i] = orientedGraph.arcs[v0][i];	//把distance初始化，一开始假定起点v0到其他各点vi的最短路线为（v0，vi）
		path[i][0] = v0;
		path[i][1] = i;		//把path初始化
	}
	distance[v0] = 0;
	final[v0] = DONE;	//v0到v0的距离为0，且v0到v0这条路线已经确定
	/*初始化完成，此时的状态为：
	1.已经完成了第一趟最短路线的更新
	2.还没有开始找对第一趟中最短的路线distanceMin
	3.由于没有找到第一趟中的最短的路线distanceMin，故没有对路线进行更新，即没有进入第二趟。
	4.求得了v0到v0的最短路径路线，且这个路线被标记为DONE状态
	5.接下来进入主循环*/

	/*主循环开始，思路为：
	先基于第n-1次循环中路线的更新，求得第n次循环中的distanceMin，完成第n趟算法；
	再在第n次循环中进行第n+1趟算法的路线的更新，为第n+1次循环中求distanceMin，并完成第n+1趟算法作铺垫*/
	for (int i = 0; i < orientedGraph.vertexsNum - 1; i++)	//只需要计算 总顶点数-1 次
	{
		int distanceMin = INFINITY;	//每次循环都要刷新一下，以确保只在点为UNDONE的路线中求最小值
		int vertexMin = 10;	//表示当前趟中最短的路径的终点，初始化为10方便测试。

		for (int v = 0; v < 10; v++)	//v for vertex，找出尚未求得最短路径中最短的那条路线，标记其长度和终点
		{
			if (final[v] == UNDONE && distance[v] < distanceMin)
			{
				vertexMin = v;
				distanceMin = distance[v];
			}

		}
		final[vertexMin] = DONE;	//最终确定下来一条最短路径

		for (int v = 0; v < 10; v++)	//更新最短路线长度和路线
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
	/*主循环结束*/

	/*传出数据*/
	for (int i = 0; i < 10; i++)
	{
		*(DISTANCEptr + i) = distance[i];
		for (int k = 0; k < 10; k++)
		{
			*(PATHptr[i] + k) = (path[i][k]);
		}
	}
}
void displaySiteTable(char vertexsName[10][20])	//显示地名和下标的对应表
{
	puts("请根据提示输入相应地名对应的编号!");
	puts("地名对应的编号如下：");
	puts("");
	for (int i = 0; i < 10; i++)
	{
		printf("%d：%-15s", i, vertexsName[i]);	//左对齐
		if ((i + 1) % 2 == 0)	puts("");	//2个换行
	}
	puts("");
	

}
int startPoint()	//输入起点
{
	int start = -1;
	int check = -1;

	do {
		printf("%s","请问你从哪里出发？（请输入起点对应的数字） ");
		check = scanf("%d", &start);	//输入起点
		if(start<0||start>9){check=-1;}
		if (getchar() != '\n')	//如果输入多个字符
		{ 
			check = -1;	//不通过循环，不能成功输入
			while (getchar() != '\n');	//清空缓存
		}
		if (check != 1) { puts("输入无效，请重新输入！"); }
	} while (check != 1);
	return start;
}
int endPoint()	//输入终点
{
	int end = -1;
	int check = -1;

	do {
		printf("%s","请问你到哪里？（请输入起点对应的数字）");
		check = scanf("%d", &end);	//输入终点
		if(end<0||end>9){check=-1;}
		if (getchar() != '\n')	//如果输入多个字符
		{
			check = -1;	//不通过循环，不能成功输入
			while (getchar() != '\n');	//清空缓存
		}
		if (check != 1) { puts("输入无效，请重新输入！"); }
	} while (check != 1);
	return end;
}
void displayShortestInfo(int distance[10], int path[10][10],char name[10][20],int endPoint)
{
     puts("");
	printf("%s\t", "最短路径为：");
	int i = 0;
	do {
        if(i!=0){printf("%s"," --> ");}
		printf("%s", name[path[endPoint][i]]);
		
		i++;
	} while (path[endPoint][i] != -1);
	puts("");
	printf("%s\t%d","距离为：",distance[endPoint]);
	puts("");
	
}
