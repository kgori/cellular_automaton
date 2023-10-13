//
// Created by Kevin Gori on 02/03/2021.
//

#pragma once
#include <SFML/Graphics.hpp>
#include "grid.h"

class SFMLRenderer {
public:
    SFMLRenderer(int screen_width, int screen_height, int grid_width, int grid_height)
        : m_Window(sf::VideoMode(screen_width, screen_height), "SFML window"),
          m_GridWidth(grid_width),
          m_GridHeight(grid_height) {
        m_Window.setVerticalSyncEnabled(true);
//        m_Window.setFramerateLimit(30);
        float cell_width = static_cast<float>(screen_width) / static_cast<float>(grid_width);
        float cell_height = static_cast<float>(screen_height) / static_cast<float>(grid_height);
        m_Cell = sf::RectangleShape(sf::Vector2(cell_width, cell_height));
        m_Cell.setFillColor(sf::Color::White);
        m_Cell.setOutlineColor(sf::Color::Black);
        m_Cell.setOutlineThickness(0);

        update();
    }

    void update() {
        sf::Event event;
        while (m_Window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                m_Window.close();
        }

        m_Window.clear();
        draw();
        m_Window.display();
    }

    void update(const Grid& grid) {
        sf::Event event;
        while (m_Window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                m_Window.close();
        }

        m_Window.clear();
        draw(grid);
        m_Window.display();
    }

    void draw() {
        float cell_width = m_Cell.getSize().x;
        float cell_height = m_Cell.getSize().y;
        for (int x = 0; x < m_GridWidth; ++x) {
            for (int y = 0; y < m_GridHeight; ++y) {
                m_Cell.setPosition(x * cell_width, y * cell_height);
                m_Window.draw(m_Cell);
            }
        }
    }

    void draw(const Grid& grid) {
        float cell_width = m_Cell.getSize().x;
        float cell_height = m_Cell.getSize().y;
        for (int y = 0; y < m_GridWidth; ++y) {
            for (int x = 0; x < m_GridHeight; ++x) {
                m_Cell.setPosition(y * cell_width, x * cell_height);
                int cell_value = grid.get_value(x, y);
                m_Cell.setFillColor(cell_value == 0 ? sf::Color::White : sf::Color::Blue);
                m_Window.draw(m_Cell);
            }
        }
    }

    bool isWindowOpen() {
        return m_Window.isOpen();
    }

private:
    sf::RenderWindow m_Window;
    sf::RectangleShape m_Cell;
    int m_GridWidth, m_GridHeight;
};
