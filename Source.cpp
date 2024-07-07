#include <iostream>
#include <memory>
#include <fstream>
#include <chrono>

#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"

#include "imgui-sfml/imgui-SFML.h"

static int idCounter = 0;

class bounceObject : public sf::CircleShape{
    public:
        std::string name = "##";
        int id = NULL;
        int bounce = 0;
        int xvel = 1;
        int yvel = 1;
        float speed = 1;
        std::chrono::steady_clock::time_point startTime;

        bounceObject(float f,std::string newname = "##", sf::Color color = sf::Color::Cyan, int radius = 100, float posx = 500, float posy = 100) : sf::CircleShape::CircleShape(f) {
            startTime = std::chrono::steady_clock::now();
            id = idCounter;
            idCounter++;
            name = newname;

            setFillColor(color);
            setRadius(radius);
            setOrigin(getRadius(), getRadius());
            setPosition(posx, posy);
            
        }
        void bounceBetweenVertical() {
            yvel = -yvel;
            speed *= 1;
            bounce++;
        }
        void bounceBetweenHorizontal() {
            xvel = -xvel;
            speed *= 1;
            bounce++;
        }
        

};



int main(int argc,char* argv[])
{
    sf::Clock deltaClock;
    const int WIDTH = 1280;
    const int HEIGHT = 720;
    static std::vector <bounceObject> BALLS;
    
    bounceObject test(100.f,"test");
    bounceObject cu(100.f,"heeeelp", sf::Color::Red, 50, 600, 200);
    BALLS.push_back(test);
    BALLS.push_back(cu);
    static bounceObject *selectedBall = &BALLS[0];

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");
       
    window.setFramerateLimit(60);


    ImGui::SFML::Init(window);

    ImGui::GetStyle().ScaleAllSizes(1.0f);

    static bool sizeswitch = false;
    static int radius = selectedBall->getRadius();
    static std::string newBallName{""};
    static bool makeNewBall = false;
    static ImVec4 color = selectedBall->getFillColor();
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
        if (ImGui::BeginCombo("HELP", selectedBall->name.c_str()))
        {
            for (int n = 0; n < BALLS.size(); n++)
            {

                bool is_selected = (selectedBall->id == BALLS[n].id);
                if (ImGui::Selectable(BALLS[n].name.c_str(), is_selected))
                {
                    selectedBall = &BALLS[n];
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   
            }
            ImGui::EndCombo();
        }


        if (radius != selectedBall->getRadius()) {
            radius = selectedBall->getRadius();
        }
        if (color != selectedBall->getFillColor()) {
            color = selectedBall->getFillColor();
        }

       
        ImGui::SliderInt("cao", &radius, 0, 500);
        ImGui::SliderFloat("vao", &selectedBall->speed, 0, 100);
        ImGui::Text("Toplam sekme : %d", selectedBall->bounce);


        if (radius != selectedBall->getRadius())
        {
            selectedBall->setRadius(radius);
            selectedBall->setOrigin(selectedBall->getRadius(), selectedBall->getRadius());
        }

        ImGui::ColorPicker4("renk secimi", (float*)&color);


        ImGui::InputText("isim",newBallName.data(), 30);

       
        if (ImGui::Button("Yeni Oluþtur"))
        {
            bounceObject newOne(100.f, newBallName.c_str(), sf::Color::White, 50, 600, 200);
            BALLS.push_back(newOne);
            selectedBall = &newOne;
        }

        ImGui::End();
        
        
        if (color != selectedBall->getFillColor()) {
            selectedBall->setFillColor(color);
        }
        
        
        

        window.clear();
 

        for (bounceObject &ball : BALLS) {
            window.draw(ball);
            //TODO toplara verilen isimler toplarýn üstünde gözükecek.
        }
        for (bounceObject &ball : BALLS) {
            ball.move(ball.xvel * ball.speed, ball.yvel * ball.speed);
        }

       
        for (bounceObject &ball : BALLS) {

            if (ball.getPosition().x + ball.getRadius() > WIDTH || ball.getPosition().x - ball.getRadius() < 0)
            {
                ball.bounceBetweenHorizontal();
            }
            if (ball.getPosition().y + ball.getRadius() > HEIGHT || ball.getPosition().y - ball.getRadius() < 0)
            {
                ball.bounceBetweenVertical();
            }
        }

        
        ImGui::SFML::Render(window);
        window.display();

    }
    

    return 0;
}