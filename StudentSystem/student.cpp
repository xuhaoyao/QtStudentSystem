#include "student.h"

Student::Student()
{
    for(int i = 0;i < 7;i++){
        course[i] = 0;
        course_score[i] = 0;
    }
}
