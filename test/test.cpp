#include <iostream>
#include <QuickConfig.h>

int main(){
    // Create a QC_Path Instance
    QuickConfig::QC_Path _path = QuickConfig::QC_Path();
    // Move Path config folder. By using go to child / go to parent
    _path.go_to_child("config");
    _path.go_to_parent();
    _path.go_to_child("config");

    // Register the config path to QuickConfig Server
    QC_Server->add_config_path(_path);

    // Create a string object
    std::string _demo_str;
    bool _demo_bool;
    double _demo_number;

    // Get value by get() method. 
    // Arguments should be (<Config_NameSpace>, <Config_Key>, <Return_Value>)
    QC_Server->get("Test_Config","str_name",_demo_str);
    std::cout << "_demo_str value : " << _demo_str << std::endl;

    QC_Server->get("Test_Config","num_name",_demo_number);
    std::cout << "_demo_number value : " << _demo_number << std::endl;
    
    QC_Server->get("Test_Config","bool_name",_demo_bool);
    if(_demo_bool){
        std::cout << "_demo_bool value : " << "true" << std::endl;
    }else{
        std::cout << "_demo_bool value : " << "false" << std::endl;
    }

    return 0;
}

