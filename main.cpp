#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <conio.h>
using namespace std;

// ---- limits ----
const int MAX_TEAMS   = 20;
const int MAX_MATCHES = 380;
const int MAX_LOGS    = 100;
const int MAX_ADMINS  = 5;

// ---- team data ----
char team_name[MAX_TEAMS][50];
int  team_points[MAX_TEAMS];
int  team_played[MAX_TEAMS];
int  team_wins[MAX_TEAMS];
int  team_losses[MAX_TEAMS];
int  team_draws[MAX_TEAMS];
int  team_gf[MAX_TEAMS];     // goals for
int  team_ga[MAX_TEAMS];     // goals against
int  total_teams = 0;

// ---- match schedule ----
int  match_home[MAX_MATCHES];
int  match_away[MAX_MATCHES];
int  match_day[MAX_MATCHES];
int  match_goals_home[MAX_MATCHES];
int  match_goals_away[MAX_MATCHES];
bool match_done[MAX_MATCHES];
int  total_matches   = 0;
int  current_day     = 0;

// for round robin rotation
int  rr_order[MAX_TEAMS];
bool rr_ready = false;

// ---- admin accounts ----
char admin_user[MAX_ADMINS][30];
char admin_pass[MAX_ADMINS][30];
char admin_secret[MAX_ADMINS][40];  // secret answer for forgot pass
int  total_admins = 0;

// who is logged in (-1 = nobody)
int logged_in_admin = -1;

// ---- history log ----
char log_lines[MAX_LOGS][120];
int  total_logs = 0;

// file names
const char* TEAMS_FILE   = "teams.dat";
const char* MATCHES_FILE = "matches.dat";
const char* ADMINS_FILE  = "admins.dat";
const char* LOGS_FILE    = "logs.dat";
const char* KEY_FILE     = "secret_key.txt";

//HELPER FUNCTIONS

void cls()
{
    system("cls");
}

void pause_key()
{
    cout << "\nPress any key...";
    getch();
}

// make string lowercase
void make_lower(char* s)
{
    for (int i = 0; s[i] != '\0'; i++)
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] += 32;
}

// compare two strings ignoring case
bool same_str(const char* a, const char* b)
{
    char x[100], y[100];
    strncpy(x, a, 99); x[99] = '\0';
    strncpy(y, b, 99); y[99] = '\0';
    make_lower(x);
    make_lower(y);
    return strcmp(x, y) == 0;
}

// check if string b is inside string a (case insensitive)
bool contains(const char* a, const char* b)
{
    char x[100], y[100];
    strncpy(x, a, 99); x[99] = '\0';
    strncpy(y, b, 99); y[99] = '\0';
    make_lower(x);
    make_lower(y);
    return strstr(x, y) != NULL;
}

// read a full line after cin >>
void read_line(char* buf, int size)
{
    cin.ignore(200, '\n');
    cin.getline(buf, size);
}

// read line without ignoring first (when no cin >> before it)
void read_line2(char* buf, int size)
{
    cin.getline(buf, size);
}

void print_line(char c, int n)
{
    for (int i = 0; i < n; i++) cout << c;
    cout << "\n";
}

void add_log(const char* msg)
{
    if (total_logs >= MAX_LOGS)
    {
        // drop oldest
        for (int i = 0; i < MAX_LOGS - 1; i++)
            strcpy(log_lines[i], log_lines[i+1]);
        total_logs = MAX_LOGS - 1;
    }

    char entry[120];
    if (logged_in_admin >= 0)
        sprintf(entry, "[%s] %s", admin_user[logged_in_admin], msg);
    else
        sprintf(entry, "[system] %s", msg);

    strncpy(log_lines[total_logs], entry, 119);
    total_logs++;
}


//  FILE SAVE / LOAD

void save_teams()
{
    ofstream f(TEAMS_FILE);
    if (!f) { cout << "Could not save teams.\n"; return; }

    f << total_teams << "\n";
    for (int i = 0; i < total_teams; i++)
    {
        f << team_name[i]    << "\n"
          << team_points[i]  << " "
          << team_played[i]  << " "
          << team_wins[i]    << " "
          << team_losses[i]  << " "
          << team_draws[i]   << " "
          << team_gf[i]      << " "
          << team_ga[i]      << "\n";
    }
    f.close();
}

void load_teams()
{
    ifstream f(TEAMS_FILE);
    if (!f) return;

    f >> total_teams;
    f.ignore();
    for (int i = 0; i < total_teams; i++)
    {
        f.getline(team_name[i], 50);
        f >> team_points[i] >> team_played[i]
          >> team_wins[i]   >> team_losses[i]
          >> team_draws[i]  >> team_gf[i] >> team_ga[i];
        f.ignore();
    }
    f.close();
}

