#include "help.h" 
#include <sstream>
#include <map>
#include <algorithm>
#include <iostream>

commandline parse(std::string codeline) {
	commandline c ; 
	std::stringstream ss(codeline) ;
	std::string item ;
	
	int tokens_found = 0 ;

	while (std::getline(ss, item, ' ')) {
		tokens_found++ ;
		if (tokens_found == 1) {
			c.function = item ;
		} else if (item.rfind("--")==0) {
			c.options.push_back(item) ;
		} else {
			c.args.push_back(addspace(item)) ;
		}
	}
	return c ;
}

void interpret(commandline c, Schedule* s) {
	switch (to_command(c.function)) {
		case QUIT:
			std::cout<<"Bye bye !"<<std::endl ;
			break ;
		
		case HELP:
			if (c.args.size() == 0) {
				help(HELP) ;
			} else {
				help(to_command(c.args[0])) ;
			}
			break ;

		case IMPORT:
			if (check_args(c, 2, 0)) {
				std::string schedule = 	c.args[0] ;
				std::string dotations = c.args[1] ;
			 s->import_html(schedule, dotations) ;
			}
			break ;
		case ADDTEACHER:
			for (std::vector<std::string>::iterator it=c.args.begin() ; 
				it!=c.args.end() ; it++) {
			 s->add_teacher(*it) ;
			}
			break ;
		case LISTTEACHER:
			if(check_args(c, 0, 0)) {
			 s->list_teachers() ;
			}
			break ;
		case LISTGROUP:
			if(check_args(c, 0, 0)) {
			 s->list_groups() ;
			}
			break ;
		case DISPLAY:
			if(check_args(c, 1, 1)) {
				std::string name = addspace(c.args[0]) ;
				int semester = handle_options(c.options) ;
                s->display(name, semester) ;
			}
			break ;
		case ASSIGN:
			{
				std::string teacher = addspace(c.args[0]) ;
                c.args.erase(c.args.begin()) ;
				int semester = handle_options(c.options) ;
                s->assign(teacher, c.args, semester) ;
			}
			break ;
		case UNASSIGN:
			if(check_args(c, 1, 1)) {
				std::string group = c.args[0] ;
				int semester = handle_options(c.options) ;
                s->unassign(group, semester) ;
			}
			break ;
		case UNASSIGNED:
			if(check_args(c, 0, 1)) {
				int semester = handle_options(c.options) ;
                s->unassigned_groups(semester) ;
			}
			break ;
		case COMPATIBLE:
			if(check_args(c, 1, 1)) {
				std::string name = addspace(c.args[0]) ;
				int semester = handle_options(c.options) ;
                s->compatible(name, semester) ;
			}
			break ;
		case COMPATIBLE2:
			if(check_args(c, 1, 1)) {
				std::string name = c.args[0] ;
				int semester = handle_options(c.options) ;
                s->compatible2(name, semester) ;
			}
			break ;
		case COMPATIBLEOMSI:
			if(check_args(c, 1, 1)) {
				std::string name = c.args[0] ;
				int semester = handle_options(c.options) ;
                s->compatible_omsi(name, semester) ;
			}
			break ;

		case DELETETEACHER:
			if(check_args(c, 1, 0)) {
				std::string name = c.args[0] ;
			 s->delete_teacher(name) ;
			}
			break ;
		case INDISP:
			if (check_args(c, 4, 1)) {
				std::string name = addspace(c.args[0]) ;
				int day = get_day(c.args[1]) ;
				int h_begin = get_hour(c.args[2]) ;
				int h_end = get_hour(c.args[3]) ;
				int semester = handle_options(c.options) ;
                s->indisp(name, semester, day, h_begin, h_end) ;
			}
			break ;
		case DISP:
			if (check_args(c, 4, 1)) {
				std::string name = c.args[0] ;
				int day = get_day(c.args[1]) ;
				int h_begin = get_hour(c.args[2]) ;
				int h_end = get_hour(c.args[3]) ;
				int semester = handle_options(c.options) ;
                s->disp(name, semester, day, h_begin, h_end) ;
			}
			break ;

		default:
			std::cout<<"Unrecognized command, try again !"<<std::endl ;
	}
}

bool check_args(commandline c,unsigned int argnum,unsigned int optnum) {
	bool b = (c.args.size()==argnum && c.options.size() <= optnum) ;
	if (!b) {
		std::cout<<"Error : function "<<c.function<<" takes "<<argnum<<" arguments, and at most "<<optnum<<" options (received "<<c.args.size()<<" arguments and "<<c.options.size()<<" options)"<<std::endl ;
	}
	return b ;
}

command to_command(std::string function) {
	std::map<std::string, command> m = {{"quit", QUIT},
					   {"help", HELP},
					   {"import", IMPORT},
					   {"add_teacher", ADDTEACHER},
					   {"indisp", INDISP},
					   {"force_disp", DISP},
					   {"list_teachers", LISTTEACHER},
					   {"list_groups", LISTGROUP},
					   {"display", DISPLAY},
					   {"assign", ASSIGN},
					   {"unassign", UNASSIGN},
					   {"unassigned_groups", UNASSIGNED},
					   {"compatible", COMPATIBLE},
					   {"compatible2", COMPATIBLE2},
					   {"compatible_OMSI", COMPATIBLEOMSI},
					   {"delete_teacher", DELETETEACHER}};
	try {
		return m.at(function) ;
	}
	catch (...) {
		return DEFAULT ;
	}
}

std::string addspace(std::string name) {
    for(unsigned int i = 0; i < name.length(); i++) {
		if(name[i] == '_') {
			name[i] = ' ';
		}
	}
	return name;
}

int handle_options(std::vector<std::string> options) {
	int semester = 3 ;
	if (options.size() == 1) {
		int i = sscanf(options[0].c_str(), "--semester=%d", &semester) ;
		if (i == 0 || semester>2 || semester<-1) {
			std::cout<<"Wrong semester option ; command will be executed on both semesters"<<std::endl ;
			semester = 3 ;
		}
	}
	return (semester-1) ;
}

int get_hour(std::string hstring) {
	int h=0, min=0 ;
	sscanf(hstring.c_str(), "%dh%d", &h, &min) ;
	min = (int)ceil(min/30) ;
	h -=8 ;
	return 2*h+min ;
}

int get_day(std::string day) {
	std::string frdays[] = {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi"} ;
	std::string endays[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"} ;
	int d_fr = std::distance(frdays, std::find(frdays, frdays+5, day)) ;
	int d_en = std::distance(endays, std::find(endays, endays+5, day)) ;
	return std::min(d_fr, d_en) ;
}
