#include "calculator.h"
#include "ui_calculator.h"

enum mathOperation
{
    Add,
    Subtract,
    Multiply,
    Divide
};

double firstValue = 0.0;
double secondValue = 0.0;
mathOperation CurrentOperator = Add;

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);
    ui->line_top->setText("");
    ui->line_bottom->setText(QString::number(0));
    QPushButton *numButtons[10];
    for(int i = 0; i<10; ++i){
        QString butName = "btn_"+QString::number(i);
        numButtons[i] = Calculator::findChild<QPushButton *>(butName);
        connect(numButtons[i], SIGNAL(released()),this,SLOT(NumPressed()));
    }
    connect(ui->btn_dot, SIGNAL(released()),this,SLOT(DecPressed()));
    connect(ui->btn_reset, SIGNAL(released()),this,SLOT(ResetPressed()));
    connect(ui->btn_add, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->btn_sub, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->btn_mult, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
    connect(ui->btn_div, SIGNAL(released()), this,
            SLOT(MathButtonPressed()));
}



Calculator::~Calculator()
{
    delete ui;
}

bool Calculator::CheckLength(){
    QString displayVal = ui->line_bottom->text();
    QString controlValue = displayVal.remove(QRegExp("[\\.]+"));
    if(controlValue.size() == 16){
        return false;
    }
    return true;
}


void Calculator::NumPressed(){

    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    QString displayVal = ui->line_bottom->text();
    if(!Calculator::CheckLength()){
        return;
    }

    switch(Calculator::CurrentState){
        case Any:
            if(displayVal.toDouble() == 0)
            {
                ui->line_bottom->setText(butVal);
            } else {
                QString newVal = displayVal+butVal;
                ui->line_bottom->setText(newVal);
                }
            break;
        case Math:
            ui->line_bottom->setText(butVal);
            break;
        case Error:
            ui->line_bottom->setText(butVal);
            firstValue = butVal.toDouble();
            break;
        case ReadyAny:
        case ReadyNumber:
        case Number:{
            QString newVal = displayVal+butVal;
            ui->line_bottom->setText(newVal);
        }
        default:break;
    }

    Calculator::CurrentTrigger = NumberPressed;
    Calculator::ChangeState();
    ui->statusBar->showMessage(QString::number(Calculator::CurrentState));

}

void Calculator::DecPressed(){
    QString displayVal = ui->line_bottom->text();
    if(!Calculator::CheckLength()){
        return;
    }
    switch(Calculator::CurrentState){
        case Any:
        case ReadyAny:{
        QString newVal = displayVal+'.';
        ui->line_bottom->setText(newVal);
        break;
        }
        case Math:
            ui->line_bottom->setText("0.");
            break;
        default: break;

    }
    Calculator::CurrentTrigger = DecimalPressed;
    Calculator::ChangeState();
    ui->statusBar->showMessage(QString::number(Calculator::CurrentState));

}

void Calculator::ResetPressed(){
    ui->line_bottom->setText("0");
    ui->line_top->setText("");
    firstValue = 0.0;
    secondValue = 0.0;
    CurrentOperator = Add;
    Calculator::CurrentState = Any;
    ui->statusBar->showMessage(QString::number(Calculator::CurrentState));
}

void Calculator::MathButtonPressed(){
    if(Calculator::CurrentState != Any && Calculator::CurrentState != Number && Calculator::CurrentState != Math){
        return;
    }
    QString displayVal = ui->line_bottom->text();
    firstValue = displayVal.toDouble();

    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    if(butVal ==  "/"){
        CurrentOperator = Divide;
    }else if(butVal ==  "*"){
        CurrentOperator = Multiply;
    }else if(butVal ==  "-"){
        CurrentOperator = Subtract;
    }else if(butVal ==  "+"){
        CurrentOperator = Add;
    }
    ui->line_top->setText(QString::number(firstValue,'g',16)+butVal);
    Calculator::CurrentTrigger = MathPressed;
    Calculator::ChangeState();
    ui->statusBar->showMessage(QString::number(Calculator::CurrentState));
}
