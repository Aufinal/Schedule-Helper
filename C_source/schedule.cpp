#include "config.h"
#include "schedule.h" 
#include <iostream>
#include <fstream>
#include <sstream>

Schedule::Schedule() {
	classes = std::map<std::string, Group>() ;
	teachers = std::map<std::string, Teacher>() ;
}


void Schedule::import_html(std::string schedule, std::string dotations) {
	//Extracts html data to csv format
	std::string command = "perl Perl_source/red_parse.pl "+schedule ;
	system(command.c_str());

    parse("TD.txt", 0) ;
    parse("Amphi.txt", 1) ;
    parse(dotations, 2) ;

    return ;
    
}

void Schedule::parse(std::string filename, int datatype) {
    std::ifstream file(filename) ;
    if (!file.is_open()) {
        std::cout<<"Error : impossible to open "<<filename<<std::endl ;
        return ;
    }
    
    std::string line ;
    std::queue<std::string> parsedline ;
    char delim = datatype==2 ? ';' : ',' ;
    while (getline(file, line)) {
        split(line, delim, parsedline) ;
    
        if (datatype == 2) {
            if (parsedline.size()!=3) {
                std::cout<<"Error : wrong format in dotation file"<<filename
                        <<std::endl ;
            }
            std::string group = parsedline.front() ;
            parsedline.pop() ;
            int t1 = std::stoi(parsedline.front()) ;
            parsedline.pop() ;
            int t2 = std::stoi(parsedline.front()) ;
            parsedline.pop() ;
            try {
                classes.at(group).total_time[0] = 2*t1 ;
                classes.at(group).total_time[1] = 2*t2 ;
            } catch (...) {
			    std::cout<<"Error : dotation file contains unknown group "
                        <<group<<std::endl ;
            }
        } else {
            if (parsedline.size() != 4) {
                std::cout<<"Error : wrong format in schedule file"<<filename
                        <<std::endl ;
            }
            std::string group = parsedline.front() ;
            parsedline.pop() ;
            int sem = std::stoi(parsedline.front()) ;
            parsedline.pop() ;
            int h_begin = std::stoi(parsedline.front()) ;
            parsedline.pop() ;
            int h_end = std::stoi(parsedline.front()) ;
            parsedline.pop() ;
            try {
                classes.at(group).add_hour(sem, h_begin, h_end) ;
            } catch (...) {
                Group g(group, (bool)datatype) ;
                g.add_hour(sem, h_begin, h_end) ;
                classes.insert(std::pair<std::string, Group>(group, g)) ;
            }
        }
    }    
    file.close() ;
    return ;
}

void Schedule::add_teacher(std::string name) {
	Teacher t(name) ;
	teachers.insert(std::pair<std::string, Teacher>(name, t)) ;
}

void Schedule::delete_teacher(std::string name) {
	int n = teachers.erase(name) ;
	if (n == 0) {
		std::cout<<"The teacher "<<name<<" didn't exist !"<<std::endl ;
		return ;
	}
	for (std::map<std::string, Group>::iterator it = classes.begin() ;
		it!=classes.end() ; it++) {
        for (int i=0 ; i<2 ; i++) {
            if (it->second.teacher[i] == name) {
                std::cout<<"Unassigned group "<<it->second.name<<std::endl ;
                it->second.teacher[i] = "" ;
            }
        }
    }
}

void Schedule::indisp(std::string name, int semester, int day, int h_begin, int h_end) {
	try {
		Teacher t = teachers.at(name) ;
		if (day == 5||h_begin<0||h_begin>HALFHOURS_A_DAY
			||h_end<0||h_end>HALFHOURS_A_DAY||h_begin>=h_end) {
			std::cout<<"Error in day/hour specification"<<std::endl ;
			return ;
		}
		t.indisp(semester, HALFHOURS_A_DAY*day + h_begin, HALFHOURS_A_DAY*day + h_end) ;
	}
	catch(...) {
		std::cout<<"The teacher "<<name<<" doesn't exist"<<std::endl ;
	}
}

void Schedule::disp(std::string name, int semester, int day, int h_begin, int h_end) {
	try {
		Teacher t = teachers.at(name) ;
		if (day == 5||h_begin<0||h_begin>HALFHOURS_A_DAY
			||h_end<0||h_end>HALFHOURS_A_DAY||h_begin>=h_end) {
			std::cout<<"Error in day/hour specification"<<std::endl ;
			return ;
		}
        for (int i = MIN_SEM ; i<=MAX_SEM ; i++) {
            t.disp(i, HALFHOURS_A_DAY*day + h_begin, HALFHOURS_A_DAY*day + h_end) ;
	    }
    }
	catch(...) {
		std::cout<<"The teacher "<<name<<" doesn't exist"<<std::endl ;
	}
}

