#include "mainUI.h"
#include "main.h"
#include "shipSelectionScreen.h"

void MainUI::onGui()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
    {
        destroy();
        new ShipSelectionScreen();
    }
    
    if (gameServer)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            engine->setGameSpeed(1.0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            engine->setGameSpeed(0.0);
#ifdef DEBUG
        text(sf::FloatRect(0, 0, 1600, 20), string(gameServer->getSendDataRate()) + " bytes per second", AlignRight, 15);
#endif
    }
    
    if (engine->getGameSpeed() == 0.0)
    {
        text(sf::FloatRect(0, 400, 1600, 100), "Game Paused", AlignCenter, 70);
        if (gameServer)
            text(sf::FloatRect(0, 480, 1600, 30), "(Press [SPACE] to resume)", AlignCenter, 30);
    }
}

void MainUI::drawStatic(float alpha)
{
    sf::Sprite staticDisplay;
    textureManager.getTexture("noise.png")->setRepeated(true);
    textureManager.setTexture(staticDisplay, "noise.png");
    staticDisplay.setTextureRect(sf::IntRect(0, 0, 2048, 2048));
    staticDisplay.setOrigin(sf::Vector2f(1024, 1024));
    staticDisplay.setScale(3.0, 3.0);
    staticDisplay.setPosition(sf::Vector2f(random(-512, 512), random(-512, 512)));
    staticDisplay.setColor(sf::Color(255, 255, 255, 255*alpha));
    getRenderTarget()->draw(staticDisplay);
}

void MainUI::drawRaderBackground(sf::Vector2f position, float size, float scale)
{
    const static float sector_size = 20000;
    sf::Vector2f player_position = mySpaceship->getPosition();
    int sector_x_min = int((player_position.x - (size / scale)) / sector_size) - 1;
    int sector_x_max = int((player_position.x + (size / scale)) / sector_size);
    int sector_y_min = int((player_position.y - (size / scale)) / sector_size) - 1;
    int sector_y_max = int((player_position.y + (size / scale)) / sector_size);
    sf::VertexArray lines_x(sf::Lines, 2 * (sector_x_max - sector_x_min + 1));
    sf::VertexArray lines_y(sf::Lines, 2 * (sector_y_max - sector_y_min + 1));
    sf::Color color(64, 64, 128);
    for(int sector_x = sector_x_min; sector_x <= sector_x_max; sector_x++)
    {
        float x = position.x + ((sector_x * sector_size) - player_position.x) * scale;
        lines_x[(sector_x - sector_x_min)*2].position = sf::Vector2f(x, 0);
        lines_x[(sector_x - sector_x_min)*2].color = color;
        lines_x[(sector_x - sector_x_min)*2+1].position = sf::Vector2f(x, 900);
        lines_x[(sector_x - sector_x_min)*2+1].color = color;
        for(int sector_y = sector_y_min; sector_y <= sector_y_max; sector_y++)
        {
            float y = position.y + ((sector_y * sector_size) - player_position.y) * scale;
            text(sf::FloatRect(x, y, 30, 30), string(char('M' + sector_y)) + string(sector_x), AlignLeft, 30, sf::Color(64, 64, 128));
        }
    }
    for(int sector_y = sector_y_min; sector_y <= sector_y_max; sector_y++)
    {
        float y = position.y + ((sector_y * sector_size) - player_position.y) * scale;
        lines_y[(sector_y - sector_y_min)*2].position = sf::Vector2f(0, y);
        lines_y[(sector_y - sector_y_min)*2].color = color;
        lines_y[(sector_y - sector_y_min)*2+1].position = sf::Vector2f(1600, y);
        lines_y[(sector_y - sector_y_min)*2+1].color = color;
    }
    getRenderTarget()->draw(lines_x);
    getRenderTarget()->draw(lines_y);
}

void MainUI::drawHeadingCircle(sf::Vector2f position, float size)
{
    sf::RenderTarget& window = *getRenderTarget();
    
    sf::VertexArray tigs(sf::Lines, 360/20*2);
    for(unsigned int n=0; n<360; n+=20)
    {
        tigs[n/20*2].position = position + sf::vector2FromAngle(float(n)) * size;
        tigs[n/20*2+1].position = position + sf::vector2FromAngle(float(n)) * (size - 20);
    }
    window.draw(tigs);
    sf::VertexArray smallTigs(sf::Lines, 360/5*2);
    for(unsigned int n=0; n<360; n+=5)
    {
        smallTigs[n/5*2].position = position + sf::vector2FromAngle(float(n)) * size;
        smallTigs[n/5*2+1].position = position + sf::vector2FromAngle(float(n)) * (size - 10);
    }
    window.draw(smallTigs);
    for(unsigned int n=0; n<360; n+=20)
    {
        sf::Text text(string(n), mainFont, 15);
        text.setPosition(position + sf::vector2FromAngle(float(n)) * (size - 25));
        text.setOrigin(text.getLocalBounds().width / 2.0, text.getLocalBounds().height / 2.0);
        text.setRotation(n);
        window.draw(text);
    }
    sf::Sprite cutOff;
    textureManager.setTexture(cutOff, "radarCutoff.png");
    cutOff.setPosition(position);
    cutOff.setScale(size / float(cutOff.getTextureRect().width) * 2.1, size / float(cutOff.getTextureRect().width) * 2.1);
    window.draw(cutOff);
    
    sf::RectangleShape rectH(sf::Vector2f(1600, position.y - size * 1.05));
    rectH.setFillColor(sf::Color::Black);
    window.draw(rectH);
    rectH.setPosition(0, position.y + size * 1.05);
    window.draw(rectH);
    sf::RectangleShape rectV(sf::Vector2f(position.x - size * 1.05, 900));
    rectV.setFillColor(sf::Color::Black);
    window.draw(rectV);
    rectV.setPosition(position.x + size * 1.05, 0);
    window.draw(rectV);
}
