#include <stdio.h> // for printf(), perror()
#include <stdlib.h> // for exit()
#include <unistd.h>  // fork(),pipe()
#include <string.h>
#include <errno.h>
#include <sys/types.h> // for waitpid()
#include <sys/wait.h>  // for waitpid()
#include <time.h>

#define client_number 10
#define movie_number 10

//to_server[0] = for read
//to_server[1] = for write
//to_client[0] = for read
//to_client[1] = for write


//Initialization Of Struct
typedef struct movies{
    int movie_id;
    int start;
    int end;
    int available_seats;
    int price;
}   movies_t;


int main(int argc,char * argv[]){
    
    srand(time(0)); // Initialization Of rand(),Use Current Time As Seed For rand

    //Initializing Attributes
    movies_t movies[movie_number];
    int to_server[client_number][2],to_client[client_number][2];
    int total_requests = 0 , total_successful_reservations = 0 , total_inclome = 0;
    int number_of_reservations[movie_number];
    int successful_reservations[movie_number];
    int unsuccessful_reservations[movie_number];

    for (int i = 0; i < movie_number; i++){
        number_of_reservations[i]=0;
        successful_reservations[i]=0;
        unsuccessful_reservations[i]=0;
    }


    //Initialization Of The 2 Sets Of Pipes
    for(int i = 0; i < client_number; i++){

        if(pipe(to_server[i]) < 0){     //Creates Unnamed Pipe
            printf("An error ocurred while trying to open the pipe\n");
            return 1;
        }

        if(pipe(to_client[i]) < 0){     //Creates Unnamed Pipe
            printf("An error ocurred while trying to open the pipe\n");
            return 2;
        }

    }


    int process,wait_process;  //Creates New Process

    for(int i = 0; i < client_number; i++){

        process = fork();   //Creates Fork

        //If Process < 0 Terminate Program
        if(process < 0){
            perror("fork() failed");
            exit(EXIT_FAILURE);
        }
        else if(process == 0){    //Child-Client Process

            int tickets = (rand() % 3 + 1);  // Random Selection For The Number Of Tickets From 1-3
            int reply;
            int movie_selection;

            write(to_server[i][1],&tickets,sizeof(int));    //Sends To Server How Many Tickets It Wants

            for(int j = 0; j < tickets; j++){   //Loop That Selects Movies For The Number Of Tickets

                movie_selection = (rand() % 10); //Random Selection Of A Movie

                write(to_server[i][1],&movie_selection,sizeof(int)); //Client Sends To Server The Movie

                read(to_client[i][0],&reply,sizeof(int)); // Server Tells Client If There's Any Available Seats

                if(reply == 1){         //If reply Equals To 1 Purchase Is Successfull
                    printf("Ticket Purchased Successfully!!\n");
                }
                else{
                    printf("Ticket Not Purchased Successfully!!\n");    //Else Purchase Not Successfull
                }
            }
            

            printf("Client Process Completed Successfully!!\n");
            i=client_number+1;  //Exit The Loop To Avoid Extra Forking

        }       // End Of Child-Client Process

    }       //End Of Loop

    if(process > 0){     //Father-Server Process

        int i;

        //Initializing Info For Movies
        //Movie 1
        movies[0].movie_id = 16578;
        movies[0].start = 6;
        movies[0].end = 8;
        movies[0].available_seats = (rand() % 5 + 1);
        movies[0].price = 8;

        //Movie 2
        movies[1].movie_id = 23759;
        movies[1].start = 10;
        movies[1].end = 12;
        movies[1].available_seats = (rand() % 5 + 1);
        movies[1].price = 8;

        //Movie 3
        movies[2].movie_id = 46883;
        movies[2].start = 6;
        movies[2].end = 8;
        movies[2].available_seats = (rand() % 5 + 1);
        movies[2].price = 6;

        //Movie 4
        movies[3].movie_id = 96437;
        movies[3].start = 7;
        movies[3].end = 9;
        movies[3].available_seats = (rand() % 5 + 1);
        movies[3].price = 8;

        //Movie 5
        movies[4].movie_id = 78953;
        movies[4].start = 6;
        movies[4].end = 9;
        movies[4].available_seats = (rand() % 5 + 1);
        movies[4].price = 8;

        //Movie 6
        movies[5].movie_id = 114789;
        movies[5].start = 6;
        movies[5].end = 8;
        movies[5].available_seats = (rand() % 5 + 1);
        movies[5].price = 5;

        //Movie 7
        movies[6].movie_id = 35842;
        movies[6].start = 9;
        movies[6].end = 11;
        movies[6].available_seats = (rand() % 5 + 1);
        movies[6].price = 8;

        //Movie 8
        movies[7].movie_id = 55690;
        movies[7].start = 10;
        movies[7].end = 12;
        movies[7].available_seats = (rand() % 5 + 1);
        movies[7].price = 10;

        //Movie 9
        movies[8].movie_id = 06546;
        movies[8].start = 5;
        movies[8].end = 7;
        movies[8].available_seats = (rand() % 5 + 1);
        movies[8].price = 7;

        //Movie 10
        movies[9].movie_id = 44651;
        movies[9].start = 8;
        movies[9].end = 10;
        movies[9].available_seats = (rand() % 5 + 1);
        movies[9].price = 9;

        for(i = 0; i < client_number; i++){

            int tickets_asked,reply,success;    //Initializing Attributes

            read(to_server[i][0],&tickets_asked,sizeof(int)); //Reads From Client The Number Of Tickets It Asked

            for(int j = 0; j < tickets_asked; j++){

                read(to_server[i][0],&reply,sizeof(int)); //Reads From Client The Movie Number In The Table movies

                //Increase Values
                total_requests++;       
                number_of_reservations[reply]++;  //Increase Number Of Attempted Reservations For The Movie That Is Selected And Shown In Variable reply [0-9]  
                

                if(movies[reply].available_seats == 0){         //If There Are No Available Seats Set success=0 , Increase Value Of unsuccessful_reservations For The Movie That Is Selected And Shown In Variable reply [0-9] 
                    printf("No Available Seats Found!!\n");
                    success=0;
                    unsuccessful_reservations[reply]++;
                }
                else{                                           //Else Set success=1 and Increase The Other Values From Below
                    success=1;
                    total_successful_reservations++;
                    total_inclome += movies[reply].price;
                    successful_reservations[reply]++;
                }

                usleep(500000); // Wait For Server To Initialize Its Data. 500000 Microseconds Is Equal To 0,5 Seconds

                write(to_client[i][1],&success,sizeof(int));    // Tell Client If The Reservation Is Successfull Or Not

            }

            wait_process=wait(NULL);      //Wait Until Child Process Finish
            close(to_server[i][1]);       //Closing File Writer
            close(to_client[i][0]);       //Closing File Reader
            close(to_server[i][0]);       //Closing File Reader
            close(to_client[i][1]);       //Closing File Writer


            //If Wait < 0 Terminate Program
            if(wait_process < 0){
                perror("An error occured.Exiting!!\n");
                exit(1);
            }  

        }
        
        printf("\n");
        printf("Server Process Completed Successfully!!\n");
        printf("\n");

        //Prints Information
        for(int i = 0; i < 10; i++){
            printf("----------------------------------\n");
            printf("Movie:%d\n",i+1);
            printf("Movie Starts At:%d\n",movies[i].start);
            printf("Movie Ends At:%d\n",movies[i].end);
            printf("Number of Request's For Reservation is:%d for Movie:%d\n",number_of_reservations[i],i+1);
            printf("Successful Request's Were:%d for Movie:%d\n",successful_reservations[i],i+1);
            printf("Unsuccessful Request's Were:%d for Movie:%d\n",unsuccessful_reservations[i],i+1);
        }    

        //Prints Information
        printf("------------------!!Results!!------------------\n");
        printf("Total Number of Request For Tickets:%d\n",total_requests);
        printf("Total Number of Successful Reservations:%d\n",total_successful_reservations);
        printf("Total Income For The Cinema:%d\n",total_inclome);
        printf("-----------------------------------------------\n");

    }   // End Of Father-Server Process
    return 0;
}       //End Of Main