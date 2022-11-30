#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 100
#define NUMITEMS 30

char message[BUFFER_SIZE] = "I'm Child Thread !!!";
int message_count = 0;

// 뮤텍스 생성 및 컨디션 생성
pthread_mutex_t insert_mutex;
pthread_mutex_t client_mutex;
pthread_mutex_t server_mutex;
pthread_cond_t client_broad;
pthread_cond_t server_sig;

receive_broadcast(int arg){
	pthread_cond_wait(&client_broad, &client_mutex);
	printf("THREAD %d 에게 전달한 메시지 : %s\n", arg,message);
}

void *server(void *arg) // 서버
{
	while (1) {
		pthread_mutex_lock(&server_mutex);  // 조건 변수를 이용한 뮤텍스 락을 건다.
		pthread_cond_wait(&server_sig, &server_mutex); // 조건변수 상에서 쓰레드를 대기
		pthread_cond_broadcast(&client_broad); // 조건변수 상에서 신호하여 대기하는 모든 쓰레드를 재실행
		message_count = 0;
		pthread_mutex_unlock(&server_mutex); // 락을 해제
	}
}

void *client(void *arg){ // 클라이언트
	int status;
	while (1) {

		if(message_count == 0){
			message_count = 1;
			sleep(1);
			printf("THREAD %d의 메시지 전송 요청 ", (int)arg);
			printf("\n");
			pthread_cond_signal(&server_sig); // 조건변수 상에서 신호하여 대기하는 쓰레드를 재실행
		}
		else
			receive_broadcast(arg);
			pthread_mutex_unlock(&client_mutex); // 락을 해제
	}
}

int main(int argc, char *argv[]){
	int status;
	int cycle;
	void *result;
	pthread_t server_tid, client_tid[8];
	pthread_mutex_init(&client_mutex, NULL); //뮤텍스를 동적으로 생성하고 초기화
	pthread_mutex_init(&server_mutex, NULL); 
	pthread_cond_init(&server_sig, NULL); //조건변수를 동적으로 생성하고 초기화
	pthread_cond_init(&client_broad, NULL);
	
	cycle = 5;
	for(int i = 0; i < cycle; i++){
		status = pthread_create (&client_tid, NULL, client, i+1);
		if (status != 0)
			perror ("Create server thread");
	}
	status = pthread_create (&server_tid, NULL, server, NULL);
	if (status != 0)
		perror ("Create server thread");
	status = pthread_join (server_tid, NULL);
	if (status != 0)
		perror ("Join server thread");
	for(int i = 0; i < cycle; i++){
		status = pthread_join (client_tid, NULL);
		if (status != 0)
			perror ("Join client thread");
	}
	return 0;
}
