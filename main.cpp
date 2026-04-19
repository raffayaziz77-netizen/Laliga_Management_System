#include <iostream>
#include <conio.h>
#include <cstdlib>
using namespace std;

int main()
{
    // Team data
    string team_names[20] = {"Barca", "Real Madrid", "Atletico Madrid", "Valencia", "Girona"};
    int points[20] = {0, 0, 0, 0, 0};
    int matches_played[20] = {0, 0, 0, 0, 0};
    int wins[20] = {0, 0, 0, 0, 0};
    int losses[20] = {0, 0, 0, 0, 0};
    int draws[20] = {0, 0, 0, 0, 0};
    int goals_for[20] = {0, 0, 0, 0, 0};
    int goals_against[20] = {0, 0, 0, 0, 0};
    int total_teams = 5;

    // Matches data
    int match_team1[200];
    int match_team2[200];
    int match_day[200];
    bool match_played[200] = {false};
    int goals1[200];
    int goals2[200];
    int total_matches = 0;
    int current_matchday = 0;
    bool first_round_completed = false;
    int order[20];
    bool order_initialized = false;

    // tracks history
    string history[10];
    int history_count = 0;

    int login_choice = 0;

    while (login_choice != 3)
    {
        system("cls");

        cout << R"(
  _               _      _____ _____          
 | |        /\   | |    |_   _/ ____|   /\    
 | |       /  \  | |      | || |  __   /  \   
 | |      / /\ \ | |      | || | |_ | / /\ \  
 | |____ / ____ \| |____ _| || |__| |/ ____ \ 
 |______/_/    \_\______|_____\_____/_/    \_\
        )";

        cout << "\nWelcome to LALIGA MANAGEMENT SYSTEM.\n";
        cout << "1.Admin Login\n";
        cout << "2.User Menu\n";
        cout << "3.Exit\n";
        cout << "Enter choice: ";
        cin >> login_choice;

        if (login_choice == 1)
        {
            bool already_login = false;
            bool login_successful = false;
            int login_tries = 0;
            string admin_username;
            int admin_password;

            while (login_tries != 3 && !already_login)
            {
                cout << "Enter user name: ";
                cin >> admin_username;
                cout << "Enter password: ";
                cin >> admin_password;

                if (admin_username == "admin" && admin_password == 1234)
                {
                    already_login = true;
                    login_successful = true;

                    int admin_choice = 0;

                    while (admin_choice != 6)
                    {
                        system("cls");

                        cout << endl;
                        cout << R"(1. Add Team
2. View Teams
3. Schedule Matchday
4. Simulate Matches
5. View Match History
6. Back)";

                        cout << "\nEnter choice: ";
                        cin >> admin_choice;

                        if (admin_choice == 1)
                        {
                            if (total_teams < 20)
                            {
                                cout << "\nEnter team name to add: ";
                                cin.ignore();
                                getline(cin, team_names[total_teams]);

                                points[total_teams] = 0;
                                matches_played[total_teams] = 0;
                                wins[total_teams] = 0;
                                losses[total_teams] = 0;
                                draws[total_teams] = 0;
                                goals_for[total_teams] = 0;
                                goals_against[total_teams] = 0;

                                total_teams++;
                                cout << "\nTeam added successfully.";
                            }
                            else
                            {
                                cout << "\nTeams limit reached.";
                            }
                        }
                        else if (admin_choice == 2)
                        {
                            if (total_teams == 0)
                            {
                                cout << "\nNo teams added yet";
                            }
                            else
                            {
                                system("cls");
                                for (int i = 0; i < total_teams; i++)
                                {
                                    cout << i + 1 << ". " << team_names[i] << endl;
                                }
                            }
                        }
                        else if (admin_choice == 3)
                        {
                            bool all_played = true;
                        
                            // Check if current matchday is completed
                            for (int i = 0; i < total_matches; i++)
                            {
                                if (match_day[i] == current_matchday && match_played[i] == false)
                                {
                                    all_played = false;
                                }
                            }
                        
                            // If previous matchday not completed → show matches
                            if (current_matchday != 0 && all_played == false)
                            {
                                cout << "\nPrevious matchday not completed\n";
                            
                                for (int i = 0; i < total_matches; i++)
                                {
                                    if (match_day[i] == current_matchday)
                                    {
                                        cout << team_names[match_team1[i]] << " vs "
                                             << team_names[match_team2[i]];
                                    
                                        if (match_played[i])
                                        {
                                            cout << " Score line: " << goals1[i] << " - " << goals2[i];
                                        }
                                        else
                                        {
                                            cout << " Not played";
                                        }
                                        cout << endl;
                                    }
                                }
                            }
                            else
                            {
                                // Initialize order only once
                                if (!order_initialized)
                                {
                                    for (int i = 0; i < total_teams; i++)
                                    {
                                        order[i] = i;
                                    }
                                    order_initialized = true;
                                }
                            
                                current_matchday++;
                            
                                int half = total_teams / 2;
                            
                                // Create matches using round-robin
                                for (int i = 0; i < half; i++)
                                {
                                    int teamA = order[i];
                                    int teamB = order[total_teams - 1 - i];
                                
                                    match_team1[total_matches] = teamA;
                                    match_team2[total_matches] = teamB;
                                    match_day[total_matches] = current_matchday;
                                    match_played[total_matches] = false;
                                
                                    total_matches++;
                                }
                            
                                // Rotate teams (except first)
                                int last = order[total_teams - 1];
                            
                                for (int i = total_teams - 1; i > 1; i--)
                                {
                                    order[i] = order[i - 1];
                                }
                            
                                order[1] = last;
                            
                                // Display scheduled matches
                                cout << "\nMatchday " << current_matchday << " scheduled:\n";
                            
                                for (int i = 0; i < total_matches; i++)
                                {
                                    if (match_day[i] == current_matchday)
                                    {
                                        cout << team_names[match_team1[i]] << " vs "
                                             << team_names[match_team2[i]] << endl;
                                    }
                                }
                            }
                        }
                        else if (admin_choice==4)
                        {
                            //simulating matches
                            bool already_played=true;
                            for (int i=0;i<total_matches;i++)
                            {
                                if (match_day[i]==current_matchday&&match_played[i]==false)
                                {
                                    already_played=false;
                                }    
                                if(!already_played)
                                {
                                    //teams taht play
                                    int team_A = match_team1[i];
                                    int team_B = match_team2[i];
                                    //generating random scoreline
                                    goals1[i] = rand()%5;
                                    goals2[i] = rand()%5;
                                    //updating match played to true
                                    match_played[i]=true;
                                    //updating matches played by that team
                                    matches_played[team_A]++;
                                    matches_played[team_B]++;
                                    //calculating g/a of team 1
                                    goals_for[team_A] += goals1[i];
                                    goals_against[team_A] += goals2[i];
                                    //calculating g/a of team 2
                                    goals_for[team_B] += goals2[i];
                                    goals_against[team_B] += goals1[i];

                                    //if team a wins
                                    if (goals1[i] > goals2[i])
                                    {
                                        points[team_A] += 3;
                                        wins[team_A]++;
                                        losses[team_B]++;
                                    }
                                    //if team b wins
                                    else if (goals2[i] > goals1[i])
                                    {
                                        points[team_B] += 3;
                                        wins[team_B]++;
                                        losses[team_A]++;
                                    }
                                    else
                                    {
                                        points[team_A]+=1;
                                        points[team_B]+=1;
                                        draws[team_A]+=1;
                                        draws[team_B]+=1;
                                    }
                                    //storing the results of match
                                    string record = team_names[team_A] + " " +
                                                    to_string(goals1[i]) + " - " +
                                                    to_string(goals2[i]) + " " +
                                                    team_names[team_B];
                                    if (history_count < 10)    
                                    {
                                            history[history_count] = record;
                                        history_count++;
                                    }
                                    else
                                    {
                                        // shift left
                                        for (int h = 0; h < 9; h++)
                                        {
                                            history[h] = history[h + 1];
                                        }
                                        history[9] = record;
                                    } 
                                    //showing results
                                    cout << team_names[team_A] << " "<< goals1[i] << " - "<< goals2[i] << " "<< team_names[team_B] << endl;               
                                }
                            }
                        }
                        else if (admin_choice==5)
                        {
                            cout<<"\nMatches history\n";
                            if(history_count==0)
                            {
                                cout<<"No matches played yet.\n";
                            }
                            else
                            {
                                for(int i =0 ;i<history_count;i++)
                                {
                                    cout<<i+1<<". "<<history[i]<<endl;
                                }
                            }
                        }
                        else if (admin_choice==6)
                        {
                            //end loop
                            break;
                        }
                        else
                        {
                            cout<<"Enter valid option.";
                        }
                        cout<<"\npress any key to countinue\n";
                        getch();
                    } // admin menu loop
                }
                else
                {
                    cout << "\nWrong username or password.\n";
                    login_tries++;
                }
            }

            if (!login_successful)
            {
                cout << "\nLogin failed.\n";
            }
        }
        else if (login_choice == 2)
        {
            // user menu
            int user_choice=0;
            system("cls");
            while(user_choice!=4)
            {
                cout<<R"(1. View League Table
2. View Matches
3. View History
4. Back)";
                cout<<"\nEnter oyur choice: ";
                cin>>user_choice;
                if (user_choice == 1)
            {
                int idx[20];
            
                for (int i = 0; i < total_teams; i++)
                {
                    idx[i] = i;
                }
            
                // sorting teams based on points (if points same then on basses of gd)
                for (int i = 0; i < total_teams - 1; i++)
                {
                    for (int j = 0; j < total_teams - i - 1; j++)
                    {
                        int a = idx[j];
                        int b = idx[j + 1];
                    
                        int gd_a = goals_for[a] - goals_against[a];
                        int gd_b = goals_for[b] - goals_against[b];
                    
                        if (
                            points[a] < points[b] ||
                            (points[a] == points[b] && gd_a < gd_b)
                           )
                        {
                            int temp = idx[j];
                            idx[j] = idx[j + 1];
                            idx[j + 1] = temp;
                        }
                    }
                }
            
                //header
                cout << "Team\tMP\tW\tL\tD\tGF\tGA\tPts\n";
            
                //league table
                for (int i = 0; i < total_teams; i++)
                {
                    int t = idx[i];
                
                    cout << team_names[t] << "\t";
                    cout<< "\t"<<matches_played[t] << "\t"
                         << wins[t] << "\t"
                         << losses[t] << "\t"
                         << draws[t] << "\t"
                         << goals_for[t] << "\t"
                         << goals_against[t] << "\t"
                         << points[t] << endl;
                }
            }
                else if (user_choice == 2)
                {
                    if (total_matches == 0)
                    {
                        cout << "\nNo matches scheduled yet.\n";
                    }
                    else
                    {
                        cout << "\n===== ALL MATCHES =====\n";
                    
                        for (int d = 1; d <= current_matchday; d++)
                        {
                            cout << "\n--- Matchday " << d << " ---\n";
                        
                            for (int i = 0; i < total_matches; i++)
                            {
                                if (match_day[i] == d)
                                {
                                    cout << team_names[match_team1[i]] << " vs "
                                         << team_names[match_team2[i]];
                                
                                    if (match_played[i])
                                    {
                                        cout << " | Score: "
                                             << goals1[i] << " - " << goals2[i];
                                    }
                                    else
                                    {
                                        cout << " | Not Played";
                                    }
                                
                                    cout << endl;
                                }
                            }
                        }
                    }
                }
                else if (user_choice==3)
                {
                    cout<<"\nMatches history\n";
                    if(history_count==0)
                    {
                        cout<<"No matches played yet.\n";
                    }
                    else
                    {
                        for(int i =0 ;i<history_count;i++)
                        {
                            cout<<i+1<<". "<<history[i]<<endl;
                        }
                    }
                }
                else if(user_choice==4)
                {
                    //end loop
                    break;
                }
                else
                {
                    cout<<"Entr valid choice.";
                }
            }
        }
        else if (login_choice == 3)
        {
            break;
        }
        else
        {
            cout << "\nEnter valid choice.";
        }

        cout << "\npress any key to continue.....";
        getch();
    }

    return 0;
}