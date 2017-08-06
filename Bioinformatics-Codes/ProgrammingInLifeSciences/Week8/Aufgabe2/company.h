#ifndef COMPANY_H
#define COMPANY_H

#include <string>
using namespace std;

class Task {
  private:
    string description;
    unsigned int hours_required;
    bool completed;

  public:
    Task(void);

    Task(const Task& task);

    Task(string description, unsigned int hours_required, bool completed);

    bool getCompleted(void) const;

    string getDescription(void) const;

    unsigned int getHours_required(void) const;

    void setCompleted(bool completed);

    void setDescription(string description);

    void setHours_required(unsigned int hours_required);
};

class Employee {

  private:
    string name;
    unsigned int hourly_wage;
    Task task;
  public:
    Employee(void);

    Employee(string name, unsigned int hourly_wage,
             string task_desc, unsigned int hours_required, bool completed);

    unsigned int getHourly_wage(void) const;

    string getName(void) const;

    Task &getTask(void);

    void setHourly_wage(unsigned int hourly_wage);

    void setName(string name);

    void setTask(Task task);

};

class Company {
  private:
    string name;
    unsigned int num_employees;
    Employee *employees;

  public:
    Company(string name, Employee *employees, unsigned int num_employees);
    Company(string name, unsigned int num_employees);
    Company(const Company &other);
    ~Company(void);

    string getName(void) const;

    void setName(string name);

    unsigned int getNum_employees(void) const;

    void setNum_employees(unsigned int num_employees);

    Employee* getEmployees(void) const;

    void setEmployees(Employee *employees);

    unsigned int totalCosts(bool future);
};

#endif /* COMPANY_H */