void save_matches()
{
    ofstream f(MATCHES_FILE);
    if (!f) return;

    f << total_matches << " " << current_day << " " << rr_ready << "\n";
    for (int i = 0; i < MAX_TEAMS; i++)
        f << rr_order[i] << " ";
    f << "\n";

    for (int i = 0; i < total_matches; i++)
    {
        f << match_home[i]       << " "
          << match_away[i]       << " "
          << match_day[i]        << " "
          << match_goals_home[i] << " "
          << match_goals_away[i] << " "
          << match_done[i]       << "\n";
    }
    f.close();
}

void load_matches()
{
    ifstream f(MATCHES_FILE);
    if (!f) return;

    int rr_int;
    f >> total_matches >> current_day >> rr_int;
    rr_ready = (rr_int != 0);

    for (int i = 0; i < MAX_TEAMS; i++)
        f >> rr_order[i];

    for (int i = 0; i < total_matches; i++)
    {
        int done_int;
        f >> match_home[i] >> match_away[i] >> match_day[i]
          >> match_goals_home[i] >> match_goals_away[i] >> done_int;
        match_done[i] = (done_int != 0);
    }
    f.close();
}

void save_admins()
{
    ofstream f(ADMINS_FILE);
    if (!f) return;

    f << total_admins << "\n";
    for (int i = 0; i < total_admins; i++)
    {
        f << admin_user[i]   << "\n"
          << admin_pass[i]   << "\n"
          << admin_secret[i] << "\n";
    }
    f.close();
}

void load_admins()
{
    ifstream f(ADMINS_FILE);
    if (!f) return;

    f >> total_admins;
    f.ignore();
    for (int i = 0; i < total_admins; i++)
    {
        f.getline(admin_user[i],   30);
        f.getline(admin_pass[i],   30);
        f.getline(admin_secret[i], 40);
    }
    f.close();
}

void save_logs()
{
    ofstream f(LOGS_FILE);
    if (!f) return;

    f << total_logs << "\n";
    for (int i = 0; i < total_logs; i++)
        f << log_lines[i] << "\n";
    f.close();
}

void load_logs()
{
    ifstream f(LOGS_FILE);
    if (!f) return;

    f >> total_logs;
    f.ignore();
    for (int i = 0; i < total_logs; i++)
        f.getline(log_lines[i], 120);
    f.close();
}

// create key file if missing
void init_key_file()
{
    ifstream check(KEY_FILE);
    if (check) { check.close(); return; }
    check.close();

    ofstream f(KEY_FILE);
    f << "LALIGA2025\n";
    f.close();
}

bool verify_secret_key(const char* entered)
{
    ifstream f(KEY_FILE);
    if (!f) return false;
    char stored[50];
    f.getline(stored, 50);
    f.close();
    return strcmp(stored, entered) == 0;
}

void save_all()
{
    save_teams();
    save_matches();
    save_admins();
    save_logs();
}

//  TEAM FUNCTIONS (admin only)

// find team index by name, returns -1 if not found
int find_team(const char* name)
{
    for (int i = 0; i < total_teams; i++)
        if (same_str(team_name[i], name))
            return i;
    return -1;
}

void reset_team_stats(int i)
{
    team_points[i]  = 0;
    team_played[i]  = 0;
    team_wins[i]    = 0;
    team_losses[i]  = 0;
    team_draws[i]   = 0;
    team_gf[i]      = 0;
    team_ga[i]      = 0;
}

void add_team()
{
    if (total_teams >= MAX_TEAMS)
    {
        cout << "Team limit reached (20).\n";
        return;
    }

    char name[50];
    cout << "Team name: ";
    read_line(name, 50);

    if (strlen(name) == 0)
    {
        cout << "Name can't be empty.\n";
        return;
    }

    if (find_team(name) != -1)
    {
        cout << "Team already exists.\n";
        return;
    }

    int idx = total_teams;
    strncpy(team_name[idx], name, 49);
    reset_team_stats(idx);
    total_teams++;

    save_teams();

    char msg[100];
    sprintf(msg, "Added team: %s", name);
    add_log(msg);

    cout << "Team added!\n";
}

void print_team_list()
{
    if (total_teams == 0) { cout << "No teams yet.\n"; return; }
    cout << "\n";
    for (int i = 0; i < total_teams; i++)
        cout << i+1 << ". " << team_name[i] << "\n";
}

