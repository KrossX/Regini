/* Copyright (c) 2014 KrossX <krossx@live.com>
 * License: http://www.opensource.org/licenses/mit-license.html  MIT License
 */

#include <string>
#include <vector>

#include "regini.h"

int main()
{
	regini::regini_file inifile;
	
	// To test the thing
	inifile.open("test.ini");

	inifile.write("Section", "integer", -50);
	inifile.write("section", "string", "test");
	inifile.write("secTion", "double", 0.55);

	inifile.save();


	return 0;
}