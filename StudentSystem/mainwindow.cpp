#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    before_init();
}

void MainWindow::before_init()
{

    ui->setupUi(this);

    this->setFixedSize(521,431);

    load_File();    //导入数据

    this->setWindowTitle("学籍管理系统");   //设置标题

    ui->stackedWidget->setCurrentIndex(1);     //设置登录界面为开始界面

    //添加课程清单
    courses << tr("高数") << tr("概率论") << tr("线性代数")
            << tr("离散数学") << tr("c++") << tr("java") << tr("python");

    //一些默认内容
    ui->Login_idLineEdit->setPlaceholderText("请输入用户名");
    ui->Login_passwardLineEdit->setPlaceholderText("请输入密码");
    ui->Register_idLineEdit->setPlaceholderText("请输入用户名");
    ui->Register_passwardLineEdit->setPlaceholderText("请输入密码");
    ui->Register_checkedWard_lineEdit->setPlaceholderText("再次确认密码");
    ui->Course_select_comboBox->addItems(courses);

    ui->Menu_Student->setDisabled(true);
    ui->Menu_course->setDisabled(true);
    ui->Menu_Score->setDisabled(true);
    ui->Menu_print->setDisabled(true);
    ui->Menu_exit->setDisabled(true);

    //按钮连接  实现各个页面的切换
    connect(ui->btn_SystemReturnToLogin,&QPushButton::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->btn_register,&QPushButton::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->btn_addstudent,&QPushButton::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->btn_register_return,&QPushButton::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->Menu_system,&QAction::triggered,[=](){
        ui->stackedWidget->setCurrentIndex(2);
    });
    connect(ui->Menu_add_student,&QAction::triggered,[=](){
        ui->stackedWidget->setCurrentIndex(4);
    });
    connect(ui->Menu_find_student,&QAction::triggered,[=](){
        before_Find_student();
    });
    connect(ui->Menu_delete_student,&QAction::triggered,[=](){
        before_delete_student();
    });
    connect(ui->Menu_change_student,&QAction::triggered,[=](){
        before_change_student();
    });
    connect(ui->Menu_add_course,&QAction::triggered,[=](){
        before_Course_Add_student();
    });
    connect(ui->Menu_delete_course,&QAction::triggered,[=](){
        before_Course_delete_student();
    });
    connect(ui->Menu_change_course,&QAction::triggered,[=](){
        on_clicked_change_course();
    });
    connect(ui->Menu_find_course,&QAction::triggered,[=](){
        before_course_find();
    });
    connect(ui->Menu_Score,&QAction::triggered,[=](){
        before_Menu_Score();
    });
    connect(ui->Menu_print,&QAction::triggered,[=](){
        print();
    });
    connect(ui->Menu_exit,&QAction::triggered,[=](){
        ui->Menu_Student->setDisabled(true);
        ui->Menu_course->setDisabled(true);
        ui->Menu_Score->setDisabled(true);
        ui->Menu_print->setDisabled(true);
        ui->Menu_exit->setDisabled(true);
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->File_save,&QAction::triggered,[=](){
        save_File();
        QMessageBox::about(this,"反馈","保存成功");
    });
}

int MainWindow::Find_student_index(QString id)
{
    for(int i = 0;i < teacher[now_teacher_Index].student.size();i++){
        if(id == teacher[now_teacher_Index].student[i].value_id())
            return i;
    }
    return -1;
}

void MainWindow::on_btn_Register_checked_clicked()  //注册界面操作
{
    //清理掉登录框用户名和密码
    ui->Login_idLineEdit->clear();
    ui->Login_passwardLineEdit->clear();

    if(ui->Register_idLineEdit->text() == "" ||
       ui->Register_passwardLineEdit->text() == "" ||
       ui->Register_checkedWard_lineEdit->text() == "")
    {
        QMessageBox::information(this,"反馈","未填写完整!",QMessageBox::Yes);
        return;
    }
    QString id = ui->Register_idLineEdit->text();
    QString passWard = ui->Register_passwardLineEdit->text();
    QString checkedPassWard = ui->Register_checkedWard_lineEdit->text();
    if(passWard != checkedPassWard){
        QMessageBox::information(this,"提示","请再次确认密码",QMessageBox::Yes);
        ui->Register_checkedWard_lineEdit->clear();
        return;
    }

    for(int i = 0;i < teacher.size();i++){
        QString haved_id = teacher[i].value_id();
        if(haved_id == id){
            QMessageBox::information(this,"反馈","该用户名已存在!",QMessageBox::Yes);
            return;
        }
    }

    teacher.push_back(Teacher());
    int index = teacher.size() - 1;
    teacher[index].getId(id);
    teacher[index].getpassWard(passWard);
    QMessageBox::information(this,"反馈","注册成功!");
    if(QMessageBox::No == QMessageBox::information(this,"反馈","是否要继续注册",QMessageBox::Yes | QMessageBox::No)){
        ui->Register_idLineEdit->clear();
        ui->Register_passwardLineEdit->clear();
        ui->Register_checkedWard_lineEdit->clear();
    }else{
        ui->Register_idLineEdit->clear();
        ui->Register_passwardLineEdit->clear();
        ui->Register_checkedWard_lineEdit->clear();
    }
}

void MainWindow::on_btn_login_clicked()
{
    if(ui->Login_idLineEdit->text() == "" || ui->Login_passwardLineEdit->text() == ""){
        QMessageBox::information(this,"反馈","存在空项!请将用户名和密码填写完整",QMessageBox::Yes);
        return;
    }
    QString id = ui->Login_idLineEdit->text();
    QString passWard = ui->Login_passwardLineEdit->text();
    for(int i = 0;i < teacher.size();i++){
        if(teacher[i].value_id() == id){
            if(teacher[i].value_passWard() == passWard){
                now_teacherID = id;
                now_teacher_Index = i;
                ui->stackedWidget->setCurrentIndex(2);
                ui->Login_idLineEdit->clear();
                ui->Login_passwardLineEdit->clear();
                ui->stackedWidget->setCurrentIndex(4);   //登录成功后转到学生管理界面
                ui->Menu_Student->setDisabled(false);
                ui->Menu_course->setDisabled(false);
                ui->Menu_Score->setDisabled(false);
                ui->Menu_print->setDisabled(false);
                ui->Menu_exit->setDisabled(false);

                //清理掉用户名和密码
                ui->Login_idLineEdit->clear();
                ui->Login_passwardLineEdit->clear();
                return;
            }else{
                QMessageBox::information(this,"反馈","密码错误",QMessageBox::Yes);
                ui->Login_passwardLineEdit->clear();
                return;
            }
        }
    }
    QMessageBox::information(this,"反馈","用户名不存在",QMessageBox::Yes);
}

void MainWindow::on_btn_deletestudent_clicked()
{
    QStringList items;
    for(int i = 0;i < teacher.size();i++){
        QString id = teacher[i].value_id();
        items<<id;
    }

    bool ok;
    QString deleteId = QInputDialog::getItem(this, tr("删除"),
                                         tr("选择用户名"), items, 0, false, &ok);

    if(!ok)return;

    bool changeMessage = false;      //判断删除的id是否是当前的用户
    if(deleteId == teacher[now_teacher_Index].value_id()){
        changeMessage = true;
        teacher.remove(now_teacher_Index);
    }
    else{
        bool ok1;
        QString haved_passWard = QInputDialog::getText(this, tr("提示"),tr("删除对象不是当前用户,请输入其密码"),QLineEdit::Password,"",&ok1);
        if(!ok1)return;
        for(int i = 0;i < teacher.size();i++){
            if(teacher[i].value_id() == deleteId){
                if(teacher[i].value_passWard() == haved_passWard)
                    teacher.remove(i);
                else{
                    QMessageBox::information(this,"反馈","密码错误!");
                    return;
                }
                break;
            }
        }
    }

    QMessageBox::information(this,"反馈","删除成功!",QMessageBox::Yes);

    if(changeMessage){
        ui->stackedWidget->setCurrentIndex(1);
        ui->Menu_Student->setDisabled(true);
        ui->Menu_course->setDisabled(true);
        ui->Menu_Score->setDisabled(true);
        ui->Menu_print->setDisabled(true);
        ui->Menu_exit->setDisabled(true);
    }
}


void MainWindow::on_btn_changepassward_clicked()
{
    QStringList items;
    for(int i = 0;i < teacher.size();i++){
        QString id = teacher[i].value_id();
        items<<id;
    }

    bool ok;
    QString changeId = QInputDialog::getItem(this, tr("更改"),
                                         tr("选择用户名"), items, 0, false, &ok);

    if(!ok)return;
    bool ok1;
    QString haved_passWard = QInputDialog::getText(this, tr("输入"),tr("请输入原密码"),QLineEdit::Password,"",&ok1);
    if(!ok1)return;

    if(teacher[now_teacher_Index].value_passWard() != haved_passWard){
        QMessageBox::information(this,"反馈","密码错误!");
        return;
    }

    bool ok2;
    QString passWard = QInputDialog::getText(this, tr("输入"),tr("请输入新密码"),QLineEdit::Password,"",&ok2);
    if(!ok2)return;

    bool ok3;
    QString checked_passWard = QInputDialog::getText(this, tr("输入"),tr("确认新密码"),QLineEdit::Password,"",&ok3);
    if(!ok3)return;

    while(passWard != checked_passWard){
        QMessageBox::information(this,"反馈","两次密码输入不一致!");
        bool ok4;
        checked_passWard = QInputDialog::getText(this, tr("输入"),tr("确认新密码"),QLineEdit::Password,"",&ok4);
        if(!ok4)return;
    }

    for(int i = 0;i < teacher.size();i++){
        if(teacher[i].value_id() == changeId){
            teacher[i].getpassWard(passWard);
            QMessageBox::information(this,"反馈","修改成功!");
            break;
        }
    }
}

void MainWindow::on_btn_student_add_clicked()
{
    QString id = ui->Addstudnet_id_LineEdit->text();
    QString name = ui->Addstudent_name_LineEdit->text();
    QString sex = ui->Addstudent_sex_LineEdit->text();
    QString classname = ui->Addstudent_class_LineEdit->text();
    if(id == "" || name == "" || sex == "" || classname == ""){
        QMessageBox::information(this,"反馈","存在空项!请填写完整",QMessageBox::Yes);
        return;
    }
    teacher[now_teacher_Index].student.push_back(Student());
    int student_Index = teacher[now_teacher_Index].student.size() - 1;

    teacher[now_teacher_Index].student[student_Index].getId(id);
    teacher[now_teacher_Index].student[student_Index].getName(name);
    teacher[now_teacher_Index].student[student_Index].getSex(sex);
    teacher[now_teacher_Index].student[student_Index].getClassName(classname);

    QMessageBox::information(this,"反馈","添加成功",QMessageBox::Yes);

    //添加成功后清除编辑框
    ui->Addstudent_class_LineEdit->clear();
    ui->Addstudent_name_LineEdit->clear();
    ui->Addstudent_sex_LineEdit->clear();
    ui->Addstudnet_id_LineEdit->clear();
}

void MainWindow::on_btn_Find_student_clicked()
{
    QString id = ui->Find_student_comboBox->currentText();

    for(int i = 0;i < teacher[now_teacher_Index].student.size();i++){
        if(id == teacher[now_teacher_Index].student[i].value_id()){
            //查询成功 显示学生信息
            ui->Find_student_nameLabel->setText(teacher[now_teacher_Index].student[i].value_name());
            ui->Find_student_sexLabel->setText(teacher[now_teacher_Index].student[i].value_sex());
            ui->Find_student_classLabel->setText(teacher[now_teacher_Index].student[i].value_classname());

            QMessageBox::information(this,"反馈","查询成功!",QMessageBox::Yes);
            return;
        }
    }
    QMessageBox::information(this,"反馈","未找到此学生",QMessageBox::Yes);
}

void MainWindow::before_Find_student()
{
    //查询前首先把上次查询的记录删除
    ui->Find_student_nameLabel->clear();
    ui->Find_student_classLabel->clear();
    ui->Find_student_sexLabel->clear();
    ui->Find_student_comboBox->clear();

    //添加学生学号的选项
    int max_len = 11;
    for(int i = 0;i < teacher[now_teacher_Index].student.size();i++){
        QString id = teacher[now_teacher_Index].student[i].value_id();
        ui->Find_student_comboBox->addItem(id);
        int len = id.size();
        //找最长字符串
        if(len > max_len)
            max_len = len;
    }
    int pt_val = this->font().pointSize();//获取字体的磅值
    ui->Find_student_comboBox->view()->setFixedWidth(max_len * pt_val);  //（字符数*每个字符的宽度(磅)）个像素 -> 自适应下拉宽度
    ui->Find_student_comboBox->setFixedWidth(max_len * pt_val * 1.2);   //自适应下拉框初始宽度
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::before_delete_student()
{
    //进入界面前先删除上次的记录
    ui->delete_student_comboBox->clear();

    //添加学生学号的选项
    int max_len = 11;
    for(int i = 0;i < teacher[now_teacher_Index].student.size();i++){
        QString id = teacher[now_teacher_Index].student[i].value_id();
        ui->delete_student_comboBox->addItem(id);
        int len = id.size();
        //找最长字符串
        if(len > max_len)
            max_len = len;
    }
    int pt_val = this->font().pointSize();//获取字体的磅值
    ui->delete_student_comboBox->view()->setFixedWidth(max_len * pt_val);  //(字符数*每个字符的宽度(磅))个像素 -> 自适应下拉宽度
    ui->delete_student_comboBox->setFixedWidth(max_len * pt_val * 1.2);   //自适应下拉框初始宽度
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::before_change_student()
{
    //进入界面前先删除上次的记录
    ui->change_student_comboBox->clear();

    //添加学生学号的选项
    int max_len = 11;
    for(int i = 0;i < teacher[now_teacher_Index].student.size();i++){
        QString id = teacher[now_teacher_Index].student[i].value_id();
        ui->change_student_comboBox->addItem(id);
        int len = id.size();
        //找最长字符串
        if(len > max_len)
            max_len = len;
    }
    int pt_val = this->font().pointSize();//获取字体的磅值
    ui->change_student_comboBox->view()->setFixedWidth(max_len * pt_val);  //(字符数*每个字符的宽度(磅))个像素 -> 自适应下拉宽度
    ui->change_student_comboBox->setFixedWidth(max_len * pt_val * 1.2);   //自适应下拉框初始宽度
    ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow::on_btn_student_delete_clicked()
{
    QString id = ui->delete_student_comboBox->currentText();
    for(int i = 0;i < teacher[now_teacher_Index].student.size();i++){
        if(teacher[now_teacher_Index].student[i].value_id() == id){
            teacher[now_teacher_Index].student.remove(i);
            QMessageBox::about(this,"反馈","删除成功!");
            before_delete_student();
            return;
        }
    }
    QMessageBox::about(this,"反馈","删除失败!");
}

void MainWindow::on_change_student_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "")return;
    for(int i = 0;i < teacher[now_teacher_Index].student.size();i++){
        if(arg1 == teacher[now_teacher_Index].student[i].value_id()){
            ui->change_student_namelineEdit->setText(teacher[now_teacher_Index].student[i].value_name());
            ui->change_student_sexlineEdit->setText(teacher[now_teacher_Index].student[i].value_sex());
            ui->change_student_classlineEdit->setText(teacher[now_teacher_Index].student[i].value_classname());
            return;
        }
    }
}

void MainWindow::on_btn_change_student_clicked()
{
    QString id = ui->change_student_comboBox->currentText();
    QString name = ui->change_student_namelineEdit->text();
    QString sex = ui->change_student_sexlineEdit->text();
    QString classname = ui->change_student_classlineEdit->text();
    for(int i = 0;i < teacher[now_teacher_Index].student.size();i++){
        if(id == teacher[now_teacher_Index].student[i].value_id()){
            teacher[now_teacher_Index].student[i].getName(name);
            teacher[now_teacher_Index].student[i].getSex(sex);
            teacher[now_teacher_Index].student[i].getClassName(classname);
            QMessageBox::about(this,"反馈","更改成功");
            return;
        }
    }
}

void MainWindow::before_Course_Add_student()
{
    //进入界面前先删除上次的记录
    ui->Course_id_comboBox->clear();
    ui->Course_name_label->clear();
    ui->Course_class_label->clear();

    //添加学生学号的选项
    int max_len = 11;
    for(int i = 0;i < teacher[now_teacher_Index].student.size();i++){
        QString id = teacher[now_teacher_Index].student[i].value_id();
        ui->Course_id_comboBox->addItem(id);
        int len = id.size();
        //找最长字符串
        if(len > max_len)
            max_len = len;
    }
    int pt_val = this->font().pointSize();//获取字体的磅值
    ui->Course_id_comboBox->view()->setFixedWidth(max_len * pt_val);  //(字符数*每个字符的宽度(磅))个像素 -> 自适应下拉宽度
    ui->Course_id_comboBox->setFixedWidth(max_len * pt_val * 1.2);   //自适应下拉框初始宽度


    ui->stackedWidget->setCurrentIndex(3);   //切换页面
}

void MainWindow::on_Course_id_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "")return;
    for(int i = 0;i < teacher[now_teacher_Index].student.size();i++){
        if(arg1 == teacher[now_teacher_Index].student[i].value_id()){
            now_student_Index = i;
            ui->Course_name_label->setText(teacher[now_teacher_Index].student[i].value_name());
            ui->Course_class_label->setText(teacher[now_teacher_Index].student[i].value_classname());
            return;
        }
    }
}