void Schedule::assign (std::string teacher, std::vector<std::string> groups, int semester) {
	std::map<std::string, Teacher>::iterator it = teachers.find(teacher) ;
    if (it == teachers.end()) {
        std::cout<<"Error : required teacher " <<teacher<<" doesn't exist"<<std::endl ;
        return ;
    }
    for (std::vector<std::string>::iterator g=groups.begin() ; g!=groups.end() ; g++) {
        std::map<std::string, Group>::iterator it2 = classes.find(*g) ;
        
        if (it2 == classes.end()) {
            std::cout<<"Error : required group "<<*g<<" doesn't exist"<<std::endl ;
            continue ;
        }
        for (int i=MIN_SEM ; i<=MAX_SEM ; i++) {
            if (!it2->second.teacher[i].empty()) {
                std::cout<<"Group "<<it2->second.name<<" is already assigned to "<<it2->second.teacher[i]<<std::endl ;
                return ;
            }
            if (!it->second.is_compatible(it2->second, i)) {
                std::cout<<"Group "<<it2->second.name<<" is not compatible with "<<it->second.name<<std::endl ;
                return ;
            }
        
            it->second.add_group(it2->second, i) ;
            it2->second.teacher[i] = it->second.name ;
        }
}
}

void Schedule::unassign (std::string group, int semester) {
	std::map<std::string, Group>::iterator it = classes.find(group) ;
	if (it == classes.end()) {
		std::cout<<"Error : required group "<<group<<" doesn't exist"<<std::endl ;
		return ;
	}
    for (int i = MIN_SEM ; i <= MAX_SEM ; i++) {
        if (it->second.teacher[i].empty()) {
            std::cout<<"Error : group "<<group<<" has no teacher at semester "<<i<<std::endl ;
           continue  ;
        }
        Teacher* teacher = &teachers[it->second.teacher[i]] ;	
        teacher->remove_group(it->second, i) ;
        it->second.teacher[i] = "" ;
    }
}


void Schedule::unassigned_groups(int semester) {
    for (int i = MIN_SEM ; i<=MAX_SEM ; i++) {
        std::cout<<"Semester "<<i<<std::endl ;
        std::cout<<"Unassigned amphis :" ;
        for (std::map<std::string, Group>::iterator it=classes.begin() ; 
        it != classes.end() ; it++) {
            if (it->second.teacher[i].empty() && it->second.isAmphi) {
                std::cout<<' '<<it->second.name<<',' ;
            }
        }
        //Deleting trailing commas
        std::cout<<"\b "<<std::endl ;

        std::cout<<"Unassigned TD groups :" ;
        for (std::map<std::string, Group>::iterator it=classes.begin() ; 
        it != classes.end() ; it++) {
            if (it->second.teacher[i].empty() && !it->second.isAmphi) {
                std::cout<<' '<<it->second.name<<',' ;
            }
        }
        std::cout<<"\b "<<std::endl ;
    }
}

void Schedule::compatible_groups(Teacher t, int semester) {
    for (int i=MIN_SEM ; i<=MAX_SEM ; i++) {
        std::cout<<"Semester "<<i<<std::endl ;
        std::cout<<"Compatible amphis :" ;
        for (std::map<std::string, Group>::iterator it=classes.begin() ; 
        it != classes.end() ; it++) {
            if (it->second.teacher[i].empty() && it->second.isAmphi
                && t.is_compatible(it->second, i)) {
                std::cout<<' '<<it->second.name<<',' ;
            }
        }
        std::cout<<"\b "<<std::endl ;

        std::cout<<"Compatible TD groups :" ;
        for (std::map<std::string, Group>::iterator it=classes.begin() ; 
        it != classes.end() ; it++) {
            if (it->second.teacher[i].empty() && !it->second.isAmphi
                &&t.is_compatible(it->second, i)) {
                std::cout<<' '<<it->second.name<<',' ;
            }
        }
        std::cout<<"\b "<<std::endl ;
    }
}

void Schedule::compatible_omsi(Teacher t, int semester) {
	char omsi_group[10] ;
    for (int i=MIN_SEM ; i<=MAX_SEM ; i++) {
        std::cout<<"Semester "<<i<<" :" ;
        for (std::map<std::string, Group>::iterator it=classes.begin() ; it != classes.end() ; it++) {
            if (sscanf(it->first.c_str(), "O%s", omsi_group)) {
                if(it->second.teacher[i].empty() && classes[omsi_group].teacher[i].empty() && t.is_compatible2(it->second, classes[omsi_group], i)) {
                    std::cout<<" "<<omsi_group<<"," ;
                }
            }
        }
        std::cout<<"\b"<<std::endl ;
    }
}

