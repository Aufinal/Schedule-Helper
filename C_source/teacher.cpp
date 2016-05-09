#include "teacher.h"
#include "config.h"
#include <iostream>

Teacher::Teacher() {
	name = "";
	Amphis = std::vector< std::set<std::string> >(2, std::set<std::string>()) ;
	TDs = std::vector< std::set<std::string> >(2, std::set<std::string>()) ;
	time_amphi = std::vector<int>(2,0) ;
	time_TD = std::vector<int>(2,0) ;
	availability = std::vector<int>(2*MAX_HOUR, 0) ; 
}



Teacher::Teacher(std::string name_c) {
	name = name_c ;
	Amphis = std::vector< std::set<std::string> >(2, std::set<std::string>()) ;
	TDs = std::vector< std::set<std::string> >(2, std::set<std::string>()) ;
	time_amphi = std::vector<int>(2,0) ;
	time_TD = std::vector<int>(2,0) ;
	availability = std::vector<int>(2*MAX_HOUR, 0) ; 
}

void Teacher::indisp(int semester, int begin, int end) {
	//This error should not happen ; check is in schedule.cpp
	if (begin>=end || begin > MAX_HOUR) {
		std::cout<<"Error in indisp : incompetent developer"<<std::endl ;
		return ;
	}
    bool warned = false ;
	
	for (int i = begin ; i < end ; i++) {
		//This function should be used for out-of-work indisponibilities (rest is automatic) ; error if incompatibility with assigned groups
		if (availability[i] == 1) {
			std::cout<<"Error : teacher was assigned a group during indisp time"<<std::endl ; 
            return ;
		}
        if (availability[i] == 2 && !warned) {
            std::cout<<"Warning : teacher had already out-of-work indisponibility"<<std::endl ;
            warned = true ;
        } 
	}
    
    for (int i = begin ; i < end ; i++) {
        availability[i] = 2 ; 
    }
}

void Teacher::disp(int semester, int begin, int end) {
	//Should not happen ; check is made in semester.cpp
	if (begin>=end || begin > MAX_HOUR) {
		std::cout<<"Error in indisp : incompetent developer"<<std::endl ;
		return ;
	}
    
	//disp is used only to override indisp ; error if groups, warning if already disp
    bool warned=false ;    
    for (int i = begin ; i < end ; i++) {
        if (availability[i]==1) {
            std::cout<<"Error : teacher was assigned a group during this time ; unassign group and try again."<<std::endl ;
            return ;
        }
        if (availability[i]==0 && !warned) {
            std::cout<<"Warning : teacher was already available at that time"<<std::endl ;
        }
    } 
	for (int i = begin ; i < end ; i++) {
		availability[i]=0 ;
	}
}

void Teacher::add_group(Group g, int semester) {
	if (g.isAmphi) {
		Amphis[semester].insert(g.name) ;
		time_amphi[semester] += g.total_time[semester] ;
	} else {
		TDs[semester].insert(g.name) ;
		time_TD[semester] += g.total_time[semester] ;
	}
	
	for (int i=0 ; i<MAX_HOUR ; i++) {
        if (g.schedule[semester*MAX_HOUR+i]) {
           availability[semester*MAX_HOUR+i] = 1 ;
        }
	}
	
}

bool Teacher::is_compatible(Group g, int semester) const {
	for (int i = 0 ; i<MAX_HOUR ; i++) {
		if (g.schedule[semester*MAX_HOUR+i] && (availability[semester*MAX_HOUR+i]!=0)) {
			return false ;
		}
	}
	return true ;
}

bool Teacher::is_compatible2(Group g, Group h, int semester) const {
	for (int i = 0 ; i<MAX_HOUR ; i++) {
		if (g.schedule[semester*MAX_HOUR+i] && h.schedule[semester*MAX_HOUR+i]) {
			return false ;
		}
	}
	return (is_compatible(g, semester) && is_compatible(h, semester)) ;
}


void Teacher::remove_group(Group g, int semester) {
	int erased_TD = TDs[semester].erase(g.name) ;
	int erased_amph = Amphis[semester].erase(g.name) ;
	if (erased_TD == 0 && erased_amph == 0) {
		std::cout<<"Error : group "<<g.name<<" was not assigned to teacher "<<name<<std::endl ;
		return ;
	}
	time_amphi[semester] -= (erased_amph*g.total_time[semester]) ;
	time_TD[semester] -= (erased_TD*g.total_time[semester]) ;

	//This can cause problems with indisp-induced unavailability (maybe ?)
	for (int i=0 ; i<MAX_HOUR ; i++) {
        if (availability[semester*MAX_HOUR+i]==1 && g.schedule[semester*MAX_HOUR+i]) {
            availability[semester*MAX_HOUR+i]=0 ;
        }
	}
}

void Teacher::display(int semester) const{

    for (int j = MIN_SEM ; j <= MAX_SEM ; j++) {

        std::cout<<"Semester "<<j+1<<std::endl ;

        std::cout<<"Amphis : "<<std::endl ;
        for (std::set<std::string>::iterator it=Amphis[j].begin() ; it!=Amphis[j].end() ; it++) {
            std::cout<<'\t'<<*it<<std::endl ;
        }
        std::cout<<"Total time : "<<hourmin(time_amphi[j])<<std::endl ;
        std::cout<<'\n' ;
        std::cout<<"TDS : "<<std::endl ;
        for (std::set<std::string>::iterator it=TDs[j].begin() ; it!=TDs[j].end() ; it++) {
            std::cout<<'\t'<<"Group "<<*it<<std::endl ;
        }
        std::cout<<"Total time : "<<hourmin(time_TD[j])<<std::endl ;
        std::cout<<'\n' ;
	
        std::cout<<"Availability :"<<std::endl ;
        const std::string days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"} ;
        for (int daynum = 0 ; daynum < 5 ; daynum++ ) {
            int i = 0 ;
            std::cout<<days[daynum]<<" : " ;
            while (i < HALFHOURS_A_DAY-1) {
                if (availability[j*MAX_HOUR+daynum*HALFHOURS_A_DAY+i]==0) {
                
                //As in group.cpp, we get beginning and end of class
                std::string hour_begin = hourmin(16+i) ;
                while (availability[j*MAX_HOUR+daynum*HALFHOURS_A_DAY+(++i)]==0 && i < HALFHOURS_A_DAY-1) {} 
                std::string hour_end = hourmin(16+i) ;
                std::cout<<hour_begin<<'-'<<hour_end<<", " ;
                } 
                i++ ;
            }
            //Deleting trailing commas
            std::cout<<"\b\b "<<std::endl<<std::endl ;
        }

     }
    if (semester == 2) {
        std::cout<<"Time worked in whole year :"<<std::endl ;
        std::cout<<"Amphis : "<<hourmin(time_amphi[0]+time_amphi[1])<<" (équivalent à "<<hourmin(1.5*(time_amphi[0]+time_amphi[1]))<<" de TD)"<<std::endl;
        std::cout<<"TDs : "<<hourmin(time_TD[0]+time_TD[1])<<std::endl;
        std::cout<<"Total : "<<hourmin(1.5*(time_amphi[0]+time_amphi[1])+time_TD[0]+time_TD[1])<<std::endl;
    }       
        
} 

