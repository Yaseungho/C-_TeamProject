#include <ncurses.h>
#include <unistd.h>
#include <deque>
#include <string>
#include "ItemSpawner.h"
#include "Utility.h"
using namespace std;

class GameManager
{
private:
    ItemSpawner itemSpawner;      // 아이템 생성을 관리하는 클래스
    static GameManager *instance; // 이 클래스의 인스턴스
    int time = 0;                 // Update가 호출된 횟수
    const int dy[5] = {0, -1, 0, 1, 0};
    const int dx[5] = {0, 0, -1, 0, 1}; // 상태별로 이동 방향을 나타내는 상대 좌표(정지,위,왼쪽,아래,오른쪽 순서)

    deque<pos> snake; // 뱀에 해당하는 좌표들을 덱 자료구조에 저장
    State nowState;   // 현재 뱀의 상태

    GameManager(const GameManager &other);
    GameManager() { Init(); } // 생성자

    int EdgeCount(int y, int x)
    {
        if (0 <= y && y < HEIGHT && 0 <= x && x < WIDTH)
        {
            int ret = 0;
            if (y == 0 || y == HEIGHT - 1)
                ret++;
            if (x == 0 || x == WIDTH - 1)
                ret++;
            return ret;
        }
        else
            return -1;
    } // 좌표값을 입력받아 해당 좌표가 모서리일 경우 1, 모퉁이일 경우 2, 범위를 벗어난 경우 -1, 그외에는 0을 반환

    void SnakeMove()
    {
        if (nowState == STOP)
            return; // 정지 상태인 경우 바로 리턴

        int _y = snake.front().y + dy[nowState];
        int _x = snake.front().x + dx[nowState]; // 이동할 다음 좌표

        map[snake.front().y][snake.front().x] = SNAKE_BODY;
        snake.push_front({_y, _x}); // 머리 이동

        switch (map[_y][_x])
        {
        case EMPTY: // 빈공간으로 이동한 경우 : 꼬리부분 1개 삭제
            map[snake.back().y][snake.back().x] = EMPTY;
            snake.pop_back();
            break;

        case POISON_ITEM: // 독 아이템을 먹은 경우 : 꼬리부분 2개 삭제
            map[snake.back().y][snake.back().x] = EMPTY;
            snake.pop_back();
            map[snake.back().y][snake.back().x] = EMPTY;
            snake.pop_back();
            break;

        case GROWTH_ITEM: // 성장 아이템을 먹은 경우 : 꼬리부분 그대로
            break;

        default: // 그 외 구조물(벽, 뱀의 다른 부위, 고정 벽)과 충돌한 경우 : 게임 오버
            gameOver = true;
            return;
        }
        map[_y][_x] = SNAKE_HEAD;
        return;

    } // 지렁이가 방향에 따라 한 칸 이동

    void UpdateWindow()
    {

        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                attron(COLOR_PAIR(map[i][j]));  // 해당 위치에 있는 오브젝트에 맞는 색상의 Attribute 사용
                mvprintw(i, j * 2, "  ");       // 화면에 출력
                attroff(COLOR_PAIR(map[i][j])); // Attribute 사용 해제
            }
        }
        refresh(); // 실제 스크린에 출력
    }

public:
    bool gameOver = false;

    static GameManager *GetInstance()
    {
        if (instance == NULL)
        {
            instance = new GameManager();
        }
        return instance;
    } // 싱글톤 패턴

    void Init()
    {
        init_pair(EMPTY, COLOR_WHITE, COLOR_WHITE);
        init_pair(WALL, COLOR_MAGENTA, COLOR_MAGENTA);
        init_pair(IMMUNE_WALL, COLOR_BLACK, COLOR_BLACK);
        init_pair(SNAKE_HEAD, COLOR_CYAN, COLOR_CYAN);
        init_pair(SNAKE_BODY, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(GROWTH_ITEM, COLOR_GREEN, COLOR_GREEN);
        init_pair(POISON_ITEM, COLOR_RED, COLOR_RED);
        // 각 오브젝트 별로 색깔 설정

        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                if (EdgeCount(i, j) == 1)
                    map[i][j] = WALL; // 모서리 부분에 벽 생성
                else if (EdgeCount(i, j) == 2)
                    map[i][j] = IMMUNE_WALL; // 모퉁이 부분에 고정 벽 생성
                else
                    map[i][j] = EMPTY; // 그 외는 빈 공간으로 생성
            }
        }
        map[HEIGHT / 2][WIDTH / 2] = SNAKE_HEAD;       // 맵의 중앙에 뱀 머리 생성
        map[HEIGHT / 2][WIDTH / 2 - 1] = SNAKE_BODY;   // 그 옆에 뱀 몸통 생성
        map[HEIGHT / 2][WIDTH / 2 - 2] = SNAKE_BODY;   // 그 옆에 뱀 몸통 생성
        snake.push_front({HEIGHT / 2, WIDTH / 2 - 2}); // 뱀 몸통을 snake에 추가
        snake.push_front({HEIGHT / 2, WIDTH / 2 - 1}); // 뱀 몸통을 snake에 추가
        snake.push_front({HEIGHT / 2, WIDTH / 2});     // 뱀 머리를 snake에 추가
        nowState = STOP;
    } // 게임의 기본 설정을 해주는 함수

    void HandleInput()
    {
        timeout(100);      // 입력에 타임아웃 적용
        noecho();          // 입력값이 보이지 않도록 설정
        char ch = getch(); // 문자 입력받기

        if (ch == 'W' || ch == 'w')
            nowState = UP;
        else if (ch == 'A' || ch == 'a')
            nowState = LEFT;
        else if (ch == 'S' || ch == 's')
            nowState = DOWN;
        else if (ch == 'D' || ch == 'd')
            nowState = RIGHT;
        // 입력에 따라 뱀이 바라보는 방향 변경
    } // 플레이어의 방향키를 입력받는 함수

    void Update()
    {
        if (snake.size() < 3)
            gameOver = true; // 길이가 3보다 작아지면 게임오버
            
        if (gameOver)
            return; // 게임오버 시 바로 종료

        time++;
        const string temp = "Time : " + to_string(time);
        mvprintw(HEIGHT + 1, 1, const_cast<char *>(temp.c_str())); // 현재 시간을 밑쪽에 표시

        itemSpawner.Update(); // 아이템 생성기의 Update 호출
        HandleInput();        // 키 입력 함수 호출
        SnakeMove();          // 뱀 이동 함수 호출
        UpdateWindow();       // 화면 업데이트
    } // 틱마다 호출되는 함수
};

GameManager *GameManager::instance = NULL;