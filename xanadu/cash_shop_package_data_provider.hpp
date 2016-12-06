//

#pragma once

#include <vector>
#include <unordered_map>

class CashShopPackageDataProvider
{
public:

	// default constructor
	CashShopPackageDataProvider() = default;

	// copy constructor
	CashShopPackageDataProvider(const CashShopPackageDataProvider &other) = delete;

	// move constructor
	CashShopPackageDataProvider(CashShopPackageDataProvider &&other) = delete;

	// destructor
	~CashShopPackageDataProvider() = default;

	// copy assignment operator
	CashShopPackageDataProvider &operator=(const CashShopPackageDataProvider &other) = delete;

	// move assignment operator
	CashShopPackageDataProvider &operator=(CashShopPackageDataProvider &&other) = delete;

	static CashShopPackageDataProvider *get_instance();
	void load_data();
	std::vector<int> get_serial_numbers_in_cash_package(int package_id);

private:

	static CashShopPackageDataProvider *singleton_;
	std::unordered_map<int, std::vector<int>> packages_;
};
