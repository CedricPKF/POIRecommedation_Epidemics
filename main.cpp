#include <iostream>
#include <fstream>
#include<cstdlib>
#include <string>
#include<vector>
#include<bits/stdc++.h>

long double toRadians(const long double degree);
long double compare_distance(long double lat1, long double long1,long double lat2, long double long2);
using namespace std;

        struct myuser{
            string id;
            string name;
            float infectious,safe_constraint;
            long double lat , longi;
            }; 
        struct locationWithTime{
            string loc;
            vector<string>placed;
            string identity;
            }; 
        struct location{
            string location;
            int capacity;
            long double lat,longi;
            }; 
        struct friendship{
            string user;
            string u_friend;
            double strength;
            }; 
        struct infection_Probability{
            vector<string>infectios_prob;
        };


int main(){
    const int NUM_EMp_USERS =26;
    int users[NUM_EMp_USERS];
    int count=0;
    int location_count=0, totalIterationLocTime=0;
    int num_users;

    //initialize infection_Probability
    infection_Probability infectiousStruct;
//============================Load all locations from struct===================================//
    location Location[NUM_EMp_USERS];// struct location 
    ifstream inputLocation;
    inputLocation.open("location.txt");
    if(inputLocation.fail()){
        cout << "File failed to open Locations." << endl;
        return 1;
        }
        while (!inputLocation.eof()){
            inputLocation >> Location[location_count].location>> Location[location_count].lat >>Location[location_count].longi >> Location[location_count].capacity;
            location_count++;
        }
        //Display Locations
        cout << "Location Details are: "<<location_count<< endl;
        for(int i = 0; i < location_count; i++){
             cout<<setprecision(20)<<fixed;
            cout << Location[i].location<<", "<< Location[i].lat<<", "<< Location[i].longi<<", "<< Location[i].capacity<< endl;
        }

//============================Append time to Location===================================//
        locationWithTime addLocation_time[NUM_EMp_USERS];
        int loct=0; //initalizes looping over location structure
        while(totalIterationLocTime<=24-3){// check on why we had to subtract 3(like even_Numbers needs subtraction of three)
            for(int time=1; time<=3;++time){
                addLocation_time[totalIterationLocTime] = {Location[loct].location+"_time"+to_string(time),{},Location[loct].location};
                //addLocation_time[totalIterationLocTime].placed.push_back(to_string(time));
                totalIterationLocTime++;
            }
            loct++;
        }
        //Display Location_With time
        cout << " \nShow Location with time :================================ "<<totalIterationLocTime<< endl;
        for(int i = 0; i < totalIterationLocTime; i++){
            cout << addLocation_time[i].loc<<", ";
            for (auto j = addLocation_time[i].placed.begin(); j != addLocation_time[i].placed.end(); ++j){
                cout << *j<<", ";
            }
            cout << addLocation_time[i].identity<< endl;
        } 
    
  //============================Append all users to struct from file===================================//
    myuser userStruct[NUM_EMp_USERS];  //initalize a structure 
    ifstream inputFIle;
    inputFIle.open("users.txt");
    if(inputFIle.fail()){
        cout << "File failed to open." << endl;
        return 1;
    }
    //cout<<"am in ";
    while (!inputFIle.eof()){
        inputFIle >> userStruct[count].id >> userStruct[count].name >> userStruct[count].lat >> userStruct[count].longi >>userStruct[count].infectious>> userStruct[count].safe_constraint;
         //inputFIle >> userStruct[count].name;
        count++;
    }
    num_users = count;
    //Display all users
    cout << "\nThe number of all users are: " << num_users << endl;
    for(int i = 0; i < num_users; i++){
        cout << userStruct[i].id<<", "<< userStruct[i].name.c_str()<<", "<< userStruct[i].lat<<", "<< userStruct[i].longi<<", "<< userStruct[i].infectious<<", "<< userStruct[i].safe_constraint<<endl;
    }

    //============================Append all Friendships to struct===================================//
    friendship friends[50];  //initalize a structure 
    ifstream friendFIle;
    friendFIle.open("friendship.txt");
    int friend_count=0;
    if(friendFIle.fail()){
        cout << "File failed to open Friends List." << endl;
        return 1;
    }
    while (!friendFIle.eof()){
        friendFIle >> friends[friend_count].user >> friends[friend_count].u_friend >> friends[friend_count].strength;
         //inputFIle >> userStruct[count].name;
        friend_count++;
    }
    //Display all frindship
    cout << "\nList of frienship and their strngth: "<< endl;
    for(int i = 0; i < friend_count; i++){
        cout << friends[i].user<<", "<< friends[i].u_friend<<", "<< friends[i].strength<< endl;
    }

    //loop over all the users as we allocate them. THE MAIN ALGORITHM
    //cout<<"Total users is: "<<num_users<<endl;
  
    for(int u=0;u<num_users;u++){//num_users
        //looping over locations
        vector<double>grading;
        vector<string>probability;
        for(int l=0;l<totalIterationLocTime;l++){ //totalIterationLocTime
            double strength=0;
            bool loop_controller=true;
            float probability_sum=0;
            int loop_controller_guide=0;
            long double lat1, long1 ,lat2 ,long2;
            for(int loc=0;loc<=location_count;loc++){
                if(addLocation_time[l].identity==Location[loc].location){
                     lat1 = Location[loc].lat;
                     long1 = Location[loc].longi;
                    break;
                }
            }
            //get the coordiantes of the user.
             lat2 = userStruct[u].lat;
             long2 = userStruct[u].longi;
            //compare the distance
            long double distance = compare_distance(lat1,long1,lat2,long2);
           // cout<<"  Distance: "<<distance<<"KM"<<endl;
          
            if(distance>1){//skip the location because it is too far away
                //cout<<"Beyond: "<<endl;
                loop_controller=false;
                loop_controller_guide=-1;
            }
              

            if(loop_controller){
                int occupied_size=0;
                for (auto m = addLocation_time[l].placed.begin(); m != addLocation_time[l].placed.end(); m++){
                    if(*m != ""){
                        occupied_size++;//size occupied by each location 
                        //cout<<"occupied_size: "<<occupied_size<<endl;
                    }
                }
                //Loop over locations to get max population allowed for each location_time
                int maxSize=0;
                for(int i=0;i<location_count;i++){
                    if(addLocation_time[l].identity == Location[i].location){
                        maxSize=Location[i].capacity;
                    }
                }
                //cout <<"Current user: "<<userStruct[u].id;
                //cout <<"  Current Location: "<<addLocation_time[l].loc<<endl;
                if(occupied_size>=maxSize){
                    loop_controller=false;
                    loop_controller_guide=-1;
                    //cout<<"user id++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++: "<<userStruct[u].id<<" location: "<<addLocation_time[l].loc<<endl;
                }
                for (auto j = addLocation_time[l].placed.begin(); j != addLocation_time[l].placed.end(); j++){
                //compare the user id with the location present in the location_time and  check if its full
                if((userStruct[u].id==*j)){
                    //after finding the match or location capacity is full we can break the loop and go to the next location_time
                    loop_controller=false;
                    loop_controller_guide-1;
                    break;
                } 
                
                }
            }//end of loop controller I

                if(loop_controller){
                    //cout<<"continue ************************************"<<addLocation_time[l].loc<<endl;
                 for(int i=0;i<friend_count;i++){
                     if(userStruct[u].id==friends[i].user){
                        //loop over all the mebers of the location_time and check if the user is in the location_time
                        //loop over structure placed
                         for (auto loop_arr = addLocation_time[l].placed.begin(); loop_arr != addLocation_time[l].placed.end(); ++loop_arr){
                            //cout<<"Value in array:"<<*loop_arr<<", Value of friend: "<<friends[i].u_friend<<endl;
                             if(friends[i].u_friend==*loop_arr){
                                strength=strength+friends[i].strength;
                                // cout<<"Strength Added: "<<friends[i].strength<<endl;
                                loop_controller_guide=strength;
                            } 
                        } 
                        
                    }
                } 

                //===========================================================================================================
                //Function to calculate the probability of getting sick
                //===========================================================================================================
                // the probability of being sick at locaation_time
                
                double prob_at_ti=1;
                for (auto loop_arr = addLocation_time[l].placed.begin(); loop_arr != addLocation_time[l].placed.end(); ++loop_arr){
                    //loop over user and sum the probability of being sick
                    double prob_at_t_1=0;
                    for(int i=0;i<friend_count;i++){
                        if(friends[i].u_friend==*loop_arr){ //friendship 
                            //userStruct[u].infectious; //probability at time t-1
                            // friends[i].strength;//get friendship relation between u and *loop_arr
                            prob_at_t_1=userStruct[u].infectious*friends[i].strength;
                        }
                    }
                    prob_at_ti=(1-prob_at_t_1)*prob_at_ti;
                    //  cout<<"The prob  *****************************************************************************************: "<<prob_at_ti<<endl;
                }

                //cout<<"The prob at t1 *****************************************************************************************: "<<prob_at_ti<<endl;
                probability_sum=(1-prob_at_ti); //get the average probability of being sick
                
               // check if the average probability is greater than personal safety constraint
                if(probability_sum>=userStruct[u].safe_constraint){
                    loop_controller=false;
                    loop_controller_guide=-1;// do not ave but skip to the next location 
                }else{
                    // cout<<"The prob  *****************************************************************************************: "<<endl;
                    probability.push_back(to_string(probability_sum));
                    probability.push_back(addLocation_time[l].loc);
                }
                
                }// end of loop controller II
                    grading.push_back(loop_controller_guide);
                    //cout<<"going Out";
                    
            }//end of looping over locations
           // find max value from grading vector 
            int max = *max_element(grading.begin(),grading.end());
            //get index of max-value in struct placed
            int index_max=distance(grading.begin(), find(grading.begin(),grading.end(),max));
            addLocation_time[index_max].placed.push_back(to_string(u+1));
            //storing to the infectios_prob loop
            string prob_vec="";
                for(int prob=0;prob < probability.size();prob++){
                //cout <<"********************************"<< prob << " "<<endl;
                    if(addLocation_time[index_max].loc==probability[prob+1]){
                        prob_vec=probability[prob];
                        //cout<<"Probability: "<<prob_vec<<endl;
                    } 
                    prob++;
                } 
            //store prob in infectios_prob
            infectiousStruct.infectios_prob.push_back(prob_vec);
        }// end of looping over users 

        //Display all infectios_prob
         /* for(int i=0; i<=num_users;i++){
         cout<<"The list of probs  "<<infectiousStruct.infectios_prob[i]<<endl;
        }  */
        //Display all
         cout << " \n\n******Final ALlocation output********** "<<location_count<< endl;
        //nest loop to display all the allocations
        for(int i=0; i<=totalIterationLocTime;i++){
            cout<<addLocation_time[i].loc<<", ";
            for(int iy=0; iy < addLocation_time[i].placed.size(); iy++){
                cout<<addLocation_time[i].placed[iy]<<":";
                    for(int p=0; p<=num_users;p++){
                        if(addLocation_time[i].placed[iy]==userStruct[p].id){
                            
                        cout<<"{"<<infectiousStruct.infectios_prob[p]<<"}"<<", ";
                            break;
                        }
                    } 
                }
            cout<< endl;
        } 
        
        FILE *fp;
        fp = fopen("Output.txt", "w");
        if (fp == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }
        /* fprintf(fp, "=================================================================");
        fprintf(fp, "\n");
        fprintf(fp, "*******************Time Complexity****************\n"); */
        fprintf(fp, "=================================================================");
        fprintf(fp, "\n");
        fprintf(fp, "******Final ALlocation output********** %d \n",totalIterationLocTime);
        fprintf(fp, "=================================================================\n");
        for(int i=0; i<=totalIterationLocTime;i++){
            fprintf(fp, "%s\n", addLocation_time[i].loc.c_str());
            fprintf(fp, "{ ");
            for(int iy=0; iy < addLocation_time[i].placed.size(); iy++){
                
                fprintf(fp, "%s", addLocation_time[i].placed[iy].c_str());
                fprintf(fp, ":");
                for(int p=0; p<=num_users;p++){
                        if(addLocation_time[i].placed[iy]==userStruct[p].id){
                            
                        //cout<<"{"<<infectiousStruct.infectios_prob[p]<<"}"<<", ";
                        
                        fprintf(fp, "%s", infectiousStruct.infectios_prob[p].c_str());
                        fprintf(fp, ",  ");
                            break;
                        }
                        
                    } 
            }
            fprintf(fp, " }\n");
        }
        fclose(fp);
        //end of append structure to the file
        //cout<<"End of the program"<<endl;




return 0; 
}



long double toRadians(const long double degree){
	long double one_deg = (M_PI) / 180;
	return (one_deg * degree);
}

long double compare_distance(long double lat1, long double long1,long double lat2, long double long2){
	lat1 = toRadians(lat1);
	long1 = toRadians(long1);
	lat2 = toRadians(lat2);
	long2 = toRadians(long2);

	long double dlong = long2 - long1;
	long double dlat = lat2 - lat1;
	long double ans = pow(sin(dlat / 2), 2) +cos(lat1) * cos(lat2) *pow(sin(dlong / 2), 2);

	ans = 2 * asin(sqrt(ans));
	long double R = 6371;
	ans = ans * R;
	return ans;
}
