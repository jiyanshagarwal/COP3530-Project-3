#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <Drawable.h>
#include <Button.h>
#include <TextBox.h>
#include <Panel.h>
#include <DropdownMenu.h>
#include <NavigationMenu.h>
#include <TitleBar.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;

void menuClickTest(std::string name) {
	cout << name << " clicked" << endl;
}

float line(float x) {
	return 0.5 * x;
}

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(1080, 720), "Find a Car", sf::Style::None, settings);
	window.setVerticalSyncEnabled(true);

	Panel windowPanel(0, 0, window.getSize().x, window.getSize().y);
	windowPanel.SetFocus(true);
	TitleBar bar(40, window);
	Panel panel(250, 40, window.getSize().x - 250, window.getSize().y - 40);
	Panel sideBar(0, 40, 250, window.getSize().y - 40);

	windowPanel.AddObject(&bar, 1);
	windowPanel.AddObject(&panel, 1);
	windowPanel.AddObject(&sideBar, 1);

	windowPanel.SetColor(sf::Color(70, 70, 70));
	windowPanel.SetBorderWidth(-1);
	bar.SetColor(sf::Color(50, 50, 50));
	panel.SetColor(sf::Color(70, 70, 70));

	sideBar.SetColor(sf::Color(50, 50, 50));
	sideBar.SetBorderColor(sf::Color(100, 100, 100));
	sideBar.SetBorderWidth(-1);
	sideBar.XScroll(true);
	sideBar.YScroll(true);

	sf::Font font;
	cout << "Arial Font Loaded: " << font.loadFromFile("res\\Arial Rounded MT Bold.TTF") << endl;

	sf::Font monospace_font;
	cout << "Courier Font Loaded: " << monospace_font.loadFromFile("res\\Courier New.TTF") << endl;

	DropdownMenu file_menu(0, 5, 70, 30, font);
	file_menu.SetName("File");
	file_menu.SetColor(sf::Color(50, 50, 50));
	file_menu.SetTextColor(sf::Color::White);

	file_menu.AddItem("New");
	file_menu.AddItem("Open");
	file_menu.AddItem("Save");
	file_menu.AddItem("Save As");

	DropdownMenu properties_menu(71, 5, 120, 30, font);
	properties_menu.SetName("Properties");
	properties_menu.SetColor(sf::Color(50, 50, 50));
	properties_menu.SetTextColor(sf::Color::White);

	properties_menu.AddItem("Bla");
	properties_menu.AddItem("Bla");

	//------------------------Set Up for the Side Menu------------------------//
	NavigationMenu side_menu(0, 0, sideBar.GetWidth(), 30, font);
	side_menu.SetName("Name");
	side_menu.Open();
	side_menu.SetColor(sf::Color(50, 50, 50));
	side_menu.SetTextColor(sf::Color::White);

	side_menu.AddItem("Item 1");
	side_menu.AddItem("Item 2");
	side_menu.AddItem("Item 3");

	side_menu.AddItem("Item 1.SubItem 1");
	side_menu.AddItem("Item 1.SubItem 2");
	side_menu.AddItem("Item 1.SubItem 3");

	side_menu.AddItem("Item 2.Potato");
	side_menu.AddItem("Item 2.Fire");
	side_menu.AddItem("Item 2.Hello");

	side_menu.AddItem("Item 3.Hi");

	side_menu.OnClick("Item 1.SubItem 1", menuClickTest);
	side_menu.OnClick("Item 2", menuClickTest);

	sideBar.AddObject(&side_menu, 0);
	windowPanel.AddObject(&file_menu, 0);
	windowPanel.AddObject(&properties_menu, 0);

	sf::Color focused_color = sf::Color(0, 76, 135);
	sf::Color unfocused_color = sf::Color(100, 100, 100);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			windowPanel.Event(event);
		}

		if (window.hasFocus()) {
			windowPanel.SetBorderColor(focused_color);
		}
		else {
			windowPanel.SetBorderColor(unfocused_color);
		}

		window.clear();

		windowPanel.SetMousePosition((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y);
		windowPanel.Tick();
		windowPanel.Draw(window);

		window.display();
	}

	return 0;
}