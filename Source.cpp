#include <iostream>
#include <memory>
#include <fstream>
#include <chrono>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"

#include "imgui-sfml/imgui-SFML.h"

static int idCounter = 0;

class bounceObject : public sf::CircleShape
{
    //TODO deðiþkenleri private yapýp setter ve getter yazmak
    public:
        std::string name = "##";
        sf::Text text;
        float density = 1;
        int id = NULL;
        int bounce = 0;
        sf::Vector2f direction = {1,1};
        float speed = 1;
        std::chrono::steady_clock::time_point startTime;

        bounceObject(float f,std::string newname,sf::Font &font, sf::Color color = sf::Color::Cyan, int radius = 100, float posx = 500, float posy = 100) : sf::CircleShape::CircleShape(f)
        {
            startTime = std::chrono::steady_clock::now();
            id = idCounter;
            idCounter++;
            name = newname;

            text.setString(newname);
            text.setCharacterSize(24);
            text.setFont(font);
            text.setFillColor(sf::Color::Black);
            text.setStyle(sf::Text::Bold | sf::Text::Underlined);

            setFillColor(color);
            setRadius(radius);
            setOrigin(getRadius(), getRadius());
            setPosition(posx, posy);
            text.setPosition(getPosition());

        }
        void bounceBetweenVertical() 
        {
            direction.y = -direction.y;
            bounce++;
        }
        void bounceBetweenHorizontal() 
        {
            direction.x = -direction.x;
            bounce++;
        }
        void setDirection(sf::Vector2f newDirection)
        {
            direction = newDirection;
        }
        bool operator == (const bounceObject &obj)
        {
            if (id == obj.id)
            {
                return true;
            }
            return false;
        }
        


        bounceObject() = default;
};



int main(int argc,char* argv[])
{
    sf::Clock deltaClock;
    const int WIDTH = 1280;
    const int HEIGHT = 720;
    const float PI = 3.14;
    static std::vector <bounceObject> BALLS;

    static sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cout << "can't load font" << std::endl;
    }
    
    bounceObject test(100.f,"test",font);
    bounceObject cu(100.f,"heeeelp",font, sf::Color::Red, 50, 100, 200);
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
    
    static bool ballToBallCollison = false;
    
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


        if (radius != selectedBall->getRadius()) 
        {
            radius = selectedBall->getRadius();
        }
        if (color != selectedBall->getFillColor()) 
        {
            color = selectedBall->getFillColor();
        }

       
        ImGui::SliderInt("cao", &radius, 0, 500);
        ImGui::SliderFloat("vao", &selectedBall->speed, 0, 100);
        ImGui::Text("Toplam sekme : %d", selectedBall->bounce);
        ImGui::ColorPicker4("renk secimi", (float*)&color);


        if (radius != selectedBall->getRadius())
        {
            selectedBall->setRadius(radius);
            selectedBall->setOrigin(selectedBall->getRadius(), selectedBall->getRadius());
        }

        if (color != selectedBall->getFillColor()) {
            selectedBall->setFillColor(color);
        }


        if (ImGui::Button("Sil"))
        {
            int positionCount = 0;
            for (auto ball : BALLS)
            {
                if (ball == *selectedBall)
                {
                    break;
                }
                else {
                    positionCount++;
                }
            }
            BALLS.erase(BALLS.begin() + positionCount);
        }

        ImGui::InputText("Olusturulacak isim",newBallName.data(), 30);

       
        if (ImGui::Button("Yeni Oluþtur") )
        {
            std::string stringBuffer = newBallName.data();
            if (stringBuffer.empty())
            {
                stringBuffer = std::to_string(idCounter + 1);
            }
            bounceObject newOne(100.f, stringBuffer.c_str(),font, sf::Color::White, 50, 600, 200);
            BALLS.push_back(newOne);
            selectedBall = &newOne;
        }

        ImGui::Checkbox("Carpisma", &ballToBallCollison);

        ImGui::End();
        
        
        
        
        
        

        window.clear();
        
        //render
        for (bounceObject &ball : BALLS) 
        {
            window.draw(ball);
            
        }
        //yazý render
        for (bounceObject &ball : BALLS)
        {
            window.draw(ball.text);

        }
        //hareket
        for (bounceObject &ball : BALLS) 
        {
            ball.move(ball.direction * ball.speed );
            ball.text.setPosition(ball.getPosition());
        }
        
        //sýnýrlardan sekme
        for (bounceObject &ball : BALLS) 
        {

            if (ball.getPosition().x + ball.getRadius() > WIDTH || ball.getPosition().x - ball.getRadius() < 0)
            {
                ball.bounceBetweenHorizontal();
            }
            if (ball.getPosition().y + ball.getRadius() > HEIGHT || ball.getPosition().y - ball.getRadius() < 0)
            {
                ball.bounceBetweenVertical();
            }
        }
        //toplardan sekme
        if (ballToBallCollison) {
            for (bounceObject &ball : BALLS)
            {
                
                for (bounceObject &otherBall : BALLS)
                {
                    if (ball == otherBall)
                        continue;
                    //kopya çekilmiþ kod
                    double d = sqrt((ball.getPosition().x - otherBall.getPosition().x) * (ball.getPosition().x - otherBall.getPosition().x)
                        + (ball.getPosition().y - otherBall.getPosition().y) * (ball.getPosition().y - otherBall.getPosition().y));
                    
                    if (d < ball.getRadius() + otherBall.getRadius()) 
                    {
                        //kütleyi yoðunluk * pi r^2 den hesaplýyoruz 
                        
                        float massOfBall = PI * ball.getRadius() * ball.getRadius() * ball.density;
                        float massOfOtherBall = PI * otherBall.getRadius() * otherBall.getRadius() * otherBall.density;
                        float ballCollisonDegree = acos(ball.direction.x * ball.direction.y);
                        //float otherBallCollisonDegree = ball
                        

                    }
                    else if (d == ball.getRadius() + otherBall.getRadius()) 
                    {
                        std::cout << "Circle touch to each other";
                        
                    }
                    else {
                      //  std::cout << "Circle not touch to each other";
                    }
                }
            }
        }

     
        ImGui::SFML::Render(window);
        window.display();

    }
    free(selectedBall);

    return 0;
}