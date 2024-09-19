#include <iostream>
#include <QuickConfig.h>


int main(){
    std::cout << "START" << std::endl;


    QuickConfig::QC_Path _path = QuickConfig::QC_Path();
    _path.go_to_child("config");
    QC_Server->add_config_path(_path);

    std::string* _s_ptr = nullptr;
    QC_Server->get("Test_Config","str_name3",_s_ptr);
    std::cout << "_s_ptr value : " << *_s_ptr << "" << std::endl;
    QC_Server->get("Test_Config","str_name2",_s_ptr);
    std::cout << "_s_ptr value : " << *_s_ptr << "" << std::endl;
    QC_Server->get("Test_Config","str_name3",_s_ptr);
    std::cout << "_s_ptr value : " << *_s_ptr << "" << std::endl;
    QC_Server->get("Test_Config","str_name",_s_ptr);
    std::cout << "_s_ptr value : " << *_s_ptr << "" << std::endl;
    std::cout << "END" << std::endl;
    return 0;
}

