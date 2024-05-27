#pragma once
const int WIDTH = 21;
const int HEIGHT = 21;
//가로, 세로 크기

enum State
{
    STOP, // 정지 상태
    UP,   // 위
    LEFT, // 왼쪽
    DOWN, // 아래
    RIGHT // 오른쪽
}; // 뱀의 현재 상태를 나타내는 열거형

enum ObjectType
{
    EMPTY = 1,   // 빈 공간
    WALL,        // 벽
    IMMUNE_WALL, // 고정 벽
    SNAKE_HEAD,  // 뱀의 머리
    SNAKE_BODY,  // 뱀의 꼬리
    GROWTH_ITEM, // 성장 아이템
    POISON_ITEM  // 독 아이템
}; // 맵의 특정 위치의 상태를 나타내는 열거형

ObjectType map[HEIGHT][WIDTH]; // 구조물이 담긴 2차원 배열

typedef struct _pos
{
    int y = 0;
    int x = 0;
} pos; // x,y좌표 표현을 위한 구조체

