#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <raylib-cpp.hpp>

#include "hidapi/hidapi.h"

#define MAX_STR 255

int main()
{
    // Initialization
    int screenWidth = 1400;
    int screenHeight = 800;

    // hidapi
    raylib::Color textColor(RED);
    raylib::Window w(screenWidth, screenHeight, "Raylib C++ Starter Kit Example");
    int res;
    wchar_t wstr[MAX_STR];
    hid_device *handle;
    int i;
    uint8_t buf[65];
    int32_t data[3];
    std::string hid_str;

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 20.0f, 20.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    // Initialize the hidapi library
    res = hid_init();

    // Open the device using the VID, PID,
    // and optionally the Serial number.
    handle = hid_open(0x0483, 0x5750, NULL);
    if (handle == nullptr)
    {
        std::cout << "Can't Open hid device" << std::endl;
        exit(-1);
    }

    //// Read the Manufacturer String
    res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
    std::cout << "---- Manufacturer String: " << wstr << std::endl;

    //// Read the Product String
    res = hid_get_product_string(handle, wstr, MAX_STR);
    std::cout << "---- Product String: " << wstr << std::endl;

    //// Read Indexed String 1
    res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
    std::cout << "---- Indexed String 1: " << wstr << std::endl;

    SetTargetFPS(144);

    // Main game loop
    while (!w.ShouldClose())  // Detect window close button or ESC key
    {
        // Update

        memset(buf, 0b0, sizeof(buf));
        // TODO: Update your variables here

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
				//buf[0] = 0x0;
				//buf[1] = 0x81;
				//res = hid_write(handle, buf, 65);
				res = hid_read(handle, buf, 65);
				for (int i = 0; i < 3; ++i)
				{
						data[i] = (buf[i * 4 + 3] << 24) | (buf[i * 4 + 2] << 16) | (buf[i * 4 + 1] << 8) | buf[i * 4 + 0];
				}
				std::sprintf((char *)buf, "-> %d %d %d <-\n", data[0], data[1], data[2]);
				hid_str.append((const char *)buf);
				textColor.DrawText("hid in :", 190, 200, 20);
				textColor.DrawText(hid_str, 190, 220, 20);
				hid_str.clear();

        BeginMode3D(camera);

        cubePosition.x = data[1] * 0.1;
        cubePosition.y = data[2] * 0.1;
        cubePosition.z = (data[0] * 0.1) - 5;
        DrawCube(cubePosition, 1.0f, 1.0f, 1.0f, RAYWHITE);
        DrawCube(Vector3{5,5,0}, 1.0f, 1.0f, 1.0f, RAYWHITE);
        DrawCubeWires(cubePosition, 1.0f, 1.0f, 1.0f, MAROON);

        DrawGrid(10, 1.0f);

        EndMode3D();

        DrawText(hid_str.c_str(), 10, 40, 20, RED);

        DrawFPS(10, 10);

        EndDrawing();
    }

    res = hid_exit();

    return 0;
}
