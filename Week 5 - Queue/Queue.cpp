#include <stack>
#include <queue>
#include <iostream>
#include <cassert>
#include <string>

// Task 1
class MyCircularQueue {
public:
    MyCircularQueue(int k) :size(k), head(0), tail(0), last(false) {
        queue = new int[k];
    }

    ~MyCircularQueue()
    {
        delete[] queue;
    }

    bool enQueue(int value) {
        if (isFull()) return false;
        queue[tail] = value;
        tail = (tail + 1) % size;
        last = true;
        return true;
    }

    bool deQueue() {
        if (isEmpty()) return false;
        head = (head + 1) % size;
        last = false;
        return true;
    }

    int Front() {
        return isEmpty() ? -1 : queue[head];
    }

    int Rear() {
        return isEmpty() ? -1 : queue[(tail - 1 + size) % size];
    }

    bool isEmpty() {
        return head == tail && !last;
    }

    bool isFull() {
        return head == tail && last;
    }

private:
    int* queue = nullptr;
    int size;
    int head;
    int tail;
    bool last;
};

// Task 2
class MyQueue {
public:
    MyQueue() {}

    void push(int x) {
        input.push(x);
    }

    int pop() {
        int el = peek();
        output.pop();
        return el;
    }

    int peek() {
        if (output.empty())
        {
            while (!input.empty())
            {
                output.push(input.top());
                input.pop();
            }
        }
        return output.top();
    }

    bool empty() {
        return input.empty() && output.empty();
    }
private:
    std::stack<int> input;
    std::stack<int> output;
};

// Task 3
const int BOARD_SIZE = 8;

struct Pos
{
    int x;
    int y;
};

Pos operator+(const Pos& rhs, const Pos& lhs)
{
    return { rhs.x + lhs.x, rhs.y + lhs.y};
}

bool operator==(const Pos& rhs, const Pos& lhs)
{
    return rhs.x == lhs.x && rhs.y == lhs.y;
}

const Pos arr[8]
{
    {1, 2}, {2, 1}, {-1, -2}, {-2,-1},
    {1, -2}, {-1, 2}, {2, -1}, {-2, 1}
};

bool isValid(Pos p)
{
    return p.x < BOARD_SIZE && p.x >= 0 && p.y < BOARD_SIZE && p.y >= 0;
}

int getMinStepsToPos(const Pos& knight, const Pos& dest)
{
    if (!isValid(knight) || !isValid(dest))
        throw std::runtime_error("Invalid input");
    bool visited[BOARD_SIZE][BOARD_SIZE]{};
    std::queue<Pos> q;
    q.push(knight);

    visited[knight.x][knight.y] = true;

    int moves = 0;
    while (!q.empty())
    {
        int levelSize = q.size();

        for (int i = 0; i < levelSize; i++) {
            Pos curr = q.front();
            q.pop();

            if (curr == dest) return moves;

            for (int d = 0; d < 8; ++d) {
                Pos next = curr + arr[d];

                if (isValid(next) && !visited[next.x][next.y])
                {
                    visited[next.x][next.y] = true;
                    q.push(next);
                }
            }
        }
        moves++;
    }
    return -1;
}

// Task 4
int kittens(const std::string& cards, unsigned int players)
{
    if (players == 0) return 0;

    std::queue<char> deck;
    for (char c : cards)
        deck.push(c);

    int* defuses = new int[players] {};
    bool* alive = new bool[players];

    for (int i = 0; i < players; i++)
        alive[i] = true;

    unsigned int activePlayers = players;
    unsigned int currentPlayer = 0;

    while (activePlayers > 1)
    {
        char card = deck.front();
        deck.pop();

        if (card == 'S')
        {
            defuses[currentPlayer]++;
        }
        else if (card == 'B')
        {
            if (defuses[currentPlayer] > 0)
            {
                defuses[currentPlayer]--;
                deck.push('B');
            }
            else
            {
                alive[currentPlayer] = false;
                activePlayers--;
            }
        }

        do
        {
            currentPlayer = (currentPlayer + 1) % players;
        } while (!alive[currentPlayer]);
        
        if (activePlayers == 1) break;
    }

    delete[] alive;
    delete[] defuses;
    return currentPlayer + 1;
}

int main()
{
    //Pos knight = { 2,1 };
    //Pos dest = { 0,1 };
    //std::cout << getMinStepsToPos(knight, dest);
    assert(kittens("BB", 3) == 3);
    assert(kittens("SSBB", 3) == 2);
    assert(kittens("SSBBOOOOOOOSBOOOOOOOOOSB", 5) == 1);
    assert(kittens("SOOSBBOOOOSOOOSBOSOOBOSOSOOBOSOSOOB", 7) == 3);
}

