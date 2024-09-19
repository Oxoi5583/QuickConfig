#include "QuickConfig.h"

namespace QuickConfig{

void QC_Path::check_path_exist(){
    if(!fs::is_directory(_std_path)){
        std::cerr 
        << "[ERROR] Exception : " 
        << "Target directory not exists. (" 
        << _std_path.string()
        << ")"
        << std::endl;
        exit(1);
    }
}

void QC_Path::init_path(const str& _path){
    str _path_cp = _path;
    if(_path_cp.starts_with("./")){
        _path_cp.erase(0,2);
    }
    if(_path_cp.starts_with("/")){
        _path_cp.erase(0,1);
    }
    _std_path = fs::absolute(fs::current_path());
    _std_path = _std_path.append(_path_cp);
    check_path_exist();
}

void QC_Path::init_here(){
    _std_path = fs::absolute(fs::current_path());
    check_path_exist();
}

QC_Path::QC_Path(const str& _path){
    if(_path == ""){
        this->init_here();
    }else{
        this->init_path(_path);
    }
}


void QC_Path::go_to_child(const str& _rel_path){
    _std_path = _std_path.append(_rel_path);
    check_path_exist();
}

void QC_Path::go_to_parent(){
    _std_path = _std_path.parent_path();
    check_path_exist();
}

str QC_Path::to_string(){
    return _std_path.string();
}






QuickConfigServer* QuickConfigServer::instance = nullptr;

QuickConfigServer* QuickConfigServer::get_instance(){
    if(QuickConfigServer::instance == nullptr){
        QuickConfigServer::instance = new QuickConfigServer();
    }
    return QuickConfigServer::instance;
}

QuickConfigServer::QuickConfigServer(){

}

QuickConfigServer::~QuickConfigServer(){

}

void QuickConfigServer::check_configfile_fomat(std::fstream* _file){
    str line;
    int line_id = 1;
    while (std::getline(*_file, line)) {
        checking_line = line_id;
        checking_line_str = line;
        std::cout << line << std::endl;
        if(line_id == 1){
            check_configfile_header(line,&checking_is_header_exists);
        }else{
            check_configfile_body(line, &checking_is_config_row_qualified);
            check_configfile_duplicated(line, &checking_is_no_duplicated);
            if(checking_is_header_exists 
                && checking_is_config_row_qualified 
                && checking_is_no_duplicated){
                add_config();
            }
        }
        line_id++;
    }

    std::cout << "is_header_exists : " << checking_is_header_exists << std::endl;
    std::cout << "is_config_row_qualified : " << checking_is_config_row_qualified << std::endl;
    std::cout << "is_no_duplicated : " << checking_is_no_duplicated << std::endl;
}



void QuickConfigServer::check_configfile_header(str _line, bool* value){
    if (_line.starts_with("[QuickConfig::") && _line.ends_with("]")){
        std::cout << _line << " is valid" << std::endl;
        str front = "[QuickConfig::";
        size_t front_c = front.size();
        size_t end_pos = _line.size()-1;
        str config_name = _line.substr(front_c, end_pos-front_c);
        checking_config_name = config_name;
        *value = true;
    }
}

bool QuickConfigServer::is_number(const std::string &s) {
    try {
        double number = std::stod(s);
        return true;
    } catch (const std::invalid_argument& e) {
        return false;
    }
}

bool QuickConfigServer::is_bool(const std::string &s) {
    str _str = s;
    std::transform(_str.begin(), _str.end(), _str.begin(),
        [](unsigned char c){ return std::tolower(c); });
    if(_str == "true"   || _str == "false"  ||
        _str == "t"     || _str == "f"      ||
        _str == "yes"   || _str == "no"     ||
        _str == "y"     || _str == "n"          ){
        return true;
    }else{
        return false;
    }
}


double QuickConfigServer::to_number(const std::string &s) {
    return std::stod(s);
}


bool QuickConfigServer::to_bool(const std::string &s) {
    str _str = s;
    std::transform(_str.begin(), _str.end(), _str.begin(),
        [](unsigned char c){ return std::tolower(c); });
    if(_str == "true" || _str == "yes" || _str == "t" || _str == "y"){
        return true;
    }else if(_str == "false" || _str == "no" || _str == "f" || _str == "n"){
        return false;
    }
}

void QuickConfigServer::check_configfile_body(str _line, bool* value){
    *value = false;

    std::unordered_set<char> available_type = {'n','s','b'};
    // n > Number || s > String || b > Boolean
    bool is_second_char_ad = _line[1] == '@';
    bool is_first_char_type = available_type.contains(_line[0]);
    if(is_first_char_type && is_second_char_ad){
        size_t value_split_pos = _line.find("::");
        size_t type_split_pos = _line.find("@");
        if (value_split_pos != std::string::npos && value_split_pos > type_split_pos) {
            str _value_string = _line.substr(value_split_pos+2,_line.size() - value_split_pos+2);
            checking_value_str = _value_string;
            std::cout << _value_string << " is value string" << std::endl;
            bool is_value_str_valid = false;
            switch (_line[0]){
            case 'n':
                if(is_number(_value_string)){
                    is_value_str_valid = true;
                }
                break;
            case 's':
                is_value_str_valid = true;
                break;
            case 'b':
                if(is_bool(_value_string)){
                    is_value_str_valid = true;
                }
                break;
            default:
                break;
            }

            if(is_value_str_valid){
                str key_name = _line.substr(_line.rfind('@')+1, _line.rfind("::")-2);
                checking_value_type = _line[0];
                checking_config_key = key_name;
                std::cout << _line << " is valid" << std::endl;
                *value = true;
            }
        }
    }
    
    if(!*value){
        raise_checking_error("Target config file have incorrect fomat.");
    }
}
void QuickConfigServer::check_configfile_duplicated(str _line,bool* value){
    // body
    std::cout << checking_config_name+"@@"+checking_config_key << " is name and key" << std::endl;
    std::cout << exists_key.contains(checking_config_name+"@@"+checking_config_key) << " is exists key " << std::endl;
    if(exists_key.contains(checking_config_name+"@@"+checking_config_key)){
        raise_checking_error("Target config key was duplicated.");
    }else{
        exists_key.insert(checking_config_name+"@@"+checking_config_key);
        *value = true;
    }
}

void QuickConfigServer::raise_checking_error(str msg){
    std::cerr 
    << "[ERROR] Exception : " 
    << msg
    << "\n    (" 
    << checking_file
    << "@@<line:"
    << checking_line
    << ">@@["
    << checking_line_str 
    << "])"
    << std::endl
    << "    Header format   : [QuickConfig::<namespace>]"
    << std::endl
    << "    Body format     : <type>@<key>::<value>"
    << std::endl
    ;
    exit(1);
}

void QuickConfigServer::add_config_path(QC_Path _path){
    for(const auto& it 
            : fs::directory_iterator(_path.to_string())){
        str extension = it.path().extension().string();
        if(it.is_regular_file() && extension == ".qc"){
            std::fstream _file = std::fstream(it.path().string());
            checking_file = it.path().string();
            check_configfile_fomat(&_file);
        }
    }

    for(auto& it : _c_map){
        str _ns = it.first;
        config_set _cs = it.second;
        for(auto& it2 : _cs){
            str _key = it2.first;
            std::pair _val = it2.second;
            QuickConfigDataType _val_t = _val.first;
            std::cout 
                << "Namespace : " << _ns << std::endl
                << "    Key : " << _key << std::endl
                << "    Val (type) : " << (size_t)_val_t << std::endl;
            void* _val_ptr = _val.second;
            bool* _val_b = (bool*)malloc(sizeof(bool));
            double* _val_n = (double*)malloc(sizeof(double));
            str* _val_s = (str*)malloc(sizeof(str));
            switch (_val_t)
            {
            case QuickConfigDataType::QC_DT_BOOLEAN:{
                _val_b = static_cast<bool*>(_val_ptr);
                std::cout 
                    << "    Val (bool) : " << *_val_b << std::endl;
                break;
            }
            case QuickConfigDataType::QC_DT_NUMBER:{
                _val_n = static_cast<double*>(_val_ptr);
                std::cout 
                    << "    Val (number) : " << *_val_n << std::endl;
                break;
            }
            case QuickConfigDataType::QC_DT_STRING:{
                _val_s = static_cast<str*>(_val_ptr);
                std::cout 
                    << "    Val (str) : " << *_val_s << std::endl;
                break;
            }
            default:
                break;
            }
        }
    }

    checking_line = 0;
    checking_file = "";
    checking_config_name = "";
    checking_config_key = "";
    checking_line_str = "";
    checking_value_str = "";
    checking_value_type = ' ';
    exists_name.clear();
    exists_key.clear();
    checking_is_header_exists = false;
    checking_is_config_row_qualified = false;
    checking_is_no_duplicated = false;
}



void QuickConfigServer::add_config(){
    if(!_c_map.contains(checking_config_name)){
        _c_map.insert_or_assign(checking_config_name, config_set());
    }
    if(!_c_map[checking_config_name].contains(checking_config_key)){
        std::map<char,QuickConfigDataType> _value_type_map = {
            {'n', QuickConfigDataType::QC_DT_NUMBER},
            {'s', QuickConfigDataType::QC_DT_STRING},
            {'b', QuickConfigDataType::QC_DT_BOOLEAN},
        };
        QuickConfigDataType _val_t = _value_type_map[checking_value_type];
        void* _val_ptr;
        bool* _val_b = (bool*)malloc(sizeof(bool));
        double* _val_n = (double*)malloc(sizeof(double));
        str* _val_s = (str*)malloc(sizeof(str));
        switch (_val_t)
        {
        case QuickConfigDataType::QC_DT_BOOLEAN:{
            _val_b = new bool{to_bool(checking_value_str)};
            _val_ptr = _val_b;
            break;
        }
        case QuickConfigDataType::QC_DT_NUMBER:{
            _val_n = new double{to_number(checking_value_str)};
            _val_ptr = _val_n;
            break;
        }
        case QuickConfigDataType::QC_DT_STRING:{
            _val_s = new str{checking_value_str};
            _val_ptr = _val_s;
            break;
        }
        default:
            break;
        }
        _c_map[checking_config_name].insert_or_assign(checking_config_key, std::make_pair(_val_t,_val_ptr));
    }
}
}


