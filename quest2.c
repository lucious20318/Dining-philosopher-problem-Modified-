
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

int state[5];
int philosopher_id[5] = {0,1,2,3,4};
int bowl_arr[4] = {0,0,0,0};
int value = 1;
int b_curr[4] = {0,0,0,0};
int pshared = 0;
int value_ = 1;

sem_t sem;
sem_t sem_arr[5];

int get_fork(int position, int c)
{
	int p = position;
	
	if(c==0)
	{
		return (p + 4) % 5;
	}
	
	if(c==1)
	{
		return (p + 1) % 5;
	}
	
}

int cur_pos(int val)
{
	for (int i=0; i<4;i++)
	{
		if (b_curr[i]==val)
		{
			int val_ = i+1;
			return val_;
		}
	}
}

int bowl(int position)
{
	int cur = position;
	
	for (int i=0;i<4;i++)
	{
		int flag = 0;
		
		if (bowl_arr[i]==0)
		{
			bowl_arr[i]=1;
			b_curr[i]=cur;
			flag = 1;
		}
		
		if(flag == 1)
		{
			return 1;
		}
	}
	
	return 0;
}

void eat(int position)
{
	int curr = position;
	
	if (state[curr] == 1 && bowl(curr) == 1) 
	{
		state[curr] = 0;
		sleep(3);
		
		printf("Philosopher %d takes bowl %d\n", curr+1, cur_pos(curr));
		printf("Philosopher %d is Eating\n", curr+1);
		sem_post(&sem_arr[curr]);
	}
}

void take_leave(int position)
{
	sem_wait(&sem);
	state[position] = 1;
	
	printf("Philosopher %d is hungry\n", position+1);
	
	eat(position);
	sem_post(&sem);
	sem_wait(&sem_arr[position]);
	sleep(1);
	
	
	sem_wait(&sem);
	state[position] = 2;
	int bowl_pos = cur_pos(position);
	sleep(1);
	printf("Philosopher %d stopped using bowl %d \n", position+1, bowl_pos);
	bowl_arr[bowl_pos -1] = 0;
	
	int l_fork = get_fork(position,0);
	int r_fork = get_fork(position,1);
	
	eat(l_fork);
	eat(r_fork);
	sem_post(&sem);
}


void* phil_id(void* val)
{
	while (1) 
	{
		sleep(1);
		int* val_ = val;
		take_leave(*val_);
	}
}

int main()
{
	int i;
	pthread_t process_[5];
	
	//pshared is 0 so that the semaphore cannot be shared between processes
	int sem_val = sem_init(&sem, pshared, value);
	if(sem_val == -1)
	{
			printf("Couldn't initialize semaphore");
			return 0;
	}
	
	else
	{
		for (i = 0; i < 5; i++)
		{
			sem_init(&sem_arr[i], 0, 0);
		}
	
		printf("All philosophers are thinking\n");
		
		for (i = 0; i < 5; i++) 
		{
			pthread_create(&process_[i], NULL, phil_id, &philosopher_id[i]);
			//pthread_join(process_[i], NULL);
		}
	
		for (i = 0; i < 5; i++)
		{
			pthread_join(process_[i], NULL);
		}	
	}
	return 0;
}

