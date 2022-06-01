#include <iostream>
#include <fstream>
#include <vector>

class Variables{
	public:
		std::string type;
		std::string var;
		Variables(std::string type, std::string var){
			this->type = type;
			this->var = var;
		}
};

void clean_up_line(std::string &str)
{
	for (int j = 0; j < str.length(); j++)
	{
		if (str[j] == '"')
			while (str[++j] != '"');
		while(str[j] == ' ' || str[j] == '\t')
			str.erase(str.begin() + j);	
	}
}

int handle_file_open(std::string filename, std::ifstream &inputFile, std::ofstream &outputFile)
{
	inputFile.open(filename);
	if (filename.find(".c") == std::string::npos || !inputFile.is_open())
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
	std::vector < Variables > vars;
	std::vector < std::string > other_lines;

	if (handle_file_open(filename, inputFile, outputFile))
		return 1;
	for (std::string line; std::getline(inputFile, line);)
	{
		if (line.find("main()") != std::string::npos)
			continue;
		else if (line.find("int ") != std::string::npos)
			vars.push_back(Variables("int", line.substr(line.find("int") + 4, line.find(";") - line.find("int") - 4)));
		else if (line.find("char ") != std::string::npos)
			vars.push_back(Variables("char", line.substr(line.find("char") + 4, line.find(";") - line.find("char") - 4)));
		else if (line.find("float ") != std::string::npos)
			vars.push_back(Variables("float", line.substr(line.find("float") + 4, line.find(";") - line.find("float") - 4)));
		else if (line.find("double ") != std::string::npos)
			vars.push_back(Variables("double", line.substr(line.find("double") + 4, line.find(";") - line.find("double") - 4)));
		else if (line.find("include") != std::string::npos)
			includes.push_back(line);
		else
			other_lines.push_back(line);
	}
	//  includes arent necessary for linux, they will generate warnings tho
	for (int i = 0; i < includes.size(); i++)
		outputFile << includes[i] << std::endl;
	for (int i = 0; i < vars.size(); i++)
	{
		clean_up_line(vars[i].var);
		if (vars[i].type == "int" || vars[i].type == "char")
			outputFile << vars[i].var << ";";
		else
			outputFile << vars[i].type << " " << vars[i].var << ";";
	}
	outputFile << "main()";
	for (int i = 0; i < other_lines.size(); i++)
	{
		clean_up_line(other_lines[i]);
		outputFile << other_lines[i];
	}
	return 0;
}