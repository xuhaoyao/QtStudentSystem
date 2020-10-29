#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "teacher.h"
#include "student.h"
#include <QVector>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QTableWidget>
#include <QWidget>
#include <QHBoxLayout>
#include <QDialog>

const int maxn = 1000;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void before_init();             //初始化操作
    int Find_student_index(QString id);      //寻找学生的下标

    void save_File();           //将数据保存在文件中
    void load_File();           //将文件中的数据导入

    void closeEvent(QCloseEvent *event);  //重写关闭窗口的函数

private:
    Ui::MainWindow *ui;
    QVector<Teacher> teacher;   //存储管理员 ->老师
    QString now_teacherID;      //登录老师的账号
    int now_teacher_Index;      //登录老师对应的teacher下标
    int now_student_Index;      //各类操作下选中的学生下标
    QStringList courses;        //课程清单

private slots:
    void on_btn_Register_checked_clicked();   //注册界面操作
    void on_btn_login_clicked();       //登录界面 点击登录

    void on_btn_deletestudent_clicked();   //系统管理界面 删除用户
    void on_btn_changepassward_clicked();  //系统管理界面 更改密码

    //学生管理界面
    void on_btn_student_add_clicked();     // 录入学生信息
    void on_btn_Find_student_clicked();    // 查询学生信息
    void on_btn_student_delete_clicked();  // 删除学生信息
    void on_btn_change_student_clicked();  // 更改学生信息
    void before_Find_student();    //查询前的操作
    void before_delete_student();  //删除学生前的操作
    void before_change_student();  //更改信息前的操作

    //课程管理界面
    void on_btn_Course_checked_clicked();  //选课
    void on_btn_course_delete_clicked();   //退课
    void on_clicked_change_course();      //改课
    void before_course_find();           //查询课程前的操作
    void before_Course_Add_student();    //选课前的操作
    void before_Course_delete_student(); //退课前的操作
    void on_btn_Course_Find_clicked();    //查询课程

    //成绩管理界面
    void before_Menu_Score();      //转到成绩管理界面前的操作

    //编辑框内容改变时为各科成绩录入分数
    void on_Score_lineEdit_highMath_editingFinished();      //高数
    void on_Score_lineEdit_posMath_editingFinished();       //概率论
    void on_Score_lineEdit_linearMath_editingFinished();    //线性代数
    void on_Score_lineEdit_discreteMath_editingFinished();  //离散数学
    void on_Score_lineEdit_cplus2_editingFinished();        //c++
    void on_Score_lineEdit_java_editingFinished();          //java
    void on_Score_lineEdit_python_editingFinished();        //python

    void on_change_student_comboBox_currentIndexChanged(const QString &arg1);   //学生管理界面 更改信息窗口 下滑栏内容变化时的一些操作
    void on_Course_id_comboBox_currentIndexChanged(const QString &arg1);        //课程管理界面 选课窗口 (id)下滑栏内容改变时的一些操作
    void on_Course_delete_idcomboBox_currentIndexChanged(const QString &arg1);  //课程管理界面 退课窗口 （id)下滑栏内容改变时的一些操作
    void on_Score_id_comboBox_currentIndexChanged(const QString &arg1);         //成绩管理界面 (id)下滑栏内容改变时的一些操作


    void print();      //打印
};


#endif // MAINWINDOW_H
