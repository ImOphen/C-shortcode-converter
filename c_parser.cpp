#include <iostream>
#include <fstream>
#include <vector>

#define SPACE_REPLACEMENT -4

int contains(std::string str, std::string find)
{
	return str.find(find) != std::string::npos;
}

void clean_up_line(std::string &str)
{
	for (int j = 0; j < str.length(); j++)
	{
		if (str[j] == '"')
			while (str[++j] != '"');
		
		while((str[j] == '\t' || str[j] == ' '))
			str.erase(str.begin() + j);	
	}
}

void tokenise_declarations(std::string &str)
{
	for (int j = 0; j < str.length(); j++)
	{
		if (str[j] == '"')
			while (str[++j] != '"');
		else if (str[j] == ' ' && str[j - 1] == 't' && str[j - 2] == 'n' && str[j - 3] == 'i')
			str[j] = SPACE_REPLACEMENT;
		else if (str[j] == ' ' && str[j - 1] == 'c' && str[j - 2] == 'h' && str[j - 3] == 'a' && str[j - 4] == 'r')
			str[j] = SPACE_REPLACEMENT;
		else if (str[j] == ' ' && str[j - 1] == 'f' && str[j - 2] == 'l' && str[j - 3] == 'o' && str[j - 4] == 'a' && str[j - 5] == 't')
			str[j] = SPACE_REPLACEMENT;
		else if (str[j] == ' ' && str[j - 1] == 'd' && str[j - 2] == 'o' && str[j - 3] == 'u' && str[j - 4] == 'b' && str[j - 5] == 'l' && str[j - 6] == 'e')
			str[j] = SPACE_REPLACEMENT;
	}
}

void fix_declarations(std::string &str)
{
	for (int j = 0; j < str.length(); j++)
	{
		if (str[j] == SPACE_REPLACEMENT)
			str[j] = ' ';
	}
}

int handle_file_open(std::string filename, std::ifstream &inputFile, std::ofstream &outputFile)
{
	inputFile.open(filename);
	if (!contains(filename, ".c") || !inputFile.is_open())
	{
		std::cout << "Error with input file" << std::endl;
		return 1;
	}
	outputFile.open(filename.erase(filename.size() - 2, filename.size()) + "_short.c");
	if (!outputFile.is_open())
	{
		std::cout << "Error creating output file" << std::endl;
		return 1;
	}
	if (system(("gcc " + filename + ".c -o ." + filename + "_test_compile 2>/dev/null").c_str()) != 0)
	{
		std::cout << "Error compiling file" << std::endl;
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[])
{

	std::string filename = argv[1];
	std::ifstream inputFile;
	std::ofstream outputFile;
	std::vector < std::string > includes;
	std::vector < std::string > other_lines;

	if (handle_file_open(filename, inputFile, outputFile))
		return 1;
	for (std::string line; std::getline(inputFile, line);)
	{
		if (line.find("main()") != std::string::npos)
			continue;
		else if (contains(line, "include"))
			includes.push_back(line);
		else
			other_lines.push_back(line);
	}
	//  includes arent necessary for linux, they will generate warnings tho
	for (int i = 0; i < includes.size(); i++)
		outputFile << includes[i] << std::endl;
	outputFile << "main()";
	for (int i = 0; i < other_lines.size(); i++)
	{
		tokenise_declarations(other_lines[i]);
		clean_up_line(other_lines[i]);
		fix_declarations(other_lines[i]);
		outputFile << other_lines[i];
	}
	return 0;
}