void MainWindow::on_btn_Course_checked_clicked()
{
    int i = ui->Course_select_comboBox->currentIndex();
    if(teacher[now_teacher_Index].student[now_student_Index].already_select(i)){
        QMessageBox::about(this,"反馈","学生已经选过这门课了!");
        return;
    }
    teacher[now_teacher_Index].student[now_student_Index].select_course(i);
    QMessageBox::about(this,"反馈","选课成功!");
}

void MainWindow::before_Course_delete_student()
{
    //进入界面前先删除上次的记录
    ui->Course_delete_idcomboBox->clear();
    ui->Course_delete_classlabel->clear();
    ui->Course_delete_namelabel->clear();

    //添加学生学号的选项
    int max_len = 11;
    for(int i = 0;i < teacher[now_teacher_Index].student.size();i++){
        QString id = teacher[now_teacher_Index].student[i].value_id();
        ui->Course_delete_idcomboBox->addItem(id);
        int len = id.size();
        //找最长字符串
        if(len > max_len)
            max_len = len;
    }
    int pt_val = this->font().pointSize();//获取字体的磅值
    ui->Course_delete_idcomboBox->view()->setFixedWidth(max_len * pt_val);  //(字符数*每个字符的宽度(磅))个像素 -> 自适应下拉宽度
    ui->Course_delete_idcomboBox->setFixedWidth(max_len * pt_val * 1.2);   //自适应下拉框初始宽度

    ui->stackedWidget->setCurrentIndex(8);  //切换页面
}

