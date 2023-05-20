//2205112E39李萍
#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

const int numPhilosophers = 5; // 哲学家数量
pthread_mutex_t forks[numPhilosophers]; // 筷子的互斥量

// 哲学家结构体
struct Philosopher {
    int id;
    pthread_t thread;
};

// 初始化互斥量
void initMutexes() {
    for (int i = 0; i < numPhilosophers; i++) {
        pthread_mutex_init(&forks[i], nullptr);
    }
}

// 销毁互斥量
void destroyMutexes() {
    for (int i = 0; i < numPhilosophers; i++) {
        pthread_mutex_destroy(&forks[i]);
    }
}

// 哲学家思考
void think(int philosopherId) {
    cout << "Philosopher " << philosopherId << " is thinking." << endl;
    sleep(1);
}

// 哲学家进餐
void eat(int philosopherId) {
    cout << "Philosopher " << philosopherId << " is eating." << endl;
    sleep(1);
}

// 获取左右筷子的编号
void getForks(int philosopherId, int& leftFork, int& rightFork) {
    leftFork = philosopherId;
    rightFork = (philosopherId + 1) % numPhilosophers;
}
// 尝试获取筷子
void tryGetForks(int philosopherId, int leftFork, int rightFork) {
    // 阻塞方式加锁互斥量
    // pthread_mutex_lock(&forks[leftFork]);
    // pthread_mutex_lock(&forks[rightFork]);

    //非阻塞方式加锁互斥量（预防死锁）
    int ret;
    ret = pthread_mutex_trylock(&forks[leftFork]);
    if (ret != 0) {
        cout << "Philosopher " << philosopherId << " failed to get left fork." << endl;
        return;
    }
    ret = pthread_mutex_trylock(&forks[rightFork]);
    if (ret != 0) {
        cout << "Philosopher " << philosopherId << " failed to get right fork." << endl;
        pthread_mutex_unlock(&forks[leftFork]);
        return;
    }

    eat(philosopherId);

    pthread_mutex_unlock(&forks[leftFork]);
    pthread_mutex_unlock(&forks[rightFork]);
}

// 哲学家线程函数
void* philosopherThread(void* arg) {
    Philosopher* philosopher = reinterpret_cast<Philosopher*>(arg);
    int philosopherId = philosopher->id;

    while (true) {
        think(philosopherId);

        int leftFork, rightFork;
        getForks(philosopherId, leftFork, rightFork);
        tryGetForks(philosopherId, leftFork, rightFork);
    }

    return nullptr;
}

int main() {
    initMutexes();

    Philosopher philosophers[numPhilosophers];
    for (int i = 0; i < numPhilosophers; i++) {
        philosophers[i].id = i;
        pthread_create(&philosophers[i].thread, nullptr, philosopherThread, &philosophers[i]);
    }

    // 主线程等待哲学家线程结束
    for (int i = 0; i < numPhilosophers; i++) {
        pthread_join(philosophers[i].thread, nullptr);
    }

    destroyMutexes();

    return 0;
}
