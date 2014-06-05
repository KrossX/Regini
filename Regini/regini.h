/* Copyright (c) 2014 KrossX <krossx@live.com>
 * License: http://www.opensource.org/licenses/mit-license.html  MIT License
 */

#ifndef REGINI_H
#define REGINI_H

namespace regini
{
	using std::string;
	using std::vector;

	struct entry_type
	{
		string key;
		string value;
	};

	struct section_type
	{
		string name;
		vector<entry_type> entry;
	};

	class regini_file
	{
		string filename;

		section_type* current_section;
		vector<section_type> section;
		
		void add_section(string in);
		void add_entry(string in);

		section_type* get_section(string section_name);
		entry_type*   get_entry(string section_name, string key_name);

		void write_string(string section, string key, string value);

	public:
		int    get_integer(string section, string key, int default_value = 0);
		string get_string(string section, string key, string default_value = "");

		template<class T>
		void write(string section, string key, T value)
		{
			string string_value = std::to_string(value);
			write_string(section, key, string_value);
		}

		template<>
		void write<const char*>(string section, string key, const char* value)
		{
			string string_value(value);
			write_string(section, key, string_value);
		}

		template<>
		void write<string>(string section, string key, string value)
		{
			write_string(section, key, value);
		}

		bool open(string filename);
		void save();
	};
}

#endif