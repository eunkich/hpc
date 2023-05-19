#include <iostream> // std::cout
#include <fstream>  // std::ofstream
#include <thread>   // std::thread, std::this_thread::sleep_for
#include <chrono>   // std::chrono::seconds
#include <random>
#include <vector>
#include <deque>
#include <set>

const int MAX_TIME_OUT = 20;

const int num_floors = 20;
int num_clients = 100;
int capacity = 10;
int done = 0;

// For selecting random loc and dest of passengers
std::mt19937_64 rng(std::random_device{}());
std::uniform_int_distribution<int> dist(0, num_floors);

std::vector<int> elev_loc = {0, 0, 0};
std::vector<int> elev_dir = {0, 0, 0};
std::vector<int> elev_pass = {0, 0, 0};
std::vector<int> elev_pass_cum = {0, 0, 0};

std::set<int> dummy;
std::vector<std::set<int>> elev_pass_id(3, dummy);
std::vector<std::set<int>> elev_dest(3, dummy);
std::vector<int> apt(num_floors, 0);
std::set<int> uniq_call_up;
std::set<int> uniq_call_down;

std::mutex m_data, m_cout;
std::condition_variable cv;

void client(int id)
{
    int start = dist(rng);
    int end = dist(rng);
    while (end == start)
    {
        int end = dist(rng);
    }
    bool requested = false;

    int dir = (end - start) / abs(end - start);

    // uniform random arrival 0 ~ 5 sec.
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng) / 20 * 1000 * 5));

    if (not requested) // call elev
    {
        m_cout.lock();
        std::cout << "(TID:" << std::this_thread::get_id() << ") "
                  << "Person " << id
                  << " wants to go from floor " << start
                  << " to floor " << end << "\n";
        m_cout.unlock();

        {
            std::unique_lock<std::mutex> lock(m_data);
            apt[start]++;
            if (end - start > 0)
            {
                uniq_call_up.insert(start);
            }
            else if (end - start < 0)
            {
                uniq_call_down.insert(start);
            }
            else
            {
                done += 1;
                m_data.unlock();
                cv.notify_all();
                return;
            }
        }

        requested = true;
    }

    int eid;

    // Wait to enter
    auto check_enter = [&]()
    {
        for (int i = 0; i < 3; i++)
        {
            if (elev_loc[i] == start and (elev_dir[i] == dir or elev_dir[i] == 0))
            {
                if (elev_pass[i] < capacity)
                {
                    eid = i;
                    return true;
                }
            }
        }

        // call again
        if (end - start > 0)
        {
            uniq_call_up.insert(start);
        }
        else if (end - start < 0)
        {
            uniq_call_down.insert(start);
        }

        return false;
    };

    {
        std::unique_lock<std::mutex> lock(m_data);
        cv.wait(lock, check_enter);
        // onboard
        elev_pass[eid]++;
        elev_pass_cum[eid]++;
        elev_pass_id[eid].insert(id);
        apt[start]--;
        // press button
        elev_dest[eid].insert(end);
    }
    cv.notify_all();

    m_cout.lock();
    std::cout << "(TID:" << std::this_thread::get_id() << ") "
              << "Person " << id << " entered Elevator " << eid << "\n";
    m_cout.unlock();

    auto check_exit = [&]()
    { return (elev_loc[eid] == end); };

    // Wait to exit
    {
        std::unique_lock<std::mutex> lock(m_data);
        cv.wait(lock, check_exit);
        elev_pass[eid]--;
        elev_pass_id[eid].erase(id);
        done++;
    }
    cv.notify_all();

    m_cout.lock();
    std::cout << "(TID:" << std::this_thread::get_id() << ") "
              << "Person " << id << " exited Elevator " << eid << "\n";
    m_cout.unlock();

    return;
}

