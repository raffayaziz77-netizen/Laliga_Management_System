#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <conio.h>
using namespace std;

//  GLOBAL STATE

// Team data
string team_names[20];
int    points[20]         = {0};
int    matches_played[20] = {0};
int    wins[20]           = {0};
int    losses[20]         = {0};
int    draws[20]          = {0};
int    goals_for[20]      = {0};
int    goals_against[20]  = {0};
int    total_teams        = 5;

// Match data
int    match_team1[200]   = {0};
int    match_team2[200]   = {0};
int    match_day[200]     = {0};
bool   match_played[200]  = {false};
int    goals1[200]        = {0};
int    goals2[200]        = {0};
int    total_matches      = 0;
int    current_matchday   = 0;

// Round-robin rotation
int    order[20]          = {0};
bool   order_initialized  = false;

// Match history (last 10 results)
string history[10];
int    history_count      = 0;

//  UTILITY

// Convert a string to lowercase in-place
void to_lower(string &s)
{
    for (int c = 0; c < (int)s.size(); c++)
        if (s[c] >= 'A' && s[c] <= 'Z')
            s[c] += 32;
}

// Pause until the user presses a key
void pause()
{
    cout << "\nPress any key to continue...\n";
    getch();
}

// Push a result string into the history
void push_history(const string &record)
{
    if (history_count < 10)
    {
        history[history_count++] = record;
    }
    else
    {
        for (int h = 0; h < 9; h++)
            history[h] = history[h + 1];
        history[9] = record;
    }
}
//  ADMIN FUNCTIONS

