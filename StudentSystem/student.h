#ifndef STUDENT_H
#define STUDENT_H
#include <QString>

//struct Course
//{
//    QString course;
//    double score;
//    void getCourse(QSrting x){ course = x; }
//    void getScore(double x) { score = x; }
//};

class Student
{
private:
    QString id;   //学号
    QString name; //姓名
    QString sex;  //性别
    QString classname; //班级

    bool course[7];          //确定是否选了该门课
    double course_score[7];  //该门课的成绩

public:
    Student();
    void getId(QString x) { id = x;}                //为id(学号)赋值
    void getName(QString x) { name = x;}            //为name(姓名)赋值
    void getSex(QString x) { sex = x;}              //为sex(性别)赋值
    void getClassName(QString x) { classname = x; } //为classname(班级)赋值

    QString value_id() { return id; }               //返回学号
    QString value_name() { return name; }           //返回姓名
    QString value_sex() { return sex; }             //返回性别
    QString value_classname() { return classname; } //返回班级

    double value_Score(int i) { return course_score[i]; }   //返回对应科目的成绩
    void getScore(int i,double x) { course_score[i] = x; }  //录入成绩

    void select_course(int i) { course[i] = 1; }   //选课
    void delete_course(int i) { course[i] = 0; }   //退课
    bool already_select(int i) { return course[i]; }  //判断是否选过该门课
    void File_Load_course(int i,int x) { course[i] = x; }  //文件导入数据需要

};

#endif // STUDENT_H