void MainWindow::on_Course_delete_idcomboBox_currentIndexChanged(const QString &arg1)
{
    //首先清除上次的记录
    ui->Course_delete_selectcomboBox->clear();

    for(int i = 0;i < teacher[now_teacher_Index].student.size();i++){
        if(arg1 == teacher[now_teacher_Index].student[i].value_id()){   //匹配学号
            //输入学生的姓名和班级
            ui->Course_delete_namelabel->setText(teacher[now_teacher_Index].student[i].value_name());
            ui->Course_delete_classlabel->setText(teacher[now_teacher_Index].student[i].value_classname());
            //导入所选课程
            for(int j = 0;j < 7;j++){   //遍历课程清单
                if(teacher[now_teacher_Index].student[i].already_select(j)){    //若学生选了这门课
                    ui->Course_delete_selectcomboBox->addItem(courses[j]);
                }
            }
            int max_len = 8;
            int pt_val = this->font().pointSize();//获取字体的磅值
            ui->Course_delete_selectcomboBox->view()->setFixedWidth(max_len * pt_val);  //(字符数*每个字符的宽度(磅))个像素 -> 自适应下拉宽度
            ui->Course_delete_selectcomboBox->setFixedWidth(max_len * pt_val * 1.2);   //自适应下拉框初始宽度

            now_student_Index = i;  //记录学生下标再退出
            break;
        }
    }
}

