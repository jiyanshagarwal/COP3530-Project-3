#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <Drawable.h>
#include <Button.h>
#include <TextBox.h>
#include <Panel.h>
#include <chrono>
#include <DropdownMenu.h>
#include <NavigationMenu.h>
#include <TitleBar.h>
#include "DataReader.h"
#include "CardManager.h"
#include "CarData.h"
#include "ImageDownloader.h"
#include "ResourceManager.h"
#include "BinaryHeap.h"
#include "RBTree.h"

using std::cout;
using std::endl;

void menuClickTest(std::string name) {
	cout << name << " clicked" << endl;
}

void search(RBTree<std::string, std::string>& tree, std::vector<std::pair<std::string, std::string>>& data, 
	std::vector<std::string>* VINS_heap, CardManager* manager, TextBox* searchbox, int id) {
	cout << "Search button clicked: " << id << endl;

	tree.clear();
	VINS_heap->clear();

	auto start = std::chrono::high_resolution_clock::now();

	std::vector<std::string> VINS_tree;

	for (int i = 0; i < data.size(); i++) {
		tree.insert(data[i].first, data[i].second);
	}

	tree.inOrder(VINS_tree);

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	cout << "Red-Black tree took " << duration.count() << " milliseconds.\n";

	start = std::chrono::high_resolution_clock::now();

	BinaryHeap<std::string, std::string>::HeapSort(data);

	for (int i = 0; i < data.size(); i++) {
		if (data[i].first.find(searchbox->GetText()) != std::string::npos) {
			VINS_heap->push_back(data[i].second);
		}
	}

	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	cout << "heap took " << duration.count() << " milliseconds.\n";


	manager->UpdateCardsToShow();
}

void searchPrice(RBTree<double, std::string>& tree, std::vector<std::pair<double, std::string>>& data,
	std::vector<std::string>* VINS_heap, CardManager* manager, TextBox* searchbox, int id) {
	cout << "Search button clicked: " << id << endl;

	tree.clear();
	VINS_heap->clear();

	auto start = std::chrono::high_resolution_clock::now();

	std::vector<std::string> VINS_tree;

	for (int i = 0; i < data.size(); i++) {
		tree.insert(data[i].first, data[i].second);
	}

	tree.inOrder(VINS_tree);

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	cout << "Red-Black tree took " << duration.count() << " milliseconds.\n";

	start = std::chrono::high_resolution_clock::now();

	BinaryHeap<double, std::string>::HeapSort(data, false);

	for (int i = 0; i < data.size(); i++) {
		if (data[i].first < std::stoi(searchbox->GetText())) {
			VINS_heap->push_back(data[i].second);
		}
	}

	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	cout << "heap took " << duration.count() << " milliseconds.\n";


	manager->UpdateCardsToShow();
}

