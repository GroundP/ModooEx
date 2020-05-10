// Modoo - Thread.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <queue>
#include <chrono>

#include "ThreadUtil.h"

#define THREAD_PRO_NUM 5
#define THREAD_CON_NUM 3
#define PAGE_NUM 5

void producer(std::queue<std::string>* Pages, std::mutex* m, int index, std::condition_variable* Cond)
{
    for (int i = 0; i < 5; i++)
    {
        // 웹사이트 다운받는데 걸리는 시간이라 생각(스레드별로 다름)
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * index));
        
        std::string content = "웹사이트 : " + std::to_string(i) + " from thread(" + std::to_string(index) + ")\n";

        std::lock_guard<std::mutex> lock(*m);
        Pages->push(content);

        // consumer에게 준비되었음을 알림
        Cond->notify_one();
    }
}


void consumer(std::queue<std::string>* Pages, std::mutex* m, int* num_processed, std::condition_variable* Cond)
{
    const int nMax = THREAD_PRO_NUM * PAGE_NUM;
    while (*num_processed < nMax)
    {
        std::unique_lock<std::mutex> lock(*m);

        Cond->wait(lock, [&Pages, &num_processed, &nMax] ()
        {
            return !Pages->empty() || *num_processed == nMax;
        });

        // 다운로드 한 페이지가 없다면 다시 대기
        if (Pages->empty())
        {
            lock.unlock();

            // 1초후 다시 확인
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }


        // 다운로드 한 페이지가 있기 때문에 대기 목록에서 제거한다.
        std::string content = Pages->front();
        Pages->pop();

        (*num_processed)++;
        lock.unlock();

        std::cout << content;
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
}

int main()
{
    std::cout << "생산자-소비자 패턴 예시입니다. \n";

    std::mutex m;
    std::queue<std::string> queue;
    std::condition_variable Cond;

    std::vector<std::thread> vThreadsPro;
    for (int i = 0; i < THREAD_PRO_NUM; ++i)
    {
        vThreadsPro.push_back(std::thread(producer, &queue, &m, i+1, &Cond));
    }

    int nIndex = 0;
    std::vector<std::thread> vThreadsCon;
    for (int i = 0; i < THREAD_CON_NUM; ++i)
    {
        vThreadsCon.push_back(std::thread(consumer, &queue, &m, &nIndex, &Cond));
    }

    for (int i = 0; i < THREAD_PRO_NUM; ++i)
    {
        vThreadsPro[i].join();
    }

    // 자고 있는(기다리고 있는) 스레드를 모두 깨운다.
    Cond.notify_all();

    for (int i = 0; i < THREAD_CON_NUM; ++i)
    {
        vThreadsCon[i].join();
    }

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
