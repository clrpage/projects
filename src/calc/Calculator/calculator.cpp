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
double solution = 0.0;
mathOperation CurrentOperator = Add;
bool solutionDisplayed = false;

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
    connect(ui->btn_eq, SIGNAL(released()), this,
            SLOT(EqualButtonPressed()));
    connect(ui->btn_reset_last, SIGNAL(released()), this,
            SLOT(ResetLastPressed()));
}



Calculator::~Calculator()
{
    delete ui;
}

int Calculator::CheckLength(){
    QString displayVal = ui->line_bottom->text();
    QString controlValue = displayVal.remove(QRegExp("[\\.]+"));
    return controlValue.size();
}

bool Calculator::CheckLengthValidity(){
    int displayLength = Calculator::CheckLength();
    if(displayLength < 16){
        return true;
    }
    return false;
}


void Calculator::NumPressed(){

    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    QString displayVal = ui->line_bottom->text();
    switch(Calculator::CurrentState){
        case Any:
            ui->line_top->setText("");
            if(displayVal.toDouble() == 0 || solutionDisplayed)
            {
                ui->line_bottom->setText(butVal);
            } else {
                if(!Calculator::CheckLengthValidity()){
                    return;
                }
                QString newVal = displayVal+butVal;
                ui->line_bottom->setText(newVal);
                }
            break;
        case Math:
            ui->line_bottom->setText(butVal);
            break;
        case Error:
            ui->line_bottom->setText(butVal);
            break;
        case ReadyAny:
        case ReadyNumber:
        case Number:{
            if(!Calculator::CheckLengthValidity()){
                return;
            }
            QString newVal = displayVal+butVal;
            ui->line_bottom->setText(newVal);
        }
        default:break;
    }
    solutionDisplayed = false;
    Calculator::CurrentTrigger = NumberPressed;
    Calculator::ChangeState();
    ui->statusBar->showMessage(QString::number(Calculator::CurrentState));

}

void Calculator::DecPressed(){
    QString displayVal = ui->line_bottom->text();
    if(!Calculator::CheckLengthValidity()){
        return;
    }
    switch(Calculator::CurrentState){
        case Any:
        case ReadyAny:{
        if(displayVal.toDouble() == 0 || solutionDisplayed){
            ui->line_bottom->setText("0.");}
        else{
            QString newVal = displayVal+'.';
            ui->line_bottom->setText(newVal);
        }
        solutionDisplayed = false;
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
    Calculator::CurrentTrigger = Reset;
    Calculator::ChangeState();
    ui->statusBar->showMessage(QString::number(Calculator::CurrentState));
}

void Calculator::MathButtonPressed(){
    //if(Calculator::CurrentState != Any && Calculator::CurrentState != Number && Calculator::CurrentState != Math){
    //    return;
    //}
    if(Calculator::CurrentState == ReadyAny || Calculator::CurrentState == ReadyNumber){
        Calculator::EqualButtonPressed();
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

void Calculator::EqualButtonPressed(){
    if(Calculator::CurrentState == Number || Calculator::CurrentState == Error || Calculator::CurrentState == Computing){
        return;
    }
    Calculator::CurrentTrigger = EqualPressed;
    Calculator::ChangeState();
    QString displayVal = ui->line_bottom->text();
    QString operatorString = "";
    if(solutionDisplayed && Calculator::CurrentState == Any){
        firstValue = solution;
    } else if(Calculator::CurrentState == Any){
        return;
    } else{
        secondValue = displayVal.toDouble();
    }
    switch(CurrentOperator){
        case Add:
            operatorString = "+";
            solution = firstValue + secondValue;
            Calculator::CurrentTrigger = Success;
            break;
        case Subtract:
            operatorString = "-";
            solution = firstValue - secondValue;
            Calculator::CurrentTrigger = Success;
            break;
        case Multiply:
            operatorString = "*";
            solution = firstValue * secondValue;
            Calculator::CurrentTrigger = Success;
            break;
        case Divide:
            operatorString = "/";
            if(secondValue==0 || secondValue==0.0){
                Calculator::CurrentTrigger = ErrorOccured;
                break;
            }
            solution = firstValue/secondValue;
            Calculator::CurrentTrigger = Success;
            break;
    }

    Calculator::ChangeState();
    ui->statusBar->showMessage(QString::number(Calculator::CurrentState));
    ui->line_top->setText(QString::number(firstValue,'g',16)+operatorString+QString::number(secondValue,'g',16)+"=");
    if(Calculator::CurrentState == Error){
        ui->line_bottom->setText("Error");
        return;
    }
    ui->line_bottom->setText(QString::number(solution,'g',16));
    solutionDisplayed = true;

}

void Calculator::ResetLastPressed(){

}