void elevator(int id)
{
    int min_dist, nearest, highest_down_call, lowest_up_call, select;
    int loc, dir;
    std::set<int> dest;

    // main loop
    while (true)
    {
        {
            std::unique_lock<std::mutex> ulock(m_data);
            loc = elev_loc[id];
            dir = elev_dir[id];
            dest = elev_dest[id];
            if (dest.empty() and (!uniq_call_up.empty() or !uniq_call_down.empty()))
            {
                min_dist = num_floors;
                highest_down_call = -1;
                lowest_up_call = -1;

                if (!uniq_call_up.empty())
                {
                    lowest_up_call = *uniq_call_up.begin();
                    nearest = lowest_up_call;
                    select = 0;
                }

                if (!uniq_call_down.empty())
                {
                    highest_down_call = *(uniq_call_down.rbegin());
                    nearest = highest_down_call;
                    select = 1;
                }

                if (!uniq_call_up.empty() and !uniq_call_down.empty())
                {
                    int idx = 0;
                    for (auto f : {lowest_up_call, highest_down_call})
                    {
                        if (abs(f - loc) < min_dist)
                        {
                            nearest = f;
                            min_dist = abs(f - loc);
                            select = idx;
                        }
                        idx++;
                    }
                }

                (select == 0) ? uniq_call_up.erase(nearest) : uniq_call_down.erase(nearest);
                dest.insert(nearest);
            }

            if (dest.empty())
            {
                dir = 0;
            }
            else // find dir
            {
                min_dist = num_floors;
                if (elev_pass[id] == 0) // remove invalid req
                {
                    std::set<int> to_erase;
                    for (auto f : dest)
                    {
                        if ((to_erase.find(f) == to_erase.end()) and (apt[f] == 0))
                        {
                            to_erase.insert(f);
                        }
                    }
                    for (auto f : to_erase)
                    {
                        dest.erase(f);
                    }
                    to_erase.clear();
                }

                for (auto f : dest)
                {
                    if (abs(f - loc) < min_dist)
                    {
                        nearest = f;
                        min_dist = abs(f - loc);
                    }
                }
                dir = (nearest - loc) / abs(nearest - loc);
            }

            m_cout.lock();
            if (dir)
            {
                std::cout << "(TID:" << std::this_thread::get_id() << ") "
                          << "Elevator " << id
                          << " moving from " << loc
                          << " to " << nearest << std::endl;
            }

            m_cout.unlock();

            loc += dir;

            // check arrive
            if (dest.find(loc) != dest.end())
            {
                dest.erase(loc);
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }

            // update status
            elev_loc[id] = loc;
            elev_dir[id] = dir;
            elev_dest[id] = dest;
        }
        cv.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

void watch()
{
    std::lock_guard<std::mutex> lock(m_data);
    std::ofstream myfile;
    myfile.open("monitor.txt", std::ios_base::app);

    myfile << "------------------------\n"
           << "| (TID:" << std::this_thread::get_id() << ")    |\n"
           << "------------------------\n"
           << " curr loc   : ("
           << elev_loc[0] << ", "
           << elev_loc[1] << ", "
           << elev_loc[2] << ") \n"
           << " curr dir   : ("
           << elev_dir[0] << ", "
           << elev_dir[1] << ", "
           << elev_dir[2] << ") \n"
           << " curr pass  : ("
           << elev_pass[0] << ", "
           << elev_pass[1] << ", "
           << elev_pass[2] << ") \n"
           << " cum pass   : ("
           << elev_pass_cum[0] << ", "
           << elev_pass_cum[1] << ", "
           << elev_pass_cum[2] << ") \n";

    myfile << " done       : " << done << "\n";
    for (int i = 0; i < 3; i++)
    {
        myfile << " elev " << i << " pass: {";
        for (auto &e : elev_pass_id[i])
        {
            myfile << e << ", ";
        }
        myfile << "}\n";
    }
    myfile << " pd up req  : {";
    for (auto &e : uniq_call_up)
    {
        myfile << e << ", ";
    }
    myfile << "}\n";

    myfile << " pd down req: {";
    for (auto &e : uniq_call_down)
    {
        myfile << e << ", ";
    }
    myfile << "}\n";

    myfile << "------------------------\n";

    // draw apartment
    for (int i = num_floors; i > 0; i--)
    {
        myfile << " ";
        myfile.width(2);
        myfile << i << "  [";
        myfile.width(2);
        myfile << apt[i] << "]  ";
        (elev_loc[0] == i) ? myfile << "[" << elev_pass[0] << "]" : myfile << "   ";
        (elev_loc[1] == i) ? myfile << "[" << elev_pass[1] << "]" : myfile << "   ";
        (elev_loc[2] == i) ? myfile << "[" << elev_pass[2] << "]" : myfile << "   ";
        myfile << "\n";
    }
    myfile << std::endl;

    myfile.close();
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: %s [num_clients] [capacity]\n", argv[0]);
        return 1;
    }
    num_clients = atoi(argv[1]);
    capacity = atoi(argv[2]);

    // Spawn clients
    std::vector<std::thread> threads(num_clients);
    for (int i = 0; i < num_clients; i++)
    {
        threads[i] = std::thread(client, i);
        threads[i].detach();
    }

    std::thread t0 = std::thread(elevator, 0);
    std::thread t1 = std::thread(elevator, 1);
    std::thread t2 = std::thread(elevator, 2);
    t0.detach();
    t1.detach();
    t2.detach();

    using fps = std::chrono::duration<double, std::ratio<1, 5>>;
    std::ofstream myfile;
    myfile.open("monitor.txt");

    int t = 0;
    while (t < 10 * MAX_TIME_OUT)
    {
        // Monitor progress
        watch();
        std::this_thread::sleep_for(fps(1));
        t++;
        {
            std::unique_lock<std::mutex> lock(m_data);
            if (done == num_clients)
            {
                break;
            }
        }
    }
    std::cout << "Job completed!" << std::endl;
}