#ifndef NAMESTYLE_HPP

#define NAMESTYLE_HPP

class NameStyle
{
	public:
		NameStyle ();

		bool check_style (const std::string& name, const std::string& style);

		void add_statistics (const std::string& name);

		std::map <std::string, int> data;

		int no_style;
};

NameStyle::NameStyle ():
	data ({ { "[a-z]+([A-Z]+[a-z]*)*", 0 },    // camelCase
			{ "[a-z]+(_[a-z]+)*",      0 },    // camel_case 
			{ "([A-Z]+[a-z]*)+",       0 } }), // CamelCase
	no_style (0)		 
{
}

bool NameStyle::check_style (const std::string& name, const std::string& style)
{
	std::cmatch result;

	return std::regex_match (name.c_str (), result, std::regex (style));
}

void NameStyle::add_statistics (const std::string& name)
{
	for (auto it = data.begin (); it != data.end (); it++)
	{
		if (check_style (name, it->first))
		{
			it->second++;

			return;
		}
	}

	no_style++;
}

#endif /* NAMESTYLE_HPP */
