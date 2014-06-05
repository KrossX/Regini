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

	public:
		int    get_integer(string section, string key, int default_value = 0);
		string get_string(string section, string key, string default_value = "");

		void write_int(string section, string key, int value);
		void write_string(string section, string key, string value);

		bool open(string filename);
		void save();
	};
}

#endif