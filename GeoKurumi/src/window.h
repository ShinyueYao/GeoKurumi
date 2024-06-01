#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imfilebrowser.h"

#include <iostream>
#include <fstream>
#include <string>

#include "camera.h"
#include "switcher.h"
#include "Model.h"

namespace MyWindow
{
    
    bool SaveFile(const std::string& sourcePath, const std::string& destinationPath);
    void show_in(const string& path,const string& filename,switcher& enable)
    {
        bool tem = enable.mouse_enable;
        enable.mouse_enable = false;
        char inputText[256] = "";
        ImGui::SetNextWindowPos(ImVec2(700,300));
        ImGui::Begin(u8"输入文件名");
        ImGui::InputText(u8"保存文件名(带后缀)", inputText, sizeof(inputText));
        string save_name(inputText);
        if (ImGui::IsKeyPressed(ImGuiKey_Enter))
        {
            SaveFile(path + "\\" + filename, "save\\" + save_name);
            enable.save = false;
            enable.mouse_enable = tem;
        }
        ImGui::End();
    }
    
    void hint(bool &show_hint, switcher& enable)
    {
        if (ImGui::IsWindowHovered())
        {
            enable.mouse_enable = false;
        }
        else enable.mouse_enable = true;
        // 设置窗口透明度
        ImGui::SetNextWindowBgAlpha(0.5f); // 设置背景透明度

        // 开始绘制窗口
        ImGui::Begin(u8"Tips");
        if (ImGui::Button(u8"关闭"))
            show_hint = false;
        // 添加UI元素
        ImGui::Text(u8"<关于GeoKurumi>");
        ImGui::Bullet();
        ImGui::Text(u8"依赖：GLFW,Glad,glm,ImGui，Assimp");
        ImGui::BulletText(u8"参考: https://learnopengl-cn.github.io/");

        ImGui::Separator();
        ImGui::BulletText(u8"WASD以移动摄像机位置");
        ImGui::BulletText(u8"按住左键，拖动鼠标可变化视角");
        ImGui::BulletText(u8"按住ALT来禁用鼠标键盘输入");
        ImGui::BulletText(u8"模型文件目录：res/model\n保存文件目录:save");
        ImGui::End();
        return;
    }

    void top_menu(ImGui::FileBrowser& fileDialog, std::string& path, std::string& filename, Model& ourModel, switcher& enable)
    {
        bool show_question_window = false;
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu(u8"文件"))
            {
                // 虚拟窗口
                if (ImGui::BeginMenu(u8"打开"))
                {
                    if (ImGui::MenuItem(u8"打开文件"))
                    {
                        // 打开文件对话框按钮
                        fileDialog.Open();
                    }
                    ImGui::EndMenu();
                }
                // 其他菜单项
                if (ImGui::BeginMenu(u8"保存"))
                {
                    if(ImGui::MenuItem(u8"保存至GeoKurumi/save"))
                    {   
                        enable.save = true;
                    }

                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }
            // 其他菜单栏选项
            ImGui::EndMainMenuBar();
        }
        // 渲染文件对话框
        fileDialog.Display();

