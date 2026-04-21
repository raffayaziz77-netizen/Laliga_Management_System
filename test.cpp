#include <iostream>
#include <conio.h>
using namespace std;

int main()
{
    // Team data
    string team_names[20] = {"Barca", "Real Madrid", "Atletico Madrid", "Valencia", "Girona"};
    int points[20]         = {0};
    int matches_played[20] = {0};
    int wins[20]           = {0};
    int losses[20]         = {0};
    int draws[20]          = {0};
    int goals_for[20]      = {0};
    int goals_against[20]  = {0};
    int total_teams = 5;

    // Matches data
    int  match_team1[200];
    int  match_team2[200];
    int  match_day[200];
    bool match_played[200] = {false};
    int  goals1[200];
    int  goals2[200];
    int  total_matches    = 0;
    int  current_matchday = 0;
    int  order[20];
    bool order_initialized = false;

    // Match history (last 10, circular)
    string history[10];
    int    history_count = 0;

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
        cout << "1. Admin Login\n";
        cout << "2. User Menu\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> login_choice;

        // ----------------------------------------------------------------
        // ADMIN LOGIN
        // ----------------------------------------------------------------
        if (login_choice == 1)
        {
            bool login_successful = false;
            int  login_tries      = 0;
            string admin_username;
            string admin_password;

            while (login_tries < 3 && !login_successful)
            {
                cout << "Enter username: ";
                cin  >> admin_username;
                cout << "Enter password: ";
                cin  >> admin_password;

                if (admin_username == "admin" && admin_password == "1234")
                {
                    login_successful = true;

                    int admin_choice = 0;

                    while (admin_choice != 7)
                    {
                        system("cls");
                        cout << "\n1. Add Team\n";
                        cout << "2. View Teams\n";
                        cout << "3. Schedule Matchday\n";
                        cout << "4. Simulate Matches\n";
                        cout << "5. View Match History\n";
                        cout << "6. Relegate Team\n";
                        cout << "7. Back\n";
                        cout << "Enter choice: ";
                        cin  >> admin_choice;

                        // ------------------------------------------------
                        // 1. ADD TEAM
                        // ------------------------------------------------
                        if (admin_choice == 1)
                        {
                            if (total_teams < 20)
                            {
                                cout << "\nEnter team name to add: ";
                                cin.ignore();
                                getline(cin, team_names[total_teams]);

                                points[total_teams]         = 0;
                                matches_played[total_teams] = 0;
                                wins[total_teams]           = 0;
                                losses[total_teams]         = 0;
                                draws[total_teams]          = 0;
                                goals_for[total_teams]      = 0;
                                goals_against[total_teams]  = 0;

                                total_teams++;
                                cout << "\nTeam added successfully.";
                            }
                            else
                            {
                                cout << "\nTeam limit reached (20).";
                            }
                        }
                        // ------------------------------------------------
                        // 2. VIEW TEAMS
                        // ------------------------------------------------
                        else if (admin_choice == 2)
                        {
                            if (total_teams == 0)
                            {
                                cout << "\nNo teams added yet.";
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
                        // ------------------------------------------------
                        // 3. SCHEDULE MATCHDAY
                        // ------------------------------------------------
                        else if (admin_choice == 3)
                        {
                            if (total_teams < 2)
                            {
                                cout << "\nNeed at least 2 teams to schedule a matchday.\n";
                            }
                            else
                            {
                                // Check if current matchday still has unplayed matches
                                bool all_played = true;
                                for (int i = 0; i < total_matches; i++)
                                {
                                    if (match_day[i] == current_matchday && !match_played[i])
                                    {
                                        all_played = false;
                                        break;
                                    }
                                }

                                if (current_matchday != 0 && !all_played)
                                {
                                    cout << "\nPrevious matchday not completed yet:\n";
                                    for (int i = 0; i < total_matches; i++)
                                    {
                                        if (match_day[i] == current_matchday)
                                        {
                                            cout << team_names[match_team1[i]] << " vs "
                                                 << team_names[match_team2[i]];
                                            if (match_played[i])
                                                cout << "  Score: " << goals1[i] << " - " << goals2[i];
                                            else
                                                cout << "  Not played";
                                            cout << endl;
                                        }
                                    }
                                }
                                else
                                {
                                    // Initialise rotation order once
                                    if (!order_initialized)
                                    {
                                        for (int i = 0; i < total_teams; i++)
                                            order[i] = i;
                                        order_initialized = true;
                                    }

                                    current_matchday++;

                                    // Handle odd number of teams with a "bye"
                                    int effective_teams = total_teams;
                                    if (total_teams % 2 != 0)
                                        effective_teams = total_teams + 1;

                                    int half = effective_teams / 2;

                                    for (int i = 0; i < half; i++)
                                    {
                                        if (total_matches >= 200)
                                        {
                                            cout << "\nMatch schedule is full (200 matches).\n";
                                            break;
                                        }

                                        int teamA = order[i];
                                        int teamB = (effective_teams - 1 - i < total_teams)
                                                    ? order[effective_teams - 1 - i]
                                                    : -1; // -1 = bye

                                        if (teamA >= total_teams || teamB == -1)
                                            continue; // bye week

                                        match_team1[total_matches]  = teamA;
                                        match_team2[total_matches]  = teamB;
                                        match_day[total_matches]    = current_matchday;
                                        match_played[total_matches] = false;
                                        total_matches++;
                                    }

                                    // Rotate order (keep position 0 fixed)
                                    int last = order[total_teams - 1];
                                    for (int i = total_teams - 1; i > 1; i--)
                                        order[i] = order[i - 1];
                                    order[1] = last;

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
                        }
                        // ------------------------------------------------
                        // 4. SIMULATE MATCHES
                        // ------------------------------------------------
                        else if (admin_choice == 4)
                        {
                            if (current_matchday == 0)
                            {
                                cout << "\nNo matchday scheduled yet.\n";
                            }
                            else
                            {
                                bool any_simulated = false;

                                for (int i = 0; i < total_matches; i++)
                                {
                                    if (match_day[i] == current_matchday && !match_played[i])
                                    {
                                        any_simulated = true;

                                        int team_A = match_team1[i];
                                        int team_B = match_team2[i];

                                        goals1[i] = rand() % 5;
                                        goals2[i] = rand() % 5;
                                        match_played[i] = true;

                                        matches_played[team_A]++;
                                        matches_played[team_B]++;

                                        goals_for[team_A]     += goals1[i];
                                        goals_against[team_A] += goals2[i];
                                        goals_for[team_B]     += goals2[i];
                                        goals_against[team_B] += goals1[i];

                                        if (goals1[i] > goals2[i])
                                        {
                                            points[team_A] += 3;
                                            wins[team_A]++;
                                            losses[team_B]++;
                                        }
                                        else if (goals2[i] > goals1[i])
                                        {
                                            points[team_B] += 3;
                                            wins[team_B]++;
                                            losses[team_A]++;
                                        }
                                        else
                                        {
                                            points[team_A]++;
                                            points[team_B]++;
                                            draws[team_A]++;
                                            draws[team_B]++;
                                        }

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
                                            for (int h = 0; h < 9; h++)
                                                history[h] = history[h + 1];
                                            history[9] = record;
                                        }

                                        cout << team_names[team_A] << " "
                                             << goals1[i] << " - " << goals2[i] << " "
                                             << team_names[team_B] << endl;
                                    }
                                }

                                if (!any_simulated)
                                    cout << "\nAll matches in current matchday are already played.\n";
                            }
                        }
                        // ------------------------------------------------
                        // 5. MATCH HISTORY
                        // ------------------------------------------------
                        else if (admin_choice == 5)
                        {
                            cout << "\nMatch history:\n";
                            if (history_count == 0)
                                cout << "No matches played yet.\n";
                            else
                                for (int i = 0; i < history_count; i++)
                                    cout << i + 1 << ". " << history[i] << endl;
                        }
                        // ------------------------------------------------
                        // 6. RELEGATE TEAM
                        // ------------------------------------------------
                        else if (admin_choice == 6)
                        {
                            if (total_teams == 0)
                            {
                                cout << "\nNo teams to relegate.\n";
                            }
                            else
                            {
                                // Show current teams so admin can pick the right name
                                cout << "\nCurrent teams:\n";
                                for (int i = 0; i < total_teams; i++)
                                    cout << i + 1 << ". " << team_names[i] << endl;

                                cout << "\nEnter the name of the team to relegate: ";
                                cin.ignore();
                                string relegate_name;
                                getline(cin, relegate_name);

                                // Find the team index
                                int relegate_idx = -1;
                                for (int i = 0; i < total_teams; i++)
                                {
                                    if (team_names[i] == relegate_name)
                                    {
                                        relegate_idx = i;
                                        break;
                                    }
                                }

                                if (relegate_idx == -1)
                                {
                                    cout << "\nTeam \"" << relegate_name << "\" not found.\n";
                                }
                                else
                                {
                                    // Step 1: remove every match that involves this team.
                                    // While compacting, adjust stored team indices for teams
                                    // that will shift down by 1 in the team arrays.
                                    int new_total_matches = 0;
                                    for (int i = 0; i < total_matches; i++)
                                    {
                                        if (match_team1[i] == relegate_idx ||
                                            match_team2[i] == relegate_idx)
                                            continue; // drop this match

                                        // Teams above the removed slot shift down by 1
                                        if (match_team1[i] > relegate_idx)
                                            match_team1[i]--;
                                        if (match_team2[i] > relegate_idx)
                                            match_team2[i]--;

                                        // Compact into the new position
                                        match_team1[new_total_matches]  = match_team1[i];
                                        match_team2[new_total_matches]  = match_team2[i];
                                        match_day[new_total_matches]    = match_day[i];
                                        match_played[new_total_matches] = match_played[i];
                                        goals1[new_total_matches]       = goals1[i];
                                        goals2[new_total_matches]       = goals2[i];
                                        new_total_matches++;
                                    }
                                    total_matches = new_total_matches;

                                    // Step 2: shift team stat arrays left to close the gap
                                    for (int i = relegate_idx; i < total_teams - 1; i++)
                                    {
                                        team_names[i]     = team_names[i + 1];
                                        points[i]         = points[i + 1];
                                        matches_played[i] = matches_played[i + 1];
                                        wins[i]           = wins[i + 1];
                                        losses[i]         = losses[i + 1];
                                        draws[i]          = draws[i + 1];
                                        goals_for[i]      = goals_for[i + 1];
                                        goals_against[i]  = goals_against[i + 1];
                                    }
                                    total_teams--;

                                    // Step 3: fix the rotation order array.
                                    // Drop the relegated team's slot and shift remaining
                                    // indices down if they were above relegate_idx.
                                    int new_order_size = 0;
                                    for (int i = 0; i <= total_teams; i++) // was total_teams+1 before decrement
                                    {
                                        if (order[i] == relegate_idx)
                                            continue; // drop this slot
                                        if (order[i] > relegate_idx)
                                            order[i]--;
                                        order[new_order_size] = order[i];
                                        new_order_size++;
                                    }
                                    // order now has total_teams valid entries

                                    cout << "\nTeam \"" << relegate_name << "\" has been relegated.\n";
                                    cout << "All their scheduled matches have been removed.\n";
                                }
                            }
                        }
                        else if (admin_choice == 7)
                        {
                            break;
                        }
                        else
                        {
                            cout << "\nEnter a valid option.";
                        }

                        cout << "\nPress any key to continue...\n";
                        getch();
                    } // end admin menu loop
                }
                else
                {
                    cout << "\nWrong username or password.\n";
                    login_tries++;
                }
            }

            if (!login_successful)
                cout << "\nLogin failed after 3 attempts.\n";
        }
        // ----------------------------------------------------------------
        // USER MENU
        // ----------------------------------------------------------------
        else if (login_choice == 2)
        {
            int user_choice = 0;
            system("cls");

            while (user_choice != 4)
            {
                cout << "1. View League Table\n";
                cout << "2. View Matches\n";
                cout << "3. View History\n";
                cout << "4. Back\n";
                cout << "Enter your choice: ";
                cin  >> user_choice;

                // ----------------------------------------------------
                // 1. LEAGUE TABLE
                // ----------------------------------------------------
                if (user_choice == 1)
                {
                    int idx[20];
                    for (int i = 0; i < total_teams; i++)
                        idx[i] = i;

                    // Bubble sort descending by points, then goal difference
                    for (int i = 0; i < total_teams - 1; i++)
                    {
                        for (int j = 0; j < total_teams - i - 1; j++)
                        {
                            int a    = idx[j];
                            int b    = idx[j + 1];
                            int gd_a = goals_for[a] - goals_against[a];
                            int gd_b = goals_for[b] - goals_against[b];

                            if (points[a] < points[b] ||
                               (points[a] == points[b] && gd_a < gd_b))
                            {
                                int tmp    = idx[j];
                                idx[j]     = idx[j + 1];
                                idx[j + 1] = tmp;
                            }
                        }
                    }

                    cout << "Team\t\tMP\tW\tL\tD\tGF\tGA\tPts\n";
                    cout << "-----------------------------------------------------------------------\n";
                    for (int i = 0; i < total_teams; i++)
                    {
                        int t = idx[i];
                        cout << team_names[t] << "\t\t"
                             << matches_played[t] << "\t"
                             << wins[t]           << "\t"
                             << losses[t]         << "\t"
                             << draws[t]          << "\t"
                             << goals_for[t]      << "\t"
                             << goals_against[t]  << "\t"
                             << points[t]         << endl;
                    }
                }
                // ----------------------------------------------------
                // 2. VIEW ALL MATCHES
                // ----------------------------------------------------
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
                                        cout << " | Score: " << goals1[i] << " - " << goals2[i];
                                    else
                                        cout << " | Not Played";
                                    cout << endl;
                                }
                            }
                        }
                    }
                }
                // ----------------------------------------------------
                // 3. HISTORY
                // ----------------------------------------------------
                else if (user_choice == 3)
                {
                    cout << "\nMatch history:\n";
                    if (history_count == 0)
                        cout << "No matches played yet.\n";
                    else
                        for (int i = 0; i < history_count; i++)
                            cout << i + 1 << ". " << history[i] << endl;
                }
                else if (user_choice == 4)
                {
                    break;
                }
                else
                {
                    cout << "\nEnter a valid choice.";
                }

                cout << "\nPress any key to continue...\n";
                getch();
            }
        }
        else if (login_choice == 3)
        {
            break;
        }
        else
        {
            cout << "\nEnter a valid choice.";
        }

        cout << "\nPress any key to continue.....\n";
        getch();
    }

    return 0;
}