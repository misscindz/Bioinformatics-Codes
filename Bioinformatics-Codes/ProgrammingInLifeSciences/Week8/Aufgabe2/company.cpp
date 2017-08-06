// Wang, Ching, Achoukhi
// Blatt 8 Aufgabe 2

#include <iostream>
#include <string>
#include <new>
#include "company.h"

using namespace std;

/************************************Task*****************************************/

Task::Task(void)
{
  this->completed = false;
}

Task::Task(const Task& task)
{
  this->description = "Copy " + task.description;
  this->hours_required = task.hours_required;
  this->completed = task.completed;
}

Task::Task(string description, unsigned int hours_required, bool completed)
{
  this->description = description;
  this->hours_required = hours_required;
  this->completed = completed;
}

bool Task::getCompleted(void) const
{
  return this->completed;
}

string Task::getDescription(void) const
{
  return this->description;
}

unsigned int Task::getHours_required(void) const
{
  return this->hours_required;
}

void Task::setCompleted(bool completed)
{
  this->completed = completed;
}

void Task::setDescription(string description)
{
  this->description = description;
}

void Task::setHours_required(unsigned int hours_required)
{
  this->hours_required = hours_required;
}

/************************************Employee*****************************************/

Employee::Employee(void)
{
}

Employee::Employee(string name, unsigned int hourly_wage, string task_desc,
                   unsigned int hours_required, bool completed)
{
  this->name = name;
  this->hourly_wage = hourly_wage;
  this->task = Task(task_desc, hours_required, completed);
}

unsigned int Employee::getHourly_wage(void) const
{
  return this->hourly_wage;
}

string Employee::getName(void) const
{
  return this->name;
}

Task &Employee::getTask(void)
{
  return this->task;
}

void Employee::setHourly_wage(unsigned int hourly_wage)
{
  this->hourly_wage = hourly_wage;
}

void Employee::setName(string name)
{
  this->name = name;
}

void Employee::setTask(Task task)
{
  this->task = task;
}

/***********************************Company******************************************/

Company::Company(string name, Employee *employees, unsigned int num_employees)
{
  this->name = name;
  this->employees = employees;
  this->num_employees = num_employees;
}

Company::Company(string name, unsigned int num_employees)
{
  this->name = name;
  this->employees = new Employee[num_employees];
  this->num_employees = num_employees;
}

Company::Company(const Company &other)
{
  this->name = other.name;
  this->employees = other.employees;
  this->num_employees = other.num_employees;
}

Company::~Company(void)
{
  delete[] this->employees;
}

string Company::getName(void) const
{
  return this->name;
}

void Company::setName(string name)
{
  this->name = name;
}

unsigned int Company::getNum_employees(void) const
{
  return this->num_employees;
}

void Company::setNum_employees(unsigned int num_employees)
{
  this->num_employees = num_employees;
}

Employee* Company::getEmployees(void) const
{
  return this->employees;
}

void Company::setEmployees(Employee *employees)
{
  this->employees = employees;
}

unsigned int Company::totalCosts(bool future)
{
  unsigned int idx = 0, total_costs = 0;

  if(future == true)
  {
    for(idx = 0; idx < this->num_employees; idx++)
    {
      if(this->employees[idx].getTask().getCompleted() == false)
      {
        total_costs = total_costs + 
                      (this->employees[idx].getHourly_wage() * 
                       this->employees[idx].getTask().getHours_required());
      }
    }
  }
  else 
  {
    for(idx = 0; idx < this->num_employees; idx++)
    {
      if(this->employees[idx].getTask().getCompleted() == true)
      {
        total_costs = total_costs +
                      (this->employees[idx].getHourly_wage() *
                       this->employees[idx].getTask().getHours_required());
      }
    }
  }
  return total_costs;
}