        if (fileDialog.HasSelected())
        {
            // 文件对话框选择了文件
            // 处理选择结果
            path = fileDialog.GetSelected().parent_path().string();
            filename = fileDialog.GetSelected().filename().string();
            std::cout << "Selected filename: " << fileDialog.GetSelected().string() << std::endl;
            
            fileDialog.ClearSelected();
            
        }
        
    }

    void left_window(switcher &enable, ImVec4& clear_color, Camera& camera, float Framerate, glm::vec4& model,glm::vec3 &shift)
    {   
        static float f = 0.0f;
        ImGui::Begin(u8"属性");                              // Create a window called "Hello, world!" and append into it.

        if (ImGui::IsWindowHovered())
        {
            enable.mouse_enable = false;
        }
        else enable.mouse_enable = true;

        ImGui::Text(u8"显示窗口");                            // Display some text (you can use a format strings too)
        ImGui::Checkbox(u8"鼠标键盘状态", &enable.mouse_enable);
        ImGui::SliderFloat(u8"缩放", &camera.Zoom, 90.0f, 0.0f);
        ImGui::ColorEdit3(u8"背景调色盘", (float*)&clear_color); // Edit 3 floats representing a color
        
        if (ImGui::Button(u8"摄像机重置"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        {
            camera.Position = glm::vec3(0.0f, 0.0f, 20.0f);
            camera.Yaw = -90.0f;
            camera.Pitch = 0.0f;
        }

        ImGui::Separator();
        ImGui::Text(u8"摄像机坐标设置");
        ImGui::InputFloat(u8"摄像机x坐标", &camera.Position.x);
        ImGui::InputFloat(u8"摄像机y坐标", &camera.Position.y);
        ImGui::InputFloat(u8"摄像机z坐标", &camera.Position.z);

        ImGui::Separator();

        ImGui::Text(u8"模型姿态设置");
        ImGui::SliderFloat(u8"绕x旋转", &model.x, -180.0f, 180.0f); 
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            enable.mouse_enable = false;
        }
        ImGui::SameLine(); ImGui::Text(u8"%.3f度", model.x);
        
        ImGui::SliderFloat(u8"绕y旋转", &model.y, -180.0f, 180.0f); 
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            enable.mouse_enable = false;
        }
        ImGui::SameLine(); ImGui::Text(u8"%.3f度", model.y);
        
        ImGui::SliderFloat(u8"绕z旋转", &model.z, -180.0f, 180.0f); 
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            enable.mouse_enable = false;
        }ImGui::SameLine(); ImGui::Text(u8"%.3f度", model.z);

        ImGui::Separator();
        ImGui::Text(u8"模型坐标设置");
        ImGui::SliderFloat(u8"模型x坐标", &shift.x, -10.0f, 10.0f); 
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            enable.mouse_enable = false;
        }
        ImGui::SameLine(); ImGui::Text(u8"%.3f", shift.x);

        ImGui::SliderFloat(u8"模型y坐标", &shift.y, -10.0f, 10.0f); 
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            enable.mouse_enable = false;
        }
        ImGui::SameLine(); ImGui::Text(u8"%.3f", shift.y);

        ImGui::SliderFloat(u8"模型z坐标", &shift.z, -10.0f, 10.0f); 
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            enable.mouse_enable = false;
        }
        ImGui::SameLine(); ImGui::Text(u8"%.3f", shift.z);
        ImGui::Separator();
        ImGui::Text(u8"libigl功能实现：");

        ImGui::BulletText(u8"MatCaps:表面材质反射增强");
        ImGui::Text(u8"      材质替换按下C启用增强材质反射\n      注意此操作不可撤回\n      多次按下C会导致材质重叠");

        ImGui::BulletText(u8"可视化增强");
        ImGui::Text(u8"      直接按下B键即可切换光线增强形式");

        ImGui::Separator();
        ImGui::Text(u8"参数表");
        ImGui::Bullet();
        ImGui::Text(u8"前进向量(%.3f , %.3f, %.3f)", camera.Front.x, camera.Front.y, camera.Front.z);
        ImGui::Bullet();
        ImGui::Text(u8"摄像机坐标 (%.3f , %.3f, %.3f)", camera.Position.x, camera.Position.y, camera.Position.z);
        ImGui::Bullet();
        ImGui::Text(u8"实时帧率 %.3f ms/frame (%.1f FPS)", 1000.0f / Framerate, Framerate);
        ImGui::End();
        return;
    }
    bool SaveFile(const std::string& sourcePath, const std::string& destinationPath)
    {
        std::ifstream sourceFile(sourcePath, std::ios::binary);
        if (!sourceFile)
        {
            std::cout << "Failed to open source file: " << sourcePath << std::endl;
            return false;
        }

        std::ofstream destinationFile(destinationPath, std::ios::binary);
        if (!destinationFile)
        {
            std::cout << "Failed to create destination file: " << destinationPath << std::endl;
            return false;
        }

        destinationFile << sourceFile.rdbuf();

        if (sourceFile.bad())
        {
            std::cout << "Failed to read from source file: " << sourcePath << std::endl;
            return false;
        }

        if (destinationFile.bad())
        {
            std::cout << "Failed to write to destination file: " << destinationPath << std::endl;
            return false;
        }
        std::cout << "File saved successfully: " << sourcePath << " -> " << destinationPath << std::endl;
        return true;
    }
}
