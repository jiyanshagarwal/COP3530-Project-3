#pragma once
#include <string>
#include <vector>

class DataReader {
public:
	enum FieldIndex { ID, URL, REGION, REGION_URL, PRICE, YEAR, MANUFACTUERER, MODEL, CONDITION, CYLINDERS, FUEL_TYPE, ODOMETER, TITLE_STATUS,
	TRANSMISSION, VIN, DRIVE, SIZE, TYPE, PAINT_COLOR, IMAGE_URL, DESCRIPTION, COUNTY, STATE, LATITUDE, LONGITUDE, POSTING_DATE};
	
	std::vector<std::vector<std::string>> vehicles;

	bool read(std::string filename, unsigned int num_lines);

private:
};