#include "pch.h"

#include <iostream>
#include <thread>

#include "ThreadUtil.h"
using std::thread;

CThreadUtil::CThreadUtil()
{
}


CThreadUtil::~CThreadUtil()
{
}


void CThreadUtil::func1()
{
    for (int i = 0; i < 10; i++)
    {
        std::cout << "쓰레드 1 작동중! \n";
    }
}

void CThreadUtil::func2()
{
    for (int i = 0; i < 10; i++)
    {
        std::cout << "쓰레드 2 작동중! \n";
    }
}

void CThreadUtil::func3()
{
    for (int i = 0; i < 10; i++)
    {
        std::cout << "쓰레드 3 작동중! \n";
    }
}

void CThreadUtil::RunThreadTest1()
{
    std::thread t1(&CThreadUtil::func1, this);
    std::thread t2(&CThreadUtil::func2, this);
    std::thread t3(&CThreadUtil::func3, this);

    t1.join();
    t2.join();
    t3.join();
}