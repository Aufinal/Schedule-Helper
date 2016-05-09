#include "help.h"
#include <iostream> 

void help(command c) {
	switch (c) {
		case HELP:
			std::cout<<"List of available commands :"<<std::endl ;
			std::cout<<"quit"<<std::endl;
			std::cout<<"import"<<std::endl;
			std::cout<<"add_teacher"<<std::endl;
			std::cout<<"delete_teacher"<<std::endl;
			std::cout<<"list_teachers"<<std::endl;
			std::cout<<"list_groups"<<std::endl;
			std::cout<<"display"<<std::endl;
			std::cout<<"assign"<<std::endl;
			std::cout<<"unassign"<<std::endl;
			std::cout<<"unassigned_groups"<<std::endl;
			std::cout<<"compatible"<<std::endl;
			std::cout<<"compatible2"<<std::endl;
			std::cout<<"compatible_OMSI"<<std::endl;
			std::cout<<"indisp"<<std::endl ;
			std::cout<<"force_disp"<<std::endl ;
			break ;
		case QUIT:
			std::cout<<"Usage : quit"<<std::endl ;
			std::cout<<"Closes the program"<<std::endl ;
			break ;
		case IMPORT:
			std::cout<<"Usage : import <filename>"<<std::endl ;
			std::cout<<"Extracts schedule data from html file"<<std::endl ;
			break ;
		case ADDTEACHER:
			std::cout<<"Usage : add_teacher <teacher_1> ... <teacher_n>"<<std::endl ;
			std::cout<<"Adds specified teachers into the database"<<std::endl ;
			break ;
		case DELETETEACHER:
			std::cout<<"Usage : delete_teacher <teacher>"<<std::endl ;
			std::cout<<"Removes teacher from the database and unassigns every group he had"<<std::endl ;
			break ;
		case LISTTEACHER:
			std::cout<<"Usage : list_teachers"<<std::endl ;
			std::cout<<"Lists all teachers currently in the database"<<std::endl ; 
			break ;
		case LISTGROUP:
			std::cout<<"Usage : list_groups"<<std::endl ;
			std::cout<<"Lists all groups currently in the database"<<std::endl ;
			break ;
		case DISPLAY:
			std::cout<<"Usage : display [--semester=i] <teacher> or display [--semester=i] <group>"<<std::endl ;
			std::cout<<"Displays details and schedule of specified teacher/group ; if option semester is present, displays only specified semester info"<<std::endl ;
			break ;
		case ASSIGN:
			std::cout<<"Usage : assign [--semester=i] <teacher> <group_1> ... <group_n>"<<std::endl ;
			std::cout<<"Assigns all groups to the specified teacher and updates his schedule accordingly ; exception is raised if schedule problems arise."<<std::endl ;
			std::cout<<"Warning : all groups prior to the incompatible one will still be assigned !"<<std::endl ;
			break ;
		case UNASSIGN:
			std::cout<<"Usage : unassign [--semester=i] <group>"<<std::endl ;
			std::cout<<"Unassigns a group from his teacher and updates the teacher's schedule accordingly"<<std::endl ;
			break ;
		case UNASSIGNED:
			std::cout<<"Usage : unassigned_groups [--semester=i]"<<std::endl ;
			std::cout<<"Displays all unassigned group for specified (or both) semester"<<std::endl ;
			break ;
		case COMPATIBLE:
			std::cout<<"Usage : compatible [--semester=i] <teacher> or compatible [--semester=i] <group>"<<std::endl ;
			std::cout<<"Displays all teachers (resp. groups) whose schedule is compatible with specified group (resp. teacher), for given (or both) semester"<<std::endl ;
			break ;
		case COMPATIBLE2:
			std::cout<<"Usage : compatible2 [--semester=i] <teacher>"<<std::endl ;
			std::cout<<"Displays all group pairs compatible with teacher's schedule"<<std::endl ;
			break ;
		case COMPATIBLEOMSI:
			std::cout<<"Usage : compatible_OMSI [--semester=i] <teacher>"<<std::endl ;
			std::cout<<"Displays all groups whose OMSI and math schedule is compatible with given teacher"<<std::endl ;
		case INDISP:
			std::cout<<"Usage : indisp [--semester=i] <teacher> <day> <hour_begin> <hour_end>"<<std::endl ;
			std::cout<<"Add teacher indisponibility during specified time"<<std::endl ;
			break ;
		case DISP:
			std::cout<<"Usage : force_disp [--semester=i] <teacher> <day> <hour_begin> <hour_end>"<<std::endl ;
			std::cout<<"Forces teacher disponibility during specified time"<<std::endl;
			std::cout<<"Warning ! Overrides existing group information, use with caution !"<<std::endl ;
			break ;
		case DEFAULT:
			std::cout<<"Not implemented yet"<<std::endl;

	}
}
