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
        ImGui::Begin(u8"�����ļ���");
        ImGui::InputText(u8"�����ļ���(����׺)", inputText, sizeof(inputText));
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
        // ���ô���͸����
        ImGui::SetNextWindowBgAlpha(0.5f); // ���ñ���͸����

        // ��ʼ���ƴ���
        ImGui::Begin(u8"Tips");
        if (ImGui::Button(u8"�ر�"))
            show_hint = false;
        // ���UIԪ��
        ImGui::Text(u8"<����GeoKurumi>");
        ImGui::Bullet();
        ImGui::Text(u8"������GLFW,Glad,glm,ImGui��Assimp");
        ImGui::BulletText(u8"�ο�: https://learnopengl-cn.github.io/");

        ImGui::Separator();
        ImGui::BulletText(u8"WASD���ƶ������λ��");
        ImGui::BulletText(u8"��ס������϶����ɱ仯�ӽ�");
        ImGui::BulletText(u8"��סALT����������������");
        ImGui::BulletText(u8"ģ���ļ�Ŀ¼��res/model\n�����ļ�Ŀ¼:save");
        ImGui::End();
        return;
    }

    void top_menu(ImGui::FileBrowser& fileDialog, std::string& path, std::string& filename, Model& ourModel, switcher& enable)
    {
        bool show_question_window = false;
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu(u8"�ļ�"))
            {
                // ���ⴰ��
                if (ImGui::BeginMenu(u8"��"))
                {
                    if (ImGui::MenuItem(u8"���ļ�"))
                    {
                        // ���ļ��Ի���ť
                        fileDialog.Open();
                    }
                    ImGui::EndMenu();
                }
                // �����˵���
                if (ImGui::BeginMenu(u8"����"))
                {
                    if(ImGui::MenuItem(u8"������GeoKurumi/save"))
                    {   
                        enable.save = true;
                    }

                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }
            // �����˵���ѡ��
            ImGui::EndMainMenuBar();
        }
        // ��Ⱦ�ļ��Ի���
        fileDialog.Display();

        if (fileDialog.HasSelected())
        {
            // �ļ��Ի���ѡ�����ļ�
            // ����ѡ����
            path = fileDialog.GetSelected().parent_path().string();
            filename = fileDialog.GetSelected().filename().string();
            std::cout << "Selected filename: " << fileDialog.GetSelected().string() << std::endl;
            
            fileDialog.ClearSelected();
            
        }
        
    }

    void left_window(switcher &enable, ImVec4& clear_color, Camera& camera, float Framerate, glm::vec4& model,glm::vec3 &shift)
    {   
        static float f = 0.0f;
        ImGui::Begin(u8"����");                              // Create a window called "Hello, world!" and append into it.

        if (ImGui::IsWindowHovered())
        {
            enable.mouse_enable = false;
        }
        else enable.mouse_enable = true;

        ImGui::Text(u8"��ʾ����");                            // Display some text (you can use a format strings too)
        ImGui::Checkbox(u8"������״̬", &enable.mouse_enable);
        ImGui::SliderFloat(u8"����", &camera.Zoom, 90.0f, 0.0f);
        ImGui::ColorEdit3(u8"������ɫ��", (float*)&clear_color); // Edit 3 floats representing a color
        
        if (ImGui::Button(u8"���������"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        {
            camera.Position = glm::vec3(0.0f, 0.0f, 20.0f);
            camera.Yaw = -90.0f;
            camera.Pitch = 0.0f;
        }

        ImGui::Separator();
        ImGui::Text(u8"�������������");
        ImGui::InputFloat(u8"�����x����", &camera.Position.x);
        ImGui::InputFloat(u8"�����y����", &camera.Position.y);
        ImGui::InputFloat(u8"�����z����", &camera.Position.z);

        ImGui::Separator();

        ImGui::Text(u8"ģ����̬����");
        ImGui::SliderFloat(u8"��x��ת", &model.x, -180.0f, 180.0f); 
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            enable.mouse_enable = false;
        }
        ImGui::SameLine(); ImGui::Text(u8"%.3f��", model.x);
        
        ImGui::SliderFloat(u8"��y��ת", &model.y, -180.0f, 180.0f); 
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            enable.mouse_enable = false;
        }
        ImGui::SameLine(); ImGui::Text(u8"%.3f��", model.y);
        
        ImGui::SliderFloat(u8"��z��ת", &model.z, -180.0f, 180.0f); 
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            enable.mouse_enable = false;
        }ImGui::SameLine(); ImGui::Text(u8"%.3f��", model.z);

        ImGui::Separator();
        ImGui::Text(u8"ģ����������");
        ImGui::SliderFloat(u8"ģ��x����", &shift.x, -10.0f, 10.0f); 
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            enable.mouse_enable = false;
        }
        ImGui::SameLine(); ImGui::Text(u8"%.3f", shift.x);

        ImGui::SliderFloat(u8"ģ��y����", &shift.y, -10.0f, 10.0f); 
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            enable.mouse_enable = false;
        }
        ImGui::SameLine(); ImGui::Text(u8"%.3f", shift.y);

        ImGui::SliderFloat(u8"ģ��z����", &shift.z, -10.0f, 10.0f); 
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            enable.mouse_enable = false;
        }
        ImGui::SameLine(); ImGui::Text(u8"%.3f", shift.z);
        ImGui::Separator();
        ImGui::Text(u8"libigl����ʵ�֣�");

        ImGui::BulletText(u8"MatCaps:������ʷ�����ǿ");
        ImGui::Text(u8"      �����滻����C������ǿ���ʷ���\n      ע��˲������ɳ���\n      ��ΰ���C�ᵼ�²����ص�");

        ImGui::BulletText(u8"���ӻ���ǿ");
        ImGui::Text(u8"      ֱ�Ӱ���B�������л�������ǿ��ʽ");

        ImGui::Separator();
        ImGui::Text(u8"������");
        ImGui::Bullet();
        ImGui::Text(u8"ǰ������(%.3f , %.3f, %.3f)", camera.Front.x, camera.Front.y, camera.Front.z);
        ImGui::Bullet();
        ImGui::Text(u8"��������� (%.3f , %.3f, %.3f)", camera.Position.x, camera.Position.y, camera.Position.z);
        ImGui::Bullet();
        ImGui::Text(u8"ʵʱ֡�� %.3f ms/frame (%.1f FPS)", 1000.0f / Framerate, Framerate);
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
