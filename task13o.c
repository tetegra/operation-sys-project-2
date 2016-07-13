#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>
struct student{

	int id;
	int numQuestions; //number of questions to ask
	int questionNum; 	//number of questions that have been answered

};

int totalStudents;
int inOffice, studentZ;  //in office - students in office; studentZ keeps track of students enter and leave
sem_t readyForQuestion, questionWait, answerWait, capacity;
//pthread_cond_t stud, prof;
pthread_t professor;
pthread_mutex_t question_lock; /*lock, lock2,*/ 
//int professorWait = 0, studentWait;
int identification;


void Professor();
void *StartProfessor();
void AnswerStart();
void AnswerDone();
void Student(int id);
void * StartStudent(void * student);
void QuestionStart();
void QuestionDone();
void nap();
void EnterOffice();
void LeaveOffice();


void Professor()
{
	//pthread_mutex_init(&lock, NULL);
	//pthread_mutex_init(&lock2, NULL);
	pthread_mutex_init(&question_lock, NULL);
	
	
	//initialize semaphores
	//second parameter signifies whether it is shared(1) or not(0)
	sem_init(&readyForQuestion,1,0);
	sem_init(&questionWait,1,0);
	sem_init(&answerWait,1,0);
	sem_init(&capacity,0,inOffice);//not using yet
	
//	pthread_cond_init(&prof, NULL);
//	pthread_cond_init(&stud, NULL);
		
	studentZ = 0;

	//pthread_mutex_lock(&lock);
	//pthread_mutex_lock(&lock2);
	
	pthread_create(&professor, NULL, StartProfessor, NULL);
}

//does professor things
void * StartProfessor()
{
	while(1)
	{
		//if no students nap
		//if(inOffice == 0) //totalStudents
		//{
			//nap();
		//}	
		
		//pthread_mutex_lock(&lock);
		
		//if(inOffice > 0)
		//{
		
			sem_post(&readyForQuestion);
			//sem wait for question
			sem_wait(&questionWait);
			
			AnswerStart();//answer start
			AnswerDone();//answer done
			
			sem_post(&answerWait);
			//pthread_mutex_unlock(&lock2);
		//}
	}
}
void AnswerStart()
{
	//struct student * std = student;
	printf("Professor starts to answer question for student %d\n", identification);
}

void AnswerDone()
{
	printf("Professor is done with answer for student %d\n", identification);
}

//makes a thread for 1 student
void Student(int id)
{
//printf("you no make student");
	struct student * newStd = malloc(sizeof(struct student));
	
	newStd->id = id;
	newStd->numQuestions = (id % 4) + 1;
	newStd->questionNum = 0;
	//EnterOffice();
	pthread_t stack;
	pthread_create(&stack, NULL, (void *) StartStudent, (void *) newStd);

}

//work for student thread
void *StartStudent(void * student)
{
	
	struct student * std = student;
	studentZ++;
	
	while(std->numQuestions > std->questionNum)
	{
			sem_wait(&readyForQuestion);
			pthread_mutex_lock(&question_lock);
			identification = std->id;
			//sem_wait(&readyForQuestion);
			QuestionStart();
			
			//semaphore answer question (post)
			sem_post(&questionWait);
			
			//sem wait for answer
			sem_wait(&answerWait);
			
			//pthread_mutex_unlock(&lock);  //professor starts answering
			//pthread_mutex_lock(&lock2);	//professor done answering
			
			QuestionDone();
			
			pthread_mutex_unlock(&question_lock);
			
			std->questionNum++;
			//printf("%d %d %d\n ", std->questionNum,std->numQuestions, inOffice);
			if(std->questionNum == std->numQuestions)
			{
				LeaveOffice();
				studentZ--;
				
				if(studentZ == 0) //this is new here
				nap();
			}
	}
}

void QuestionStart()
{
	printf("Student %d asks a question\n", identification);
}

void QuestionDone()
{
	printf("Student %d is satisfied\n", identification);
}

void nap()
{
	printf("professor is napping...\n");
}

void EnterOffice()
{
	printf("student %d shows up in the office\n", identification);
}

void LeaveOffice()
{
	printf("Student %d leaves the office\n", identification);
}

int main(int argc, char *argv[])
{
	
	totalStudents = atoi(argv[1]);
	inOffice = atoi(argv[2]);
	int i;
	
	Professor();
	
	for(i = 0 ; i < totalStudents; i++)
	{
		Student(i);	
	}
	
	pthread_exit(NULL);
}
