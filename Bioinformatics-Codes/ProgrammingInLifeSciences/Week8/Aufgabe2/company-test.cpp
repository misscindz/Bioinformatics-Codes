#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "company.h"
using namespace std;

int main() {
  Employee *employee;
  Company  *company;

  Employee employee1("Smith", 15, "Task 1", 8, true);
  Employee employee2("Johnson", 14, "Task 2", 4, false);
  Employee employee3("Williams", 10, "Task 3", 5, false);
  Employee employee4;

  Task task;

  task.setDescription("Task 4");
  task.setHours_required(50);
  task.setCompleted(true);

  employee4.setName("Jones");
  employee4.setHourly_wage(12);
  employee4.setTask(task);

  employee = new Employee[4];

  employee[0] = employee1;
  employee[1] = employee2;
  employee[2] = employee3;
  employee[3] = employee4;

  company = new Company("ABC", employee, 4);

  company->getEmployees()[1].setHourly_wage(20);
  company->getEmployees()[2].getTask().setCompleted(true);

  assert(company->getName().compare("ABC") == 0);
  assert(company->getEmployees()[0].getName().compare("Smith") == 0);
  assert(company->getEmployees()[1].getTask().getDescription()
         .compare("Task 2") == 0);
  assert(company->getEmployees()[1].getHourly_wage() == 20);

  assert(company->totalCosts(false) == 770);
  assert(company->totalCosts(true) == 80);

  assert(employee[3].getTask().getDescription().compare("Copy Task 4") == 0);

  delete company;

  cout << "test finished\n";

  return EXIT_SUCCESS;
}
