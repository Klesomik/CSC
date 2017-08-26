#ifndef NAMESTYLE_HPP

#define NAMESTYLE_HPP

class NameStyle
{
	public:
		NameStyle ();

		bool is_first (const std::string& name);
		bool is_second (const std::string& name);
		bool is_third (const std::string& name);

		void add_statistics (const std::string& name);

		int first,  // camelCase
			second, // camel_case 
			third;  // CamelCase
};

NameStyle::NameStyle ():
	first (0),
	second (0),
	third (0)
{
}

bool NameStyle::is_first (const std::string& name)
{
}

bool NameStyle::is_second (const std::string& name)
{
}

bool NameStyle::is_third (const std::string& name)
{
}

void NameStyle::add_statistics (const std::string& name)
{
}

#endif /* NAMESTYLE_HPP */
