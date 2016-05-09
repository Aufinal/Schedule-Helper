#pragma once
#include <string>
#include <vector>

/* File group.h : Defining TD and Amphi groups
 *
 * Groups are composed from the following fields :
 * 	- group name (string)
 * 	- nature of group (amphi or not, bool)
 * 	- group schedule (bool array of size MAX_HOUR)
 * 	- teacher name (string)
 * 	- total time worked in semester (int)
 * 
 * Member functions are 
 * 	- display() : displays information about the group
 * 	- add_hour : set working hours, used in importation
 */

class Group {
	public:
	std::string name ;
	bool isAmphi ;
	std::vector<int> total_time ;
	std::vector<std::string> teacher ;
	std::vector<bool> schedule ;
	
	Group() ;
	Group(std::string name, bool amph) ;
	
	void display(int semester) const;
	void add_hour(int semester, int begin, int end) ;
	
} ;
bool operator< (const Group& lhs, const Group& rhs) ;

std::string hourmin(int i) ;
