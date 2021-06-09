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
        Success,
        Reset,
        ResetLast,

    };


    State CurrentState = Any;
    Trigger CurrentTrigger = NumberPressed;

    int CheckLength();
    bool CheckLengthValidity();

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
                    case Reset:
                        CurrentState = Any;
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
                    case ResetLast:
                    case Reset:
                        CurrentState = Any;
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
                        break;
                    case EqualPressed:
                        CurrentState = Computing;
                        break;
                    case Reset:
                        CurrentState = Any;
                        break;
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
                    case MathPressed:
                    case EqualPressed:
                        CurrentState = Computing;
                        break;
                    case Reset:
                        CurrentState = Any;
                        break;
                    default:
                        break;
                };
                break;
           case ReadyNumber:
                switch(CurrentTrigger)
                {
                    case MathPressed:
                    case EqualPressed:
                        CurrentState = Computing;
                        break;
                    case Reset:
                        CurrentState = Any;
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
                    case Reset:
                        CurrentState = Any;
                        break;
                    default:
                        CurrentState = Any;
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
                    case Reset:
                        CurrentState = Any;
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
    void EqualButtonPressed();
    void ResetLastPressed();
};
#endif // CALCULATOR_H
