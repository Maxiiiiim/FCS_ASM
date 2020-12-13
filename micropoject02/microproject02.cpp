/*
    Минец Максим
    БПИ-193
    Вариант 15

    Задача о гостинице - 3 (дамы и джентльмены). В гостинице 10 номеров рассчитаны на 
    одного человека и 15 номеров рассчитаны на двух человек. В гостиницу приходят 
    клиенты дамы и клиенты джентльмены, и конечно они могут провести ночь в номере 
    только с представителем своего пола. Если для клиента не находится подходящего 
    номера, он уходит искать ночлег в другое место. Создать многопоточное приложение, 
    моделирующее работу гостиницы.
*/

#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <chrono>
#include <random>

using namespace std;

constexpr int SINGLE_ROOMS = 10, DOUBLE_ROOMS = 15;
int female = -1, male = -1, lastRoom = -1, counter = 0;
mutex mut;

char* singleRoom = new char[SINGLE_ROOMS]; // NOLINT(cert-err58-cpp)
pair<char, char>* doubleRoom = new pair<char, char>[DOUBLE_ROOMS]; // NOLINT(cert-err58-cpp)

int settleIntoDoubleRoom(bool man) {
    int last, opp;
    char client;
    if (man) {
        last = male;
        opp = female;
        client = 'G';
    }
    else {
        last = female;
        opp = male;
        client = 'L';
    }

    if (last != -1) {
        if (doubleRoom[last].second == 'x')
            doubleRoom[last].second = client;
        else {
            ++last;
            while (doubleRoom[last].second != 'x' || last == opp) ++last;

            doubleRoom[last].first = client;
        }
    }
    else {
        ++last;
        while (doubleRoom[last].second != 'x' || last == opp)
            ++last;
        doubleRoom[last].first = client;
    }

    return last;
}

void printRoomNumber(const string& person, const string& type, const int& num) {
    string pronoun;
    if (person == "Gentleman")
        pronoun = "He";
    else
        pronoun = "She";

    cout << pronoun << " settled into a " << type << " room " << num + 1 <<
        "! (Thread id_" << this_thread::get_id() << ")\n";
}

void checkedInSingleRoom(const string& person) {
    if (lastRoom >= SINGLE_ROOMS - 1) {
        mut.lock();
        ++counter;
        mut.unlock();
        cout << "There are no free rooms. " << person <<
            " have to go away and find another place to sleep (Thread id_" << this_thread::get_id() << ")\n";
        return;
    }

    mut.lock();
    if (person == "Gentleman")
        singleRoom[++lastRoom] = 'G';
    else
        singleRoom[++lastRoom] = 'L';
    mut.unlock();

    printRoomNumber(person, "single", lastRoom);
}

void checkedInDoubleRoom(const string& person) {
    mut.lock();
    if (person == "Gentleman")
        male = settleIntoDoubleRoom(true);
    else
        female = settleIntoDoubleRoom(false);
    mut.unlock();

    printRoomNumber(person, "double", (person == "Gentleman" ? male : female));
}

void reception(const string& person) {
    cout << person + " would like to book a room! ";
    int lastCurrent, opposite;

    if (person[0] != 'L') {
        lastCurrent = male;
        opposite = female;
    }
    else {
        lastCurrent = female;
        opposite = male;
    }

    if (lastCurrent < DOUBLE_ROOMS && (doubleRoom[DOUBLE_ROOMS - 1].second == 'x' &&
        opposite != 14 || doubleRoom[lastCurrent].second == 'x'))
        checkedInDoubleRoom(person);
    else
        checkedInSingleRoom(person);
}

void arrivalOfGuest() {
    random_device rnd;
    mt19937 mersenne(rnd());

    while (singleRoom[SINGLE_ROOMS - 1] == 'x' || doubleRoom[DOUBLE_ROOMS - 1].second == 'x') {
        auto t = new thread(reception, mersenne() % 2 == 0 ? "Gentleman" : "Lady");
        t->join();
    }
}

void printHotel(const string& flag) {
    cout << "  _____________________\n";
    cout << " /    Four Seasons     \\\n";
    cout << "/-----------------------\\\n";


    for (int i = 0; i < SINGLE_ROOMS; ++i) {
        if (i == 4)
            cout << "| Cafe  |\n|-----------------------|\n";
        cout << "| " << singleRoom[i] << " ";
    }

    cout << "|\n|-----------------------|\n";

    for (int i = 0; i < 15; ++i) {
        if (i != 0 && i % 4 == 0)
            cout << "|\n|-----------------------|\n";

        cout << "| " << doubleRoom[i].first << '_' << doubleRoom[i].second << ' ';

    }

    cout << "| Rec |\n\n";

    if (flag == "full")
    if (counter == 0)
        cout << "We catered all guests!\n";
    else
        cout << "We did not manage to cater " << counter << " visitors...\n";
}

void emptyHotel() {
    for (int i = 0; i < SINGLE_ROOMS; ++i)
        singleRoom[i] = 'x';

    for (int i = 0; i < DOUBLE_ROOMS; ++i)
        doubleRoom[i] = { 'x', 'x' };

    printHotel("empty");
}

string checkGuestInput() {
    string check;
    cin >> check;

    while (check != "L" && check != "G" && check != "l" && check != "g") {
        cout << "Incorrect input. You should input 'G' or 'L' (Gentleman and Lady respectively): ";
        cin >> check;
    }

    if (check == "G" || check == "g")
        check = "Gentleman";
    else
        check = "Lady";

    return check;
}

void inputFromConsole() {
    string guest;
    int i = 1;
    while (singleRoom[SINGLE_ROOMS - 1] == 'x' || doubleRoom[DOUBLE_ROOMS - 1].second == 'x') {
        cout << "Input " << i++ << " guest (letter 'L' or 'G'): ";
        guest = checkGuestInput();
        auto t = new thread(reception, guest);
        t->join();
    }
}

int checkChoiceInput() {
    int check;
    cin >> check;
    while (check != 1 && check != 2) {
        cout << "Incorrect input. You should input '1' or '2': ";
        cin >> check;
    }
    return check;
}

void chooseInputMethod() {
    cout << "Choose the way of generating guests:\n" <<
        "1) Generate randomly;\n" <<
        "2) Input each guest with the help of console.\n" <<
        "Input your choice here: ";

    int choice = checkChoiceInput();

    if (choice == 1)
        arrivalOfGuest();
    else
        inputFromConsole();
}


int main() {
    emptyHotel();
    chooseInputMethod();
    printHotel("full");
    return 0;
}