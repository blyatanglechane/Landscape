#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>
#include <random>
#include <string>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

using namespace std;

int SIZE = 257;
vector<vector<int>> heightMap(SIZE, vector<int>(SIZE));

int randomInt(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(-1, 2);
    return dis(gen);
}

void SquareStep(int current_long, int roughness) {
    int half_long = (current_long / 2);

    for (int general_y = half_long; general_y < SIZE - 1; general_y += current_long) {
        for (int general_x = half_long; general_x < SIZE - 1; general_x += current_long) {
            int average = (heightMap[general_y - half_long][general_x - half_long] +
                heightMap[general_y - half_long][general_x + half_long] +
                heightMap[general_y + half_long][general_x - half_long] +
                heightMap[general_y + half_long][general_x + half_long]) / 4;
            int random = randomInt();
            if (average + random < 10 && average + random >= 0)
                heightMap[general_y][general_x] = (average + random) * roughness;
            else if (average + random >= 10)
                heightMap[general_y][general_x] = 9;
            else if (average + random < 0)
                heightMap[general_y][general_x] = 0;
        }
    }
}

void DiamondStep(int current_long, int roughness) {
    int half_long = current_long / 2;

    for (int general_y = 0; general_y < SIZE; general_y += half_long) {
        for (int general_x = (general_y + half_long) % current_long; general_x < SIZE; general_x += current_long) {
            int sum = 0;
            int count = 0;

            if (general_y - half_long >= 0) {
                sum += heightMap[general_y - half_long][general_x];
                count++;
            }
            if (general_y + half_long < SIZE) {
                sum += heightMap[general_y + half_long][general_x];
                count++;
            }
            if (general_x - half_long >= 0) {
                sum += heightMap[general_y][general_x - half_long];
                count++;
            }
            if (general_x + half_long < SIZE) {
                sum += heightMap[general_y][general_x + half_long];
                count++;
            }

            int average = sum / count;
            int random = randomInt();

            if (average + random < 10 && average + random >= 0)
                heightMap[general_y][general_x] = (average + random) * roughness;
            else if (average + random >= 10)
                heightMap[general_y][general_x] = 9;
            else if (average + random < 0)
                heightMap[general_y][general_x] = 0;
        }
    }
}


void DiamondSquareAlgorithm(int h_1, int h_2, int h_3, int h_4, int roughness){
    heightMap[0][0] = h_1;
    heightMap[0][SIZE - 1] = h_2;
    heightMap[SIZE - 1][SIZE - 1] = h_3;
    heightMap[SIZE - 1][0] = h_4;

    int current_long = SIZE;
    while (current_long > 1){
        int half = current_long / 2;
        SquareStep(current_long, roughness);
        DiamondStep(current_long, roughness);
        current_long /= 2;
    }
}


int main()
{
    int h_1, h_2, h_3, h_4, roughness;
    cout << "cin >> h_1 >> h_2 >> h_3 >> h_4  >> roughness;\n";
    // 6 4 2 8 1
    cin >> h_1 >> h_2 >> h_3 >> h_4 >> roughness;
    DiamondSquareAlgorithm(h_1, h_2, h_3, h_4, roughness);
    for (int y = 0; y < SIZE; ++y)
    {
        for (int x = 0; x < SIZE; ++x)
        {
            if (heightMap[y][x] == 0)
                cout << ANSI_COLOR_BLUE << to_string(heightMap[y][x]) << ANSI_COLOR_RESET << " ";
            else if (0 < heightMap[y][x] && heightMap[y][x] <= 3)
                cout << ANSI_COLOR_YELLOW << to_string(heightMap[y][x]) << ANSI_COLOR_RESET << " ";
            else if (3 < heightMap[y][x] && heightMap[y][x] <= 6)
                cout << ANSI_COLOR_GREEN << to_string(heightMap[y][x]) << ANSI_COLOR_RESET << " ";
            else if (7 <= heightMap[y][x] && heightMap[y][x] <= 9)
                cout << to_string(heightMap[y][x]) << " ";
        }
        cout << "\n";
    }
    return 0;
}