void MainWindow::on_btn_course_delete_clicked()
{
    QString delete_course = ui->Course_delete_selectcomboBox->currentText();
    for(int i = 0;i < 7;i++){
        if(courses[i] == delete_course){
            teacher[now_teacher_Index].student[now_student_Index].delete_course(i);
            QMessageBox::about(this,"反馈","退课成功!");

            //退课之后 该门课不能出现在下滑栏 应在下滑栏清除这门课
            ui->Course_delete_selectcomboBox->clear();
            //重新录入已选课程
            QString id = ui->Course_delete_idcomboBox->currentText();
            for(int i = 0;i < teacher[now_teacher_Index].student.size();i++){
                if(id == teacher[now_teacher_Index].student[i].value_id()){   //匹配学号
                    //导入所选课程
                    for(int j = 0;j < 7;j++){   //遍历课程清单
                        if(teacher[now_teacher_Index].student[i].already_select(j)){    //若学生选了这门课
                            ui->Course_delete_selectcomboBox->addItem(courses[j]);
                        }
                    }
                }
            }
            break;
        }
    }
}

void MainWindow::on_clicked_change_course()
{
    //首先选择用户名
    QStringList items;
    for(int i = 0;i < teacher[now_teacher_Index].student.size();i++){
        QString id = teacher[now_teacher_Index].student[i].value_id();
        items<<id;
    }
    bool ok;
    QString id = QInputDialog::getItem(this, tr("改课"),
                                         tr("选择用户名"), items,0,false,&ok);
    if(!ok)return;

    now_student_Index = Find_student_index(id);    //找到学生下标

    //选择要更改的课程
    QStringList items1;
    for(int i = 0;i < 7;i++){
        if(teacher[now_teacher_Index].student[now_student_Index].already_select(i))
            items1 << courses[i];
    }
    bool ok1;
    QString Want_Change_Course = QInputDialog::getItem(this,tr("改课"),
                                                       tr("选择要更改的课程"),items1,0,false,&ok1);
    if(!ok1)return;

    //选择心仪的课程
    QStringList items2;
    for(int i = 0;i < 7;i++){
        if(0 == teacher[now_teacher_Index].student[now_student_Index].already_select(i))
            items2 << courses[i];
    }
    bool ok2;
    QString To_Change_Course = QInputDialog::getItem(this,tr("改课"),
                                                       tr("选择心仪的课程"),items2,0,false,&ok2);
    if(!ok2)return;
    for(int i = 0;i < 7;i++){
        if(Want_Change_Course == courses[i])
            teacher[now_teacher_Index].student[now_student_Index].delete_course(i);   //退课
        if(To_Change_Course == courses[i])
            teacher[now_teacher_Index].student[now_student_Index].select_course(i);  //选课
    }
    QMessageBox::about(this,"反馈","改课成功!");
}

