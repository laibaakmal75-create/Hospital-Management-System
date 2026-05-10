#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

// Color Palette
namespace Colors {
    const sf::Color BG = sf::Color(246, 244, 232);   // F6F4E8 - cream background
    const sf::Color SIDEBAR = sf::Color(229, 238, 228);   // E5EEE4 - soft green sidebar
    const sf::Color ACCENT = sf::Color(220, 155, 155);   // DC9B9B - dusty rose accent
    const sf::Color MINT = sf::Color(192, 225, 210);   // C0E1D2 - mint green
    const sf::Color TEXT_DARK = sf::Color(60, 50, 50);      // dark text
    const sf::Color TEXT_MID = sf::Color(120, 100, 100);   // medium text
    const sf::Color TEXT_LITE = sf::Color(160, 145, 145);   // light text
    const sf::Color WHITE = sf::Color(255, 255, 255);
    const sf::Color SUCCESS = sf::Color(100, 180, 140);
    const sf::Color ERROR_COL = sf::Color(210, 90, 90);
    const sf::Color CARD_BG = sf::Color(255, 253, 245);
    const sf::Color BORDER = sf::Color(210, 200, 190);
}

// Button 
struct Button {
    sf::RectangleShape shape;
    sf::Text label;
    sf::Color normalColor;
    sf::Color hoverColor;
    bool hovered = false;
    std::function<void()> onClick;

    void draw(sf::RenderWindow& win) {
        shape.setFillColor(hovered ? hoverColor : normalColor);
        win.draw(shape);
        win.draw(label);
    }

    bool handleEvent(sf::Event& e, sf::RenderWindow& win) {
        sf::Vector2i mouse = sf::Mouse::getPosition(win);
        hovered = shape.getGlobalBounds().contains((float)mouse.x, (float)mouse.y);
        if (e.type == sf::Event::MouseButtonReleased &&
            e.mouseButton.button == sf::Mouse::Left && hovered) {
            if (onClick) onClick();
            return true;
        }
        return false;
    }
};

// TextInput 
struct TextInput {
    sf::RectangleShape box;
    sf::Text displayText;
    sf::Text placeholder;
    std::string value;
    bool focused = false;
    bool isPassword = false;
    int maxLen = 50;

    void draw(sf::RenderWindow& win) {
        box.setOutlineColor(focused ? Colors::ACCENT : Colors::BORDER);
        box.setOutlineThickness(focused ? 2.f : 1.f);
        win.draw(box);
        if (value.empty() && !focused) {
            win.draw(placeholder);
        }
        else {
            std::string shown = isPassword ? std::string(value.size(), '*') : value;
            displayText.setString(shown);
            win.draw(displayText);
        }
    }

    bool handleEvent(sf::Event& e, sf::RenderWindow& win) {
        sf::Vector2i mouse = sf::Mouse::getPosition(win);
        if (e.type == sf::Event::MouseButtonPressed)
            focused = box.getGlobalBounds().contains((float)mouse.x, (float)mouse.y);

        if (focused && e.type == sf::Event::TextEntered) {
            if (e.text.unicode == 8 && !value.empty())
                value.pop_back();
            else if (e.text.unicode >= 32 && (int)value.size() < maxLen)
                value += (char)e.text.unicode;
            return true;
        }
        return false;
    }

    void clear() { value = ""; focused = false; }
};

// Panel (rounded rect) 
struct Panel {
    sf::RectangleShape shape;
    void draw(sf::RenderWindow& win) { win.draw(shape); }
};

// Toast notification 
struct Toast {
    sf::RectangleShape box;
    sf::Text msg;
    sf::Clock timer;
    bool visible = false;
    bool isError = false;
    float duration = 3.0f;

    void show(const std::string& message, bool error = false) {
        msg.setString(message);
        isError = error;
        box.setFillColor(error ? Colors::ERROR_COL : Colors::SUCCESS);
        visible = true;
        timer.restart();
    }

    void draw(sf::RenderWindow& win) {
        if (!visible) return;
        if (timer.getElapsedTime().asSeconds() > duration) {
            visible = false;
            return;
        }
        win.draw(box);
        win.draw(msg);
    }
};

// Helper: make a button
inline Button makeButton(float x, float y, float w, float h,
    const std::string& text, sf::Font& font,
    sf::Color bg, sf::Color hover,
    sf::Color textColor = Colors::WHITE, int fontSize = 14)
{
    Button b;
    b.shape.setPosition(x, y);
    b.shape.setSize({ w, h });
    b.shape.setFillColor(bg);
    b.normalColor = bg;
    b.hoverColor = hover;

    b.label.setFont(font);
    b.label.setString(text);
    b.label.setCharacterSize(fontSize);
    b.label.setFillColor(textColor);
    sf::FloatRect tb = b.label.getLocalBounds();
    b.label.setPosition(x + (w - tb.width) / 2.f - tb.left,
        y + (h - tb.height) / 2.f - tb.top);
    return b;
}

// Helper: make a text input 
inline TextInput makeInput(float x, float y, float w, float h,
    const std::string& ph, sf::Font& font,
    bool password = false, int maxLen = 50)
{
    TextInput t;
    t.box.setPosition(x, y);
    t.box.setSize({ w, h });
    t.box.setFillColor(Colors::WHITE);
    t.box.setOutlineThickness(1.f);
    t.box.setOutlineColor(Colors::BORDER);
    t.isPassword = password;
    t.maxLen = maxLen;

    t.placeholder.setFont(font);
    t.placeholder.setString(ph);
    t.placeholder.setCharacterSize(13);
    t.placeholder.setFillColor(Colors::TEXT_LITE);
    t.placeholder.setPosition(x + 12, y + (h - 16) / 2.f);

    t.displayText.setFont(font);
    t.displayText.setCharacterSize(13);
    t.displayText.setFillColor(Colors::TEXT_DARK);
    t.displayText.setPosition(x + 12, y + (h - 16) / 2.f);

    return t;
}

// Helper: make a text label 
inline sf::Text makeText(const std::string& str, sf::Font& font,
    int size, sf::Color color, float x, float y)
{
    sf::Text t;
    t.setFont(font);
    t.setString(str);
    t.setCharacterSize(size);
    t.setFillColor(color);
    t.setPosition(x, y);
    return t;
}