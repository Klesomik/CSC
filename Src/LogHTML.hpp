#ifndef LOGHTML_HPP

#define LOGHTML_HPP

#include <bits/stdc++.h>

class LogHTML
{
	public:
		LogHTML ();
		LogHTML (const std::string& name);
		~LogHTML ();

		void open (const std::string& name);
		void close ();

		void begin (const std::string& tag);
		void end (const std::string& tag);

		void set (const std::string& cmd, int value);
		void set (const std::string& cmd, const std::string& value);

		void set_color (const std::string& color);
		void set_font_color (const std::string& color);
		void set_size (const int size);
		void set_style (const std::string& style);

		void output (const char* message, ...);

	//private:
		FILE* data;
};

LogHTML::LogHTML ():
	data (nullptr)
{
}

LogHTML::LogHTML (const std::string& name):
	data ()
{
	open (name);
}

LogHTML::~LogHTML ()
{
	close ();
}

void LogHTML::open (const std::string& name)
{
	if (!data)
		data = fopen (name.c_str (), "w");
}

void LogHTML::close ()
{
	if (data)
	{
		fclose (data);

		data = nullptr;
	}
}

void LogHTML::begin (const std::string& tag)
{
	fprintf (data, "<%s>\n", tag.c_str ());
}

void LogHTML::end (const std::string& tag)
{
	fprintf (data, "</%s>\n", tag.c_str ());
}

void LogHTML::set (const std::string& cmd, int value)
{
	fprintf (data, "<%s = %d>\n", cmd.c_str (), value);
}

void LogHTML::set (const std::string& cmd, const std::string& value)
{
	fprintf (data, "<%s = %s>\n", cmd.c_str (), value.c_str ());
}

void LogHTML::set_color (const std::string& color)
{
    set ("font color", color);
}

void LogHTML::set_font_color (const std::string& color)
{
    set ("body bgcolor", color);
}

void LogHTML::set_size (const int size)
{
    set ("font size", size);
}

void LogHTML::set_style (const std::string& style)
{
    set ("font face", style);
}

void LogHTML::output (const char* message, ...)
{
    va_list args;

    va_start (args, message);

    vfprintf (data, message, args);

    va_end (args);
}

#endif /* LOGHTML_HPP */