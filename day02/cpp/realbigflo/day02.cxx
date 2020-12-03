#include <algorithm>
#include <charconv>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

class PasswordValidator {
public:
  const char m_letter;
  const int m_minimum;
  const int m_maximum;
  const std::string m_password;

  PasswordValidator(char letter, int minimum, int maximum,
                    const std::string &password)
      : m_letter(letter), m_minimum(minimum), m_maximum(maximum),
        m_password(password) {
    auto counter = std::count(password.cbegin(), password.cend(), m_letter);
    this->m_password_is_valid = counter >= m_minimum && counter <= m_maximum;
  }

  bool is_password_valid() const { return this->m_password_is_valid; }

protected:
  bool m_password_is_valid;
};

class PasswordValidator_2 : public PasswordValidator {
public:
  PasswordValidator_2(char letter, int minimum, int maximum,
                      const std::string &password)
      : PasswordValidator(letter, minimum, maximum, password) {
    this->m_password_is_valid = m_maximum <= password.size() &&
                                ((password.at(m_minimum - 1) == m_letter &&
                                  password.at(m_maximum - 1) != letter) ||
                                 (password.at(m_minimum - 1) != m_letter &&
                                  password.at(m_maximum - 1) == letter));
  }
};

PasswordValidator get_validator_from_line(const std::string_view &line,
                                          int policy) {
  int dash_pos = line.find("-");
  int space_pos = line.find(" ");
  int colon_pos = line.find(":");

  int min = 0;
  int max = 0;
  char letter = '\0';

  std::from_chars(line.data(), line.data() + dash_pos, min);
  std::from_chars(line.data() + dash_pos + 1, line.data() + space_pos, max);
  letter = line.substr(space_pos + 1, colon_pos - space_pos + 1).data()[0];
  std::string password;
  for (const char letter : line.substr(colon_pos + 2)) {
    password.push_back(letter);
  }
  if (policy == 0) {
    return PasswordValidator(letter, min, max, password);
  } else {
    return PasswordValidator_2(letter, min, max, password);
  }
}

int main() {
  int valid_counter = 0;
  int valid_counter_2 = 0;
  std::fstream in_file;
  std::string line;
  in_file.open("input.txt", ::std::ios_base::in);
  while (getline(in_file, line)) {
    auto validator = get_validator_from_line(line, 0);
    if (validator.is_password_valid()) {
      ++valid_counter;
    }
    auto validator_2 = get_validator_from_line(line, 1);
    if (validator_2.is_password_valid()) {
      ++valid_counter_2;
    }
  }
  std::cout << "Valid counter: " << valid_counter << "\n";
  std::cout << "Valid counter 2: " << valid_counter_2 << "\n";
  return 0;
}