// edit team name
void edit_team()
{
    if (total_teams == 0) { cout << "No teams.\n"; return; }

    print_team_list();

    char old_name[50];
    cout << "Enter team name to edit: ";
    read_line(old_name, 50);

    int idx = find_team(old_name);
    if (idx == -1) { cout << "Team not found.\n"; return; }

    char new_name[50];
    cout << "New name: ";
    read_line2(new_name, 50);

    if (strlen(new_name) == 0) { cout << "Can't be empty.\n"; return; }
    if (find_team(new_name) != -1) { cout << "Name taken.\n"; return; }

    char msg[100];
    sprintf(msg, "Renamed team %s -> %s", team_name[idx], new_name);
    strncpy(team_name[idx], new_name, 49);
    save_teams();
    add_log(msg);
    cout << "Team renamed.\n";
}

// delete/relegate a team
void delete_team()
{
    if (total_teams == 0) { cout << "No teams.\n"; return; }

    print_team_list();
    char name[50];
    cout << "Enter team name to relegate/remove: ";
    read_line(name, 50);

    int idx = find_team(name);
    if (idx == -1) { cout << "Not found.\n"; return; }

    char confirm;
    cout << "Sure? This resets the season. (y/n): ";
    cin >> confirm;
    if (confirm != 'y' && confirm != 'Y') { cout << "Cancelled.\n"; return; }

    // remove matches involving this team
    int new_total = 0;
    for (int i = 0; i < total_matches; i++)
    {
        if (match_home[i] == idx || match_away[i] == idx) continue;

        if (match_home[i] > idx) match_home[i]--;
        if (match_away[i] > idx) match_away[i]--;

        match_home[new_total]       = match_home[i];
        match_away[new_total]       = match_away[i];
        match_day[new_total]        = match_day[i];
        match_goals_home[new_total] = match_goals_home[i];
        match_goals_away[new_total] = match_goals_away[i];
        match_done[new_total]       = match_done[i];
        new_total++;
    }
    total_matches = new_total;

    // shift teams
    for (int i = idx; i < total_teams - 1; i++)
    {
        strcpy(team_name[i], team_name[i+1]);
        team_points[i]  = team_points[i+1];
        team_played[i]  = team_played[i+1];
        team_wins[i]    = team_wins[i+1];
        team_losses[i]  = team_losses[i+1];
        team_draws[i]   = team_draws[i+1];
        team_gf[i]      = team_gf[i+1];
        team_ga[i]      = team_ga[i+1];
    }
    total_teams--;

    // reset season
    current_day = 0;
    rr_ready    = false;

    save_all();

    char msg[100];
    sprintf(msg, "Relegated team: %s - season reset", name);
    add_log(msg);

    cout << "Team removed. Season has been reset.\n";
}

//  MATCH SCHEDULING

bool prev_day_done()
{
    for (int i = 0; i < total_matches; i++)
        if (match_day[i] == current_day && !match_done[i])
            return false;
    return true;
}

void schedule_matchday()
{
    if (total_teams < 2) { cout << "Need at least 2 teams.\n"; return; }
    if (total_teams % 2 != 0) { cout << "Need even number of teams.\n"; return; }

    int max_days = (total_teams - 1) * 2;
    if (current_day >= max_days)
    {
        cout << "All matchdays scheduled already.\n";
        return;
    }

    // cant schedule next day if current isnt done
    if (current_day > 0 && !prev_day_done())
    {
        cout << "Finish matchday " << current_day << " first!\n";
        return;
    }

    if (!rr_ready)
    {
        for (int i = 0; i < total_teams; i++)
            rr_order[i] = i;
        rr_ready = true;
    }

    current_day++;
    int half = total_teams / 2;

    for (int i = 0; i < half; i++)
    {
        if (total_matches >= MAX_MATCHES) { cout << "Match limit hit!\n"; break; }

        int a = rr_order[i];
        int b = rr_order[total_teams - 1 - i];

        // swap home/away in return leg
        if (current_day > (total_teams - 1))
        {
            int tmp = a; a = b; b = tmp;
        }

        match_home[total_matches]       = a;
        match_away[total_matches]       = b;
        match_day[total_matches]        = current_day;
        match_goals_home[total_matches] = 0;
        match_goals_away[total_matches] = 0;
        match_done[total_matches]       = false;
        total_matches++;
    }

    // rotate (keep index 0 fixed)
    int last = rr_order[total_teams - 1];
    for (int i = total_teams - 1; i > 1; i--)
        rr_order[i] = rr_order[i - 1];
    rr_order[1] = last;

    save_matches();
    add_log("Scheduled a matchday");

    cout << "\n-- Matchday " << current_day << " --\n";
    for (int i = 0; i < total_matches; i++)
        if (match_day[i] == current_day)
            cout << team_name[match_home[i]] << " vs " << team_name[match_away[i]] << "\n";
}

