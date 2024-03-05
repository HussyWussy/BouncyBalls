#include <iostream>
#include <memory>
#include <fstream>

#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"

#include "imgui-sfml/imgui-SFML.h"
int main(int argc,char* argv[])
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");
    sf::CircleShape shape(100.f);
    window.setFramerateLimit(60);
    shape.setFillColor(sf::Color::Green);



    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    ImGui::GetStyle().ScaleAllSizes(1.0f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::Begin("Lao");
        //begin ile end arasýna arayüzü yazýyoruz
        ImGui::Text("Bao");
        ImGui::SameLine();
        ImGui::End();
        
        
        
        
        


        window.clear();
        //window.draw(shape);

        ImGui::SFML::Render(window);
        window.display();

    }
    

    return 0;
}