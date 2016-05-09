#pragma once
#include <string>
#include <vector>
#include "schedule.h"

/* File command.h : user command interpretation
 *
 * Functions (in order of execution) :
 * 	- parse : gets string input and returns command, args and potential options
 * 	- to_command : translates command into enum keyword (to be able to switch)
 * 	- handle_options : for now, only gets semester ; if needed more options will be added
 * 	- check_args : checks number of arguments and options ; if it fails command is rejected
 * 	- interpret : actually executes command ; translates everything into schedule member function
 *
 * Other useful functions :
 * 	- addspace : adds space to group & teacher names (commandline can't handle spaces in names)
 * 	- get_hour : goes from the format %dh%d to a number from 0 to HALFHOURS_A_DAY
 * 	- get_day : goes from string (french or english) to day number
 *
 */

struct commandline {
	std::string function ;
	std::vector<std::string> options ;
	std::vector<std::string> args ;
} ;
typedef struct commandline commandline ;

enum command {IMPORT, ADDTEACHER, ASSIGN, UNASSIGN, DELETETEACHER, INDISP, DISP,
	 UNASSIGNED, COMPATIBLE, COMPATIBLE2, COMPATIBLEOMSI,
 	LISTTEACHER, LISTGROUP, DISPLAY, QUIT, HELP, DEFAULT} ;

commandline parse(std::string codeline) ;
void interpret(commandline c, Schedule* s) ;

bool check_args(commandline c, unsigned int argnum, unsigned int optnum) ;
command to_command(std::string function) ;
std::string addspace(std::string name) ;
int handle_options(std::vector<std::string> options) ;
int get_hour(std::string hstring) ;
int get_day(std::string day) ;