void add_team()
{
    if (total_teams >= 20)
    {
        cout << "\nTeam limit reached (20).";
        return;
    }

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

void view_teams()
{
    if (total_teams == 0)
    {
        cout << "\nNo teams added yet.";
        return;
    }

    system("cls");
    for (int i = 0; i < total_teams; i++)
        cout << i + 1 << ". " << team_names[i] << endl;
}

void schedule_matchday()
{
    if (total_teams < 2)
    {
        cout << "\nNeed at least 2 teams to schedule a matchday.\n";
        return;
    }
    if (total_teams % 2 != 0)
    {
        cout << "\nNumber of teams must be EVEN to schedule matches.\n";
        return;
    }

    int max_matchdays = (total_teams - 1) * 2;
    if (current_matchday >= max_matchdays)
    {
        cout << "\nAll matchdays have already been scheduled.\n";
        return;
    }

    // Check that the previous matchday is fully played
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
        return;
    }

    // Initialise rotation array once per season
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

        // Swap home/away in the return leg
        if (current_matchday > (total_teams - 1))
        {
            int tmp = teamA;
            teamA   = teamB;
            teamB   = tmp;
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
        if (match_day[i] == current_matchday)
            cout << team_names[match_team1[i]] << " vs "
                 << team_names[match_team2[i]] << endl;
}

void simulate_matches()
{
    if (current_matchday == 0)
    {
        cout << "\nNo matchday scheduled yet.\n";
        return;
    }

    bool any_simulated = false;

    for (int i = 0; i < total_matches; i++)
    {
        if (match_day[i] != current_matchday || match_played[i])
            continue;

        any_simulated = true;
        int team_A = match_team1[i];
        int team_B = match_team2[i];

        goals1[i]       = rand() % 5;
        goals2[i]       = rand() % 5;
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
        push_history(record);

        cout << team_names[team_A] << " "
             << goals1[i] << " - " << goals2[i] << " "
             << team_names[team_B] << endl;
    }

    if (!any_simulated)
        cout << "\nAll matches in current matchday are already played.\n";
}

void view_match_history()
{
    cout << "\nMatch history (last " << history_count << " results):\n";
    if (history_count == 0)
    {
        cout << "No matches played yet.\n";
        return;
    }
    for (int i = 0; i < history_count; i++)
        cout << i + 1 << ". " << history[i] << endl;
}

void relegate_team()
{
    if (total_teams == 0)
    {
        cout << "\nNo teams to relegate.\n";
        return;
    }

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
        return;
    }

    // Remove every match involving the relegated team and fix indices
    int new_total_matches = 0;
    for (int i = 0; i < total_matches; i++)
    {
        if (match_team1[i] == relegate_idx || match_team2[i] == relegate_idx)
            continue;

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

    // Shift team stat arrays left
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

    // Update rotation order, removing the relegated team's slot
    int old_size       = total_teams + 1;
    int new_order_size = 0;
    for (int i = 0; i < old_size; i++)
    {
        if (order[i] == relegate_idx)
            continue;
        if (order[i] > relegate_idx)
            order[i]--;
        order[new_order_size++] = order[i];
    }

    // Reset season state for new roster
    current_matchday  = 0;
    total_matches     = 0;
    order_initialized = false;
    history_count     = 0;

    cout << "\nTeam \"" << relegate_name << "\" has been relegated.\n";
    cout << "Schedule and history have been reset for the new season.\n";
}


//  USER FUNCTIONS
void view_league_table()
{
    if (total_teams == 0)
    {
        cout << "\nNo teams in the league yet.\n";
        return;
    }

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

    // Find the longest team name so the column width adapts
    int name_col = 4; // minimum width ("Team" header)
    for (int i = 0; i < total_teams; i++)
        if ((int)team_names[i].size() > name_col)
            name_col = (int)team_names[i].size();
    name_col += 2; // add a little padding

    // Header
    cout << "\n"
         << left  << setw(name_col) << "Team"
         << right << setw(5)  << "MP"
                  << setw(5)  << "W"
                  << setw(5)  << "L"
                  << setw(5)  << "D"
                  << setw(5)  << "GF"
                  << setw(5)  << "GA"
                  << setw(5)  << "GD"
                  << setw(6)  << "Pts"
         << "\n";

    // Separator line sized to match total header width
    int total_width = name_col + 5*7 + 6; // 7 numeric cols of width 5, Pts of width 6
    for (int i = 0; i < total_width; i++) cout << '-';
    cout << "\n";

    // Rows
    for (int i = 0; i < total_teams; i++)
    {
        int t  = idx[i];
        int gd = goals_for[t] - goals_against[t];

        cout << left  << setw(name_col) << team_names[t]
             << right << setw(5)  << matches_played[t]
                      << setw(5)  << wins[t]
                      << setw(5)  << losses[t]
                      << setw(5)  << draws[t]
                      << setw(5)  << goals_for[t]
                      << setw(5)  << goals_against[t]
                      << setw(5)  << (gd >= 0 ? "+" : "") + to_string(gd)
                      << setw(6)  << points[t]
             << "\n";
    }
}

void view_all_matches()
{
    if (total_matches == 0)
    {
        cout << "\nNo matches scheduled yet.\n";
        return;
    }

    cout << "\n===== ALL MATCHES =====\n";
    for (int d = 1; d <= current_matchday; d++)
    {
        cout << "\n--- Matchday " << d << " ---\n";
        for (int i = 0; i < total_matches; i++)
        {
            if (match_day[i] != d) continue;

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

void view_current_matchday()
{
    if (current_matchday == 0)
    {
        cout << "\nNo matchday scheduled yet.\n";
        return;
    }

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
            if (match_day[i] != current_matchday) continue;
            cout << team_names[match_team1[i]] << " vs "
                 << team_names[match_team2[i]];
            if (match_played[i])
                cout << " | Score: " << goals1[i] << " - " << goals2[i];
            else
                cout << " | Not Played";
            cout << endl;
        }
    }
    else
    {
        cout << "\n=== Previous Matchday Results (Matchday "
             << current_matchday << ") ===\n";
        for (int i = 0; i < total_matches; i++)
        {
            if (match_day[i] == current_matchday)
                cout << team_names[match_team1[i]] << " "
                     << goals1[i] << " - " << goals2[i] << " "
                     << team_names[match_team2[i]] << endl;
        }
    }
}

void search_team_stats()
{
    if (total_teams == 0)
    {
        cout << "\nNo teams in the league yet.\n";
        return;
    }

    cout << "\nEnter team name to search: ";
    cin.ignore();
    string search_name;
    getline(cin, search_name);

    string search_lower = search_name;
    to_lower(search_lower);

    int found_idx = -1;
    for (int i = 0; i < total_teams; i++)
    {
        string name_lower = team_names[i];
        to_lower(name_lower);
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
        return;
    }

    int t  = found_idx;
    int gd = goals_for[t] - goals_against[t];

    // Determine league position
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
    cout << "  Goal Difference : " << (gd >= 0 ? "+" : "") << gd << "\n";
    cout << "  Points          : " << points[t]         << "\n";

    if (matches_played[t] > 0)
    {
        int wr = (wins[t] * 100) / matches_played[t];
        cout << "  Win Rate        : " << wr << "%\n";
    }

    cout << "\n  --- Match results ---\n";
    bool any_result = false;
    for (int i = 0; i < total_matches; i++)
    {
        if (!match_played[i]) continue;
        if (match_team1[i] != t && match_team2[i] != t) continue;

        any_result = true;

        int    g_for, g_against;
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

        cout << "  MD" << match_day[i] << "  " << result << "  "
             << team_names[t] << " " << g_for << " - " << g_against
             << " " << opponent << "\n";
    }

    if (!any_result)
        cout << "  No results yet.\n";

    cout << "========================================\n";
}

void head_to_head()
{
    if (total_teams < 2)
    {
        cout << "\nNeed at least 2 teams in the league.\n";
        return;
    }

    cin.ignore();

    cout << "\nEnter first team name: ";
    string name1;
    getline(cin, name1);

    cout << "Enter second team name: ";
    string name2;
    getline(cin, name2);

    string low1 = name1, low2 = name2;
    to_lower(low1);
    to_lower(low2);

    int idx1 = -1, idx2 = -1;
    for (int i = 0; i < total_teams; i++)
    {
        string nl = team_names[i];
        to_lower(nl);
        if (nl == low1) idx1 = i;
        if (nl == low2) idx2 = i;
    }

    if      (idx1 == -1) { cout << "\nTeam \"" << name1 << "\" not found.\n"; return; }
    else if (idx2 == -1) { cout << "\nTeam \"" << name2 << "\" not found.\n"; return; }
    else if (idx1 == idx2) { cout << "\nPlease enter two different teams.\n"; return; }

    int  h2h_wins1 = 0, h2h_wins2 = 0, h2h_draws = 0;
    int  h2h_gf1   = 0, h2h_gf2   = 0;
    bool any_played = false;

    cout << "\n========================================\n";
    cout << "  " << team_names[idx1] << "  vs  " << team_names[idx2] << "\n";
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
                 << team_names[match_team2[i]] << "  (not played yet)\n";
            continue;
        }

        any_played = true;

        int g1, g2;
        if (match_team1[i] == idx1) { g1 = goals1[i]; g2 = goals2[i]; }
        else                        { g1 = goals2[i]; g2 = goals1[i]; }

        h2h_gf1 += g1;
        h2h_gf2 += g2;

        string result;
        if      (g1 > g2) { result = team_names[idx1] + " win"; h2h_wins1++; }
        else if (g2 > g1) { result = team_names[idx2] + " win"; h2h_wins2++; }
        else              { result = "Draw";                     h2h_draws++;  }

        cout << "  MD" << match_day[i] << "  "
             << team_names[idx1] << " " << g1 << " - " << g2
             << " " << team_names[idx2] << "  (" << result << ")\n";
    }

    if (!any_played)
    {
        cout << "  No completed matches between these teams yet.\n";
    }
    else
    {
        int total_h2h = h2h_wins1 + h2h_wins2 + h2h_draws;
        cout << "\n  --- Summary ---\n";
        cout << "  Matches played  : " << total_h2h << "\n";
        cout << "  " << team_names[idx1] << " wins    : " << h2h_wins1 << "\n";
        cout << "  Draws           : " << h2h_draws  << "\n";
        cout << "  " << team_names[idx2] << " wins    : " << h2h_wins2 << "\n";
        cout << "  Goals           : "
             << team_names[idx1] << " " << h2h_gf1
             << "  -  " << h2h_gf2 << " " << team_names[idx2] << "\n";

        cout << "  Overall leader  : ";
        if      (h2h_wins1 > h2h_wins2) cout << team_names[idx1] << "\n";
        else if (h2h_wins2 > h2h_wins1) cout << team_names[idx2] << "\n";
        else                             cout << "Even\n";
    }
    cout << "========================================\n";
}
//  MENU FUNCTIONS
bool admin_login()
{
    int    login_tries = 0;
    string username, password;

    while (login_tries < 3)
    {
        cout << "Enter username: ";
        cin  >> username;
        cout << "Enter password: ";
        cin  >> password;

        if (username == "admin" && password == "1234")
            return true;

        cout << "\nWrong username or password.\n";
        login_tries++;
    }

    cout << "\nLogin failed after 3 attempts.\n";
    return false;
}

