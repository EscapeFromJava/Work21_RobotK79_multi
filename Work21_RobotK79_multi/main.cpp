#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int SIZE = 20;

enum RobotOrientation {
    UP, RIGHT, DOWN, LEFT
};

struct Coordinates {
    int x = 0, y = 0;
};

struct ERobot {
    string program;
    Coordinates coord;
    RobotOrientation orientation = RIGHT;
    int stepCounter = 0;
    vector <Coordinates> history;
    void moveForward(int field[SIZE][SIZE]) {
        switch (orientation) {
        case UP:    coord.y++; break;
        case DOWN:  coord.y--; break;
        case LEFT:  coord.x--; break;
        case RIGHT: coord.x++; break;
        }
        field[coord.x][coord.y]++;
        stepCounter++;
        history.push_back(coord);
    }
};

ERobot inputRobot(int field[SIZE][SIZE], int i)
{
    ERobot rob;
    cout << "Введите координаты " << i + 1 << " робота по оси \"x\" и \"у\": ";
    cin >> rob.coord.x >> rob.coord.y;
    field[rob.coord.x][rob.coord.y] = 7;
    rob.history.push_back(rob.coord);
    cout << "Введите маршрут " << i + 1 << " робота: ";
    cin >> rob.program;
    return rob;
}

void moveForward(ERobot* rob, int field[SIZE][SIZE], int numRob)
{
    switch (rob->orientation) {
    case UP:    rob->coord.y++; break;
    case DOWN:  rob->coord.y--; break;
    case LEFT:  rob->coord.x--; break;
    case RIGHT: rob->coord.x++; break;
    }
    field[rob->coord.x][rob->coord.y] = numRob + 1;
    rob->stepCounter++;
    rob->history.push_back(rob->coord);
}

void turnLeft(ERobot* rob)
{
    switch (rob->orientation) {
    case UP:    rob->orientation = LEFT; break;
    case DOWN:  rob->orientation = RIGHT; break;
    case LEFT:  rob->orientation = DOWN; break;
    case RIGHT: rob->orientation = UP; break;
    }
}

void turnRight(ERobot* rob)
{
    switch (rob->orientation) {
    case UP:    rob->orientation = RIGHT; break;
    case DOWN:  rob->orientation = LEFT; break;
    case LEFT:  rob->orientation = UP; break;
    case RIGHT: rob->orientation = DOWN; break;
    }
}

vector<ERobot> robotWay(vector<ERobot> vec, int field[SIZE][SIZE])
{
    int maxProgramSize = 0;
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i].program.size() > maxProgramSize) {
            maxProgramSize = vec[i].program.size();
        }
    }
    for (int i = 0; i < maxProgramSize; i++) {
        for (int j = 0; j < vec.size();) {
            if (vec[j].program.size() <= i) {
                j++;
            }
            if (vec[j].program[i] == 'S')
                vec[j].moveForward(field);
            if (vec[j].program[i] == 'L')
                turnLeft(&vec[j]);
            if (vec[j].program[i] == 'R')
                turnRight(&vec[j]);
            j++;
        }
    }
    return vec;
}

void minDistanceBetweenRobots(vector<ERobot> vec, int numberOfRobots, int rob1, int rob2)
{
    int temp_x = 32000, temp_y = 32000, nearest_x1 = 0, nearest_y1 = 0, nearest_x2 = 0, nearest_y2 = 0, stepRob1 = 0, stepRob2 = 0;
    float temp_dis = 32000;
    for (int j = 0; j < vec[rob1].history.size(); j++) {
        for (int k = 0; k < vec[rob2].history.size(); k++) {
            temp_x = abs(vec[rob1].history[j].x - vec[rob2].history[k].x);
            temp_y = abs(vec[rob1].history[j].y - vec[rob2].history[k].y);
            if (sqrt(temp_x * temp_x + temp_y * temp_y) < temp_dis) {
                temp_dis = sqrt(temp_x * temp_x + temp_y * temp_y);
                nearest_x1 = vec[rob1].history[j].x;
                nearest_y1 = vec[rob1].history[j].y;
                nearest_x2 = vec[rob2].history[k].x;
                nearest_y2 = vec[rob2].history[k].y;
                stepRob1 = j;
                stepRob2 = k;
            }
        }
    }
    cout << "Минимальное расстояние между " << rob1 + 1 << " роботом и " << rob2 + 1 << " роботом  = " << temp_dis << endl;
    cout << "Робот " << rob1 + 1 << " находился в координатах: x = " << nearest_x1 << " y = " << nearest_y1 << " и совершил в этот момент " << stepRob1 << " шагов" << endl;
    cout << "Робот " << rob2 + 1 << " находился в координатах: x = " << nearest_x2 << " y = " << nearest_y2 << " и совершил в этот момент " << stepRob2 << " шагов" << endl;
}

void printField(int field[SIZE][SIZE])
{
    for (int i = SIZE - 1; i > -1; i--) {
        for (int j = 0; j < SIZE; j++) {
            cout << field[j][i];
        }
        cout << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "rus");


    int numberOfRobots;
    int field[SIZE][SIZE] = { 0 };

    cout << "Введите количество роботов: ";
    cin >> numberOfRobots;

    vector <ERobot> robVector;
    for (int i = 0; i < numberOfRobots; i++) {
        robVector.push_back(inputRobot(field, i));
    }

    vector<ERobot> robVectorWithRobotWay = robotWay(robVector, field);

    for (int i = 0; i < robVectorWithRobotWay.size() - 1; i++) {
        for (int j = i + 1; j < robVectorWithRobotWay.size(); j++) {
            minDistanceBetweenRobots(robVectorWithRobotWay, numberOfRobots, i, j);
        }
    }

    printField(field);

    return 0;
}
