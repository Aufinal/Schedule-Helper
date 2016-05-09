#include "schedule.h"
#include "command.h"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[]) {
	std::ifstream backup ;
	std::ofstream backup_w ;
	std::string line ;
    Schedule s ;
	if (argc != 2) {
		std::cout<<"Error : wrong number of arguments ; usage is schedule_helper <filename>"<<std::endl ;
		exit(1) ;
	} else {
		system("reset") ;
		std::cout<<"Welcome to Schedule Helper !"<<std::endl ;
		std::cout<<"Type a command, or type 'help' to list available commands"<<std::endl ;	
		backup.open(argv[1]) ;
		while (getline(backup, line)) {
			interpret(parse(line), &s) ;
		}	;
		backup.close() ;
		backup_w.open(argv[1], std::ios::app) ;
	}
	std::string codeline ;
	commandline c ;
	while (c.function != "quit") {
	std::cout<<">> " ;
	std::getline(std::cin, codeline) ;
	c = parse(codeline) ;
	if (to_command(c.function) <= DISP) {
		backup_w<<codeline<<std::endl ;
	}
	interpret(c,&s) ;
	std::cout<<std::endl ;
	}
	backup_w.close() ;
	return 0 ;
}
