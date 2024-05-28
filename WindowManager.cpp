#include "WindowManager.h"

WindowManager::WindowManager(GameData *_data)
{
    data = _data;
    init_pair(EMPTY, COLOR_WHITE, COLOR_WHITE);
    init_pair(WALL, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(IMMUNE_WALL, COLOR_BLACK, COLOR_BLACK);
    init_pair(SNAKE_HEAD, COLOR_CYAN, COLOR_CYAN);
    init_pair(SNAKE_BODY, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(GROWTH_ITEM, COLOR_GREEN, COLOR_GREEN);
    init_pair(POISON_ITEM, COLOR_RED, COLOR_RED);
    // 각 오브젝트 별로 색깔 설정

} // 생성자의 구현부

void WindowManager::Update()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            attron(COLOR_PAIR(data->map[i][j]));  // 해당 위치에 있는 오브젝트에 맞는 색상의 Attribute 사용
            mvprintw(i, j * 2, "  ");             // 화면에 출력
            attroff(COLOR_PAIR(data->map[i][j])); // Attribute 사용 해제
        }
    }
    const std::string temp = "Time : " + std::to_string(data->nowTime);
    mvprintw(HEIGHT + 1, 1, const_cast<char *>(temp.c_str())); // 현재 시간을 밑쪽에 표시

    refresh(); // 실제 스크린에 출력

} // Update 함수의 구현부