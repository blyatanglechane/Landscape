#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>
#include <random>

using namespace std;

int SIZE = 65;
vector<vector<int>> heightMap(SIZE, vector<int>(SIZE));

int randomFloat(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(-1, 1);
    return dis(gen);
}

//void drawMap() {
//    double maxHeight = 0.0;
//    double minHeight = numeric_limits<double>::max();
//
//    // Находим максимальное и минимальное значение высоты в heightMap
//    for (int y = 0; y < SIZE; y++) {
//        for (int x = 0; x < SIZE; x++) {
//            double height = heightMap[y][x];
//            maxHeight = max(maxHeight, height);
//            minHeight = min(minHeight, height);
//        }
//    }
//
//    // Отрисовываем карту высот
//    glPointSize(1.0);
//    glBegin(GL_POINTS);
//    for (int y = 0; y < SIZE; y++) {
//        for (int x = 0; x < SIZE; x++) {
//            double height = heightMap[y][x];
//            double normalizedHeight = (height - minHeight) / (maxHeight - minHeight);
//
//            // Выбираем цвет в зависимости от нормализованной высоты
//            if (normalizedHeight < 0.3) {
//                glColor3f(0.0, 0.2, normalizedHeight * 1.5);
//            }
//            else if (normalizedHeight < 0.6) {
//                glColor3f(0.0, normalizedHeight * 1.5, 0.0);
//            }
//            else {
//                glColor3f(normalizedHeight * 0.8, normalizedHeight * 0.8, normalizedHeight * 0.8);
//            }
//
//            glVertex2i(x, y);
//        }
//    }
//    glEnd();
//}

void SquareStep(int current_long){
    int half_long = (current_long / 2);

    for (int general_y = half_long; general_y < SIZE - 1; general_y += current_long){
        for (int general_x = half_long; general_x < SIZE - 1; general_x += current_long){
            int average = (heightMap[general_y - half_long][general_x - half_long] +
                heightMap[general_y - half_long][general_x + half_long] +
                heightMap[general_y + half_long][general_x - half_long] +
                heightMap[general_y + half_long][general_x + half_long]) / 4.0;

            heightMap[general_y][general_x] = average + randomFloat();
        }
    }
}

//void DiamondStep(int current_long){
//    int half_long = (current_long / 2);
//    for (int general_y = 0; general_y < SIZE; general_y += half_long) {
//        for (int general_x = (general_y + half_long) % current_long; general_x < SIZE; general_x += current_long) {   
//            int sum = 0;
//            int count = 0;
//
//            if (general_y - half_long >= 0){
//                sum += heightMap[general_y - half_long][general_x];
//                count++;
//            }
//            if (general_y + half_long < SIZE){
//                sum += heightMap[general_y + half_long][general_x];
//                count++;
//            }
//            if (general_x - half_long >= 0){
//                sum += heightMap[general_y][general_x - half_long];
//                count++;
//            }
//            if (general_x + half_long < SIZE){
//                sum += heightMap[general_y][general_x + half_long];
//                count++;
//            }
//
//            int average = sum / count;
//            heightMap[general_y][general_x] = average + randomFloat();
//
//
//
//        }
//    }
//}
void DiamondStep(int current_long)
{
    int half_long = current_long / 2;

    for (int general_y = 0; general_y < SIZE; general_y += half_long)
    {
        for (int general_x = (general_y + half_long) % current_long; general_x < SIZE; general_x += current_long)
        {
            int sum = 0;
            int count = 0;

            if (general_y - half_long >= 0)
            {
                sum += heightMap[general_y - half_long][general_x];
                count++;
            }
            if (general_y + half_long < SIZE)
            {
                sum += heightMap[general_y + half_long][general_x];
                count++;
            }
            if (general_x - half_long >= 0)
            {
                sum += heightMap[general_y][general_x - half_long];
                count++;
            }
            if (general_x + half_long < SIZE)
            {
                sum += heightMap[general_y][general_x + half_long];
                count++;
            }

            int average = sum / count;
            heightMap[general_y][general_x] = average + randomFloat();
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
        SquareStep(current_long);
        DiamondStep(current_long);
        current_long /= 2;
        roughness /= 2;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    static double lastX = xpos, lastY = ypos;
    static bool isDragging = false;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        if (!isDragging){
            isDragging = true;
            lastX = xpos;
            lastY = ypos;
        }
        else{
            float deltaX = (float)(xpos - lastX) / 600.0f;
            float deltaY = (float)(ypos - lastY) / 600.0f;
            glTranslatef(deltaX, -deltaY, 0.0f);
            lastX = xpos;
            lastY = ypos;
        }
    }
    else{
        isDragging = false;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    double scaleFactor = 1.1;
    if (yoffset > 0){
        glScalef(scaleFactor, scaleFactor, 1.0);
    }
    else{
        glScalef(1.0 / scaleFactor, 1.0 / scaleFactor, 1.0);
    }
}



int main()
{


    //setlocale(LC_ALL, "Russian");
    //if (!glfwInit())
    //    return -1;

    //GLFWwindow* window = glfwCreateWindow(900, 900, "Algorithm 'midpoint displacement'", NULL, NULL);
    //if (!window){
    //    glfwTerminate();
    //    return -1;
    //}
    //glfwSetCursorPosCallback(window, cursor_position_callback);
    //glfwMakeContextCurrent(window);
    //glClearColor(0.0f, 0.0f, 0.8f, 1.0f);
    int h_1, h_2, h_3, h_4, roughness;
    cout << "cin >> h_1 >> h_2 >> h_3 >> h_4  >> roughness;\n";
    // 13 8 5 11 1
    cin >> h_1 >> h_2 >> h_3 >> h_4 >> roughness;
    DiamondSquareAlgorithm(h_1, h_2, h_3, h_4, roughness);
    for (int y = 0; y < SIZE; ++y)
    {
        for (int x = 0; x < SIZE; ++x)
        {
            cout << heightMap[y][x] << " ";
        }
        cout << "\n";
    }
    //glfwSetScrollCallback(window, scroll_callback);
    //glfwSetCursorPosCallback(window, cursor_position_callback);
    //while (!glfwWindowShouldClose(window)){
    //    glClear(GL_COLOR_BUFFER_BIT);
    //    drawMap();
    //    glfwSwapBuffers(window);
    //    glfwPollEvents();
    //}
    //glfwTerminate();
    return 0;
}