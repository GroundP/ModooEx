// Modoo - Thread.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <thread>
#include <vector>

#include "ThreadUtil.h"

void Dosum(std::vector<int>::iterator start, std::vector<int>::iterator end, int& result)
{
    int sum = 0;
    for (auto itr = start; itr < end; ++itr)
    {
        sum += *itr;
    }

    result = sum;

    //std::thread::id Thread_id = std::this_thread::get_id();
    printf("쓰레드 %d 부터 %d까지 계산한 결과 : %d \n", *start, *(end - 1), sum);
}

int main()
{
    std::cout << "Thread 프로그래밍 익숙해지기 연습중입니다. \n"; 

    std::vector<int> vData(10000);
    

    for (int i = 0; i < 10000; ++i)
    {
        vData[i] = i;
    }

    // 4개의 스레드에서 계산된 각각의 결과
    std::vector<int> vPartial_sum(4);


    std::vector<std::thread> vSumResults;
    for (int i = 0; i < 4; i++)
    {
        vSumResults.push_back(std::thread(Dosum, vData.begin() + i * 2500, vData.begin() + (i + 1) * 2500, std::ref(vPartial_sum[i])));
    }

    for (int i = 0; i < 4; i++) 
    {
        vSumResults[i].join();
    }

    int total = 0;
    for (int i = 0; i < 4; i++) 
    {
        total += vPartial_sum[i];
    }

    std::cout << "전체 합 : " << total << std::endl;

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