// update stats after a result
void apply_result(int home, int away, int gh, int ga)
{
    team_played[home]++; team_played[away]++;
    team_gf[home] += gh; team_ga[home] += ga;
    team_gf[away] += ga; team_ga[away] += gh;

    if (gh > ga)
    {
        team_points[home] += 3;
        team_wins[home]++;
        team_losses[away]++;
    }
    else if (ga > gh)
    {
        team_points[away] += 3;
        team_wins[away]++;
        team_losses[home]++;
    }
    else
    {
        team_points[home]++;
        team_points[away]++;
        team_draws[home]++;
        team_draws[away]++;
    }
}

// reverse stats (used when editing result)
void undo_result(int home, int away, int gh, int ga)
{
    team_played[home]--; team_played[away]--;
    team_gf[home] -= gh; team_ga[home] -= ga;
    team_gf[away] -= ga; team_ga[away] -= gh;

    if (gh > ga)
    {
        team_points[home] -= 3;
        team_wins[home]--;
        team_losses[away]--;
    }
    else if (ga > gh)
    {
        team_points[away] -= 3;
        team_wins[away]--;
        team_losses[home]--;
    }
    else
    {
        team_points[home]--;
        team_points[away]--;
        team_draws[home]--;
        team_draws[away]--;
    }
}

void enter_result()
{
    if (current_day == 0) { cout << "No matchday scheduled.\n"; return; }

    // show unplayed matches
    cout << "\nUnplayed matches - Matchday " << current_day << ":\n";
    bool any = false;
    for (int i = 0; i < total_matches; i++)
    {
        if (match_day[i] == current_day && !match_done[i])
        {
            cout << i+1 << ". " << team_name[match_home[i]]
                 << " vs " << team_name[match_away[i]] << "\n";
            any = true;
        }
    }
    if (!any) { cout << "All matches done.\n"; return; }

    int pick;
    cout << "Enter match number: ";
    cin  >> pick;
    pick--;

    if (pick < 0 || pick >= total_matches || match_day[pick] != current_day)
    {
        cout << "Invalid.\n"; return;
    }
    if (match_done[pick]) { cout << "Already played.\n"; return; }

    int gh, ga;
    cout << team_name[match_home[pick]] << " goals: "; cin >> gh;
    cout << team_name[match_away[pick]] << " goals: "; cin >> ga;

    if (gh < 0 || ga < 0) { cout << "Goals can't be negative.\n"; return; }

    match_goals_home[pick] = gh;
    match_goals_away[pick] = ga;
    match_done[pick] = true;

    apply_result(match_home[pick], match_away[pick], gh, ga);
    save_all();

    char msg[120];
    sprintf(msg, "Result: %s %d-%d %s",
            team_name[match_home[pick]], gh, ga,
            team_name[match_away[pick]]);
    add_log(msg);

    cout << "Result saved.\n";
}

// let admin fix a wrong score
void edit_result()
{
    if (total_matches == 0) { cout << "No matches.\n"; return; }

    cout << "Enter match number to edit (from view matches): ";
    int pick; cin >> pick; pick--;

    if (pick < 0 || pick >= total_matches)
    {
        cout << "Invalid.\n"; return;
    }
    if (!match_done[pick]) { cout << "Not played yet.\n"; return; }

    cout << "Current: " << team_name[match_home[pick]]
         << " " << match_goals_home[pick]
         << " - " << match_goals_away[pick]
         << " " << team_name[match_away[pick]] << "\n";

    int gh, ga;
    cout << "New score - " << team_name[match_home[pick]] << ": "; cin >> gh;
    cout << "New score - " << team_name[match_away[pick]] << ": "; cin >> ga;

    if (gh < 0 || ga < 0) { cout << "Invalid scores.\n"; return; }

    // undo old then apply new
    undo_result(match_home[pick], match_away[pick],
                match_goals_home[pick], match_goals_away[pick]);

    match_goals_home[pick] = gh;
    match_goals_away[pick] = ga;

    apply_result(match_home[pick], match_away[pick], gh, ga);

    save_all();
    add_log("Edited a match result");
    cout << "Score updated.\n";
}

void simulate_day()
{
    if (current_day == 0) { cout << "Schedule a matchday first.\n"; return; }

    bool any = false;
    for (int i = 0; i < total_matches; i++)
    {
        if (match_day[i] != current_day || match_done[i]) continue;

        any = true;
        int gh = rand() % 5;
        int ga = rand() % 5;
        match_goals_home[i] = gh;
        match_goals_away[i] = ga;
        match_done[i] = true;

        apply_result(match_home[i], match_away[i], gh, ga);

        cout << team_name[match_home[i]] << " "
             << gh << " - " << ga << " "
             << team_name[match_away[i]] << "\n";

        char msg[120];
        sprintf(msg, "Simulated: %s %d-%d %s",
                team_name[match_home[i]], gh, ga,
                team_name[match_away[i]]);
        add_log(msg);
    }

    if (!any) cout << "All matches already played.\n";
    else save_all();
}

