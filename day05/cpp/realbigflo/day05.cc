#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

class Seat {
public:
  Seat(const std::string_view view) : m_row(0), m_seat(0), m_seat_id(0) {
    std::string seat_id_string(view);
    std::replace(seat_id_string.begin(), seat_id_string.end(), 'B', '1');
    std::replace(seat_id_string.begin(), seat_id_string.end(), 'F', '0');
    std::replace(seat_id_string.begin(), seat_id_string.end(), 'R', '1');
    std::replace(seat_id_string.begin(), seat_id_string.end(), 'L', '0');
    this->m_seat_id = std::stoi(seat_id_string, 0, 2);
    this->m_row = this->m_seat_id >> 3;
    this->m_seat = this->m_seat_id % 8;
  }

  unsigned int get_seat_id() const { return this->m_seat_id; }

  std::string to_string() const {
    return "Row: '" + std::to_string(this->m_row) + "' - Seat: '" +
           std::to_string(this->m_seat) + "' (" +
           std::to_string(this->get_seat_id()) + ")";
  }

protected:
  unsigned int m_row;
  unsigned int m_seat;
  unsigned int m_seat_id;
};

std::vector<std::unique_ptr<Seat>> get_seats_file() {
  std::fstream in_file;
  std::string line;
  std::vector<std::unique_ptr<Seat>> seats;
  in_file.open("input.txt", ::std::ios_base::in);
  while (getline(in_file, line)) {
    if (line.size() > 0) {
      seats.push_back(std::make_unique<Seat>(line));
    }
  }
  return seats;
}

int main() {
  auto seats = get_seats_file();
  unsigned int highest_id = 0;
  std::for_each(seats.cbegin(), seats.cend(),
                [&highest_id](const std::unique_ptr<Seat> &seat) {
                  if (highest_id < seat->get_seat_id()) {
                    highest_id = seat->get_seat_id();
                  }
                });
  std::cout << "Highest id: " << highest_id << "\n";
  std::vector<int> all_ids;
  for (int i = 0; i < 1024; ++i) {
    all_ids.push_back(i);
  }
  std::for_each(seats.cbegin(), seats.cend(),
                [&all_ids](const std::unique_ptr<Seat> &seat) {
                  auto iter = all_ids.begin();
                  do {
                    if (*iter == seat->get_seat_id()) {
                      all_ids.erase(iter);
                      break;
                    }
                    ++iter;
                  } while (iter != all_ids.end());
                });
  auto iter = all_ids.begin();
  do {
    if (iter + 1 != all_ids.end() && (*iter) + 1 == *(iter + 1)) {
      all_ids.erase(iter + 1);
      all_ids.erase(iter);
      iter = all_ids.begin();
    } else {
      ++iter;
    }
  } while (iter != all_ids.end());
  std::for_each(all_ids.cbegin(), all_ids.cend(),
                [](int i) { std::cout << "I: " << i << "\n"; });
}