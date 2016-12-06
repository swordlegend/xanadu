//

#include "cash_shop_package_data_provider.hpp"
#include "wznode.hpp"
#include "wzmain.hpp"
#include "world.hpp"

// singleton

CashShopPackageDataProvider *CashShopPackageDataProvider::singleton_ = nullptr;

CashShopPackageDataProvider *CashShopPackageDataProvider::get_instance()
{
	if (!singleton_)
	{
		singleton_ = new CashShopPackageDataProvider();
	}

	return singleton_;
}

void CashShopPackageDataProvider::load_data()
{
	// this shows how to use the loader

	// WZ::Node nn = wz_reader->Base[wzfilename][.img name][name of value][(sometimes only) sub names of value][value (int/string or so return)]

	// Etc.wz contains cash package data
	// in CashPackage.img
	// name of value is the package id
	// sub value is "SN"
	// return values are cash item serial numbers, value name starts with 0 and increasing

	WZMain *wz_reader = World::get_instance()->wz_reader_;

	WZNode node1 = wz_reader->base_["Etc"]["CashPackage"];

	std::vector<int> temp_package_ids;
	std::unordered_map<int, int> package_ids;

	for (auto it : node1)
	{
		std::string valuename = it.name();

		int package_id = std::stoi(valuename);

		WZNode node2 = wz_reader->base_["Etc"]["CashPackage"][valuename.c_str()]["SN"];

		std::vector<int> serial_numbers;

		for (auto it2 : node2)
		{
			int serial_number = it2.get_int_value();
			serial_numbers.push_back(serial_number);
		}

		packages_[package_id] = serial_numbers;
	}
}

std::vector<int> CashShopPackageDataProvider::get_serial_numbers_in_cash_package(int package_id)
{
	if (packages_.find(package_id) == packages_.end())
	{
		return std::vector<int>();
	}

	return packages_[package_id];
}