void MainWindow::before_course_find()
{
    //清除上次操作
    ui->Course_Find_comboBox->clear();
    ui->Course_Find_label_cplus2->clear();
    ui->Course_Find_label_discreteMath->clear();
    ui->Course_Find_label_highmath->clear();
    ui->Course_Find_label_java->clear();
    ui->Course_Find_label_linearMath->clear();
    ui->Course_Find_label_posmath->clear();
    ui->Course_Find_label_python->clear();

    //为下滑栏添加学生学号
    QStringList items;
    int max_len = 11;
    for(int i = 0;i < teacher[now_teacher_Index].student.size();i++){
        QString id = teacher[now_teacher_Index].student[i].value_id();
        items<<id;
        int len = id.size();
        if(len > max_len)
            max_len = len;
    }
    ui->Course_Find_comboBox->addItems(items);

    int pt_val = this->font().pointSize();//获取字体的磅值
    ui->Course_Find_comboBox->view()->setFixedWidth(max_len * pt_val);  //(字符数*每个字符的宽度(磅))个像素 -> 自适应下拉宽度
    ui->Course_Find_comboBox->setFixedWidth(max_len * pt_val * 1.2);   //自适应下拉框初始宽度

    ui->stackedWidget->setCurrentIndex(9);
}

void MainWindow::on_btn_Course_Find_clicked()
{
    QString id = ui->Course_Find_comboBox->currentText();
    now_student_Index = Find_student_index(id);

    //查询结果  三元运算符
    ui->Course_Find_label_highmath->setText((teacher[now_teacher_Index].student[now_student_Index].already_select(0) ? "已选" : "未选"));
    ui->Course_Find_label_posmath->setText((teacher[now_teacher_Index].student[now_student_Index].already_select(1) ? "已选" : "未选"));
    ui->Course_Find_label_linearMath->setText((teacher[now_teacher_Index].student[now_student_Index].already_select(2) ? "已选" : "未选"));
    ui->Course_Find_label_discreteMath->setText((teacher[now_teacher_Index].student[now_student_Index].already_select(3) ? "已选" : "未选"));
    ui->Course_Find_label_cplus2->setText((teacher[now_teacher_Index].student[now_student_Index].already_select(4) ? "已选" : "未选"));
    ui->Course_Find_label_java->setText((teacher[now_teacher_Index].student[now_student_Index].already_select(5) ? "已选" : "未选"));
    ui->Course_Find_label_python->setText((teacher[now_teacher_Index].student[now_student_Index].already_select(6) ? "已选" : "未选"));

    QMessageBox::information(this,"反馈","查询成功!");
}

