#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <vector>

class Group {
public:
  Group(std::string_view view) : m_answers_yes(), m_all_answers_yes() {
    std::for_each(view.cbegin(), view.cend(), [this](const char c) {
      if (c != '+')
        this->m_answers_yes.insert(c);
    });
    auto next_pos_old = view.find("+");
    std::string_view ref = view.substr(0, next_pos_old);
    std::for_each(ref.cbegin(), ref.cend(),
                  [this](const char c) { this->m_all_answers_yes.insert(c); });
    auto next_pos = view.find("+", next_pos_old + 1);
    while (next_pos != view.npos) {
      std::string_view comp =
          view.substr(next_pos_old, next_pos - next_pos_old + 1);
      std::set<char> comp_set;
      std::for_each(comp.cbegin(), comp.cend(),
                    [this, &comp_set](const char c) {
                      if (this->m_all_answers_yes.find(c) !=
                          this->m_all_answers_yes.end())
                        comp_set.insert(c);
                    });
      this->m_all_answers_yes = comp_set;
      if (this->m_all_answers_yes.size() == 0)
        break;
      next_pos_old = next_pos + 1;
      next_pos = view.find("+", next_pos_old);
    }
  }

  std::set<char> m_answers_yes;
  std::set<char> m_all_answers_yes;
};

std::vector<std::unique_ptr<Group>> get_answers() {
  std::fstream in_file;
  std::string line;
  std::vector<std::unique_ptr<Group>> groups;
  in_file.open("input.txt", ::std::ios_base::in);
  std::string temp_line;
  while (getline(in_file, line)) {
    if (line.size() == 0) {
      groups.push_back(std::make_unique<Group>(temp_line));
      temp_line = "";
    } else {
      temp_line += line + "+";
    }
  }
  groups.push_back(std::make_unique<Group>(temp_line));
  return groups;
}

int main() {
  auto answers = get_answers();
  int sum = 0;
  int all = 0;
  std::for_each(answers.cbegin(), answers.cend(),
                [&sum, &all](const std::unique_ptr<Group> &group) {
                  sum += group->m_answers_yes.size();
                  all += group->m_all_answers_yes.size();
                });
  std::cout << sum << "\n";
  std::cout << all << "\n";
}