#pragma once
#include "Utility.h"
#include <stdlib.h>

class ItemSpawner
{
private:
    const int duration = 15; // 아이템의 최대 지속시간
    pos poisonPos;           // 독 아이템의 위치
    int poisonTime = 0;      // 현재 독 아이템이 나와있었던 시간
    pos growthPos;           // 성장 아이템의 위치
    int growthTime = 0;      // 현재 성장 아이템이 나와있었던 시간

    void SpawnItem(ObjectType itemType)
    {
        int ry, rx;
        while (1)
        {
            ry = rand() % HEIGHT;
            rx = rand() % WIDTH;
            if (map[ry][rx] == EMPTY)
                break;
        } // 맵에서 비어있는 공간 무작위로 고르기

        map[ry][rx] = itemType; //아이템 생성

        if (itemType == POISON_ITEM)
        {
            if (map[poisonPos.y][poisonPos.x] == POISON_ITEM)
                map[poisonPos.y][poisonPos.x] = EMPTY;
            poisonPos = {ry, rx};
            poisonTime = 0;
        }
        else if (itemType == GROWTH_ITEM)
        {
            if (map[growthPos.y][growthPos.x] == GROWTH_ITEM)
                map[growthPos.y][growthPos.x] = EMPTY;
            growthPos = {ry, rx};
            growthTime = 0;
        } // 기존에 있던 아이템에 대한 정보 초기화

    } // 파라미터로 받은 유형의 아이템을 랜덤 위치에 생성하는 함수

public:
    void Update()
    {
        poisonTime++;
        growthTime++;
        if (map[poisonPos.y][poisonPos.x] != POISON_ITEM || duration <= poisonTime)
            SpawnItem(POISON_ITEM);
        if (map[growthPos.y][growthPos.x] != GROWTH_ITEM || duration <= growthTime)
            SpawnItem(GROWTH_ITEM);
        // 이미 먹었거나, 지속시간을 다 한 경우 새로 생성한다
    }
};