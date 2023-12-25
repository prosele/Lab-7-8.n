#include <iostream>
#include <string>

using namespace std;

class BankAccount {
protected:
    long long accNumber;
    double accMoney;
public:
    BankAccount(long long accnumber, double accmoney) :  accNumber(accnumber), accMoney(accmoney) {}
    // Вклад
    void deposit(double amount) {
        accMoney += amount;
    }
    // Снятие средств
    void withdrawals(double amount) {
        if (accMoney >= amount) {
            accMoney -= amount;
        }
        else {
            cout << "На вашем счёте недостаточно средств." << endl;
        }
    }
    bool chekType(string operation) {
        return operation == "deposit";
    }
    long long getAccNumber() {
        return accNumber;
    }
    double getAccMoney() {
        return accMoney;
    }
    //перегрузка оператора == для сравнения счетов
    bool operator ==(const BankAccount & other) {
        return accMoney == other.accMoney;
    }
    // = 0 - чистая виртуальная функция, т.е. она ничего не выполняет. Создана только для переопределения во всех производных классах от класса, в котором имеется такая функция.
    virtual void printAccount() = 0;
    virtual void processTransaction() = 0;
    
};
    
class Transaction {
private:
    string date;
    
    long long numAcc;
    double balAcc;
    
public:
    Transaction(string day, BankAccount& acc) : date(day), numAcc(acc.getAccNumber()), balAcc(acc.getAccMoney()) {}
    
    // перевод денег с одного счёта на другой
    static void transactionFirst(BankAccount& accFrom, BankAccount& accTo) {
        double amount;
        cout << "Введите сумму, которую хотите перевести: ";
        cin >> amount;
        if (amount <= accFrom.getAccMoney()) {
            accFrom.withdrawals(amount);
            accTo.deposit(amount);
            cout << "Перевод со счёта " << accFrom.getAccNumber() << " на счёт " << accTo.getAccNumber() << " успешно осуществлён." << endl;
        }
        else {
            cout << "На вашем счёте недостаточно средств." << endl;
        }
    }
};
    
// наследник - сберегательный счёт
class SavingsAccount : public BankAccount {
private:
    string typeTransaction;
    double interestRate;
public:
    SavingsAccount(long long accnumber, double accmoney, double rate) : BankAccount(accnumber,accmoney), interestRate(rate) {}
    // в зависимости от ставки происходит получение процентного дохода
    void setRate() {
        accMoney += interestRate/100 * accMoney;
    }

    void processTransaction() override {
        double amount;
        cout << "Введите тип транзакции: ";
        cin >> typeTransaction;
        if (chekType(typeTransaction)) {
            cout << "Введите сумму, которую хотите положить на счёт: ";
            cin >> amount;
            deposit(amount);
            setRate();
        }
        if (!chekType(typeTransaction)) {
            cout << "Введите сумму, которую хотите снять со счёта: ";
            cin >> amount;
            withdrawals(amount);
        }
    }
    void printAccount() override {
        cout << "Номер сберегательного счёта: " << accNumber << ". Баланс: " << accMoney << "." << endl;
    }
};
// наследник - текущий счёт
class CheckingAccount : public BankAccount {
private:
    string typeTransaction;
public:
    CheckingAccount(long long accnumber, double accmoney) : BankAccount(accnumber,accmoney) {
    }
    void processTransaction() override {
        double amount;
        cout << "Введите тип транзакции: ";
        cin >> typeTransaction;
        if (chekType(typeTransaction)) {
            cout << "Введите сумму, которую хотите положить на счёт: ";
            cin >> amount;
            deposit(amount);
        }
        if (!chekType(typeTransaction)) {
            cout << "Введите сумму, которую хотите снять со счёта: ";
            cin >> amount;
            withdrawals(amount);
        }
    }
    void printAccount() override {
        cout << "Номер текущего счёта: " << accNumber << ". Баланс: " << accMoney << "." << endl;
    }
};

class Customer {
private:
    string firstName;
    string secondName;
    SavingsAccount* savings[3]; // 3 - макс. кол-во сберегательных счетов у клиента
    CheckingAccount* chekings[3]; // 3 - макс. кол-во текущих счетов у клиента
    int numberSave, numberCheck;
public:
    Customer(string fname, string sname) : firstName(fname), secondName(sname), numberSave(0), numberCheck(0) {
    }
    
    void setSave(SavingsAccount * acc) {
        if (numberSave < 3) {
            savings[numberSave] = acc;
            numberSave++;
        }
        else {
            cout << "Превышен лимит одновременно открытых сберегательных счетов." << endl;
        }
    }
    void setCheck(CheckingAccount * acc) {
        if (numberCheck < 3) {
            chekings[numberCheck] = acc;
            numberCheck++;
        }
        else {
            cout << "Превышен лимит одновременно открытых текущих счетов." << endl;
        }
    }
   
    void printCustomer() {
        cout << "Клиент " << firstName << " " << secondName << " " << endl;
        for (int i = 0; i < numberSave; i++) {
            savings[i]->printAccount();
        }
        for (int i = 0; i < numberCheck; i++) {
            chekings[i]->printAccount();
        }
    }
};

int main() {
    SavingsAccount firstsave(1,300,13);
    long long i;
    double j, r;
    cout << "Введите номер сберегательного счёта, баланс и ключевую ставку: ";
    cin >> i >> j >> r;
    SavingsAccount secondsave(i,j,r);
    SavingsAccount thirdsave(3,100,17.5);
    CheckingAccount firstcheck(1,400);
    CheckingAccount secondcheck(2,60000);
    CheckingAccount thirdcheck(3,500);
    Customer Ivan("Ivan","Ivanov");
    Ivan.setSave(&firstsave);
    Ivan.setSave(&secondsave);
    Ivan.setCheck(&firstcheck);
    Ivan.setCheck(&thirdcheck);
    Ivan.printCustomer();
    Transaction::transactionFirst(firstcheck, secondcheck);
    firstcheck.printAccount();
    secondcheck.printAccount();
    return 0;
}
