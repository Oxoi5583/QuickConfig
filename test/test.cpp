#include <iostream>
#include <QuickConfig.h>


int main(){
    

    std::cout << "Start" << std::endl;
    QuickConfig::QC_Path _path = QuickConfig::QC_Path();
    std::cout << _path.to_string() << std::endl;
    _path.go_to_child("config");
    std::cout << _path.to_string() << std::endl;

    QuickConfig::QC_Path _path2 = QuickConfig::QC_Path("./config");
    std::cout << _path2.to_string() << std::endl;
    _path2.go_to_parent();
    std::cout << _path2.to_string() << std::endl;

    QC_Server->add_config_path(_path);

    std::cout << "End" << std::endl;
    return 0;
} 