void MainWindow::before_Menu_Score()
{
    ui->Score_id_comboBox->clear();
    //为下滑栏添加学生学号
    QStringList items;
    int max_len = 11;
    for(int i = 0;i < teacher[now_teacher_Index].student.size();i++){
        QString id = teacher[now_teacher_Index].student[i].value_id();
        items<<id;
        int len = id.size();
        if(len > max_len)
            max_len = len;
    }
    ui->Score_id_comboBox->addItems(items);

    int pt_val = this->font().pointSize();//获取字体的磅值
    ui->Score_id_comboBox->view()->setFixedWidth(max_len * pt_val);  //(字符数*每个字符的宽度(磅))个像素 -> 自适应下拉宽度
    ui->Score_id_comboBox->setFixedWidth(max_len * pt_val * 1.2);   //自适应下拉框初始宽度

    ui->stackedWidget->setCurrentIndex(10);
}

void MainWindow::on_Score_id_comboBox_currentIndexChanged(const QString &arg1)
{
    //添加课程清单
    courses << tr("高数") << tr("概率论") << tr("线性代数")
            << tr("离散数学") << tr("c++") << tr("java") << tr("python");

    now_student_Index = Find_student_index(arg1);
    if(now_student_Index == -1) return;
    //学号对应的姓名和班级
    ui->Score_name_label->setText(teacher[now_teacher_Index].student[now_student_Index].value_name());
    ui->Score_class_label->setText(teacher[now_teacher_Index].student[now_student_Index].value_classname());

    //学号对应的各科成绩
    if(teacher[now_teacher_Index].student[now_student_Index].already_select(0)){  //高数
        ui->Score_lineEdit_highMath->setDisabled(false);
        ui->Score_lineEdit_highMath->setText(QString::number(teacher[now_teacher_Index].student[now_student_Index].value_Score(0)));
    }
    else{
        ui->Score_lineEdit_highMath->setDisabled(true);
        ui->Score_lineEdit_highMath->setText("未选");
    }

    if(teacher[now_teacher_Index].student[now_student_Index].already_select(1)){  //概率论
        ui->Score_lineEdit_posMath->setDisabled(false);
        ui->Score_lineEdit_posMath->setText(QString::number(teacher[now_teacher_Index].student[now_student_Index].value_Score(1)));
    }
    else{
        ui->Score_lineEdit_posMath->setDisabled(true);
        ui->Score_lineEdit_posMath->setText("未选");
    }

    if(teacher[now_teacher_Index].student[now_student_Index].already_select(2)){  //线性代数
        ui->Score_lineEdit_linearMath->setDisabled(false);
        ui->Score_lineEdit_linearMath->setText(QString::number(teacher[now_teacher_Index].student[now_student_Index].value_Score(2)));
    }
    else{
        ui->Score_lineEdit_linearMath->setDisabled(true);
        ui->Score_lineEdit_linearMath->setText("未选");
    }

    if(teacher[now_teacher_Index].student[now_student_Index].already_select(3)){  //离散数学
        ui->Score_lineEdit_discreteMath->setDisabled(false);
        ui->Score_lineEdit_discreteMath->setText(QString::number(teacher[now_teacher_Index].student[now_student_Index].value_Score(3)));
    }
    else{
        ui->Score_lineEdit_discreteMath->setDisabled(true);
        ui->Score_lineEdit_discreteMath->setText("未选");
    }

    if(teacher[now_teacher_Index].student[now_student_Index].already_select(4)){  //c++
        ui->Score_lineEdit_cplus2->setDisabled(false);
        ui->Score_lineEdit_cplus2->setText(QString::number(teacher[now_teacher_Index].student[now_student_Index].value_Score(4)));
    }
    else{
        ui->Score_lineEdit_cplus2->setDisabled(true);
        ui->Score_lineEdit_cplus2->setText("未选");
    }

    if(teacher[now_teacher_Index].student[now_student_Index].already_select(5)){  //java
        ui->Score_lineEdit_java->setDisabled(false);
        ui->Score_lineEdit_java->setText(QString::number(teacher[now_teacher_Index].student[now_student_Index].value_Score(5)));
    }
    else{
        ui->Score_lineEdit_java->setDisabled(true);
        ui->Score_lineEdit_java->setText("未选");
    }

    if(teacher[now_teacher_Index].student[now_student_Index].already_select(6)){  //python
        ui->Score_lineEdit_python->setDisabled(false);
        ui->Score_lineEdit_python->setText(QString::number(teacher[now_teacher_Index].student[now_student_Index].value_Score(6)));
    }
    else{
        ui->Score_lineEdit_python->setDisabled(true);
        ui->Score_lineEdit_python->setText("未选");
    }
}

