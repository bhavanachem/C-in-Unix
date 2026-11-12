#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include <assert.h>
// structures
typedef struct StudentDetails{
 
int studentid;
 
int priority;
 
struct StudentDetails *next;
}StudentDetails;
typedef struct StudentQueue{
 
StudentDetails *Head;
 
StudentDetails *Tail;
 
int count;
}StudentQueue;
// global variables
int helps; 
// variable to track number of requests sent
int maxhelps; 
// max possible help requests
int helpcount; 
// max number of helps per student (also used for priority)
sem_t *tutor_to_students;
sem_t *endsession; 
// semaphore for signalling students to end session
sem_t *startsession; 
// semaphore for signalling students to start session
int *student_tutor_map; 
// mapping between tutorid and studentid for each session
bool coordinatoractive;
int chaircount; 
// number of chairs in the csmc waiting area
// shared variables
int num_availablechairs;
pthread_mutex_t chair_lock;
sem_t student_to_coordinator;
sem_t coordinator_to_tutor;
sem_t students_exited;
StudentQueue studentqueue; 
// FIFO queue handled by student and coordinator
pthread_mutex_t studentqueue_lock;
StudentQueue *tutoringqueue; 
// Multi level FIFO queue handled by coordinator and Tutors
pthread_mutex_t tutoringqueue_lock;
int tutorsactive;
int totalstudentshelped;
pthread_mutex_t tutorstat_lock;
// Helper functions which don't need to be modified
void addToStudentQueue(int studentid, int priority)
{
 
StudentDetails *details = malloc(sizeof(StudentDetails));
 
details -> studentid = studentid;
 
details -> priority = priority;
 
details -> next = NULL;
 
if(studentqueue.Head == NULL && studentqueue.Tail == NULL) //then queue is empty
 
{
 
studentqueue.Head = details;
 
studentqueue.Tail = details;
 
}
 
else if(studentqueue.Tail != NULL) //add to end of the queue
 
{
 
studentqueue.Tail->next = details;
 
studentqueue.Tail = details;
 
}
 
studentqueue.count++;
}
StudentDetails* removeFromStudentQueue()
{
 
if(studentqueue.Head == NULL) // No student in the queue
 
return NULL;
 
StudentDetails *studentrecord = studentqueue.Head;
 
studentqueue.Head = studentqueue.Head->next;
 
studentrecord -> next = NULL;

 
if(studentqueue.Head == NULL) // set tail to NULL when queue is empty
 
studentqueue.Tail = NULL;
 
studentqueue.count--;
 
return studentrecord;
}
void addToTutoringQueue(StudentDetails *details)
{
 
int priority = details -> priority;
 
StudentQueue *priorityqueue = &tutoringqueue[priority];
 
if(priorityqueue->Head == NULL && priorityqueue->Tail == NULL) //then queue is empty
 
{
 
priorityqueue->Head = details;
 
priorityqueue->Tail = details;
 
}
 
else if(priorityqueue->Tail != NULL) //add to end of the queue
 
{
 
priorityqueue->Tail->next = details;
 
priorityqueue->Tail = details;
 
}
 
priorityqueue->count++;
}
StudentDetails* removeFromTutoringQueue() //will always remove student with max priority
{
 
StudentQueue *priorityqueue = NULL;
 
int priority;
 
for(priority = helpcount; priority > 0; priority--)
 
{
 
priorityqueue = &tutoringqueue[priority];
 
if(priorityqueue->count > 0)
 
break;
 
}
 
 
assert(priorityqueue != NULL);
 
assert(priorityqueue->Head != NULL);
 
StudentDetails *studentrecord = priorityqueue->Head;
 
priorityqueue->Head = priorityqueue->Head->next;
 
studentrecord -> next = NULL;
 
if(priorityqueue->Head == NULL) // set tail to NULL when queue is empty
 
priorityqueue->Tail = NULL;
 
priorityqueue->count--;
 
return studentrecord;
}
int getTutoringQueueWaitCount()
{
 
StudentQueue *priorityqueue = NULL;
 
int priority, waitcount = 0;
 
for(priority = helpcount; priority > 0; priority--)
 
{
 
priorityqueue = &tutoringqueue[priority];
 
waitcount += priorityqueue->count;
 
}
 
return waitcount;
}
void tutoring()
{
 
usleep(200); // will sleep for 0.2 ms
}
void doprogramming()
{
 
usleep(2000); // will sleep for 2 ms
}
// Three Implemetation functions which lack all sem_wait and sem_posts, as well as queue references and locks.
// Missing code is labeled "<REDACTED>", while edited comments are labeled "<omitted>"
void *student_thread(void *arg)
{
 
int studentid = *(int*)arg;
 
int remaininghelps = helpcount;
 
free(arg);

 
while (remaininghelps > 0)
 
{
 
bool chairfound = false;
 
while (!chairfound)
 
{
 
pthread_mutex_lock(&chair_lock);
 
if (num_availablechairs > 0)
 
{
 
num_availablechairs--;
 
printf("S: Student %d takes a seat. Empty chairs = %d\n", studentid, num_availablechairs);
 
fflush(stdout);
 
chairfound = true;
 
pthread_mutex_unlock(&chair_lock);
 
pthread_mutex_lock(&studentqueue_lock);
 
addToStudentQueue(studentid, remaininghelps);
 
sem_post(&student_to_coordinator);
 
pthread_mutex_unlock(&studentqueue_lock);
 
}
 
else
 
{
 
printf("S: Student %d found no empty chair. Will try again later.\n", studentid);
 
fflush(stdout);
 
pthread_mutex_unlock(&chair_lock);
 
doprogramming();
 
}
 
}
 
sem_wait(&tutor_to_students[studentid - 1]);
 
pthread_mutex_lock(&chair_lock);
 
num_availablechairs++;
 
pthread_mutex_unlock(&chair_lock);
 
// 
printf("S: Student %d is waiting for STARTsession\n", studentid);
 
// 
fflush(stdout);
 
sem_wait(&startsession[studentid - 1]);
 
tutoring();
 
sem_wait(&endsession[studentid - 1]);
 
pthread_mutex_lock(&tutorstat_lock);
 
printf("S: Student %d received help from Tutor %d\n", studentid, student_tutor_map[studentid - 1]);
 
student_tutor_map[studentid - 1] = 0;
 
pthread_mutex_unlock(&tutorstat_lock);
 
remaininghelps--;
 
doprogramming();
 
}
 
return NULL;
}
void *tutor_thread(void *arg)
{
 
int tutorid = *(int*)arg;
 
free(arg);
 // fprintf(stderr, "[Tutor %d] Thread started\n", tutorid);
 // fflush(stderr);
 
while (true)
 
{
 
// fprintf(stderr, "[Tutor %d] Waiting on coordinator_to_tutor\n", tutorid);
 
// fflush(stderr);
 
sem_wait(&coordinator_to_tutor);
 // fprintf(stderr, "[Tutor %d] WOKE UP from sem_wait\n", tutorid);
 
// fflush(stderr);
 
if (!coordinatoractive)
 
{
 
// fprintf(stderr, "[Tutor %d] Exiting - coordinator inactive\n", tutorid);
 
// fflush(stderr);
 
break;
 
}
 
// Get student
 
StudentDetails *student;
 
pthread_mutex_lock(&tutoringqueue_lock);
 
student = removeFromTutoringQueue();
 
pthread_mutex_unlock(&tutoringqueue_lock);
 
assert(student!=NULL);

 
// Notify student to enter tutoring area
 
sem_post(&tutor_to_students[student->studentid - 1]);
 
usleep(1000); // small sync delay
 
//fprintf(stderr, "[Tutor %d] Posted tutor_to_students for STUDENT %d\n", tutorid, student->studentid);
 
//fflush(stderr);
 
 
// Update stats
 
pthread_mutex_lock(&tutorstat_lock);
 
tutorsactive++;
 
student_tutor_map[student->studentid - 1] = tutorid;
 
pthread_mutex_unlock(&tutorstat_lock);
 
// Start session
 
sem_post(&startsession[student->studentid - 1]);
 
// fprintf(stderr, "[Tutor %d] Posted startsession for STUDENTT %d\n", tutorid, student->studentid);
 
// fflush(stderr);
 
tutoring();
 
// End session
 
sem_post(&endsession[student->studentid - 1]);
 
// fprintf(stderr, "[Tutor %d] Posted endsession for Student %d\n", tutorid, student->studentid);
 
// fflush(stderr);
 
// Final stats
 
pthread_mutex_lock(&tutorstat_lock);
 
tutorsactive--;
 
totalstudentshelped++;
 
pthread_mutex_unlock(&tutorstat_lock);
 
fprintf(stderr, "T: Student %d tutored by Tutor %d. Students tutored now = %d. Total sessions tutored = %d\n",
 
student->studentid, tutorid, tutorsactive, totalstudentshelped);
 
fflush(stderr);
 
free(student);
 
}
 
return NULL;
}
void *coordinator_thread(void *arg)
{
 
coordinatoractive = true;
 
int tutorcount = *(int*)arg;
 
helps = 0;
 
while(helps < maxhelps)
 
{
 
// wait for student to notify(student adds itself to the <omitted> queue),
 
sem_wait(&student_to_coordinator);
 
StudentDetails *student = NULL;
 
while (student == NULL)
 
{
 
pthread_mutex_lock(&studentqueue_lock);
 
if (studentqueue.count > 0) {
 
student = removeFromStudentQueue();
 
}
 
pthread_mutex_unlock(&studentqueue_lock);
 
if (student == NULL) {
 
usleep(10); // give student threads time to enqueue
 
}
 
}
 
 
assert(student!=NULL);
 
 
helps++;
 
// queue the student to tutors based on student priority
 
pthread_mutex_lock(&tutoringqueue_lock);
 
addToTutoringQueue(student); 
//this uses student->priority internally
 
pthread_mutex_unlock(&tutoringqueue_lock);
 
int waitingcount = getTutoringQueueWaitCount();
 
sem_post(&coordinator_to_tutor);
 
//fprintf(stderr, "[Coordinator] Posted to coordinator_to_tutor\n");
 
// fflush(stderr);
 
assert(waitingcount <= chaircount);
 
printf("C: Student %d with priority %d added to the queue. Waiting students now = %d. Total requests = %d.\n", student->studentid, student->priority, waitingcount, helps); 

 
fflush(stdout);
 
// notify <omitted>
 
// <REDACTED>
 
}
 
 
// wait for all students to finish. signal sent from main thread
 
sem_wait(&students_exited);
 
coordinatoractive = false;
 
// post to all <omitted> threads so that they can exit
 
int i;
 
for(i = 0; i < tutorcount; i++)
 
{
 
sem_post(&coordinator_to_tutor);
 
}
 
return NULL;
}
// main does not need modifications
int main(int argc, char **argv)
{
 
 
if(argc != 5)
 
{
 
exit(0);
 
}
 
int studentscount = strtol(argv[1], NULL, 10);
 
int tutorscount = strtol(argv[2], NULL, 10);
 
chaircount = num_availablechairs = strtol(argv[3], NULL, 10);
 
helpcount = strtol(argv[4], NULL, 10); //this will set the priority for each student
 
maxhelps = helpcount * studentscount;
 
pthread_mutex_init(&chair_lock, NULL);
 
pthread_mutex_init(&tutorstat_lock, NULL);
 
pthread_mutex_init(&tutoringqueue_lock, NULL);
 
pthread_mutex_init(&studentqueue_lock, NULL);
 
sem_init(&student_to_coordinator, 0, 0);
 
sem_init(&coordinator_to_tutor, 0, 0);
 
sem_init(&students_exited, 0, 0);
 
int i;
 
tutor_to_students = malloc(sizeof(sem_t) * studentscount); //a semaphore for each student
 
startsession = malloc(sizeof(sem_t) * studentscount);
 
endsession = malloc(sizeof(sem_t) * studentscount);
 
tutoringqueue = calloc(helpcount+1, sizeof(StudentQueue));
 
student_tutor_map = malloc(sizeof(int) * studentscount);
 
for(i = 0; i < studentscount; i++)
 
{
 
sem_init(&tutor_to_students[i], 0, 0);
 
sem_init(&startsession[i], 0, 0);
 
sem_init(&endsession[i], 0, 0);
 
}
 
 
pthread_t tutorstids[tutorscount];
 
for(i = 0; i < tutorscount; i++)
 
{
 
int *tutorid = malloc(sizeof(int));
 
*tutorid = i+1;
 
assert(pthread_create(&tutorstids[i], NULL, tutor_thread, tutorid) == 0);
 
}
 
usleep(10000);
 
 
pthread_t coordinatortid;
 
assert(pthread_create(&coordinatortid, NULL, coordinator_thread, &tutorscount) == 0);
 
 
pthread_t studenttids[studentscount];
 
for(i = 0; i < studentscount; i++)
 
{
 
int *studentid = malloc(sizeof(int)); //dynamic allocation avoids the use of locks
 
*studentid = i+1;
 
assert(pthread_create(&studenttids[i], NULL, student_thread, studentid) == 0);
 
}
 
// wait for all student threads to finish
 
for(i = 0; i < studentscount; i++)
 
{
 
pthread_join(studenttids[i], NULL);
 
}
 
// notify the coordinator to start exiting threads

 
sem_post(&students_exited);
 
// wait for all tutors threads to finish
 
for(i = 0; i < tutorscount; i++)
 
{
 
pthread_join(tutorstids[i], NULL);
 
}
 
// wait for coordinator thread to finish
 
pthread_join(coordinatortid, NULL);
 
printf(" ");
 
fflush(stdout);
 
 
return EXIT_SUCCESS;
}

