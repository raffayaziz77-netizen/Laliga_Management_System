#include <iostream>
#include <cstdlib>
#include <conio.h>
using namespace std;

int main()
{
    // Team data
    string team_names[20] = {"Barca", "Madrid", "Atleti", "bilbao", "Girona"};
    int points[20]         = {0};
    int matches_played[20] = {0};
    int wins[20]           = {0};
    int losses[20]         = {0};
    int draws[20]          = {0};
    int goals_for[20]      = {0};
    int goals_against[20]  = {0};
    int total_teams = 5;

    // Matches data
    int  match_team1[200]  = {0};
    int  match_team2[200]  = {0};
    int  match_day[200]    = {0};
    bool match_played[200] = {false};
    int  goals1[200]       = {0};
    int  goals2[200]       = {0};
    int  total_matches     = 0;
    int  current_matchday  = 0;
    int  order[20]         = {0};
    bool order_initialized = false;

    // Match history 
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
        cin  >> login_choice;

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

                        // add team
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

                        // view teams
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
                                    cout << i + 1 << ". " << team_names[i] << endl;
                            }
                        }

                        // schedule matchday
                        else if (admin_choice == 3)
                        {
                            if (total_teams < 2)
                            {
                                cout << "\nNeed at least 2 teams to schedule a matchday.\n";
                            }
                            else if (total_teams % 2 != 0)
                            {
                                cout << "\nNumber of teams must be EVEN to schedule matches.\n";
                            }
                            else
                            {
                                int max_matchdays = (total_teams - 1) * 2;

                                if (current_matchday >= max_matchdays)
                                {
                                    cout << "\nAll matchdays have already been scheduled.\n";
                                }
                                else
                                {
                                    // Check if previous matchday is complete before scheduling next
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
                                                    cout << " | Score: " << goals1[i] << " - " << goals2[i];
                                                else
                                                    cout << " | Not Played";
                                                cout << endl;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        // Initialize rotation array once per season
                                        if (!order_initialized)
                                        {
                                            for (int i = 0; i < total_teams; i++)
                                                order[i] = i;
                                            order_initialized = true;
                                        }

                                        current_matchday++;
                                        int half = total_teams / 2;

                                        for (int i = 0; i < half; i++)
                                        {
                                            if (total_matches >= 200)
                                            {
                                                cout << "\nMatch schedule is full (200 matches).\n";
                                                break;
                                            }

                                            int teamA = order[i];
                                            int teamB = order[total_teams - 1 - i];

                                            // Swap home/away in the return leg (second half of season)
                                            if (current_matchday > (total_teams - 1))
                                            {
                                                int temp = teamA;
                                                teamA = teamB;
                                                teamB = temp;
                                            }

                                            match_team1[total_matches]  = teamA;
                                            match_team2[total_matches]  = teamB;
                                            match_day[total_matches]    = current_matchday;
                                            match_played[total_matches] = false;
                                            total_matches++;
                                        }

                                        // Rotate all slots except index 0 (round-robin algorithm)
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
                        }

                        //Simulate matches
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
                                            // rop oldest, append newest
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

                        // Matchday history
                        else if (admin_choice == 5)
                        {
                            cout << "\nMatch history (last " << history_count << " results):\n";
                            if (history_count == 0)
                                cout << "No matches played yet.\n";
                            else
                                for (int i = 0; i < history_count; i++)
                                    cout << i + 1 << ". " << history[i] << endl;
                        }

                        // Relegate team
                        else if (admin_choice == 6)
                        {
                            if (total_teams == 0)
                            {
                                cout << "\nNo teams to relegate.\n";
                            }
                            else
                            {
                                cout << "\nCurrent teams:\n";
                                for (int i = 0; i < total_teams; i++)
                                    cout << i + 1 << ". " << team_names[i] << endl;

                                cout << "\nEnter the name of the team to relegate: ";
                                cin.ignore();
                                string relegate_name;
                                getline(cin, relegate_name);

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
                                    // Step 1: Remove every match involving the relegated team.
                                    //         Adjust stored indices for teams that shift down.
                                    int new_total_matches = 0;
                                    for (int i = 0; i < total_matches; i++)
                                    {
                                        if (match_team1[i] == relegate_idx ||
                                            match_team2[i] == relegate_idx)
                                            continue; // drop this match

                                        if (match_team1[i] > relegate_idx) match_team1[i]--;
                                        if (match_team2[i] > relegate_idx) match_team2[i]--;

                                        match_team1[new_total_matches]  = match_team1[i];
                                        match_team2[new_total_matches]  = match_team2[i];
                                        match_day[new_total_matches]    = match_day[i];
                                        match_played[new_total_matches] = match_played[i];
                                        goals1[new_total_matches]       = goals1[i];
                                        goals2[new_total_matches]       = goals2[i];
                                        new_total_matches++;
                                    }
                                    total_matches = new_total_matches;

                                    // Step 2: Shift team stat arrays left to close the gap.
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

                                    // Use old total_teams count (total_teams+1 after decrement)
                                    // and then reset scheduling state so the next season
                                    // uses a fresh rotation with the updated roster.
                                    int old_size = total_teams + 1; // value before decrement
                                    int new_order_size = 0;
                                    for (int i = 0; i < old_size; i++)
                                    {
                                        if (order[i] == relegate_idx)
                                            continue;
                                        if (order[i] > relegate_idx)
                                            order[i]--;
                                        order[new_order_size] = order[i];
                                        new_order_size++;
                                    }
                                    //Reset scheduling state so matches and matchday
                                    //counter are consistent with the new team count.
                                    current_matchday  = 0;
                                    total_matches     = 0;
                                    order_initialized = false;
                                    history_count     = 0;

                                    cout << "\nTeam \"" << relegate_name << "\" has been relegated.\n";
                                    cout << "Schedule and history have been reset for the new season.\n";
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

        // USER MENU
        else if (login_choice == 2)
        {
            int user_choice = 0;
            while (user_choice != 6)
            {
                system("cls");
                cout << "1. View League Table\n";
                cout << "2. View Matches\n";
                cout << "3. View Current Matchday\n";
                cout << "4. Search Team Stats\n";
                cout << "5. Head-to-Head\n";
                cout << "6. Back\n";
                cout << "Enter your choice: ";
                cin  >> user_choice;

                //League table
                if (user_choice == 1)
                {
                    if (total_teams == 0)
                    {
                        cout << "\nNo teams in the league yet.\n";
                    }
                    else
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

                        cout << "\nTeam\t\tMP\tW\tL\tD\tGF\tGA\tPts\n";
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
                }

                //view all matches
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

                //Current matchday
                else if (user_choice == 3)
                {
                    if (current_matchday == 0)
                    {
                        cout << "\nNo matchday scheduled yet.\n";
                    }
                    else
                    {
                        bool has_unplayed = false;
                        for (int i = 0; i < total_matches; i++)
                        {
                            if (match_day[i] == current_matchday && !match_played[i])
                            {
                                has_unplayed = true;
                                break;
                            }
                        }

                        if (has_unplayed)
                        {
                            cout << "\n=== Current Matchday " << current_matchday << " ===\n";
                            for (int i = 0; i < total_matches; i++)
                            {
                                if (match_day[i] == current_matchday)
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
                        else
                        {
                            cout << "\n=== Previous Matchday Results (Matchday "
                                 << current_matchday << ") ===\n";
                            for (int i = 0; i < total_matches; i++)
                            {
                                if (match_day[i] == current_matchday)
                                {
                                    cout << team_names[match_team1[i]] << " "
                                         << goals1[i] << " - " << goals2[i] << " "
                                         << team_names[match_team2[i]] << endl;
                                }
                            }
                        }
                    }
                }

                //search team stats
                else if (user_choice == 4)
                {
                    if (total_teams == 0)
                    {
                        cout << "\nNo teams in the league yet.\n";
                    }
                    else
                    {
                        cout << "\nEnter team name to search: ";
                        cin.ignore();
                        string search_name;
                        getline(cin, search_name);

                        // make a lowercase copy of search for case sensitive
                        string search_lower = search_name;
                        for (int c = 0; c < (int)search_lower.size(); c++)
                            if (search_lower[c] >= 'A' && search_lower[c] <= 'Z')
                                search_lower[c] = search_lower[c] + 32;

                        int found_idx = -1;
                        for (int i = 0; i < total_teams; i++)
                        {
                            string name_lower = team_names[i];
                            for (int c = 0; c < (int)name_lower.size(); c++)
                                if (name_lower[c] >= 'A' && name_lower[c] <= 'Z')
                                    name_lower[c] = name_lower[c] + 32;

                            if (name_lower == search_lower)
                            {
                                found_idx = i;
                                break;
                            }
                        }

                        if (found_idx == -1)
                        {
                            cout << "\nTeam \"" << search_name << "\" not found.\n";
                            cout << "\nAvailable teams:\n";
                            for (int i = 0; i < total_teams; i++)
                                cout << "  " << i + 1 << ". " << team_names[i] << "\n";
                        }
                        else
                        {
                            int t  = found_idx;
                            int gd = goals_for[t] - goals_against[t];

                            // Find league position by counting teams with more points
                            // or equal points but better goal difference
                            int position = 1;
                            for (int i = 0; i < total_teams; i++)
                            {
                                if (i == t) continue;
                                int gd_i = goals_for[i] - goals_against[i];
                                if (points[i] > points[t] ||
                                   (points[i] == points[t] && gd_i > gd))
                                    position++;
                            }

                            cout << "\n========================================\n";
                            cout << "  " << team_names[t] << "\n";
                            cout << "========================================\n";
                            cout << "  League Position : " << position << " / " << total_teams << "\n";
                            cout << "  Matches Played  : " << matches_played[t] << "\n";
                            cout << "  Wins            : " << wins[t]           << "\n";
                            cout << "  Draws           : " << draws[t]          << "\n";
                            cout << "  Losses          : " << losses[t]         << "\n";
                            cout << "  Goals For       : " << goals_for[t]      << "\n";
                            cout << "  Goals Against   : " << goals_against[t]  << "\n";
                            cout << "  Goal Difference : "
                                 << (gd >= 0 ? "+" : "") << gd  << "\n";
                            cout << "  Points          : " << points[t]         << "\n";

                            // Win rate (avoid divide by zero)
                            if (matches_played[t] > 0)
                            {
                                int wr = (wins[t] * 100) / matches_played[t];
                                cout << "  Win Rate        : " << wr << "%\n";
                            }

                            // Show this team's results from match history
                            cout << "\n  --- Match results ---\n";
                            bool any_result = false;
                            for (int i = 0; i < total_matches; i++)
                            {
                                if (!match_played[i]) continue;
                                if (match_team1[i] == t || match_team2[i] == t)
                                {
                                    any_result = true;
                                    // Determine result from this team's perspective
                                    int g_for, g_against;
                                    string opponent;
                                    if (match_team1[i] == t)
                                    {
                                        g_for     = goals1[i];
                                        g_against = goals2[i];
                                        opponent  = team_names[match_team2[i]];
                                    }
                                    else
                                    {
                                        g_for     = goals2[i];
                                        g_against = goals1[i];
                                        opponent  = team_names[match_team1[i]];
                                    }

                                    string result;
                                    if      (g_for > g_against) result = "W";
                                    else if (g_for < g_against) result = "L";
                                    else                        result = "D";

                                    cout << "  MD" << match_day[i] << "  "
                                         << result << "  "
                                         << team_names[t] << " "
                                         << g_for << " - " << g_against
                                         << " " << opponent << "\n";
                                }
                            }
                            if (!any_result)
                                cout << "  No results yet.\n";

                            cout << "========================================\n";
                        }
                    }
                }

                // head to head
                else if (user_choice == 5)
                {
                    if (total_teams < 2)
                    {
                        cout << "\nNeed at least 2 teams in the league.\n";
                    }
                    else
                    {
                        cin.ignore();

                        cout << "\nEnter first team name: ";
                        string name1;
                        getline(cin, name1);

                        cout << "Enter second team name: ";
                        string name2;
                        getline(cin, name2);

                        // Lowercase both for case-insensitive matching
                        string low1 = name1, low2 = name2;
                        for (int c = 0; c < (int)low1.size(); c++)
                            if (low1[c] >= 'A' && low1[c] <= 'Z') low1[c] += 32;
                        for (int c = 0; c < (int)low2.size(); c++)
                            if (low2[c] >= 'A' && low2[c] <= 'Z') low2[c] += 32;

                        int idx1 = -1, idx2 = -1;
                        for (int i = 0; i < total_teams; i++)
                        {
                            string nl = team_names[i];
                            for (int c = 0; c < (int)nl.size(); c++)
                                if (nl[c] >= 'A' && nl[c] <= 'Z') nl[c] += 32;

                            if (nl == low1) idx1 = i;
                            if (nl == low2) idx2 = i;
                        }

                        if (idx1 == -1)
                            cout << "\nTeam \"" << name1 << "\" not found.\n";
                        else if (idx2 == -1)
                            cout << "\nTeam \"" << name2 << "\" not found.\n";
                        else if (idx1 == idx2)
                            cout << "\nPlease enter two different teams.\n";
                        else
                        {
                            // Tally head-to-head stats
                            int h2h_wins1   = 0, h2h_wins2   = 0, h2h_draws = 0;
                            int h2h_gf1     = 0, h2h_gf2     = 0;
                            bool any_played = false;

                            cout << "\n========================================\n";
                            cout << "  " << team_names[idx1]
                                 << "  vs  " << team_names[idx2] << "\n";
                            cout << "========================================\n";
                            cout << "  --- All encounters ---\n";

                            for (int i = 0; i < total_matches; i++)
                            {
                                bool is_h2h =
                                    (match_team1[i] == idx1 && match_team2[i] == idx2) ||
                                    (match_team1[i] == idx2 && match_team2[i] == idx1);

                                if (!is_h2h) continue;

                                if (!match_played[i])
                                {
                                    cout << "  MD" << match_day[i] << "  "
                                         << team_names[match_team1[i]] << " vs "
                                         << team_names[match_team2[i]]
                                         << "  (not played yet)\n";
                                    continue;
                                }

                                any_played = true;

                                // Always show from idx1's perspective
                                int g1, g2; // goals scored by idx1 and idx2
                                if (match_team1[i] == idx1)
                                { g1 = goals1[i]; g2 = goals2[i]; }
                                else
                                { g1 = goals2[i]; g2 = goals1[i]; }

                                h2h_gf1 += g1;
                                h2h_gf2 += g2;

                                string result;
                                if      (g1 > g2) { result = team_names[idx1] + " win"; h2h_wins1++; }
                                else if (g2 > g1) { result = team_names[idx2] + " win"; h2h_wins2++; }
                                else              { result = "Draw";                     h2h_draws++;  }

                                cout << "  MD" << match_day[i] << "  "
                                     << team_names[idx1] << " "
                                     << g1 << " - " << g2
                                     << " " << team_names[idx2]
                                     << "  (" << result << ")\n";
                            }

                            if (!any_played)
                            {
                                cout << "  No completed matches between these teams yet.\n";
                            }
                            else
                            {
                                int total_h2h = h2h_wins1 + h2h_wins2 + h2h_draws;
                                cout << "\n  --- Summary ---\n";
                                cout << "  Matches played  : " << total_h2h   << "\n";
                                cout << "  " << team_names[idx1]
                                     << " wins    : " << h2h_wins1 << "\n";
                                cout << "  Draws           : " << h2h_draws   << "\n";
                                cout << "  " << team_names[idx2]
                                     << " wins    : " << h2h_wins2 << "\n";
                                cout << "  Goals           : "
                                     << team_names[idx1] << " " << h2h_gf1
                                     << "  -  " << h2h_gf2
                                     << " " << team_names[idx2] << "\n";

                                // Determine overall head-to-head leader
                                cout << "  Overall leader  : ";
                                if      (h2h_wins1 > h2h_wins2) cout << team_names[idx1] << "\n";
                                else if (h2h_wins2 > h2h_wins1) cout << team_names[idx2] << "\n";
                                else                             cout << "Even\n";
                            }
                            cout << "========================================\n";
                        }
                    }
                }

                else if (user_choice == 6)
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