/*
PLEASE, DO NOT CHANGE void display(bool verbose, bool testing), int getUsableDay() and int getUsableTime() FUNCTIONS.
YOU HAVE TO WRITE THE REQUIRED  FUNCTIONS THAT IS MENTIONED ABOVE. YOU CAN ADD NEW FUNCTIONS IF YOU NEED.
*/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>

#include "task_management_tool.h"

using namespace std; 


void WorkPlan::display(bool verbose,bool testing)
{
	string inone="***";
	if (head!=NULL)
	{
		Task *pivot =new Task;
		Task *compeer =new Task;
			
		pivot=head;
		do
		{
			if(testing)
				inone+=" ";
			else
				cout << pivot->day <<". DAY"<< endl;
			compeer=pivot;
			while(compeer!=NULL)
			{
				string PREV= compeer->previous!=NULL?compeer->previous->name:"NULL";
				string NEXT=compeer->next!=NULL?compeer->next->name:"NULL";
				string CONT=compeer->counterpart!=NULL?compeer->counterpart->name:"NULL";
				if (testing)
					inone+=compeer->name;
				else
					if(verbose)
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< PREV<<"\t<- "<< compeer->name <<"("<< compeer->priority <<")->\t"<<NEXT <<"\t |_"<<CONT<<endl;
					else
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< compeer->name <<"("<< compeer->priority <<")"<<endl;				
				compeer=compeer->counterpart;
			}
			pivot=pivot->next;
		}while(pivot!=head);
		if(testing)
		{
			cout<<inone<<endl;
			cout<<"(checking cycled list:";
			if (checkCycledList())
				cout<<" PASS)"<<endl;
			else
				cout<<" FAIL)"<<endl;
		}
	}
	else
		cout<<"There is no task yet!"<<endl;
}

int WorkPlan::getUsableDay()
{
	return usable_day;
}

int WorkPlan::getUsableTime()
{
	return usable_time;
}


void WorkPlan::create()
{		
	//THIS FUNCTION WILL BE CODED BY YOU
	head = NULL;

	usable_day = -1;
	usable_time = -1;
}

void WorkPlan::close()
{
	//THIS FUNCTION WILL BE CODED BY YOU
}