//  SORTING (for league table)

// returns goal diff
int goal_diff(int i) { return team_gf[i] - team_ga[i]; }

// bubble sort indices by points then GD (descending)
void sort_by_points(int* idx, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            int a = idx[j], b = idx[j+1];
            bool swap_needed = false;

            if (team_points[a] < team_points[b]) swap_needed = true;
            else if (team_points[a] == team_points[b] && goal_diff(a) < goal_diff(b))
                swap_needed = true;

            if (swap_needed)
            {
                int tmp = idx[j];
                idx[j]  = idx[j+1];
                idx[j+1] = tmp;
            }
        }
    }
}

// sort by team name alphabetically
void sort_by_name(int* idx, int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (strcmp(team_name[idx[j]], team_name[idx[j+1]]) > 0)
            {
                int tmp = idx[j]; idx[j] = idx[j+1]; idx[j+1] = tmp;
            }
}


//  DISPLAY FUNCTIONS (user + admin)

void show_league_table(int sort_mode)
{
    if (total_teams == 0) { cout << "No teams.\n"; return; }

    int idx[MAX_TEAMS];
    for (int i = 0; i < total_teams; i++) idx[i] = i;

    if (sort_mode == 1) sort_by_points(idx, total_teams);
    else                sort_by_name(idx, total_teams);

    cout << "\n";
    print_line('=', 65);
    cout << "  LALIGA TABLE\n";
    print_line('=', 65);
    cout << "Pos  Team               MP   W   L   D   GF  GA  GD  Pts\n";
    print_line('-', 65);

    for (int i = 0; i < total_teams; i++)
    {
        int t  = idx[i];
        int gd = goal_diff(t);
        char gd_str[10];
        if (gd >= 0) sprintf(gd_str, "+%d", gd);
        else         sprintf(gd_str, "%d", gd);

        printf(" %-3d %-18s %-4d %-4d %-4d %-4d %-4d %-4d %-4s %-4d\n",
               i+1, team_name[t],
               team_played[t], team_wins[t], team_losses[t], team_draws[t],
               team_gf[t], team_ga[t], gd_str, team_points[t]);
    }
    print_line('=', 65);
}

void view_sort_menu()
{
    int ch;
    cout << "\n1. Sort by Points\n2. Sort by Name\nChoice: ";
    cin  >> ch;

    if (ch == 1 || ch == 2) show_league_table(ch);
    else cout << "Invalid.\n";
}

void show_all_matches()
{
    if (total_matches == 0) { cout << "No matches scheduled.\n"; return; }

    for (int d = 1; d <= current_day; d++)
    {
        cout << "\n-- Matchday " << d << " --\n";
        for (int i = 0; i < total_matches; i++)
        {
            if (match_day[i] != d) continue;
            cout << i+1 << ". "
                 << team_name[match_home[i]] << " vs "
                 << team_name[match_away[i]];
            if (match_done[i])
                cout << "  |  " << match_goals_home[i]
                     << " - " << match_goals_away[i];
            else
                cout << "  |  Not played";
            cout << "\n";
        }
    }
}

void show_current_day()
{
    if (current_day == 0) { cout << "No matchday scheduled.\n"; return; }

    cout << "\n-- Matchday " << current_day << " --\n";
    for (int i = 0; i < total_matches; i++)
    {
        if (match_day[i] != current_day) continue;
        cout << team_name[match_home[i]] << " vs " << team_name[match_away[i]];
        if (match_done[i])
            cout << "  |  " << match_goals_home[i] << " - " << match_goals_away[i];
        else
            cout << "  |  Upcoming";
        cout << "\n";
    }
}

// show logs
void show_logs()
{
    if (total_logs == 0) { cout << "No logs yet.\n"; return; }
    cout << "\n-- Activity Log --\n";
    for (int i = 0; i < total_logs; i++)
        cout << i+1 << ". " << log_lines[i] << "\n";
}


//  SEARCH FUNCTIONS

