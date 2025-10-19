#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <tuple>
#include "tgaimage.h"
#include "model.h"


constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

class TimeTracker
{
public:
    TimeTracker(const char* function_name) : _function_name(function_name)
    {
        _start_time = std::chrono::steady_clock::now();
    }
    
    ~TimeTracker()
    {
        std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
        auto lapse = end_time - _start_time;
        using _seconds = std::chrono::seconds;
        auto time_in_seconds = std::chrono::duration_cast<_seconds>(lapse);
        std::cout << "function " << _function_name << " took: " <<  time_in_seconds;
    }

private:
    std::chrono::steady_clock::time_point _start_time;
    const char* _function_name;
};

void plot_line(int a_x, int a_y, int b_x, int b_y, TGAImage& frame_buffer, TGAColor color)
{
    bool steep = std::abs(a_x-b_x) < std::abs(a_y-b_y);
    if (steep) { // if the line is steep, we transpose the image
        std::swap(a_x, a_y);
        std::swap(b_x, b_y);
    }
    if (a_x>b_x) { // make it left−to−right
        std::swap(a_x, b_x);
        std::swap(a_y, b_y);
    }
    int y = a_y;
    int ierror = 0;
    for (int x=a_x; x<=b_x; x++) 
    {
        if (steep) // if transposed, de−transpose
            frame_buffer.set(y, x, color);
        else
            frame_buffer.set(x, y, color);
        ierror += 2 * std::abs(b_y-a_y);
        if (ierror > b_x - a_x) {
            y += b_y > a_y ? 1 : -1;
            ierror -= 2 * (b_x-a_x);
        }
    }
}


std::tuple<int, int> project(Vec3f vec3, float offset_x, float offset_y, int width, int height)
{
    return {
        round((vec3.x + offset_x) * width * 0.5),
        round((vec3.y + offset_y) * height * 0.5)
    };
}

int main(int argc, char** argv) {
    constexpr int width  = 800;
    constexpr int height = 800;
    TGAImage framebuffer(width, height, TGAImage::RGB);
    std::string model_path = "../obj/diablo3_pose/diablo3_pose.obj";
    Model model = loadModel(model_path);

    std::srand(std::time({}));
    TimeTracker timer = TimeTracker("draw lines method");
    for (int i=0; i< model.numberOfFaces(); i++) 
    {
        Vec3f a = model.getVertexFromFace(i, 0);
        Vec3f b = model.getVertexFromFace(i, 1);
        Vec3f c = model.getVertexFromFace(i, 2);
        auto [ax, ay] = project(a, 1.0, 1.0, width, height);
        auto [bx, by] = project(b, 1.0, 1.0, width, height);
        auto [cx, cy] = project(c, 1.0, 1.0, width, height);

        plot_line(ax, ay, bx, by, framebuffer, red);
        plot_line(bx, by, cx, cy, framebuffer, green);
        plot_line(cx, cy, ax, ay, framebuffer, blue);

    }

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}