void WorkPlan::add(Task *task)
{
	//THIS FUNCTION WILL BE CODED BY YOU
	Task *newTask = new Task(); // create the task to be added
	newTask->name = new char [strlen(task->name)];
	strcpy(newTask->name, task->name);
	newTask->priority = task->priority;
	newTask->day = task->day;
	newTask->time = task->time;

	if (head == NULL) {
		head = newTask;
		head->next = head;
		head->previous = head;
		head->counterpart = NULL;
		return; // implement as first node
	} 
	
	Task *traverse = new Task();
	traverse = head;
	do {
		if (newTask->day == traverse->day) {
			Task *compeer = new Task();
			Task *tail = new Task();
			compeer = traverse;
			while (compeer != NULL) {
				if (newTask->time == compeer->time) {
					if (newTask->priority > compeer->priority) {
						if (compeer->next != NULL || compeer->previous != NULL) {
							if (compeer == head) {
								head = newTask;
								head->next = compeer->next;
								head->previous = compeer->previous;
								compeer->previous->next = head;
								compeer->next->previous = head;
								compeer->next = NULL;
								compeer->previous = NULL;
								head->counterpart = compeer;
								return; // add as the first task of the first day
							} else {
								newTask->next = compeer->next;
								newTask->previous = compeer->previous;
								compeer->previous->next = newTask;
								compeer->next->previous = newTask;
								compeer->next = NULL;
								compeer->previous = NULL;
								newTask->counterpart = compeer;
								return; // add before first task of a day
							}
						}
						// add task instead of compeer
						checkAvailableNextTimesFor(compeer);
						compeer->day = usable_day;
						compeer->time = usable_time;
						add(compeer);
						return;
					} else {
						// delay task to another time
						checkAvailableNextTimesFor(newTask);
						newTask->day = usable_day;
						newTask->time = usable_time;
						add(newTask);
						return;
					}
					return;
				} else if (newTask->time < compeer->time) {
					if (compeer->next != NULL || compeer->previous != NULL) {
						if (compeer == head) {
							head = newTask;
							head->next = compeer->next;
							head->previous = compeer->previous;
							compeer->previous->next = head;
							compeer->next->previous = head;
							compeer->next = NULL;
							compeer->previous = NULL;
							head->counterpart = compeer;
							return; // add as the first task of the first day
						} else {
							newTask->next = compeer->next;
							newTask->previous = compeer->previous;
							compeer->previous->next = newTask;
							compeer->next->previous = newTask;
							compeer->next = NULL;
							compeer->previous = NULL;
							newTask->counterpart = compeer;
							return; // add before first task of a day
						}
					}
					else {
						newTask->next = NULL;
						newTask->previous = NULL;
						tail->counterpart = newTask;
						newTask->counterpart = compeer;
						return; // add in between tasks of the day
					}
				}
				tail = compeer;
				compeer = compeer->counterpart;
			}
			newTask->next = NULL;
			newTask->previous = NULL;
			newTask->counterpart = NULL;
			tail->counterpart = newTask; // add as the last task of the day
			return; // add task to an existing day
		}
		else if (newTask->day < traverse->day) {
			(traverse->previous)->next = newTask;
			newTask->previous = traverse->previous;
			newTask->next = traverse;
			traverse->previous = newTask;
			newTask->counterpart = NULL;
			if (traverse == head) {
				head = newTask; // add as the head node
			}
			return; // add in between days
		}
		traverse = traverse->next;
	} while (traverse != head);

	(traverse->previous)->next = newTask;
	newTask->previous = traverse->previous;
	newTask->next = traverse;
	traverse->previous = newTask;
	newTask->counterpart = NULL;
	return;
	// add as tail
} 

Task * WorkPlan::getTask(int day, int time)
{
	//THIS FUNCTION WILL BE CODED BY YOU
	return NULL; // TO BE CODED LATER
}


void WorkPlan::checkAvailableNextTimesFor(Task *delayed)	
{	//THIS FUNCTION WILL BE CODED BY YOU
	Task* traverse = new Task;
	traverse = head;
	while (delayed->day != traverse->day) {
		traverse = traverse->next;
		if (traverse == head) break;
	}
	Task* compeer = new Task;
	compeer = traverse;
	while (delayed->time != compeer->time) {
		compeer = compeer->counterpart;
	}
	while (compeer->counterpart != NULL && compeer->time + 1 == compeer->counterpart->time) {
		compeer = compeer->counterpart;
	}
	if (compeer->time == 14) {
		do {
			compeer = traverse;
			while (compeer->counterpart != NULL) {
				if (compeer->time + 1 != compeer->counterpart->time) {
					usable_day = compeer->day;
					usable_time = compeer->time + 1;
					return;
				}
				compeer = compeer->counterpart;
			}
			traverse = traverse->next;
		} while (traverse != head);
	}
	else {
		usable_day = compeer->day;
		usable_time = compeer->time + 1;
		return;
	}
	usable_day = -1;
	usable_time = -1;
	return;
}

void WorkPlan::delayAllTasksOfDay(int day)
{
	//THIS FUNCTION WILL BE CODED BY YOU
}

void WorkPlan::remove(Task *target)
{
	//THIS FUNCTION WILL BE CODED BY YOU
}

bool WorkPlan::checkCycledList()
{				
	Task *pivot=new Task();
	pivot=head;
	int patient=100;
	bool r=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->previous;
		if(pivot==head)
		{
			r=true;
			break;
		}
	}
	cout<<"("<<100-patient<<")";
	patient=100;
	bool l=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->next;
		if(pivot==head)
		{
			l=true;
			break;
		}
	}
	return r&l;
}