void search_team_stats()
{
    if (total_teams == 0) { cout << "No teams.\n"; return; }

    char kw[50];
    cout << "Enter team name: ";
    read_line(kw, 50);

    int idx = find_team(kw);
    if (idx == -1)
    {
        // try partial
        cout << "Exact match not found. Searching partial...\n\n";
        bool any = false;
        for (int i = 0; i < total_teams; i++)
        {
            if (contains(team_name[i], kw))
            {
                idx = i; any = true;
                cout << "Found: " << team_name[i] << "\n";
            }
        }
        if (!any) { cout << "No team found.\n"; return; }
        if (idx == -1) return;
    }

    // calculate position
    int pos = 1;
    for (int i = 0; i < total_teams; i++)
    {
        if (i == idx) continue;
        if (team_points[i] > team_points[idx] ||
           (team_points[i] == team_points[idx] && goal_diff(i) > goal_diff(idx)))
            pos++;
    }

    int gd = goal_diff(idx);
    char gd_str[10];
    if (gd >= 0) sprintf(gd_str, "+%d", gd);
    else         sprintf(gd_str, "%d", gd);

    print_line('=', 40);
    cout << "  " << team_name[idx] << "\n";
    print_line('-', 40);
    cout << "  Position     : " << pos << " / " << total_teams << "\n";
    cout << "  Played       : " << team_played[idx]  << "\n";
    cout << "  Wins         : " << team_wins[idx]    << "\n";
    cout << "  Draws        : " << team_draws[idx]   << "\n";
    cout << "  Losses       : " << team_losses[idx]  << "\n";
    cout << "  Goals For    : " << team_gf[idx]      << "\n";
    cout << "  Goals Against: " << team_ga[idx]      << "\n";
    cout << "  Goal Diff    : " << gd_str            << "\n";
    cout << "  Points       : " << team_points[idx]  << "\n";

    if (team_played[idx] > 0)
    {
        int wr = (team_wins[idx] * 100) / team_played[idx];
        cout << "  Win Rate     : " << wr << "%\n";
    }

    print_line('-', 40);
    cout << "  Match History:\n";
    bool got_one = false;
    for (int i = 0; i < total_matches; i++)
    {
        if (!match_done[i]) continue;
        if (match_home[i] != idx && match_away[i] != idx) continue;

        got_one = true;
        int gf, ga;
        char opp[50];
        if (match_home[i] == idx)
        {
            gf = match_goals_home[i]; ga = match_goals_away[i];
            strcpy(opp, team_name[match_away[i]]);
        }
        else
        {
            gf = match_goals_away[i]; ga = match_goals_home[i];
            strcpy(opp, team_name[match_home[i]]);
        }

        char res = (gf > ga) ? 'W' : (gf < ga) ? 'L' : 'D';
        cout << "  MD" << match_day[i] << "  " << res
             << "  " << gf << "-" << ga << "  vs " << opp << "\n";
    }
    if (!got_one) cout << "  No results yet.\n";
    print_line('=', 40);

    add_log("Searched team stats");
}

void head_to_head()
{
    if (total_teams < 2) { cout << "Need 2 teams.\n"; return; }

    char n1[50], n2[50];
    cout << "Team 1: "; read_line(n1, 50);
    cout << "Team 2: "; read_line2(n2, 50);

    int i1 = find_team(n1), i2 = find_team(n2);
    if (i1 == -1) { cout << n1 << " not found.\n"; return; }
    if (i2 == -1) { cout << n2 << " not found.\n"; return; }
    if (i1 == i2) { cout << "Same team.\n"; return; }

    int w1 = 0, w2 = 0, dr = 0, gf1 = 0, gf2 = 0;

    print_line('=', 44);
    cout << "  H2H: " << team_name[i1] << " vs " << team_name[i2] << "\n";
    print_line('-', 44);

    bool any = false;
    for (int i = 0; i < total_matches; i++)
    {
        bool match = (match_home[i] == i1 && match_away[i] == i2) ||
                     (match_home[i] == i2 && match_away[i] == i1);
        if (!match) continue;

        if (!match_done[i])
        {
            cout << "  MD" << match_day[i] << "  "
                 << team_name[match_home[i]] << " vs "
                 << team_name[match_away[i]] << " (not played)\n";
            continue;
        }

        any = true;
        int g1, g2;
        if (match_home[i] == i1) { g1 = match_goals_home[i]; g2 = match_goals_away[i]; }
        else                     { g1 = match_goals_away[i];  g2 = match_goals_home[i]; }

        gf1 += g1; gf2 += g2;
        char res[30];
        if      (g1 > g2) { strcpy(res, team_name[i1]); strcat(res, " win"); w1++; }
        else if (g2 > g1) { strcpy(res, team_name[i2]); strcat(res, " win"); w2++; }
        else              { strcpy(res, "Draw"); dr++; }

        cout << "  MD" << match_day[i] << "  "
             << team_name[i1] << " " << g1 << " - " << g2
             << " " << team_name[i2] << "  (" << res << ")\n";
    }

    if (!any) { cout << "  No results yet.\n"; }
    else
    {
        print_line('-', 44);
        cout << "  " << team_name[i1] << " wins : " << w1 << "\n";
        cout << "  Draws             : " << dr << "\n";
        cout << "  " << team_name[i2] << " wins : " << w2 << "\n";
        cout << "  Goals: " << team_name[i1] << " " << gf1
             << " - " << gf2 << " " << team_name[i2] << "\n";
        cout << "  Leader: ";
        if      (w1 > w2) cout << team_name[i1] << "\n";
        else if (w2 > w1) cout << team_name[i2] << "\n";
        else              cout << "Even\n";
    }
    print_line('=', 44);
    add_log("Viewed H2H stats");
}


