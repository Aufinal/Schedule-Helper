#pragma once
#include "teacher.h"
#include <map>
#include <queue>

/* File Schedule.h : Schedule definition
 *
 * Schedule only has two attributes :
 *  - classes : string->Group map 
 *  - teachers : string->Teacher map 
 *
 * Note : other classes don't have access to group and teacher details ; every correspondence is made here (to be enforced ?)
 * 
 * Member functions :
 * 	- import_html(filename) : gets group schedule from calc-created html file, and dotations from csv file 
 * 	- add/delete_teacher : add/remove teacher from the database
 * 	- indisp/disp : adds availability/unavailablilty to teacher ; overrides existing info
 * 	- assign/unassign : assigns/unassigns group to teacher
 * 	- unassigned_groups : displays unassigned groups (duh)
 * 	- compatible(name) : checks if name is group or teacher, then returns list of teachers (or groups) whose schedule is compatible
 * 	- list_teachers/groups : lists teachers/groups in database
 * 	- display(name) : checks if name is group or teacher, then displays details
 *
 *
 * 	Note : basically those functions make the link between user-given commands and Teacher/Group member functions (+ error-checking)
 */

class Schedule {
	public:
		Schedule() ;
		
		void import_html(std::string filename, std::string dotations) ;
		void add_teacher(std::string name) ;
		void delete_teacher(std::string name) ;
		void indisp(std::string name, int semester, int day, int h_begin, int h_end) ;
		void disp(std::string name, int semester, int day, int h_begin, int h_end) ;
		void assign(std::string teacher, std::vector<std::string> group, int semester) ;
		void unassign(std::string group, int semester) ;
		void unassigned_groups(int semester) ;
		void compatible(std::string name, int semester) ;
		void compatible2(std::string name, int semester) ;
		void compatible_omsi(std::string name, int semester) ;
		void list_teachers() ;
		void list_groups();
		void display(std::string name, int semester) ;

		std::map<std::string, Group> classes;
		std::map<std::string, Teacher> teachers ;


	private:
		void compatible_groups(Teacher t, int semester) ;
		void compatible_2groups(Teacher t, int semester) ;
		void compatible_teachers(Group g, int semester) ;
		void compatible_omsi(Teacher t, int semester) ;

        void parse(std::string filename, int datatype) ;
}; 

static void split(std::string line, char delim, std::queue<std::string>& q) ;