void admin_menu()
{
    int choice = 0;
    while (choice != 7)
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
        cin  >> choice;

        if      (choice == 1) add_team();
        else if (choice == 2) view_teams();
        else if (choice == 3) schedule_matchday();
        else if (choice == 4) simulate_matches();
        else if (choice == 5) view_match_history();
        else if (choice == 6) relegate_team();
        else if (choice == 7) { /* back */ }
        else                  cout << "\nEnter a valid option.";

        if (choice != 7)
            pause();
    }
}

void user_menu()
{
    int choice = 0;
    while (choice != 6)
    {
        system("cls");
        cout << "1. View League Table\n";
        cout << "2. View Matches\n";
        cout << "3. View Current Matchday\n";
        cout << "4. Search Team Stats\n";
        cout << "5. Head-to-Head\n";
        cout << "6. Back\n";
        cout << "Enter your choice: ";
        cin  >> choice;

        if      (choice == 1) view_league_table();
        else if (choice == 2) view_all_matches();
        else if (choice == 3) view_current_matchday();
        else if (choice == 4) search_team_stats();
        else if (choice == 5) head_to_head();
        else if (choice == 6) { /* back */ }
        else                  cout << "\nEnter a valid choice.";

        if (choice != 6)
            pause();
    }
}

void print_banner()
{
    cout << R"(
  _               _      _____ _____
 | |        /\   | |    |_   _/ ____|   /\
 | |       /  \  | |      | || |  __   /  \
 | |      / /\ \ | |      | || | |_ | / /\ \
 | |____ / ____ \| |____ _| || |__| |/ ____ \
 |______/_/    \_\______|_____\_____/_/    \_\
    )";
    cout << "\nWelcome to LALIGA MANAGEMENT SYSTEM.\n";
}

// ============================================================
//  MAIN
// ============================================================

int main()
{
    // Initialise default teams
    team_names[0] = "Barca";
    team_names[1] = "Madrid";
    team_names[2] = "Atleti";
    team_names[3] = "bilbao";
    team_names[4] = "Girona";

    int login_choice = 0;
    while (login_choice != 3)
    {
        system("cls");
        print_banner();

        cout << "1. Admin Login\n";
        cout << "2. User Menu\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin  >> login_choice;

        if (login_choice == 1)
        {
            if (admin_login())
                admin_menu();
        }
        else if (login_choice == 2)
        {
            user_menu();
        }
        else if (login_choice == 3)
        {
            // exit — loop condition handles it 
        }
        else
        {
            cout << "\nEnter a valid choice.";
        }

        if (login_choice != 3)
            pause();
    }

    return 0;
}