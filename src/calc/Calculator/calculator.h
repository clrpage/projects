#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <sstream>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private:
    Ui::Calculator *ui;
    enum State
    {
        Any,
        Number,
        Math,
        ReadyAny,
        ReadyNumber,
        Computing,
        Error
    };

    enum Trigger
    {
        NumberPressed,
        MathPressed,
        DecimalPressed,
        EqualPressed,
        ErrorOccured,

    };


    State CurrentState = Any;
    Trigger CurrentTrigger = NumberPressed;

    bool CheckLength();

    void ChangeState()
    {
        switch(CurrentState)
        {
            case Any:
                switch(CurrentTrigger)
                {
                    case MathPressed:
                        CurrentState = Math;
                        break;
                    case DecimalPressed:
                        CurrentState = Number;
                        break;
                    default:
                        break;
                };
                break;
            case Number:
                switch(CurrentTrigger)
                {
                    case MathPressed:
                        CurrentState = Math;
                        break;
                    default:
                        break;
                };
                break;
            case Math:
                switch(CurrentTrigger)
                {
                    case NumberPressed:
                        CurrentState = ReadyAny;
                        break;
                    case DecimalPressed:
                        CurrentState = ReadyNumber;
                    default:
                        break;
                };
                break;
            case ReadyAny:
                switch(CurrentTrigger)
                {
                    case DecimalPressed:
                        CurrentState = ReadyNumber;
                        break;
                    case EqualPressed:
                        CurrentState = Computing;
                    default:
                        break;
                };
                break;
           case ReadyNumber:
                switch(CurrentTrigger)
                {
                    case EqualPressed:
                        CurrentState = Computing;
                        break;
                    default:
                        break;
                }
                break;
          case Computing:
                switch(CurrentTrigger)
                {
                    case ErrorOccured:
                        CurrentState = Error;
                        break;
                    default:
                        CurrentState = Math;
                        break;
                }
                break;
         case Error:
                switch(CurrentTrigger)
                {
                    case NumberPressed:
                        CurrentState = Any;
                        break;
                    case DecimalPressed:
                        CurrentState = Number;
                        break;
                    default:
                        break;
                }
                break;

        }
    };

private slots:
    void NumPressed();
    void DecPressed();
    void ResetPressed();
    void MathButtonPressed();
//    void EqualButtonPressed();
};
#endif // CALCULATOR_H