//  ADMIN ACCOUNT MANAGEMENT

int find_admin(const char* uname)
{
    for (int i = 0; i < total_admins; i++)
        if (same_str(admin_user[i], uname))
            return i;
    return -1;
}

void change_username()
{
    char new_name[30];
    cout << "New username: ";
    read_line(new_name, 30);

    if (strlen(new_name) < 3) { cout << "Too short.\n"; return; }
    if (find_admin(new_name) != -1) { cout << "Taken.\n"; return; }

    char msg[80];
    sprintf(msg, "Username changed: %s -> %s", admin_user[logged_in_admin], new_name);
    strcpy(admin_user[logged_in_admin], new_name);
    save_admins();
    add_log(msg);
    cout << "Username changed.\n";
}

void change_password()
{
    char old_p[30], new_p[30], confirm[30];

    cout << "Current password: ";
    read_line(old_p, 30);

    if (strcmp(old_p, admin_pass[logged_in_admin]) != 0)
    {
        cout << "Wrong password.\n"; return;
    }

    cout << "New password: ";
    read_line2(new_p, 30);
    cout << "Confirm: ";
    read_line2(confirm, 30);

    if (strcmp(new_p, confirm) != 0) { cout << "Don't match.\n"; return; }
    if (strlen(new_p) < 4) { cout << "Min 4 chars.\n"; return; }

    strcpy(admin_pass[logged_in_admin], new_p);
    save_admins();
    add_log("Admin changed password");
    cout << "Password changed.\n";
}

void account_settings()
{
    int ch;
    cout << "\n1. Change Username\n2. Change Password\nChoice: ";
    cin  >> ch;
    if      (ch == 1) change_username();
    else if (ch == 2) change_password();
    else              cout << "Invalid.\n";
}


//  LOGIN / AUTH

// forgot password using secret answer
void forgot_password()
{
    char uname[30];
    cout << "Your username: ";
    read_line(uname, 30);

    int idx = find_admin(uname);
    if (idx == -1) { cout << "Not found.\n"; return; }

    cout << "Secret answer: ";
    char ans[40];
    read_line2(ans, 40);

    if (!same_str(ans, admin_secret[idx]))
    {
        cout << "Wrong answer.\n"; return;
    }

    char np[30], confirm[30];
    cout << "New password: ";
    read_line2(np, 30);
    cout << "Confirm: ";
    read_line2(confirm, 30);

    if (strcmp(np, confirm) != 0) { cout << "Don't match.\n"; return; }
    if (strlen(np) < 4) { cout << "Too short.\n"; return; }

    strcpy(admin_pass[idx], np);
    save_admins();
    add_log("Used forgot password");
    cout << "Password reset! Login again.\n";
}

// after 3 failed attempts - use secret key from file
void admin_key_recovery()
{
    cout << "\n-- Emergency Recovery --\n";
    cout << "Enter secret key from " << KEY_FILE << ": ";
    char key[50];
    read_line(key, 50);

    if (!verify_secret_key(key))
    {
        cout << "Wrong key. Access denied.\n";
        return;
    }

    cout << "Key correct!\n";
    cout << "Which admin to reset: ";
    char uname[30];
    read_line2(uname, 30);

    int idx = find_admin(uname);
    if (idx == -1) { cout << "Admin not found.\n"; return; }

    char np[30], confirm[30];
    cout << "New password: ";
    read_line2(np, 30);
    cout << "Confirm: ";
    read_line2(confirm, 30);

    if (strcmp(np, confirm) != 0) { cout << "Don't match.\n"; return; }

    strcpy(admin_pass[idx], np);
    save_admins();
    add_log("Used key recovery to reset admin password");
    cout << "Password reset successfully!\n";
}

