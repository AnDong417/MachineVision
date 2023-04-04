#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define ROW 10
#define COL 10

//定义节点
typedef struct
{
    int x;
    int y;
    int g;  //实际代价
    int h;  //估计代价
    int f;  //总代价
} Node;

int map[ROW][COL] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

Node *create_node(int x, int y);    //定义节点操作函数
void node_cost(Node *node, int g, int h);   //节点代价函数
int heuristic(int x, int y, int target_x, int target_y);    //起点到目标点直线距离的近似
void astar(Node *start, Node *target);

int main()
{
    Node *start = create_node(0, 0);
    Node *target = create_node(9, 9);

    astar(start, target);

    return 0;
}

Node *create_node(int x, int y)
{
    Node *node = (Node *)malloc(sizeof(Node));

    node->x = x;
    node->y = y;
    node->g = 0;
    node->h = 0;
    node->f = 0;

    return node;
}

void node_cost(Node *node, int g, int h)
{
    node->g = g;
    node->h = h;
    node->f = g + h;
}

int heuristic(int x, int y, int target_x, int target_y)
{
    return abs(target_x - x) + abs(target_y - y);
}

void astar(Node *start, Node *target)
{
    //初始化开放列表和关闭列表
    Node *open_list[ROW * COL] = {NULL};
    Node *close_list[ROW * COL] = {NULL};
    int open_count = 0;
    int close_count = 0;

    //将起点加入开放列表
    open_list[open_count++] = start;

    while(open_count > 0)
    {
        //找出 f 最小的节点
        Node *current = open_list[0];
        int current_index = 0;
        for(int i = 1; i < open_count; i++)
        {
            if(open_list[i]->f < current->f)
            {
                current = open_list[i];
                current_index = i;
            }
        }

        //如果这个节点是终点，则结束搜索
        if(current->x == target->x && current->y == target->y)
        {
            printf("Path found!\n");
            return;
        }

        //将这个节点从开放列表删除，并加入关闭列表
        open_list[current_index] = NULL;
        close_list[close_count++] = current;

        //扩展当前节点的相邻节点
        for(int i = -1; i <= 1; i++)
        {
            for(int j = -1; j <= 1; j++)
            {
                if(i == 0 && j == 0)
                {
                    continue;
                }

                int neighbor_x = current->x + i;
                int neighbor_y = current->y + j;

                //判断是否在地图内
                if(neighbor_x >= 0 && neighbor_x < ROW && neighbor_y >= 0 && neighbor_y < COL)
                {
                    Node *neighbor = create_node(neighbor_x, neighbor_y);

                    //判断是否在关闭列表中
                    bool in_close_list = false;
                    for(int k = 0; k < close_count; k++)
                    {
                        if(close_list[k] != NULL && neighbor_x == close_list[k]->x && neighbor_y == close_list[k]->y)
                        {
                            in_close_list = true;
                            break;
                        }
                    }
                    if(in_close_list)
                    {
                        continue;
                    }

                    //判断是否可通过
                    if(map[neighbor_x][neighbor_y] == 1)
                    {
                        continue;
                    }

                    //计算代价
                    int g = current->g + 1;
                    int h = heuristic(neighbor_x, neighbor_y, target->x, target->y);
                    node_cost(neighbor, g, h);

                    //判断是否在开放列表中，如果是，则与新计算的代价相比较
                    bool in_open_list = false;
                    for(int k = 0; k < open_count; k++)
                    {
                        if(open_list[k] != NULL && neighbor_x == open_list[k]->x && neighbor_y == open_list[k]->y)
                        {
                            in_open_list = true;
                            if(g < open_list[k]->g)
                            {
                                node_cost(open_list[k], g, h);
                            }
                            break;
                        }
                    }
                    
                    //如果节点不在开放列表，则将其加入开放列表
                    if(!in_open_list)
                    {
                        open_list[open_count++] = neighbor;
                    }
                }
            }
        }
    }

    //开放列表为空，仍没有找到终点，则搜索失败
    printf("Path not found!\n");
}