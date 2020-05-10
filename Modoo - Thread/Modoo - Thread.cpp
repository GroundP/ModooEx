// Modoo - Thread.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

#include "ThreadUtil.h"

#define THREAD_NUM 4

void Dosum(int& counter, std::mutex& m)
{
    for (int i = 0; i < 10000; i++)
    {
        // lock 생성 시에 m.lock() 을 실행한다고 보면 된다.
        std::lock_guard<std::mutex> lock(m);
        counter += 1;

        // scope 를 빠져 나가면 lock 이 소멸되면서
        // m 을 알아서 unlock 한다.
    }
}

int main()
{
    std::cout << "Mutex 사용 예시입니다. \n"; 

    int counter = 0;

    std::vector<std::thread> vThreads;
    std::mutex m;   // mutex 객체

    for (int i = 0; i < THREAD_NUM; ++i)
    {
        vThreads.push_back(std::thread(Dosum, std::ref(counter), std::ref(m)));
    }

    for (int i = 0; i < THREAD_NUM; ++i)
    {
        vThreads[i].join();
    }

    std::cout << "Counter 최종 값 : " << counter << std::endl;

    return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
