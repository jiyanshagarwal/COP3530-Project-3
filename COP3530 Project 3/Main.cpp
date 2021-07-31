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
#include "DataReader.h"
#include "CardManager.h"
#include "CarData.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

void menuClickTest(std::string name) {
	cout << name << " clicked" << endl;
}

int main() {
	DataReader reader;
	reader.read("res\\small_vehicles_data.csv", 1000);

	std::map<int, std::string> vehiclesByPrice;

	DataReader::FieldIndex price = DataReader::PRICE;
	DataReader::FieldIndex year = DataReader::YEAR;
	DataReader::FieldIndex manufactuerer = DataReader::MANUFACTUERER;
	DataReader::FieldIndex model = DataReader::MODEL;
	DataReader::FieldIndex odometer = DataReader::ODOMETER;

	for (auto& it : reader.vehicles) {
		int thisPrice = stoi(it[price]);
		std::string info;
		info.append(it[year]);
		info.append(" ");
		info.append(it[manufactuerer]);
		info.append(" ");
		info.append(it[model]);
		info.append(" Mileage: ");
		info.append(it[odometer]);
		vehiclesByPrice.insert(std::pair<int, std::string>(thisPrice, info));
	}

#pragma region <Main Window Setup>
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
#pragma endregion

#pragma region <Load Fonts>
	sf::Font arial_font;
	cout << "Arial Font Loaded: " << arial_font.loadFromFile("res\\arial.TTF") << endl;

	sf::Font arial_rounded_MT_bold;
	cout << "Arial Rounded MT Bold Loaded: " << arial_rounded_MT_bold.loadFromFile("res\\Arial Rounded MT Bold.TTF") << endl;

	sf::Font courier_font;
	cout << "Courier Font Loaded: " << courier_font.loadFromFile("res\\Courier New.TTF") << endl;
#pragma endregion

#pragma region <Setup for the Side Menu>
	NavigationMenu side_menu(0, 0, sideBar.GetWidth(), 30, arial_rounded_MT_bold);
	side_menu.SetName("Filter");
	side_menu.Open();
	side_menu.SetColor(sf::Color(50, 50, 50));
	side_menu.SetTextColor(sf::Color::White);

	side_menu.AddItem("Brand");
	side_menu.AddItem("Brand.Lexus");
	side_menu.AddItem("Brand.Toyota");
	side_menu.AddItem("Brand.BMW");

	side_menu.AddItem("Price");
	side_menu.AddItem("Price.1,000-9,999");
	side_menu.AddItem("Price.10,000-19,999");
	side_menu.AddItem("Price.20,000-29,999");

	side_menu.AddItem("Year");
	for (int i = 2000; i <= 2021; i++) {
		side_menu.AddItem("Year." + std::to_string(i));
	}

	side_menu.AddItem("Cylinders");
	side_menu.AddItem("Cylinders.2");
	side_menu.AddItem("Cylinders.4");
	side_menu.AddItem("Cylinders.6");
	side_menu.AddItem("Cylinders.8");

	side_menu.OnClick("Brand.Lexus", menuClickTest);
	side_menu.OnClick("Brand.Toyota", menuClickTest);
#pragma endregion

	TextBox searchBox(50, 50, 600, 30, window);
	searchBox.SetFont(courier_font);
	searchBox.SetCharacterLimit(40);
	searchBox.SetColor(sf::Color(50, 50, 50));
	searchBox.SetTextColor(sf::Color::White);
	searchBox.SetBorderColor(sf::Color::Black);
	searchBox.SetHighlightColor(sf::Color(100, 150, 180, 150));

	Button searchButton(680, 50, 100, 30);
	searchButton.SetText("Search");
	searchButton.SetFont(arial_rounded_MT_bold);
	searchButton.SetColor(sf::Color(252, 127, 0));
	searchButton.SetTextColor(sf::Color::White);
	searchButton.SetBorderColor(sf::Color::Black);

	CardManager searchCards(50, 150, 730, 530, 200, arial_font, &window);
	searchCards.SetColor(sf::Color(70, 70, 70));
	searchCards.YScroll(true);
	
	CarData data;
	data.page_url = "https://auburn.craigslist.org";
	data.car_name = "Ford F-150";
	data.car_VIN = "AJJS23298392AAKIQ";
	data.car_price = "2402.67";
	data.car_description = "192k miles brand new TSLs 4x4 jeep xj. 4x4 works amazing. Hot heat, ac needs recharged. Text for more info. TEXT ONLY. Cash only!";

	searchCards.AddCard(data);
	searchCards.AddCard(data);
	searchCards.AddCard(data);

	panel.AddObject(&searchBox, 0);
	panel.AddObject(&searchButton, 0);
	panel.AddObject(&searchCards, 0);
	sideBar.AddObject(&side_menu, 0);

	sf::Color focused_color = sf::Color(0, 76, 135);
	sf::Color unfocused_color = sf::Color(100, 100, 100);

#pragma region <Event Loop>
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
#pragma endregion

	return 0;
}