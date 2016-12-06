//

#pragma once

#include <unordered_map>

class SkillData;

class SkillDataProvider
{
public:

	// default constructor
	SkillDataProvider() = default;

	// copy constructor
	SkillDataProvider(const SkillDataProvider &other) = delete;

	// move constructor
	SkillDataProvider(SkillDataProvider &&other) = delete;

	// destructor
	~SkillDataProvider() = default;

	// copy assignment operator
	SkillDataProvider &operator=(const SkillDataProvider &other) = delete;

	// move assignment operator
	SkillDataProvider &operator=(SkillDataProvider &&other) = delete;

	static SkillDataProvider *get_instance();
	void load_data();
	SkillData *get_skill_data(int skill_id);
	std::unordered_map<int, SkillData *> *get_data();

private:

	static SkillDataProvider *singleton_;
	std::unordered_map<int, SkillData *> skills_;
};
