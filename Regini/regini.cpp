/* Copyright (c) 2014 KrossX <krossx@live.com>
 * License: http://www.opensource.org/licenses/mit-license.html  MIT License
 */

#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "regini.h"

namespace regini
{
	const string winreg = "Windows Registry Editor Version 5.00";

	static inline string string_tolower(string in)
	{
		string out = in;
		std::transform(out.begin(), out.end(), out.begin(), ::tolower);
		return out;
	}

	void regini_file::add_section(string in)
	{
		in = in.substr(1, in.size() - 2);

		for (size_t i = 0; i < section.size(); i++)
		{
			if (section[i].name.compare(in) == 0)
			{
				current_section = &section[i];
				return;
			}
		}

		section_type newsection;
		newsection.name = in;
		section.push_back(newsection);
		current_section= &section.back();
	}

	void regini_file::add_entry(string in)
	{
		size_t pos = in.find('=');

		entry_type newentry;
		newentry.key = in.substr(1, pos - 2);

		for (entry_type et : current_section->entry)
		{
			if (et.key.compare(newentry.key) == 0)
			{
				et.value = in.substr(pos + 2, in.size() - pos - 3);
				return;
			}
		}

		newentry.value = in.substr(pos + 2, in.size() - pos - 3);
		current_section->entry.push_back(newentry);
	}


	bool regini_file::open(string filename)
	{
		this->filename = filename;
		std::ifstream file(filename);
		vector<string> line;

		section.clear();

		if (file.is_open())
		{
			while (!file.eof())
			{
				string newline;
				std::getline(file, newline);
				line.push_back(newline);
			}

			file.close();

			if (!line.empty())
			{
				current_section = nullptr;

				for (string str : line)
				{
					if (str.empty()) continue;

					size_t size = str.size();

					if (str[0] == '[' && str[size - 1] == ']')
					{
						add_section(str);
					}
					else if (str[0] == '"' && str.find('=') != string::npos && current_section != 0)
					{
						add_entry(str);
					}
				}
			}
		}

		return !section.empty();
	}

	void regini_file::save()
	{
		if (section.empty()) return;

		std::ofstream file(filename, std::ios::out | std::ios::trunc);

		if (file.is_open())
		{
			//file << winreg << "\n\n";

			for (section_type sec : section)
			{
				file << "[" << sec.name << "]\n";

				for (entry_type entry : sec.entry)
				{
					file << "\"" << entry.key << "\"" << "=\"" << entry.value << "\"\n";
				}

				file << "\n";
			}

			file.close();
		}
	}

	section_type* regini_file::get_section(string section_name)
	{
		section_name = string_tolower(section_name);

		for (size_t i = 0; i < section.size(); i++)
		{
			string sname = string_tolower(section[i].name);

			if (sname.compare(section_name) == 0)
				return &section[i];
		}

		return nullptr;
	}

	entry_type* regini_file::get_entry(string section_name, string key_name)
	{
		section_type* sec = get_section(section_name);

		if (sec != nullptr)
		{
			key_name = string_tolower(key_name);

			for (size_t i = 0; i < sec->entry.size(); i++)
			{
				string ekeyname = string_tolower(sec->entry[i].key);

				if (ekeyname.compare(key_name) == 0)
					return &sec->entry[i];
			}
		}

		return nullptr;
	}

	int regini_file::get_integer(string section, string key, int default_value)
	{
		entry_type *entry = get_entry(section, key);

		if (entry != nullptr)
		{
			default_value = std::stoi(entry->value, nullptr, 10);
		}

		return default_value;
	}

	string regini_file::get_string(string section, string key, string default_value)
	{
		entry_type *entry = get_entry(section, key);

		if (entry != nullptr)
		{
			default_value = entry->value;
		}

		return default_value;
	}

	void regini_file::write_int(string section, string key, int value)
	{
		section_type *sec = get_section(section);

		entry_type newentry;
		newentry.key = key;
		newentry.value = std::to_string(value);

		if (sec == nullptr)
		{
			section_type newsection;
			newsection.name = section;
			newsection.entry.push_back(newentry);
			this->section.push_back(newsection);
		}
		else
		{
			entry_type *entry = get_entry(section, key);

			if (entry == nullptr)
			{
				sec->entry.push_back(newentry);
			}
			else
			{
				entry->value = newentry.value;
			}
		}
	}

	void regini_file::write_string(string section, string key, string value)
	{
		section_type *sec = get_section(section);

		entry_type newentry;
		newentry.key = key;
		newentry.value = value;

		if (sec == nullptr)
		{
			section_type newsection;
			newsection.name = section;
			newsection.entry.push_back(newentry);
			this->section.push_back(newsection);
		}
		else
		{
			entry_type *entry = get_entry(section, key);

			if (entry == nullptr)
			{
				sec->entry.push_back(newentry);
			}
			else
			{
				entry->value = newentry.value;
			}
		}
	}

} // end regini namespace



