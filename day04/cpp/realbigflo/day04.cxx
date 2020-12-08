#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

/*

    byr (Birth Year)
    iyr (Issue Year)
    eyr (Expiration Year)
    hgt (Height)
    hcl (Hair Color)
    ecl (Eye Color)
    pid (Passport ID)
    cid (Country ID)

Passport data is validated in batch files (your puzzle input). Each passport is
represented as a sequence of key:value pairs separated by spaces or newlines.
Passports are separated by blank lines.

Here is an example batch file containing four passports:


*/

class Passport {

public:
  explicit Passport(const std::string_view view)
      : m_birthday_year(), m_issue_year(), m_expiration_year(), m_height(),
        m_hair_color(), m_eye_color(), m_passport_id(), m_country_id() {
    int next_pair_pos_old = 0;
    int next_pair_pos = 0;

    this->m_pass_values = {
        {"ecl", this->m_eye_color},   {"byr", this->m_birthday_year},
        {"iyr", this->m_issue_year},  {"eyr", this->m_expiration_year},
        {"hgt", this->m_height},      {"hcl", this->m_hair_color},
        {"pid", this->m_passport_id}, {"cid", this->m_country_id}};

    while ((next_pair_pos = view.find(" ", next_pair_pos_old)) !=
           std::string_view::npos) {
      store_value_from_view(view, next_pair_pos_old, next_pair_pos);
      next_pair_pos_old = next_pair_pos + 1;
    }
    store_value_from_view(view, next_pair_pos_old, view.size());
  }
  virtual ~Passport() {}

  virtual bool is_valid() const {
    return std::all_of(this->m_pass_values.cbegin(), this->m_pass_values.cend(),
                       [](const auto &pair) {
                         return ((pair.first.compare("cid") == 0) ||
                                 (pair.second.size() > 0));
                       });
  }

  void print_passport() const {
    std::cout << "Passport: (" << this->is_valid() << ") - ";
    for (const auto pair : this->m_pass_values) {
      std::cout << pair.first << " - '" << pair.second << "' ";
    }
    std::cout << "\n";
  }

protected:
  std::string m_birthday_year;
  std::string m_issue_year;
  std::string m_expiration_year;
  std::string m_height;
  std::string m_hair_color;
  std::string m_eye_color;
  std::string m_passport_id;
  std::string m_country_id;

  std::map<const std::string, std::string &> m_pass_values;

  void store_value_from_view(const std::string_view &view,
                             int next_pair_pos_old, int next_pair_pos) {
    const auto pair =
        view.substr(next_pair_pos_old, next_pair_pos - next_pair_pos_old);
    if (pair.size() != 0) {
      const std::string key = std::string{pair.substr(0, pair.find(":"))};
      try {
        this->m_pass_values.at(key) =
            std::string{pair.substr(pair.find(":") + 1)};
      } catch (std::out_of_range &exc) {
        std::cerr << "Invalid key: '" << key << "' (" << exc.what() << ")\n";
      }
    }
  }
};

class Star2Passport : public Passport {
public:
  explicit Star2Passport(const std::string_view view) : Passport(view) {}
  virtual ~Star2Passport() {}
  bool is_valid() const override {
    return this->is_birthyear_valid() && this->is_expirationyear_valid() &&
           this->is_eyecolor_valid() && this->is_haircolor_valid() &&
           this->is_height_valid() && this->is_issueyear_valid() &&
           this->is_passport_id_valid();
  }

private:
  bool is_birthyear_valid() const {
    try {
      int year = std::stoi(this->m_birthday_year);
      return year >= 1920 && year <= 2002;
    } catch (std::exception) {
      return false;
    }
  }

  bool is_issueyear_valid() const {
    try {
      int year = std::stoi(this->m_issue_year);
      return year >= 2010 && year <= 2020;
    } catch (std::exception) {
      return false;
    }
  }

  bool is_expirationyear_valid() const {
    try {
      int year = std::stoi(this->m_expiration_year);
      return year >= 2020 && year <= 2030;
    } catch (std::exception) {
      return false;
    }
  }

  bool is_height_valid() const {
    try {
      int height = std::stoi(m_height.substr(0, this->m_height.size() - 2));
      std::string_view t = this->m_height;
      if (t.substr(this->m_height.size() - 2, 2).compare("cm") == 0) {
        return height >= 150 && height <= 193;
      } else if (t.substr(this->m_height.size() - 2, 2).compare("in") == 0) {
        return height >= 59 && height <= 76;
      }
      return false;
    } catch (std::exception) {
      return false;
    }
  }

  bool is_haircolor_valid() const {
    try {
      if (this->m_hair_color.at(0) != '#') {
        return false;
      }
      if (this->m_hair_color.size() != 7) {
        return false;
      }

      std::size_t valid = 0;
      std::stoll(this->m_hair_color.substr(1), &valid, 16);
      return valid == 6;
    } catch (std::exception) {
      return false;
    }
  }

  bool is_eyecolor_valid() const {
    std::vector valid_colors{"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
    return std::any_of(valid_colors.cbegin(), valid_colors.cend(),
                       [this](const std::string &t) {
                         return t.compare(this->m_eye_color) == 0;
                       });
  }

  bool is_passport_id_valid() const {
    try {
      std::size_t valid = 0;
      std::stoll(this->m_passport_id, &valid);
      return valid == 9;
    } catch (std::exception) {
      return false;
    }
  }
};

std::vector<std::unique_ptr<Passport>> get_passports_from_file(bool star1) {
  std::fstream in_file;
  std::string line;
  std::vector<std::unique_ptr<Passport>> passports;
  in_file.open("input.txt", ::std::ios_base::in);
  std::string temp_line;
  while (getline(in_file, line)) {
    if (line.size() == 0) {
      if (star1) {
        passports.push_back(std::make_unique<Passport>(temp_line));
      } else {
        passports.push_back(std::make_unique<Star2Passport>(temp_line));
      }
      temp_line = "";
    } else {
      temp_line += " " + line;
    }
  }
  if (star1) {
    passports.push_back(std::make_unique<Passport>(temp_line));
  } else {
    passports.push_back(std::make_unique<Star2Passport>(temp_line));
  }
  return passports;
}

int main() {
  auto passports = get_passports_from_file(true);
  std::cout << std::count_if(passports.cbegin(), passports.cend(),
                             [](const std::unique_ptr<Passport> &passport) {
                               return passport->is_valid();
                             })
            << "\n";

  passports = get_passports_from_file(false);
  std::cout << std::count_if(passports.cbegin(), passports.cend(),
                             [](const std::unique_ptr<Passport> &passport) {
                               return passport->is_valid();
                             })
            << "\n";

  return 0;
}