int main() {

#pragma region <Load Resources>
	sf::Font arial_font;
	cout << "Arial Font Loaded: " << arial_font.loadFromFile("res\\arial.TTF") << endl;

	sf::Font arial_rounded_MT_bold;
	cout << "Arial Rounded MT Bold Loaded: " << arial_rounded_MT_bold.loadFromFile("res\\Arial Rounded MT Bold.TTF") << endl;

	sf::Font courier_font;
	cout << "Courier Font Loaded: " << courier_font.loadFromFile("res\\Courier New.TTF") << endl;
#pragma endregion

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

	sf::Text title("Find a Car", arial_rounded_MT_bold, 16);
	title.setPosition(50, 10);

	sf::Texture logo_texture;
	logo_texture.loadFromFile("res\\Logo.png");
	sf::RectangleShape logo(sf::Vector2f(30, 30));
	logo.setTexture(&logo_texture);
	logo.setPosition(10, 5);
#pragma endregion

	DataReader reader;
	reader.read("res\\small_vehicles_data.csv", 1400);
	ImageDownloader images("Find a Car", reader.vehicles);

	RBTree<std::string, std::string> name_sorted_tree;
	RBTree<double, std::string> price_sorted_tree;

	std::unordered_map<std::string, CarData> data;

	for (int i = 0; i < reader.vehicles.size(); i++) {
		std::vector<std::string> vehicle = reader.vehicles[i];
		CarData car;
		car.page_url = vehicle[DataReader::URL];
		car.image_url = vehicle[DataReader::IMAGE_URL];
		car.car_name = vehicle[DataReader::MANUFACTUERER] + " " + vehicle[DataReader::MODEL];
		car.car_VIN = vehicle[DataReader::VIN];
		car.car_price = vehicle[DataReader::PRICE];
		car.car_description = vehicle[DataReader::DESCRIPTION];

		data.emplace(car.car_VIN, car);
	}

	std::vector<std::pair<std::string, std::string>> names_VINS;

	for (auto iter = data.begin(); iter != data.end(); iter++) {
		names_VINS.push_back(std::make_pair(iter->second.car_name, iter->first));
	}

	std::vector<std::pair<double, std::string>> prices_VINS;

	for (auto iter = data.begin(); iter != data.end(); iter++) {
		prices_VINS.push_back(std::make_pair(std::stoi(iter->second.car_price), iter->first));
	}

	std::vector<std::string> VINS_heap;
	ResourceManager<ImageDownloader> resources(window, arial_font, images);

	CardManager searchCards(50, 150, 730, 530, 200, VINS_heap, resources);
	searchCards.SetColor(sf::Color(70, 70, 70));
	searchCards.YScroll(true);
	
	for (int i = 1; i < reader.vehicles.size(); i++) {
		std::vector<std::string> vehicle = reader.vehicles[i];
		CarData data;
		data.page_url = vehicle[DataReader::URL];
		data.image_url = vehicle[DataReader::IMAGE_URL];
		data.car_name = vehicle[DataReader::MANUFACTUERER] + " " + vehicle[DataReader::MODEL];
		data.car_VIN = vehicle[DataReader::VIN];
		data.car_price = vehicle[DataReader::PRICE];
		data.car_description = vehicle[DataReader::DESCRIPTION];

		searchCards.AddCard(data);
	}

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

#pragma region <Search Cards>
	TextBox searchBox(50, 50, 600, 30, window);
	searchBox.SetFont(courier_font);
	searchBox.SetCharacterLimit(40);
	searchBox.SetColor(sf::Color(50, 50, 50));
	searchBox.SetTextColor(sf::Color::White);
	searchBox.SetBorderColor(sf::Color::Black);
	searchBox.SetHighlightColor(sf::Color(100, 150, 180, 150));

	Button searchButton(680, 30, 100, 30);
	searchButton.SetText("Search");
	searchButton.SetFont(arial_rounded_MT_bold);
	searchButton.SetColor(sf::Color(252, 127, 0));
	searchButton.SetTextColor(sf::Color::White);
	searchButton.SetBorderColor(sf::Color::Black);
	searchButton.OnClick(std::bind(search, name_sorted_tree, names_VINS, &VINS_heap, &searchCards, &searchBox, std::placeholders::_1));

	Button searchButtonPrice(680, 80, 130, 30);
	searchButtonPrice.SetText("Search Price");
	searchButtonPrice.SetFont(arial_rounded_MT_bold);
	searchButtonPrice.SetColor(sf::Color(252, 127, 0));
	searchButtonPrice.SetTextColor(sf::Color::White);
	searchButtonPrice.SetBorderColor(sf::Color::Black);
	searchButtonPrice.OnClick(std::bind(searchPrice, price_sorted_tree, prices_VINS, &VINS_heap, &searchCards, &searchBox, std::placeholders::_1));

#pragma endregion

	panel.AddObject(&searchBox, 0);
	panel.AddObject(&searchButton, 0);
	panel.AddObject(&searchButtonPrice, 0);
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
		window.draw(title);
		window.draw(logo);

		window.display();
	}
#pragma endregion

	return 0;
}
