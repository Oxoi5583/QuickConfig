#ifndef QUICK_CONFIG_H
#define QUICK_CONFIG_H

#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <map>
#include <vector>
#include <unordered_set>
#include <assert.h>

namespace fs = std::filesystem;

namespace QuickConfig{
    
// trim from start (in place)
inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}
inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}


typedef std::string str;
typedef fs::path std_path;

struct QC_Path{
public:
    QC_Path(const str &_path = "");
    void go_to_child(const str& _rel_path);
    void go_to_parent();
    str to_string();
private:
    std_path _std_path;
    void init_path(const str& _path);
    void init_here();
    void check_path_exist();
};


enum class QuickConfigDataType{
    QC_DT_NUMBER,
    QC_DT_STRING,
    QC_DT_BOOLEAN,
};

typedef str config_name;
typedef QuickConfigDataType data_type;
typedef void* value_ptr;
typedef std::pair<data_type, value_ptr> config_value;
typedef std::map<config_name, config_value> config_set;
typedef str config_set_name;
typedef std::map<config_set_name, config_set> config_map;
typedef str file_path;
typedef str file_content;

class QuickConfigServer{
private:
    int checking_line = 0;
    str checking_file = "";
    str checking_config_name = "";
    str checking_config_key = "";
    str checking_line_str = "";
    str checking_value_str = "";
    char checking_value_type = ' ';
    bool checking_is_header_exists = false;
    bool checking_is_config_row_qualified = false;
    bool checking_is_no_duplicated = false;
    std::vector<std::pair<file_path,file_content>> file_content_cache = {};
    std::unordered_set<str> exists_name = {};
    std::unordered_set<str> exists_key = {};

    std::map<QuickConfigDataType, str> type_2_str_map = {
        {QuickConfigDataType::QC_DT_NUMBER ,"Number"},
        {QuickConfigDataType::QC_DT_STRING ,"String"},
        {QuickConfigDataType::QC_DT_BOOLEAN,"Boolean"},
    };

    static QuickConfigServer* instance;
    config_map _c_map = config_map();
    QuickConfigServer();
    void check_configfile_fomat(std::fstream* _file);
    void check_configfile_header(str _line, bool* value);
    void check_configfile_body(str _line, bool* value);
    void check_configfile_duplicated(str _line, bool* value);
    void add_config();
    void raise_checking_error(str msg);

    bool is_number(const std::string &s);
    bool is_bool(const std::string &s);
    double to_number(const std::string &s);
    bool to_bool(const std::string &s);
    static bool _b_val;
    static str _s_val;
    static double _n_val;

public:
    static QuickConfigServer* get_instance();
    void add_config_path(QC_Path _path);

    void init();

    void get(const str& name, const str& key, double& ret_ptr);
    void get(const str& name, const str& key, str& ret_ptr);
    void get(const str& name, const str& key, bool& ret_ptr);

    void set(const str& name, const str& key, double ret_ptr);
    void set(const str& name, const str& key, float ret_ptr);
    void set(const str& name, const str& key, int ret_ptr);
    void set(const str& name, const str& key, str ret_ptr);
    void set(const str& name, const str& key, const char* ret_ptr);
    void set(const str& name, const str& key, bool ret_ptr);

    str get_str(const str& name, const str& key);

    void save();

    ~QuickConfigServer();
};

}

#define QC_Server QuickConfig::QuickConfigServer::get_instance()

#endif