void MainWindow::on_Score_lineEdit_highMath_editingFinished()
{
    teacher[now_teacher_Index].student[now_student_Index].getScore(0,ui->Score_lineEdit_highMath->text().toDouble());
}

void MainWindow::on_Score_lineEdit_posMath_editingFinished()
{
    teacher[now_teacher_Index].student[now_student_Index].getScore(1,ui->Score_lineEdit_posMath->text().toDouble());
}

void MainWindow::on_Score_lineEdit_linearMath_editingFinished()
{
    teacher[now_teacher_Index].student[now_student_Index].getScore(2,ui->Score_lineEdit_linearMath->text().toDouble());
}

void MainWindow::on_Score_lineEdit_discreteMath_editingFinished()
{
    teacher[now_teacher_Index].student[now_student_Index].getScore(3,ui->Score_lineEdit_discreteMath->text().toDouble());
}

void MainWindow::on_Score_lineEdit_cplus2_editingFinished()
{
    teacher[now_teacher_Index].student[now_student_Index].getScore(4,ui->Score_lineEdit_cplus2->text().toDouble());
}

void MainWindow::on_Score_lineEdit_java_editingFinished()
{
    teacher[now_teacher_Index].student[now_student_Index].getScore(5,ui->Score_lineEdit_java->text().toDouble());
}

void MainWindow::on_Score_lineEdit_python_editingFinished()
{
    teacher[now_teacher_Index].student[now_student_Index].getScore(6,ui->Score_lineEdit_python->text().toDouble());
}