void Schedule::compatible_2groups(Teacher t, int semester) {
    for (int i=MIN_SEM ; i<=MAX_SEM ; i++) {
        std::cout<<"Semester "<<i<<" : " ;
        for (std::map<std::string, Group>::iterator it1=classes.begin() ; it1!=classes.end() ; it1++) {
            for (std::map<std::string, Group>::iterator it2=it1 ; it2!=classes.end() ; it2++) {
                if(it1->second.teacher[i].empty() && it2->second.teacher[i].empty() && t.is_compatible2(it1->second, it2->second, i)) {
                    std::cout<<it1->second.name<<" + "<<it2->second.name<<" ; "<<std::endl ;
                }
            }	
        }
        std::cout<<"\b\b"<<std::endl ;
    }
}

void Schedule::compatible_teachers(Group g, int semester) {
    for (int i=MIN_SEM ; i<=MAX_SEM ; i++) {
        if (!g.teacher[i].empty()) {
            std::cout<<"Error : group "<<g.name<<" is already assigned to "<<g.teacher[i]<<" in semester "<<i<<std::endl ;
            continue ;
        }
        std::cout<<"Semester "<<i<<" :" ;
        for (std::map<std::string, Teacher>::iterator it=teachers.begin() ;
            it !=teachers.end() ; it++) {
            if (it->second.is_compatible(g, i)) {
                std::cout<<' '<<it->second.name<<',' ;			
            }
        }
        std::cout<<"\b"<<std::endl ;
    }
}

void Schedule::compatible(std::string name, int semester) {
	std::map<std::string, Group>::iterator it1 = classes.find(name) ;
	std::map<std::string, Teacher>::iterator it2 = teachers.find(name) ;

	if (it1 == classes.end() && it2 == teachers.end()) {
		std::cout<<"The name "<<name<<" is neither a group nor a teacher"<<std::endl ;
		return ;
	} else if (it1 != classes.end()) {
		compatible_teachers(it1->second, semester) ;	
	} else {
		compatible_groups(it2->second, semester) ;
	}
}

void Schedule::compatible2(std::string name, int semester) {
	try {
		Teacher t = teachers.at(name) ;
		compatible_2groups(t, semester) ;
	}
	catch (...) {
		std::cout<<"Error : teacher "<<name<<" doesn't exist"<<std::endl ;
	}
}

void Schedule::compatible_omsi(std::string name, int semester) {
	try {
		Teacher t = teachers.at(name) ;
		compatible_omsi(t, semester) ;
	}
	catch (...) {
		std::cout<<"Error : teacher "<<name<<" doesn't exist"<<std::endl ;
	}
}

void Schedule::list_teachers() {
	std::cout<<"Teachers currently in database :" ;
	for (std::map<std::string, Teacher>::iterator it=teachers.begin() ;
			it !=teachers.end() ; it++) {
		std::cout<<' '<<it->second.name<<',' ;			
	}
	std::cout<<"\b "<<std::endl ;

}

void Schedule::list_groups() {
	std::cout<<"Amphis in database :" ;
	for (std::map<std::string, Group>::iterator it=classes.begin() ; 
	it != classes.end() ; it++) {
		if (it->second.isAmphi) {
			std::cout<<' '<<it->second.name<<',' ;
		}
	}
	std::cout<<"\b "<<std::endl ;

	std::cout<<"TD groups in database :" ;
	for (std::map<std::string, Group>::iterator it=classes.begin() ; 
	it != classes.end() ; it++) {
		if (!it->second.isAmphi) {
			std::cout<<' '<<it->second.name<<',' ;
		}
	}
	std::cout<<"\b "<<std::endl ;
}

void Schedule::display(std::string name, int semester) {
	std::map<std::string, Group>::iterator it1 = classes.find(name) ;
	std::map<std::string, Teacher>::iterator it2 = teachers.find(name) ;

	if (it1 == classes.end() && it2 == teachers.end()) {
		std::cout<<"The name "<<name<<" is neither a group nor a teacher"<<std::endl ;
		return ;
	} else if (it1 != classes.end()) {
		it1->second.display(semester) ;
	} else {
		it2->second.display(semester) ;
	}
}

static void split(std::string line, char delim, std::queue<std::string>& q) {
    std::stringstream ss(line) ;
    std::string str ;
    while (std::getline(ss, str, delim)) {
        q.push(str) ;       
    }
    return ;
}

