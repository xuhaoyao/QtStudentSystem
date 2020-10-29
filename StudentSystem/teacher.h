#ifndef TEACHER_H
#define TEACHER_H
#include "student.h"
#include <QVector>


class Teacher
{
private:
    QString id;          //用户名
    QString passward;    //密码

public:
    QVector<Student> student;       //为了编写函数方便，这里将学生设为公有成员

    Teacher();
    void getpassWard(QString x){ passward = x; }     //为passWard(密码)赋值
    void getId(QString x) { id = x;}                 //为id(用户名)赋值

    QString value_id() { return id; }                //返回用户名
    QString value_passWard() { return passward; }    //返回密码
};

#endif // TEACHER_H
