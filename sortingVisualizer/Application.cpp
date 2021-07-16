#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"

#include <stack>
#include <limits> 
#include <ios> 
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include <stdlib.h>
#include <windows.h>
#include <chrono>
#include <conio.h>
#include <time.h>
#include <queue>

using namespace std;
void Draw(vector<int>& colors, Renderer& renderer, Shader& shader, vector<VertexArray>& vas, vector<glm::vec3>& translations, glm::mat4& result, IndexBuffer& ib, GLFWwindow* window)
{
    renderer.Clear();

    for (int i = 0; i < 30; i++)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translations[i]);
        glm::mat4 mvp = result * model;
        if (colors[i] == 0)
        {
            shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

        }
        else if (colors[i] == 1)
        {
            shader.SetUniform4f("u_Color", 1.0f, 0.63f, 1.0f, 1.0f);

        }
        else if (colors[i] == 2)
        {
            shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);

        }
        shader.Bind();
        vas[i].Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(vas[i], ib, shader);
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
}
int partition(int arr[], int low, int high)
{
    int pivot = arr[high]; // pivot
    int i = (low - 1); // Index of smaller element and indicates the right position of pivot found so far

    for (int j = low; j <= high - 1; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j] < pivot)
        {
            i++; // increment index of smaller element
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}
void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
        at right place */
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
void bubblesort(vector<int>& colors,vector<float> &array, int const n, Renderer& renderer, Shader& shader, vector<VertexArray>& vas, vector<glm::vec3>& translations, glm::mat4& result, IndexBuffer& ib, GLFWwindow* window)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
    {
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
        {
            colors[j] = 1;
            colors[j + 1] = 1;
            Draw(colors, renderer, shader, vas, translations, result, ib, window);
            if (array[j] > array[j + 1])
            {
                colors[j] = 2;
                colors[j + 1] = 2;
                Draw(colors, renderer, shader, vas, translations, result, ib, window);
                colors[j] = 0;
                colors[j + 1] = 0;

                swap(array[j], array[j + 1]);
                //swap(vas[j], vas[j + 1]);
                float position1[] = {
                   0.0f,   0.0f,   //Bottom Left
                   20.0f,  0.0f,   //Bottom Right
                   20.f,   array[j], //Top right
                   0.0f,   array[j] //Top Left 
                };
                float position2[] = {
                   0.0f,   0.0f,   //Bottom Left
                   20.0f,  0.0f,   //Bottom Right
                   20.f,   array[j+1], //Top right
                   0.0f,   array[j+1] //Top Left 
                };
                vas[j].changeData(position1);
                vas[j].UnBind();
                vas[j+1].changeData(position2);
                vas[j+1].UnBind();
                //swap(translations[j].x, translations[j + 1].x);
                Draw(colors, renderer, shader, vas, translations, result, ib, window);

            }
            else
            {
                colors[j] = 0;
                colors[j + 1] = 0;
                Draw(colors, renderer, shader, vas, translations, result, ib, window);

            }
            
        }
    }
}

// begin is for left index and end is
// right index of the sub-array
// of arr to be sorted */
//void mergeSort(vector<int>& colors,vector<float>array, int const begin, int const end, Renderer& renderer, Shader& shader, vector<VertexArray>& vas, vector<glm::vec3>& translations, glm::mat4& result, IndexBuffer& ib, GLFWwindow* window)
//{
//    if (begin >= end)
//        return; // Returns recursivly
//
//    auto mid = begin + (end - begin) / 2;
//    mergeSort(colors,array, begin, mid, renderer, shader, vas, translations, result, ib, window);
//    mergeSort(colors,array, mid + 1, end, renderer, shader, vas, translations, result, ib, window);
//    merge(colors,array, begin, mid, end, renderer, shader, vas, translations, result, ib, window);
//}


int main(void)
{

    GLFWwindow* window;
    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(960, 960, "Searching Visualizer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewInit();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //////////////////////////////////////////////////////////////

    vector<vector<float>> positions(30);
    vector<VertexArray> vas(30);
    VertexBufferLayout layouts;
    layouts.Push<float>(2);

    //    = {
    //    0.0f,   0.0f,   //Bottom Left
    //    20.0f,  0.0f,   //Bottom Right
    //    20.0f,  20.0f,  //Top right
    //    0.0f,   20.0f,  //Top Left 
    //};
    srand((unsigned)time(0));
    vector<float> heights(30);

    for (int i = 0; i < 30; i++)
    {
        float result = 10 + (rand() % 960);
        heights[i] = result;
        float position[] = {
            0.0f,   0.0f,   //Bottom Left
            20.0f,  0.0f,   //Bottom Right
            20.f,   result, //Top right
            0.0f,   result  //Top Left 
        };
        VertexBuffer* vb = new VertexBuffer(position, 8 * sizeof(float));
        vas[i].AddBuffer(*vb, layouts);
    }


    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    IndexBuffer ib(indices, 6);

    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 960.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);
    glm::mat4 result = proj * view;

    Shader ss;
    ss.Bind();
    ib.UnBind();
    /*for (int i = 0; i < 20; i++)
    {
        vas[i].UnBind();
    }*/

    Renderer renderer;
    vector<glm::vec3> translations(30);

    Shader shader;
    float stepX = 0;
    vector<int> colors(30);
    for (int i = 0; i < 30; i++)
    {
        translations[i].x = stepX;
        translations[i].y = 0;
        stepX += 25;
    }

    bubblesort(colors, heights, 30, renderer, shader, vas, translations, result, ib, window);
    while (!glfwWindowShouldClose(window))
    {
        Draw(colors,renderer,shader,vas,translations,result,ib,window);
    }
    glfwTerminate();
    return 0;
}