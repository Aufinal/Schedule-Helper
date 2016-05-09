#pragma once 
#include "group.h"
#include <set>

/* File teacher.h : defining teacher attributes
 *
 * Teachers are composed of the following fields :
 * 	- teacher name (string)
 * 	- remaining available time (int array of size MAX_HOUR) : 0 means available, 1 means worked hour, 2 means forced indisponibility
 * 	- time worked (separating amphis and TDs, two ints)
 * 	- groups assigned (separating again, two string sets)
 *
 * Note : a teacher doesn't know the details of its assigned group, except their names
 *
 * Member functions :
 * 	- display() : displays information on the teacher
 * 	- addGroup(Group) : assigns group to teacher and updates schedule and time worked
 *  - removeGroup(Group) : reverts preceding method
 *  - isCompatible(Group) : checks compatibility between teacher's and group's schedule
 *  - isCompatible2(Group, Group) : idem, but with two groups at once
 *  - indisp(int, int) : adds indisponibility between specified hours
 *  - disp(int, int) : reverts preceding method
 *
 * Warning : disp overrides any unavailability due to group assignment ; should be used with caution
 */

class Teacher {
	public:
		std::string name ;
		Teacher() ;
		Teacher(std::string name) ;
	
		void display(int semester) const;
		void indisp(int semester, int begin, int end) ;
		void disp(int semester, int begin, int end) ;
		void add_group(Group g, int semester) ;
		void remove_group(Group g, int semester) ;
		bool is_compatible(Group g, int semester) const;
		bool is_compatible2 (Group g, Group h, int semester) const;
		std::vector<int> time_amphi ;
		std::vector<int> time_TD ;

		
	private:
		std::vector<int> availability ;
		std::vector< std::set<std::string> > TDs ;
		std::vector< std::set<std::string> > Amphis ;
} ;
