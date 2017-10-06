#ifndef NAMESTYLE_HPP

#define NAMESTYLE_HPP

class NameStyle
{
	public:
		NameStyle ();

		void init_data ();

		bool check_style (const std::string& name, const std::string& style);

		void add_statistics (const std::string& name);

		bool is_valid_name (const std::string& name);

		std::vector <std::pair <std::string, int>> data;

		int no_style;
};

NameStyle::NameStyle ():
	data (),
	no_style (0)		 
{
	init_data ();
}

void NameStyle::init_data ()
{
	data.push_back ({ "[a-z]+([A-Z]+[a-z]*)+",           0 }); // camelCase
	data.push_back ({ "[a-z]+(_[a-z]+)*",                0 }); // camel_case
	data.push_back ({ "([A-Z]+[a-z]*)+",                 0 }); // CamelCase
	data.push_back ({ "([A-Z]+[a-z]*)+(_[A-Z]+[a-z]*)+", 0 }); // Camel_Case
}

bool NameStyle::check_style (const std::string& name, const std::string& style)
{
	std::cmatch result;

	return std::regex_match (name.c_str (), result, std::regex (style));
}

void NameStyle::add_statistics (const std::string& name)
{
	bool flag = false;

	for (auto& it : data)
	{
		if (check_style (name, it.first))
		{
			it.second++;

			flag = true;
		}
	}

	if (!flag)
		no_style++;
}

bool NameStyle::is_valid_name (const std::string& name)
{
	std::pair <std::string, int> result (data[0]);

	for (int i = 1; i < (int) data.size (); i++)
		if (data[i].second > result.second)
			result = data[i];

	return check_style (name, result.first);
}

#endif /* NAMESTYLE_HPP */