void MainWindow::save_File()
{
    //将老师的数据导入文本
    QFile file("teacher.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        QMessageBox::about(NULL,"反馈","数据文件打开失败!");
        return;
    }

    QTextStream out(&file);
    for(int i = 0;i < teacher.size();i++){
        out << teacher[i].value_id() << " " << teacher[i].value_passWard() << endl;
    }
    file.close();

    //将学生的数据导入文本
    QFile file1("student.txt");
    file1.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file1.isOpen()){
        QMessageBox::about(NULL,"反馈","数据文件打开失败!");
        return;
    }
    QTextStream out1(&file1);
    for(int i = 0;i < teacher.size();i++){
        for(int j = 0;j < teacher[i].student.size();j++){
            out1 << teacher[i].student[j].value_id() << " " << teacher[i].student[j].value_name()
                 << " " <<teacher[i].student[j].value_sex() << " " << teacher[i].student[j].value_classname();
            for(int k = 0;k < 7;k++)
                out1 << " " << teacher[i].student[j].already_select(k);
            for(int t = 0;t < 7;t++)
                out1 << " " << teacher[i].student[j].value_Score(t);
            out1 << endl;
        }
        if(i != teacher.size() - 1) out1 << QString("更换用户名") << endl;
    }
    file1.close();

}

void MainWindow::load_File()
{
    //将老师的数据从文本导入
    QFile file("teacher.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(!file.isOpen()){
        return;
    }

   QTextStream inp(&file);
   while(!inp.atEnd()){
       QString id,passWard;
       inp >>id >>passWard;
       if(inp.atEnd()) break;
       teacher.push_back(Teacher());
       int i = teacher.size() - 1;
       teacher[i].getId(id);
       teacher[i].getpassWard(passWard);
   }
   file.close();

   //将学生的数据从文本导入
   QFile file1("student.txt");
   file1.open(QIODevice::ReadOnly | QIODevice::Text);
   if(!file1.isOpen()){
       QMessageBox::about(NULL,"反馈","数据文件打开失败!");
       return;
   }

   QTextStream inp1(&file1);
   int i = 0,j = 0;
   QString flag,x;
   while(!inp1.atEnd()){
       QString id,name,sex,classname;
       inp1 >> id;
       if(id == "更换用户名"){
           i++;j = 0;
           inp1 >> id >> name >> sex >> classname;
       }
       else
           inp1 >> name >> sex >> classname;
       teacher[i].student.push_back(Student());
       teacher[i].student[j].getId(id);
       teacher[i].student[j].getName(name);
       teacher[i].student[j].getSex(sex);
       teacher[i].student[j].getClassName(classname);
       for(int k = 0;k < 7;k++){
           inp1 >> flag;
           teacher[i].student[j].File_Load_course(k,flag.toInt());
       }
       for(int k = 0;k < 7;k++){
           inp1 >> x;
           teacher[i].student[j].getScore(k,x.toDouble());
       }
       j++;
       if(inp1.atEnd())
              teacher[i].student.pop_back();
   }
   file1.close();
}

void MainWindow::closeEvent(QCloseEvent *event) //系统自带退出确定程序
{
    int choose;
    choose= QMessageBox::question(this, tr("退出程序"),
                                   QString(tr("退出程序前是否保存此次登录的操作?")),
                                   QMessageBox::Yes | QMessageBox::No);
    if(choose == QMessageBox::Yes)
        save_File();
}

void MainWindow::print()
{
    //建立打印窗口
    QDialog* dlg = new QDialog(this);
    dlg->resize(1400,400);
    dlg->setWindowTitle("打印");

    //打印的信息装在这里
    QTableWidget* Twidget = new QTableWidget(this);
    QStringList info;
    info << tr("学号") <<tr("姓名") <<tr("性别")<<tr("班级") << tr("高数") << tr("概率论") << tr("线性代数")
         << tr("离散数学") << tr("c++") << tr("java") << tr("python");
    Twidget->setColumnCount(info.size());    //列数
    Twidget->setRowCount(teacher[now_teacher_Index].student.size());   //行数
    Twidget->setHorizontalHeaderLabels(info);
    Twidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);   //自适应宽度

    int row = 0,col = 0;
    for(int i = 0;i < teacher[now_teacher_Index].student.size();i++){
        Twidget->setItem(row,col++,new QTableWidgetItem(teacher[now_teacher_Index].student[i].value_id()));
        Twidget->setItem(row,col++,new QTableWidgetItem(teacher[now_teacher_Index].student[i].value_name()));
        Twidget->setItem(row,col++,new QTableWidgetItem(teacher[now_teacher_Index].student[i].value_sex()));
        Twidget->setItem(row,col++,new QTableWidgetItem(teacher[now_teacher_Index].student[i].value_classname()));

        for(int j = 0;j < 7;j++){
            if(teacher[now_teacher_Index].student[i].already_select(j))
                Twidget->setItem(row,col++,new QTableWidgetItem(QString::number(teacher[now_teacher_Index].student[i].value_Score(j))));
            else
                Twidget->setItem(row,col++,new QTableWidgetItem(QString("未选")));
        }
    }


    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(Twidget);
    dlg->setLayout(layout);
    dlg->exec();


}


MainWindow::~MainWindow()
{
    delete ui;
}
