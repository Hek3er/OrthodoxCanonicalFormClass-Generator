#include <iostream>
#include <fstream>
#include <unistd.h>
#include <dirent.h>
#include <vector>

void	ReplaceTemplate(std::string ClassName, std::string extention) {
	std::string StringToReplace = "$REP";
	std::string NewString = ClassName;
	std::string FileName = "templates/sample" + extention.substr(1);
	std::string OutputFileName = ClassName + extention;
	std::string str;
	int			pos = 0;

	std::ifstream file(FileName);

	if (!file) {
		std::cerr << FileName << ": Unable To open file" << std::endl;
		return ;
	}

	// std::cout << "file.peek() = " << file.peek() << std::endl;
	if (file.peek() == std::ifstream::traits_type::eof())
	{
		std::cerr << FileName <<  ": File is empty" << std::endl;
		return ;
	}

	std::ofstream output(OutputFileName);

	while (!file.eof()) {
		std::getline(file, str);
			while (1) {
				pos = str.find(StringToReplace);
				if (pos != std::string::npos) {
					str.erase(pos, StringToReplace.length());
					str.insert(pos, NewString);
				}
				else {
					break;
				}
			}
		output << str << std::endl;
	}

	file.close();
	output.close();
}

int main(int ac, char **av) {

	if (ac < 2) {
		std::cerr << "\033[31mError:\n	Usage: " << av[0] << " [Class_Names]...\033[0m" << std::endl;
		return (1);
	}

	bool IsThereMakefile = false;
	bool IsThereMain = false;
	bool UseTemplate = false;

	DIR* dir;
	struct dirent *ft;
	if ((dir = opendir(getcwd(NULL, 0)))!= NULL ) {
		while ((ft = readdir(dir)) != NULL) {
			if (std::string(ft->d_name) == std::string("main.cpp")) {
				IsThereMain = true;
			}
			if (std::string(ft->d_name) == std::string("Makefile")) {
				IsThereMakefile = true;
			}
		}
	}

	int index = 1;
	if (std::string(av[1]) == std::string("-t")) {
		if (ac < 3) {
			std::cerr << "ClassName is not provided!" << std::endl;
			exit(1);
		}
		index++;
		UseTemplate = true;
	}

	std::vector<std::string> ClassName;
	std::vector<std::string> ClassCpp;
	std::vector<std::string> ClassHpp;

	for (int i = index; i < ac; i++) {
		std::string name(av[i]);

		ClassName.push_back(name);
		ClassCpp.push_back((name + ".cpp"));
		ClassHpp.push_back((name + ".hpp"));

		std::cout << "\033[33mGenerating [" << ClassCpp[i - index] << "] at \"" << getcwd(NULL, 0) << ".cpp\" \033[0m" << std::endl;
		std::cout << "\033[33mGenerating [" << ClassHpp[i - index] << "] at \"" << getcwd(NULL, 0) << ".hpp\" \033[0m" << std::endl;


		if (UseTemplate == false) {
			std::ofstream	OutputCpp(ClassCpp[i - index]);
			if (OutputCpp.fail()) {
				std::cerr << "\033[31mError:\n Failed To Create " << ClassCpp[i - index] << "\033[0m" << std::endl;
				exit(1);
			}

			OutputCpp << "#include \"" << ClassHpp[i - index] << "\"\n" << std::endl;

			OutputCpp << name << "::" << name + "() {" << std::endl;
			OutputCpp << "	" << "std::cout << \"Default Constructor Called\" << std::endl;" << std::endl;
			OutputCpp << std::endl;
			OutputCpp << "}\n" << std::endl;

			OutputCpp << name + "::" << name + "(" << " " + name + "&" << " obj ) {" << std::endl;
			OutputCpp << "	" << "std::cout << \"Copy Constructor Called\" << std::endl;" << std::endl;
			OutputCpp << std::endl;
			OutputCpp << "	*this = obj;" << std::endl;
			OutputCpp << "}\n" << std::endl;

			OutputCpp << name + "& " <<name + "::operator=( const " + name + "& obj) {" << std::endl;
			OutputCpp << std::endl;
			OutputCpp << "	" << "return (*this);" << std::endl;
			OutputCpp << "}\n" << std::endl;

			OutputCpp << name << "::~" << name + "() {" << std::endl;
			OutputCpp << "	" << "std::cout << \"Deconstructor Called\" << std::endl;" << std::endl;
			OutputCpp << "}\n" << std::endl;

			OutputCpp.close();

			std::ofstream	OutputHpp(ClassHpp[i - index]);
			if (OutputHpp.fail()) {
				std::cerr << "\033[31mError:\n Failed To Create " << ClassHpp[i - index] << "\033[0m" << std::endl;
				exit(1);
			}

			OutputHpp << "#pragma once\n" << std::endl;
			OutputHpp << "# include <iostream>\n" << std::endl;
			OutputHpp << "class " + name + " {\n" << std::endl;
			OutputHpp << "public:\n" << std::endl;
			OutputHpp << "	" <<name + "();" << std::endl;
			OutputHpp << "	" <<name + "( " + name + "& " + "obj );" << std::endl;
			OutputHpp << "	" <<name + "& operator=( const " + name + "& obj );" << std::endl;
			OutputHpp << "	" <<"~" + name + "();\n" << std::endl;
			OutputHpp << "private:\n" << std::endl;
			OutputHpp << "};";

			OutputHpp.close();
		} else {
			ReplaceTemplate(name, ".cpp");
			ReplaceTemplate(name, ".hpp");
		}
	}

	std::string Compiler = "c++";
	std::string Name = "main";
	std::string CompileFlags = "-Wall -Werror -Wextra -std=c++98";
	std::string sources = "main.cpp ";
	for (int i = 0; i < ClassCpp.size(); i++) {
		sources += ClassCpp[i] + " ";
	}
	std::string headers = " ";
	for (int i = 0; i < ClassHpp.size(); i++) {
		headers += ClassHpp[i] + " ";
	}

	if (IsThereMakefile == false) {
		std::ofstream	make("Makefile");
		if (make.fail()) {
			std::cerr << "\033[31mError:\n Failed To Create Makefile \033[0m" << std::endl;
			exit(1);
		}

		make << "SRC = " + sources << std::endl;
		make << "CC = " + Compiler << std::endl;
		make << "NAME = " + Name << std::endl;
		make << "CFLAGS = " + CompileFlags << std::endl;
		make << "HEADER = " + headers << std::endl;
		make << "OSRC = $(SRC:.cpp=.o)\n" << std::endl;

		make << "all : $(NAME)\n" << std::endl;
		make << "$(NAME) : $(OSRC)" << std::endl;
		make << "	@echo \"compiling.. \"" << std::endl; 
		make << "	@$(CC) $(CFLAGS) $(OSRC) -o $(NAME)\n" << std::endl;

		make << "%.o : %.cpp $(HEADER)" << std::endl;
		make << "	@$(CC) $(CFLAGS) -c $< -o $@\n" << std::endl;
		make << "clean:" << std::endl;
		make << "	@echo \"Removing Object files..\"" << std::endl;
		make << "	@rm -rf $(OSRC)\n" << std::endl;

		make << "fclean: clean" << std::endl;
		make << "	@echo \"Removing Exec\"" << std::endl;
		make << "	@rm -rf $(NAME)\n" << std::endl;

		make << "re: fclean all\n" << std::endl;

		make << ".PHONY: clean" << std::endl;

		make.close();
		std::cout << "\033b[31;1m Makefile is generated\033[0m" << std::endl;
	} else {
		std::cout << "\033b[31;1m Makefile is not generated because it already exist\033[0m" << std::endl;
	}

	if (IsThereMain == false) {
		std::ofstream main("main.cpp");
		if (main.fail()) {
			std::cerr << "\033[31mError:\n Failed To Create main.cpp \033[0m" << std::endl;
			exit(1);
		}
		
		main << "//#include \" \"\n" << std::endl;
		main << "int main() {" << std::endl;
		main << "	return (0);" << std::endl;
		main << "}" << std::endl;

		main.close();
		std::cout << "\033[32;1m Main.cpp is generated\033[0m" << std::endl;
	} else {
		std::cout << "\033[31;1m main.cpp is not generated because it already exist\033[0m" << std::endl;
	}
}