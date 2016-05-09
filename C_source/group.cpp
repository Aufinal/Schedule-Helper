#include "config.h"
#include "group.h"
#include <iostream>
#include <sstream>
#include <cmath> 
#include <cstdlib>


Group::Group() {
	name = std::string() ;
	isAmphi = false ;
    total_time = std::vector<int>(2,0) ;
	teacher = std::vector<std::string>(2, std::string()) ;
	schedule = std::vector<bool>(2*MAX_HOUR, false) ;
}

Group::Group(std::string name_c, bool amph) {
	name = name_c ;
	isAmphi = amph ;
    total_time = std::vector<int>(2,0) ;
	teacher = std::vector<std::string>(2, std::string()) ;
	schedule = std::vector<bool>(2*MAX_HOUR, false) ;
}

void Group::display(int semester) const {
    std::cout<<"Group "<<name<<std::endl ;    

    for (int j=MIN_SEM ; j <= MAX_SEM ; j++) {
        
        std::cout<<"Semester "<<j+1<<" : "<<std::endl ;        

        if (!teacher[j].empty()) {
            std::cout<<"This group is assigned to "<<teacher[j]<<std::endl ;
        } else {
            std::cout<<"This group has no teacher"<<std::endl ;
        }
        int i = 0 ;
        const std::string days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"} ;
        while (i < MAX_HOUR) {
            if (schedule[j*MAX_HOUR+i]) {
                std::string day = days[(int)floor(i/21)] ;

                // Getting range from beginning to end of class
                std::string hour_begin = hourmin(16+i%21) ;
                while (schedule[++i]) {} //I'm ashamed for that...
                std::string hour_end = hourmin(16+i%21) ;
			
                std::cout<<"Class on "<<day<<" from "<<hour_begin<<" to "<<hour_end<<std::endl ;
		} 
            i++ ;
        }
        std::cout<<"Total time worked in semester : "<<hourmin(total_time[j])<<std::endl<<std::endl ;
    }

    std::cout<<"Total time worked during year : "<<hourmin(total_time[0]+total_time[1])<<std::endl ;
}

void Group::add_hour(int semester, int begin, int end) {
	// This error should never happen ; if so, check red_parse.pl to find error
	if (begin>=end || begin > MAX_HOUR) {
		std::cout<<"Error in add_hour : incompetent developer"<<std::endl ;
		exit(2) ;
	}
	for (int i=begin ; i<end ; i++) {
		schedule[semester*MAX_HOUR+i] = true ;
	}
}

bool operator< (const Group& lhs, const Group& rhs) {
	return lhs.name < rhs.name ;
}

//Converts number to readable hour
std::string hourmin(int i) {
	std::stringstream ss ;
	int hour = (int)floor(i/2) ;
	std::string min = (i%2)?"30":"" ;
	ss <<hour << 'h' << min ;
	
	return ss.str() ;
}