// returns true if login worked
bool do_admin_login()
{
    char uname[30], pass[30];
    int tries = 0;

    while (tries < 3)
    {
        cout << "Username: "; cin >> uname;
        cout << "Password: "; cin >> pass;

        int idx = find_admin(uname);
        if (idx != -1 && strcmp(pass, admin_pass[idx]) == 0)
        {
            logged_in_admin = idx;
            return true;
        }

        tries++;
        cout << "Wrong! Attempt " << tries << "/3\n";
    }

    cout << "3 failed attempts.\n";

    char ch;
    cout << "Use emergency key recovery? (y/n): ";
    cin  >> ch;
    if (ch == 'y' || ch == 'Y') admin_key_recovery();

    return false;
}


//  MENUS

void banner()
{
    cout << R"(
  _               _      _____ _____
 | |        /\   | |    |_   _/ ____|   /\
 | |       /  \  | |      | || |  __   /  \
 | |      / /\ \ | |      | || | |_ | / /\ \
 | |____ / ____ \| |____ _| || |__| |/ ____ \
 |______/_/    \_\______|_____\_____/_/    \_\
)";
    cout << "  Welcome to LaLiga Management System\n\n";
}

void admin_menu()
{
    int ch = 0;
    while (ch != 11)
    {
        cls();
        cout << "==== ADMIN PANEL [" << admin_user[logged_in_admin] << "] ====\n";
        cout << " 1. Add Team\n";
        cout << " 2. Edit Team Name\n";
        cout << " 3. Remove/Relegate Team\n";
        cout << " 4. Schedule Matchday\n";
        cout << " 5. Enter Match Result\n";
        cout << " 6. Edit Match Result\n";
        cout << " 7. Simulate Matchday\n";
        cout << " 8. View Activity Logs\n";
        cout << " 9. Account Settings\n";
        cout << "10. View League Table\n";
        cout << "11. Logout\n";
        cout << "Choice: ";
        cin  >> ch;

        if      (ch == 1)  add_team();
        else if (ch == 2)  edit_team();
        else if (ch == 3)  delete_team();
        else if (ch == 4)  schedule_matchday();
        else if (ch == 5)  enter_result();
        else if (ch == 6)  edit_result();
        else if (ch == 7)  simulate_day();
        else if (ch == 8)  show_logs();
        else if (ch == 9)  account_settings();
        else if (ch == 10) show_league_table(1);
        else if (ch == 11) { logged_in_admin = -1; add_log("Logged out"); }
        else               cout << "Invalid.\n";

        if (ch != 11) pause_key();
    }
}

void user_menu()
{
    int ch = 0;
    while (ch != 7)
    {
        cls();
        cout << "==== LaLiga - Fan View ====\n";
        cout << "1. League Table\n";
        cout << "2. All Matches\n";
        cout << "3. Current Matchday\n";
        cout << "4. Search Team Stats\n";
        cout << "5. Head-to-Head\n";
        cout << "6. Sort Options\n";
        cout << "7. Back\n";
        cout << "Choice: ";
        cin  >> ch;

        if      (ch == 1) show_league_table(1);
        else if (ch == 2) show_all_matches();
        else if (ch == 3) show_current_day();
        else if (ch == 4) search_team_stats();
        else if (ch == 5) head_to_head();
        else if (ch == 6) view_sort_menu();
        else if (ch == 7) { /* back */ }
        else              cout << "Invalid.\n";

        if (ch != 7) pause_key();
    }
}


//  MAIN

int main()
{
    srand(42);

    // load saved data
    load_admins();
    load_teams();
    load_matches();
    load_logs();
    init_key_file();

    // first run - make default admin
    if (total_admins == 0)
    {
        strcpy(admin_user[0],   "admin");
        strcpy(admin_pass[0],   "1234");
        strcpy(admin_secret[0], "laliga");
        total_admins = 1;
        save_admins();
    }

    // default teams if none
    if (total_teams == 0)
    {
        const char* defaults[] = {"Barca", "Madrid", "Atletico madrid", "Bilbao", "Girona", "Betis"};
        for (int i = 0; i < 6; i++)
        {
            strcpy(team_name[i], defaults[i]);
            reset_team_stats(i);
        }
        total_teams = 6;
        save_teams();
    }

    int choice = 0;
    while (choice != 4)
    {
        cls();
        banner();
        cout << "1. Admin Login\n";
        cout << "2. View as Fan\n";
        cout << "3. Forgot Password\n";
        cout << "4. Exit\n";
        cout << "Choice: ";
        cin  >> choice;

        if (choice == 1)
        {
            cls(); banner();
            if (do_admin_login())
            {
                add_log("Admin logged in");
                admin_menu();
            }
        }
        else if (choice == 2)
        {
            user_menu();
        }
        else if (choice == 3)
        {
            cls();
            forgot_password();
            pause_key();
        }
        else if (choice == 4)
        {
            cout << "Bye!\n";
        }
        else
        {
            cout << "Invalid.\n";
            pause_key();
        }
    }

    return 0;
}